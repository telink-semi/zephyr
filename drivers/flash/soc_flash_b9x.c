/*
 * Copyright (c) 2021-2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT telink_b9x_flash_controller
#define FLASH_SIZE   DT_REG_SIZE(DT_INST(0, soc_nv_flash))
#define FLASH_ORIGIN DT_REG_ADDR(DT_INST(0, soc_nv_flash))

#include <clock.h>
#include <watchdog.h>
#include <flash.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/kernel.h>

/* driver definitions */
#define SECTOR_SIZE            (0x1000u)

#define FLASH_B9X_ACCESS_TIMEOUT_MS  30


/* driver data structure */
struct flash_b9x_data {
	struct k_mutex flash_lock;
	uint8_t sector[SECTOR_SIZE];
};

/* driver parameters structure */
static const struct flash_parameters flash_b9x_parameters = {
	.write_block_size = DT_PROP(DT_INST(0, soc_nv_flash), write_block_size),
	.erase_value = 0xff,
};


/* Check if flash page area is clean */
static bool flash_b9x_is_clean(const struct flash_b9x_data *dev_data, uintptr_t offset, size_t len)
{
	bool result = true;

	for (size_t i = 0; i < len; i++) {
		if (dev_data->sector[offset + i] != flash_b9x_parameters.erase_value) {
			result = false;
			break;
		}
	}
	return result;
}

/* Modify flash data */
static void flash_b9x_modify(struct flash_b9x_data *dev_data, uintptr_t offset,
	const void *data, size_t len)
{
	uintptr_t addr_flash = CONFIG_FLASH_BASE_ADDRESS + offset;
	uintptr_t off_sector = addr_flash % SECTOR_SIZE;

	while (len) {
		size_t len_page_end =  SECTOR_SIZE - off_sector;
		size_t len_current = len_page_end;

		if (len < len_page_end) {
			len_current = len;
		}
		flash_read_page(addr_flash, len_current, &dev_data->sector[off_sector]);
		if (!flash_b9x_is_clean(dev_data, off_sector, len_current)) {
			if (off_sector) {
				flash_read_page(addr_flash - off_sector, off_sector,
					&dev_data->sector[0]);
			}
			if (len < len_page_end) {
				flash_read_page(addr_flash + len_current,
					len_page_end - len_current,
					&dev_data->sector[off_sector + len_current]);
			}
			flash_erase_sector(addr_flash - off_sector);
			if (off_sector) {
				flash_write_page(addr_flash - off_sector, off_sector,
					&dev_data->sector[0]);
			}
			if (len < len_page_end) {
				flash_write_page(addr_flash + len_current,
					len_page_end - len_current,
					&dev_data->sector[off_sector + len_current]);
			}
		}
		if (data) {
			flash_write_page(addr_flash, len_current, (unsigned char *)data);
			data = (uint8_t *)data + len_current;
		}
		len -= len_current;
		addr_flash += len_current;
		off_sector = 0;
	}
}


/* Check for correct offset and length */
static bool flash_b9x_is_range_valid(off_t offset, size_t len)
{
	if ((offset < 0) || (len < 1) || ((offset + len) > FLASH_SIZE)) {
		return false;
	}
	return true;
}

/* API implementation: driver initialization */
static int flash_b9x_init(const struct device *dev)
{
	struct flash_b9x_data *dev_data = dev->data;

	k_mutex_init(&dev_data->flash_lock);

	flash_change_rw_func(flash_4read, flash_quad_page_program);

	return 0;
}

/* API implementation: erase */
static int flash_b9x_erase(const struct device *dev, off_t offset, size_t len)
{
	struct flash_b9x_data *dev_data = dev->data;

	/* check for valid range */
	if (!flash_b9x_is_range_valid(offset, len)) {
		return -EINVAL;
	}

	if (k_mutex_lock(&dev_data->flash_lock, K_MSEC(FLASH_B9X_ACCESS_TIMEOUT_MS))) {
		return -EACCES;
	}

	bool wdt_been_enabled = false;

	if (BM_IS_SET(reg_tmr_ctrl2, FLD_TMR_WD_EN)) {
		BM_CLR(reg_tmr_ctrl2, FLD_TMR_WD_EN);
		wdt_been_enabled = true;
	}

	flash_b9x_modify(dev_data, offset, NULL, len);

	if (wdt_been_enabled) {
		BM_SET(reg_tmr_ctrl2, FLD_TMR_WD_EN);
	}

	k_mutex_unlock(&dev_data->flash_lock);

	return 0;
}

/* API implementation: write */
static int flash_b9x_write(const struct device *dev, off_t offset,
			   const void *data, size_t len)
{
	void *buf = NULL;
	struct flash_b9x_data *dev_data = dev->data;

	/* check for valid range */
	if (!flash_b9x_is_range_valid(offset, len)) {
		return -EINVAL;
	}

	if (k_mutex_lock(&dev_data->flash_lock, K_MSEC(FLASH_B9X_ACCESS_TIMEOUT_MS))) {
		return -EACCES;
	}

	bool wdt_been_enabled = false;

	if (BM_IS_SET(reg_tmr_ctrl2, FLD_TMR_WD_EN)) {
		BM_CLR(reg_tmr_ctrl2, FLD_TMR_WD_EN);
		wdt_been_enabled = true;
	}

	/* need to store data in intermediate RAM buffer in case from flash to flash write */
	if (((uint32_t)data >= FLASH_ORIGIN) &&
		((uint32_t)data < (FLASH_ORIGIN + FLASH_SIZE))) {

		buf = k_malloc(len);
		if (buf == NULL) {
			if (wdt_been_enabled) {
				BM_SET(reg_tmr_ctrl2, FLD_TMR_WD_EN);
			}
			k_mutex_unlock(&dev_data->flash_lock);
			return -ENOMEM;
		}

		/* copy Flash data to RAM */
		flash_read_page((unsigned long)data, len, (unsigned char *)buf);

		/* substitute data with allocated buffer */
		data = buf;
	}
	/* write flash */
	flash_b9x_modify(dev_data, offset, data, len);

	/* if ram memory is allocated for flash writing it should be free */
	if (buf != NULL) {
		k_free(buf);
	}

	if (wdt_been_enabled) {
		BM_SET(reg_tmr_ctrl2, FLD_TMR_WD_EN);
	}

	k_mutex_unlock(&dev_data->flash_lock);

	return 0;
}

/* API implementation: read */
static int flash_b9x_read(const struct device *dev, off_t offset,
			  void *data, size_t len)
{
	struct flash_b9x_data *dev_data = dev->data;

	/* return SUCCESS if len equals 0 (required by tests/drivers/flash) */
	if (!len) {
		return 0;
	}

	/* check for valid range */
	if (!flash_b9x_is_range_valid(offset, len)) {
		return -EINVAL;
	}

	if (k_mutex_lock(&dev_data->flash_lock, K_MSEC(FLASH_B9X_ACCESS_TIMEOUT_MS))) {
		return -EACCES;
	}

	bool wdt_been_enabled = false;

	if (BM_IS_SET(reg_tmr_ctrl2, FLD_TMR_WD_EN)) {
		BM_CLR(reg_tmr_ctrl2, FLD_TMR_WD_EN);
		wdt_been_enabled = true;
	}

	/* read flash */
	flash_read_page(CONFIG_FLASH_BASE_ADDRESS + offset, len, (unsigned char *)data);

	if (wdt_been_enabled) {
		BM_SET(reg_tmr_ctrl2, FLD_TMR_WD_EN);
	}

	k_mutex_unlock(&dev_data->flash_lock);

	return 0;
}

/* API implementation: get_parameters */
static const struct flash_parameters *
flash_b9x_get_parameters(const struct device *dev)
{
	ARG_UNUSED(dev);

	return &flash_b9x_parameters;
}

/* API implementation: page_layout */
#if CONFIG_FLASH_PAGE_LAYOUT
static const struct flash_pages_layout dev_layout = {
	.pages_count = FLASH_SIZE / SECTOR_SIZE,
	.pages_size = SECTOR_SIZE,
};

static void flash_b9x_pages_layout(const struct device *dev,
				   const struct flash_pages_layout **layout,
				   size_t *layout_size)
{
	*layout = &dev_layout;
	*layout_size = 1;
}
#endif /* CONFIG_FLASH_PAGE_LAYOUT */

static struct flash_b9x_data flash_data;

static const struct flash_driver_api flash_b9x_api = {
	.erase = flash_b9x_erase,
	.write = flash_b9x_write,
	.read = flash_b9x_read,
	.get_parameters = flash_b9x_get_parameters,
#if CONFIG_FLASH_PAGE_LAYOUT
	.page_layout = flash_b9x_pages_layout,
#endif
};

/* Driver registration */
DEVICE_DT_INST_DEFINE(0, flash_b9x_init,
		      NULL, &flash_data, NULL, POST_KERNEL,
		      CONFIG_FLASH_INIT_PRIORITY, &flash_b9x_api);
