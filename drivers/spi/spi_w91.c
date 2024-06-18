/*
 * Copyright (c) 2021-2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT telink_w91_spi

typedef enum{
	LSPI_CSN_PE0_PIN 		= 0,
	LSPI_CLK_PE1_PIN 		= 1,
	LSPI_MOSI_IO0_PE2_PIN 	= 2,
	LSPI_MISO_IO1_PE3_PIN 	= 3,
	LSPI_IO2_PE4_PIN   		= 4,
	LSPI_IO3_PE5_PIN 		= 5,
	GPIO_PA1 = 6
};
// #include <include/clock.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(spi_telink);

#include <zephyr/drivers/spi.h>
#include "spi_context.h"
#include <zephyr/drivers/pinctrl.h>
#include <ipc/ipc_based_driver.h>

enum {
	IPC_DISPATCHER_SPI_CONFIGURE_EVENT = IPC_DISPATCHER_SPI,
	IPC_DISPATCHER_SPI_MASTER_READ_EVENT,
	IPC_DISPATCHER_SPI_MASTER_WRITE_EVENT,
};

#define CHIP_SELECT_COUNT               3u
#define SPI_WORD_SIZE                   8u
#define SPI_WR_RD_CHUNK_SIZE_MAX        16u


/* SPI configuration structure */
struct spi_w91_cfg {
	uint8_t cs_pin[CHIP_SELECT_COUNT];
	const struct pinctrl_dev_config *pcfg;
	uint8_t instance_id;
};
#define SPI_CFG(dev)                    ((struct spi_w91_cfg *) ((dev)->config))

/* SPI data structure */
struct spi_w91_data {
	struct spi_context ctx;
	// struct k_mutex mutex;
	struct ipc_based_driver ipc; /* ipc driver part */
};
#define SPI_DATA(dev)                   ((struct spi_w91_data *) ((dev)->data))

enum spi_role {
	SPI_ROLE_MASTER,
	SPI_ROLE_SLAVE,
};

/* ATCSPI200 config request */
enum spi_mode {
	SPI_MODE_0, /* active high, odd edge sampling */
	SPI_MODE_1, /* active high, even edge sampling */
	SPI_MODE_2, /* active low, odd edge sampling */
	SPI_MODE_3, /* active low, even edge sampling */
};

enum spi_data_io_format {
	SPI_DATA_IO_FORMAT_SINGLE,
	SPI_DATA_IO_FORMAT_DUAL,
	SPI_DATA_IO_FORMAT_QUAD,
};

enum spi_bit_order {
	SPI_BIT_MSB_FIRST,
	SPI_BIT_LSB_FIRST,
};

enum spi_clk_src {
	/* spi0, spi1 and spi2 use XTAL 40Mhz */
	SPI_CLK_SRC_XTAL,
	/* spi0 and spi1 use 240Mhz, spi2 uses 80Mhz */
	SPI_CLK_SRC_PLL,
};

struct spi_cfg_req {
	enum spi_role role;
	enum spi_mode mode;
	enum spi_data_io_format data_io_format;
	enum spi_bit_order bit_order;
	enum spi_clk_src clk_src;
	uint8_t clk_div_2mul;
	uint8_t dma_enable;
};

struct spi_master_tx_req {
	int tx_len;
	uint8_t *tx_buf;
};

struct spi_master_rx_req {
	uint32_t rx_len;
};

/* disable hardware cs flow control */
static void spi_w91_hw_cs_disable(const struct spi_w91_cfg *config)
{
	uint8_t pin;

	/* loop through all cs pins (cs0..cs2) */
	for (int i = 0; i < CHIP_SELECT_COUNT; i++) {
		/* get CS pin defined in device tree */
		pin = config->cs_pin[i];

		/* if CS pin is defined in device tree */
		if (pin != 0) {
// #if CONFIG_SOC_RISCV_TELINK_B91
// 			if (config->peripheral_id == PSPI_MODULE) {
// 				pspi_cs_pin_dis(pin);
// 			} else {
// 				hspi_cs_pin_dis(pin);
// 			}
// #elif CONFIG_SOC_RISCV_TELINK_B92 || CONFIG_SOC_RISCV_TELINK_B95
// 			if (config->peripheral_id == LSPI_MODULE) {
// 				/* Note: lspi_cs_pin_dis has not added to SPI driver for B92 */
// 			} else {
// 				gspi_cs_pin_dis(pin);
// 			}
// #endif
		}
	}
}

/* config cs flow control: hardware or software */
static bool spi_w91_config_cs(const struct device *dev,
			      const struct spi_config *config)
{
	uint8_t cs_pin = 0;
	const struct spi_w91_cfg *w91_config = SPI_CFG(dev);

	/* software flow control */
	if (spi_cs_is_gpio(config)) {
		/* disable all hardware CS pins */
		spi_w91_hw_cs_disable(w91_config);
		return true;
	}

	/* hardware flow control */

	/* check for correct slave id */
	if (config->slave >= CHIP_SELECT_COUNT) {
		LOG_ERR("Slave %d not supported (max. %d)", config->slave, CHIP_SELECT_COUNT - 1);
		return false;
	}

	/* loop through all cs pins: cs0, cs1 and cs2 */
	for (int cs_id = 0; cs_id < CHIP_SELECT_COUNT; cs_id++) {
		/* get cs pin defined in device tree */
		cs_pin = w91_config->cs_pin[cs_id];

		/*  if cs pin is not defined for the selected slave, return error */
		if ((cs_pin == 0) && (cs_id == config->slave)) {
			LOG_ERR("cs%d-pin is not defined in device tree", config->slave);
			return false;
		}

		/* disable cs pin if it is defined and is not requested */
		if ((cs_pin != 0) && (cs_id != config->slave)) {
// #if CONFIG_SOC_RISCV_TELINK_B91
// 			if (w91_config->peripheral_id == PSPI_MODULE) {
// 				pspi_cs_pin_dis(cs_pin);
// 			} else {
// 				hspi_cs_pin_dis(cs_pin);
// 			}
// #elif CONFIG_SOC_RISCV_TELINK_B92 || CONFIG_SOC_RISCV_TELINK_B95
// 			if (w91_config->peripheral_id == LSPI_MODULE) {
// 				/* Note: lspi_cs_pin_dis has not added to SPI driver for B92 */
// 			} else {
// 				gspi_cs_pin_dis(cs_pin);
// 			}
// #endif
		}

		/* enable cs pin if it is defined and is requested */
		if ((cs_pin != 0) && (cs_id == config->slave)) {
// #if CONFIG_SOC_RISCV_TELINK_B91
// 			if (w91_config->peripheral_id == PSPI_MODULE) {
// 				pspi_cs_pin_en(cs_pin);
// 			} else {
// 				hspi_cs_pin_en(cs_pin);
// 			}
// #elif CONFIG_SOC_RISCV_TELINK_B92 || CONFIG_SOC_RISCV_TELINK_B95
// 			if (w91_config->peripheral_id == LSPI_MODULE) {
// 				/* Note: lspi_cs_pin_en has not added to SPI driver for B92,
// 				 * lspi_cs_pin_en must call lspi_set_pin_mux
// 				 */
// 				lspi_set_pin_mux(cs_pin);
// 			} else {
// 				gspi_cs_pin_en(cs_pin);
// 			}
// #endif
		}
	}

	return true;
}

/* get spi transaction length */
static uint32_t spi_w91_get_txrx_len(const struct spi_buf_set *tx_bufs,
				     const struct spi_buf_set *rx_bufs)
{
	uint32_t len_tx = 0;
	uint32_t len_rx = 0;

	/* calculate tx len */
	if (tx_bufs) {
		const struct spi_buf *tx_buf = tx_bufs->buffers;

		for (int i = 0; i < tx_bufs->count; i++) {
			len_tx += tx_buf->len;
			tx_buf++;
		}
	}

	/* calculate rx len */
	if (rx_bufs) {
		const struct spi_buf *rx_buf = rx_bufs->buffers;

		for (int i = 0; i < rx_bufs->count; i++) {
			len_rx += rx_buf->len;
			rx_buf++;
		}
	}

	return MAX(len_tx, len_rx);
}

/* APIs implementation: spi_write */
static size_t pack_spi_w91_ipc_master_write(uint8_t inst, void *unpack_data, uint8_t *pack_data)
{
	struct spi_master_tx_req *p_spi_master_tx = unpack_data;
	uint8_t *temp = unpack_data;
	size_t pack_data_len = sizeof(uint32_t) + sizeof(p_spi_master_tx->tx_len) +
		p_spi_master_tx->tx_len;
	if (pack_data != NULL) {
		uint32_t id = IPC_DISPATCHER_MK_ID(IPC_DISPATCHER_SPI_MASTER_WRITE_EVENT, inst);

		IPC_DISPATCHER_PACK_FIELD(pack_data, id);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_spi_master_tx->tx_len);
		IPC_DISPATCHER_PACK_ARRAY(pack_data, p_spi_master_tx->tx_buf,
					  p_spi_master_tx->tx_len);
	}
	return pack_data_len;
}

IPC_DISPATCHER_UNPACK_FUNC_ONLY_WITH_ERROR_PARAM(spi_w91_ipc_master_write);

static int spi_w91_ipc_master_write(const struct device *dev, uint8_t *tx_buf,
				    uint32_t len)
{
	int err = -1;
	struct spi_master_tx_req spi_master_tx = {
		.tx_len = len,
		.tx_buf = tx_buf,
	};
	struct ipc_based_driver *ipc_data = &((struct spi_w91_data *)dev->data)->ipc;
	uint8_t inst = ((struct spi_w91_cfg *)dev->config)->instance_id;

	IPC_DISPATCHER_HOST_SEND_DATA(ipc_data, inst, spi_w91_ipc_master_write, &spi_master_tx,
				      &err, CONFIG_SPI_TELINK_W91_IPC_RESPONSE_TIMEOUT_MS);
	return err;
}

/* process tx data */
/*_attribute_ram_code_sec_*/
static void spi_w91_tx(struct spi_context *ctx, uint8_t len)
{
	uint8_t tx;

	for (int i = 0; i < len; i++) {
		if (spi_context_tx_buf_on(ctx)) {
			tx = *(uint8_t *)(ctx->tx_buf);
		} else {
			tx = 0;
		}
		spi_context_update_tx(ctx, 1, 1);
		// spi_w91_ipc_master_write(peripheral_id, &tx, 1);
	}
}

/* process rx data */
/*_attribute_ram_code_sec_*/
static void spi_w91_rx(struct spi_context *ctx, uint8_t len)
{
	uint8_t rx = 0;

	for (int i = 0; i < len; i++) {
		// hal_spi_read(&rx, 1);
		if (spi_context_rx_buf_on(ctx)) {
			*ctx->rx_buf = rx;
		}
		// spi_context_update_rx(ctx, 1, 1);
	}
}

/* SPI transceive internal */
/*_attribute_ram_code_sec_*/
static void spi_w91_txrx(const struct device *dev, uint32_t len)
{
	unsigned int chunk_size = SPI_WR_RD_CHUNK_SIZE_MAX;
	struct spi_w91_cfg *cfg = SPI_CFG(dev);
	struct spi_context *ctx = &SPI_DATA(dev)->ctx;

	/* prepare SPI module */
	// spi_set_transmode(cfg->peripheral_id, SPI_MODE_WRITE_AND_READ);
	// spi_set_cmd(cfg->peripheral_id, 0);
	// spi_tx_cnt(cfg->peripheral_id, len);
	// spi_rx_cnt(cfg->peripheral_id, len);

	/* write and read bytes in chunks */
	for (int i = 0; i < len; i = i + chunk_size) {
		/* check for tail */
		if (chunk_size > (len - i)) {
			chunk_size = len - i;
		}

		/* write bytes */
		spi_w91_tx(ctx, chunk_size);

		/* read bytes */
		if (len <= SPI_WR_RD_CHUNK_SIZE_MAX) {
			/* read all bytes if len is less than chunk size */
			spi_w91_rx(ctx, chunk_size);
		} else if (i == 0) {
			/* head, read 1 byte less than is sent */
			spi_w91_rx(ctx, chunk_size - 1);
		} else if ((len - i) > SPI_WR_RD_CHUNK_SIZE_MAX) {
			/* body, read so many bytes as is sent*/
			spi_w91_rx(ctx, chunk_size);
		} else {
			/* tail, read the rest bytes */
			spi_w91_rx(ctx, chunk_size + 1);
		}

		/* clear TX and RX fifo */
// #if CONFIG_SOC_RISCV_TELINK_B91
// 		BM_SET(reg_spi_fifo_state(cfg->peripheral_id), FLD_SPI_TXF_CLR);
// 		BM_SET(reg_spi_fifo_state(cfg->peripheral_id), FLD_SPI_RXF_CLR);
// #elif CONFIG_SOC_RISCV_TELINK_B92 || CONFIG_SOC_RISCV_TELINK_B95
// 		BM_SET(reg_spi_status(cfg->peripheral_id), FLD_SPI_TXF_CLR_LEVEL);
// 		BM_SET(reg_spi_status(cfg->peripheral_id), FLD_SPI_RXF_CLR_LEVEL);
// #endif
	}

	/* wait fot SPI is ready */
	// while (spi_is_busy(cfg->peripheral_id)) {
	// };

	/* context complete */
	spi_context_complete(ctx, dev, 0);
}

/* Check for supported configuration */
static bool spi_w91_is_config_supported(const struct spi_config *config,
					struct spi_w91_cfg *w91_config)
{
	if (config->operation & SPI_HALF_DUPLEX) {
		LOG_ERR("Half-duplex not supported");
		return false;
	}

	/* check for loop back */
	if (config->operation & SPI_MODE_LOOP) {
		LOG_ERR("Loop back mode not supported");
		return false;
	}

	/* check for transfer LSB first */
	if (config->operation & SPI_TRANSFER_LSB) {
		LOG_ERR("LSB first not supported");
		return false;
	}

	/* check word size */
	if (SPI_WORD_SIZE_GET(config->operation) != SPI_WORD_SIZE) {
		LOG_ERR("Word size must be %d", SPI_WORD_SIZE);
		return false;
	}

	/* check for CS active high */
	if (config->operation & SPI_CS_ACTIVE_HIGH) {
		LOG_ERR("CS active high not supported for HW flow control");
		return false;
	}

	/* check for lines configuration */
	if (IS_ENABLED(CONFIG_SPI_EXTENDED_MODES)) {
		if ((config->operation & SPI_LINES_MASK) == SPI_LINES_OCTAL) {
			LOG_ERR("SPI lines Octal is not supported");
			return false;
		}
// #if CONFIG_SOC_RISCV_TELINK_B91
// 		else if (((config->operation & SPI_LINES_MASK) == SPI_LINES_QUAD)
// 				&& (w91_config->peripheral_id == PSPI_MODULE)) {
// 			LOG_ERR("SPI lines Quad is not supported by PSPI");
// 			return false;
// 		}
// #endif
	}

	/* check for slave configuration */
	if (SPI_OP_MODE_GET(config->operation) == SPI_OP_MODE_SLAVE) {
		LOG_ERR("SPI Slave is not implemented");
		return -ENOTSUP;
	}

	return true;
}

/* API implementation: SPI configure */
static size_t pack_spi_w91_ipc_configure(uint8_t inst, void *unpack_data, uint8_t *pack_data)
{
	struct spi_cfg_req *p_spi_cfg = unpack_data;
	size_t pack_data_len = sizeof(uint32_t) + sizeof(p_spi_cfg->role) +
			       sizeof(p_spi_cfg->mode) + sizeof(p_spi_cfg->data_io_format) +
			       sizeof(p_spi_cfg->bit_order) + sizeof(p_spi_cfg->clk_src) +
				   sizeof(p_spi_cfg->clk_div_2mul) + sizeof(p_spi_cfg->dma_enable);
	if (pack_data != NULL) {
		uint32_t id = IPC_DISPATCHER_MK_ID(IPC_DISPATCHER_SPI_CONFIGURE_EVENT, inst);

		IPC_DISPATCHER_PACK_FIELD(pack_data, id);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_spi_cfg->role);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_spi_cfg->mode);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_spi_cfg->data_io_format);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_spi_cfg->bit_order);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_spi_cfg->clk_src);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_spi_cfg->clk_div_2mul);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_spi_cfg->dma_enable);
	}
	return pack_data_len;
}

IPC_DISPATCHER_UNPACK_FUNC_ONLY_WITH_ERROR_PARAM(spi_w91_ipc_configure);

/* SPI configuration */
static int spi_w91_config(const struct device *dev,
			  const struct spi_config *config)
{
	printk("[spi_w91_config] \n");
	int status = 0;
	struct spi_w91_cfg *w91_config = SPI_CFG(dev);
	struct spi_w91_data *w91_data = SPI_DATA(dev);
	const pinctrl_soc_pin_t *pins = w91_config->pcfg->states->pins;

	struct spi_cfg_req w91_cfg_req = {
		.role = SPI_ROLE_MASTER,
	};

	/* check for unsupported configuration */
	if (!spi_w91_is_config_supported(config, w91_config)) {
		return -ENOTSUP;
	}

	/* config slave selection (CS): hw or sw */
	if (!spi_w91_config_cs(dev, config)) {
		return -ENOTSUP;
	}

	/* set clock source and divider */
	w91_cfg_req.clk_src = SPI_CLK_SRC_XTAL;
	w91_cfg_req.clk_div_2mul = 20;

	w91_cfg_req.bit_order = SPI_BIT_MSB_FIRST;
	w91_cfg_req.dma_enable = 0;

	/* set SPI mode */
	if (((config->operation & SPI_MODE_CPHA) == 0) &&
	    ((config->operation & SPI_MODE_CPOL) == 0)) {
		w91_cfg_req.mode = SPI_MODE_0;
	} else if (((config->operation & SPI_MODE_CPHA) == 0) &&
		   ((config->operation & SPI_MODE_CPOL) == SPI_MODE_CPOL)) {
		w91_cfg_req.mode = SPI_MODE_1;
	} else if (((config->operation & SPI_MODE_CPHA) == SPI_MODE_CPHA) &&
		   ((config->operation & SPI_MODE_CPOL) == 0)) {
		w91_cfg_req.mode = SPI_MODE_2;
	} else if (((config->operation & SPI_MODE_CPHA) == SPI_MODE_CPHA) &&
		   ((config->operation & SPI_MODE_CPOL) == SPI_MODE_CPOL)) {
		w91_cfg_req.mode = SPI_MODE_3;
	}

	/* set lines configuration */
	if (IS_ENABLED(CONFIG_SPI_EXTENDED_MODES)) {
		uint32_t lines = config->operation & SPI_LINES_MASK;

		if (lines == SPI_LINES_SINGLE) {
			w91_cfg_req.data_io_format = SPI_DATA_IO_FORMAT_SINGLE;
		} else if (lines == SPI_LINES_DUAL) {
			w91_cfg_req.data_io_format = SPI_DATA_IO_FORMAT_DUAL;
		} else if (lines == SPI_LINES_QUAD) {
			w91_cfg_req.data_io_format = SPI_DATA_IO_FORMAT_QUAD;
		}
	}

	struct ipc_based_driver *ipc_data = &((struct spi_w91_data *)dev->data)->ipc;

	int ipc_err = -1;
	IPC_DISPATCHER_HOST_SEND_DATA(ipc_data, 0,
		spi_w91_ipc_configure, &w91_cfg_req, &ipc_err,
		CONFIG_SPI_TELINK_W91_IPC_RESPONSE_TIMEOUT_MS);
	if (ipc_err != 0) {
		LOG_ERR("SPI set IPC config failed\n");
		return -EHOSTDOWN;
	}

	/* save context config */
	w91_data->ctx.config = config;

	printk("[spi_w91_config] END\n");
	return 0;
}

/* API implementation: init */
static int spi_w91_init(const struct device *dev)
{
	int err;
	struct spi_w91_data *data = dev->data;
	const struct spi_w91_cfg *cfg = dev->config;

	ipc_based_driver_init(&data->ipc);

	/* configure pins */
	err = pinctrl_apply_state(cfg->pcfg, PINCTRL_STATE_DEFAULT);
	if (err < 0) {
		LOG_ERR("Failed to configure SPI pins");
		return err;
	}

	err = spi_context_cs_configure_all(&data->ctx);
	if (err < 0) {
		return err;
	}

	spi_context_unlock_unconditionally(&data->ctx);

	return 0;
}

/* API implementation: transceive */
static int spi_w91_transceive(const struct device *dev,
			      const struct spi_config *config,
			      const struct spi_buf_set *tx_bufs,
			      const struct spi_buf_set *rx_bufs)
{
	int status = 0;
	struct spi_w91_data *data = SPI_DATA(dev);
	uint32_t txrx_len = spi_w91_get_txrx_len(tx_bufs, rx_bufs);

	/* set configuration */
	status = spi_w91_config(dev, config);
	if (status) {
		return status;
	}

	/* context setup */
	spi_context_lock(&data->ctx, false, NULL, NULL, config);
	spi_context_buffers_setup(&data->ctx, tx_bufs, rx_bufs, 1);

	/* if cs is defined: software cs control, set active true */
	if (spi_cs_is_gpio(config)) {
		spi_context_cs_control(&data->ctx, true);
	}

	/* transceive data */
	spi_w91_txrx(dev, txrx_len);

	/* if cs is defined: software cs control, set active false */
	if (spi_cs_is_gpio(config)) {
		spi_context_cs_control(&data->ctx, false);
	}

	/* release context */
	/* spi_context_wait_for_completion may be refactored to IRQ driven routines from N22 */
	status = spi_context_wait_for_completion(&data->ctx);
	spi_context_release(&data->ctx, status);

	return status;
}

#ifdef CONFIG_SPI_ASYNC
/* API implementation: transceive_async */
static int spi_w91_transceive_async(const struct device *dev,
				    const struct spi_config *config,
				    const struct spi_buf_set *tx_bufs,
				    const struct spi_buf_set *rx_bufs,
				    spi_callback_t cb,
				    void *userdata)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(config);
	ARG_UNUSED(tx_bufs);
	ARG_UNUSED(rx_bufs);
	ARG_UNUSED(cb);
	ARG_UNUSED(userdata);

	return -ENOTSUP;
}
#endif /* CONFIG_SPI_ASYNC */

/* API implementation: release */
static int spi_w91_release(const struct device *dev,
			   const struct spi_config *config)
{
	struct spi_w91_data *data = SPI_DATA(dev);

	if (!spi_context_configured(&data->ctx, config)) {
		return -EINVAL;
	}

	spi_context_unlock_unconditionally(&data->ctx);

	return 0;
}

/* SPI driver APIs structure */
static struct spi_driver_api spi_w91_api = {
	.transceive = spi_w91_transceive,
	.release = spi_w91_release,
#ifdef CONFIG_SPI_ASYNC
	.transceive_async = spi_w91_transceive_async,
#endif /* CONFIG_SPI_ASYNC */
};

/* SPI driver registration */
#define SPI_W91_INIT(inst)						  \
									  \
	PINCTRL_DT_INST_DEFINE(inst);					  \
									  \
	static struct spi_w91_data spi_w91_data_##inst = {		  \
		SPI_CONTEXT_INIT_LOCK(spi_w91_data_##inst, ctx),	  \
		SPI_CONTEXT_INIT_SYNC(spi_w91_data_##inst, ctx),	  \
		SPI_CONTEXT_CS_GPIOS_INITIALIZE(DT_DRV_INST(inst), ctx)	  \
	};								  \
									  \
	const static struct spi_w91_cfg spi_w91_cfg_##inst = {		  \
		.cs_pin[0] = DT_INST_STRING_TOKEN(inst, cs0_pin),	  \
		.cs_pin[1] = DT_INST_STRING_TOKEN(inst, cs1_pin),	  \
		.cs_pin[2] = DT_INST_STRING_TOKEN(inst, cs2_pin),	  \
		.pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(inst),		  \
		.instance_id = inst,	      \
	};								  \
									  \
	DEVICE_DT_INST_DEFINE(inst, spi_w91_init,			  \
			      NULL,					  \
			      &spi_w91_data_##inst,			  \
			      &spi_w91_cfg_##inst,			  \
			      POST_KERNEL,				  \
			      CONFIG_TELINK_W91_IPC_DRIVERS_INIT_PRIORITY,			  \
			      &spi_w91_api);

DT_INST_FOREACH_STATUS_OKAY(SPI_W91_INIT)
