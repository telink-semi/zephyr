/*
 * Copyright (c) 2021 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/mbox.h>

#define TX_ID (0)
#define RX_ID (1)

#define CORE_NAME "N22 core -> "

static void callback(const struct device *dev, uint32_t channel,
		     void *user_data, struct mbox_msg *data)
{
	debug_printf(CORE_NAME "Pong (on channel %d)\n", channel);
}

int main(void)
{
	struct mbox_channel tx_channel;
	struct mbox_channel rx_channel;
	const struct device *dev;

	debug_printf(CORE_NAME "Hello from NET\n");

	dev = DEVICE_DT_GET(DT_NODELABEL(mbox));

	mbox_init_channel(&tx_channel, dev, TX_ID);
	mbox_init_channel(&rx_channel, dev, RX_ID);

	if (mbox_register_callback(&rx_channel, callback, NULL)) {
		debug_printf(CORE_NAME "mbox_register_callback() error\n");
		return 0;
	}

	if (mbox_set_enabled(&rx_channel, 1)) {
		debug_printf(CORE_NAME "mbox_set_enable() error\n");
		return 0;
	}

	while (1) {

		debug_printf(CORE_NAME "Ping (on channel %d)\n", tx_channel.id);

		if (mbox_send(&tx_channel, NULL) < 0) {
			debug_printf(CORE_NAME "mbox_send() error\n");
			return 0;
		}

		k_sleep(K_MSEC(3000));
	}
	return 0;
}
