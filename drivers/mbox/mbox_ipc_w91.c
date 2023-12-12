/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2023 Telink Semiconductor (Shanghai) Co., Ltd.
 */

#include <zephyr/drivers/mbox.h>
#include <plic.h>

#define LOG_LEVEL CONFIG_MBOX_LOG_LEVEL
#include <zephyr/logging/log.h>
#include <zephyr/irq.h>
LOG_MODULE_REGISTER(mbox_ipc_w91);

#define DT_DRV_COMPAT telink_mbox_ipc_w91

#define MAX_CHANNEL_NUM 					2

#define GET_SW_IRQ_NUMB(channel) 			(channel + 1)
#define GET_CHANNEL_NUMB(sw_irq_source)		(sw_irq_source - 1)

struct mbox_w91_data {
	mbox_callback_t cb[MAX_CHANNEL_NUM];
	void *user_data[MAX_CHANNEL_NUM];
	const struct device *dev;
	uint32_t enabled_mask;
};

static struct mbox_w91_data w91_mbox_data;

static int mbox_w91_send(const struct device *dev, uint32_t channel,
			 const struct mbox_msg *msg)
{
	if (channel >= MAX_CHANNEL_NUM) {
		return -EINVAL;
	}

	if (msg) {
		LOG_WRN("Sending data not supported");
	}

	plic_sw_set_pending(GET_SW_IRQ_NUMB(channel));

	return 0;
}

static void mbox_dispatcher(uint32_t source)
{
	uint32_t channel = GET_CHANNEL_NUMB(source);
	struct mbox_w91_data *data = &w91_mbox_data;

	if ((channel < MAX_CHANNEL_NUM) &&
			(data->enabled_mask & BIT(channel)) &&
			(data->cb[channel] != NULL)) {
		data->cb[channel](data->dev, channel, data->user_data[channel], NULL);
	}
}

static int mbox_w91_register_callback(const struct device *dev, uint32_t channel,
				      mbox_callback_t cb, void *user_data)
{
	if (channel >= MAX_CHANNEL_NUM) {
		return -EINVAL;
	}

	struct mbox_w91_data *data = dev->data;

	data->cb[channel] = cb;
	data->user_data[channel] = user_data;

	plic_sw_set_callback(GET_SW_IRQ_NUMB(channel), mbox_dispatcher);

	return 0;
}

static int mbox_w91_mtu_get(const struct device *dev)
{
	/* Only support signalling */
	return 0;
}

static uint32_t mbox_w91_max_channels_get(const struct device *dev)
{
	return MAX_CHANNEL_NUM;
}

static int mbox_w91_set_enabled(const struct device *dev, uint32_t channel, bool enable)
{
	if (channel >= MAX_CHANNEL_NUM) {
		return -EINVAL;
	}

	struct mbox_w91_data *data = dev->data;

	if ((!enable && (!(data->enabled_mask & BIT(channel)))) ||
		(enable && (data->enabled_mask & BIT(channel)))) {
		return -EALREADY;
	}

	if (enable && (data->cb[channel] == NULL)) {
		LOG_WRN("Enabling channel without a registered callback\n");
	}

	if (enable) {
		data->enabled_mask |= BIT(channel);
		plic_sw_interrupt_enable(GET_SW_IRQ_NUMB(channel));
	} else {
		plic_sw_interrupt_disable(GET_SW_IRQ_NUMB(channel));
		data->enabled_mask &= ~BIT(channel);
	}

	return 0;
}

static int mbox_w91_init(const struct device *dev)
{
	struct mbox_w91_data *data = dev->data;

	data->dev = dev;

	/* Enable sw interrupt */
	core_interrupt_enable();
	IRQ_CONNECT(RISCV_MACHINE_SOFT_IRQ, 0, plic_irq_trap_handler, NULL, 0);
	irq_enable(RISCV_MACHINE_SOFT_IRQ);

	return 0;
}

static const struct mbox_driver_api mbox_w91_driver_api = {
	.send = mbox_w91_send,
	.register_callback = mbox_w91_register_callback,
	.mtu_get = mbox_w91_mtu_get,
	.max_channels_get = mbox_w91_max_channels_get,
	.set_enabled = mbox_w91_set_enabled,
};

DEVICE_DT_INST_DEFINE(0, mbox_w91_init, NULL, &w91_mbox_data, NULL,
		    POST_KERNEL, CONFIG_MBOX_INIT_PRIORITY,
		    &mbox_w91_driver_api);
