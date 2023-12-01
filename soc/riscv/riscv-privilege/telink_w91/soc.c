/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sys.h>
// #include <clock.h>
// #include <gpio.h>
// #include <ext_driver/ext_pm.h>
// #include "rf.h"
// #include "flash.h"
// #include <watchdog.h>

#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>

/* Software reset defines */
#define reg_reset                   REG_ADDR8(0x1401ef)
#define SOFT_RESET                  0x20u

/* List of supported CCLK frequencies */
#define CLK_160MHZ                  160000000u
// #define CLK_240MHZ                  240000000u // EXPERIMENTAL

/* MID register flash size */
// #define FLASH_MID_SIZE_OFFSET       16
// #define FLASH_MID_SIZE_MASK         0x00ff0000

/* Power Mode value */
// #if DT_ENUM_IDX(DT_NODELABEL(power), power_mode) == 0
// 	#define POWER_MODE      LDO_1P4_LDO_2P0
// #elif DT_ENUM_IDX(DT_NODELABEL(power), power_mode) == 1
// 	#define POWER_MODE      DCDC_1P4_LDO_2P0
// #elif DT_ENUM_IDX(DT_NODELABEL(power), power_mode) == 2
// 	#define POWER_MODE      DCDC_1P4_DCDC_2P0
// #else
// 	#error "Wrong value for power-mode parameter"
// #endif

/* Vbat Type value */
// #if DT_ENUM_IDX(DT_NODELABEL(power), vbat_type) == 0
// 	#define VBAT_TYPE       VBAT_MAX_VALUE_LESS_THAN_3V6
// #elif DT_ENUM_IDX(DT_NODELABEL(power), vbat_type) == 1
// 	#define VBAT_TYPE       VBAT_MAX_VALUE_GREATER_THAN_3V6
// #else
// 	#error "Wrong value for vbat-type parameter"
// #endif

/* Check System Clock value. */
#if ((DT_PROP(DT_PATH(cpus, cpu_0), clock_frequency) != CLK_160MHZ))
	#error "Unsupported clock-frequency. Supported values: 160 MHz"
#endif

/**
 * @brief Perform basic initialization at boot.
 *
 * @return 0
 */
static int soc_w91_init(void)
{
	// Done by FreeRTOS clock_init() and clock_postinit() -> hal/soc/scm2010/soc.c (hal/drivers/clk/clk-scm2010.c)
	
	// unsigned int cclk = DT_PROP(DT_PATH(cpus, cpu_0), clock_frequency);

	/* system init */
	// sys_init(POWER_MODE, VBAT_TYPE, GPIO_VOLTAGE_3V3);

	/* clocks init: CCLK, HCLK, PCLK */
	// switch (cclk) {
	// case CLK_160MHZ:
	// 	CCLK_160M_HCLK_16M_PCLK_16M;
	// 	break;

	// case CLK_240MHZ:
	// 	CCLK_240M_HCLK_24M_PCLK_24M;
	// 	break;
	// }

	/* Init Machine Timer source clock: 32 KHz RC */
	// clock_32k_init(CLK_32K_RC);
	// clock_cal_32k_rc();

	return 0;
}

/**
 * @brief Reset the system.
 */
void sys_arch_reboot(int type)
{
	ARG_UNUSED(type);

	reg_reset = SOFT_RESET;
}

/**
 * @brief Check mounted flash size (should be greater than in .dts).
 */
// static int soc_w91_check_flash(void)
// {
// 	static const size_t dts_flash_size = DT_REG_SIZE(DT_CHOSEN(zephyr_flash));
// 	size_t hw_flash_size = 0;

// 	const flash_capacity_e hw_flash_cap =
// 		(flash_read_mid() & FLASH_MID_SIZE_MASK) >> FLASH_MID_SIZE_OFFSET;

// 	switch (hw_flash_cap) {
// 	case FLASH_SIZE_1M:
// 		hw_flash_size = 1 * 1024 * 1024;
// 		break;
// 	case FLASH_SIZE_2M:
// 		hw_flash_size = 2 * 1024 * 1024;
// 		break;
// 	case FLASH_SIZE_4M:
// 		hw_flash_size = 4 * 1024 * 1024;
// 		break;
// 	case FLASH_SIZE_8M:
// 		hw_flash_size = 8 * 1024 * 1024;
// 		break;
// 	case FLASH_SIZE_16M:
// 		hw_flash_size = 16 * 1024 * 1024;
// 		break;
// 	default:
// 		break;
// 	}

// 	if (hw_flash_size < dts_flash_size) {
// 		// printk("!!! flash error: expected (.dts) %u, actually %u\n",
// 		// 	dts_flash_size, hw_flash_size);
// 		extern void abort(void);
// 		abort();
// 	}

// 	return 0;
// }

SYS_INIT(soc_w91_init, PRE_KERNEL_1, 0);

// SYS_INIT(soc_w91_check_flash, POST_KERNEL, 0);
