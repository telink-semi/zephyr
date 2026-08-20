/* main.c - Application main entry point */

/*
 * Copyright (c) 2025 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/settings/settings.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "app_public.h"
#include "driver.h"

#define LOG_LEVEL LOG_LEVEL_DBG
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

/*
 * Override the weak SOC hook to configure the LPC + PEM + DMA based flash
 * protection used when running at CLK_192MHZ on TL322X.
 */
void tl322x_pem_flash_prot_config(void)
{
	lpc_pem_flash_prot_config(LPC_INPUT_PB5, PEM0, DMA7);
}

int main(void)
{
	keyboard_comm_init();

	gpio_function_en(GPIO_PD5 | GPIO_PD6 | GPIO_PD7);
	gpio_output_en(GPIO_PD5 | GPIO_PD6 | GPIO_PD7);
	gpio_input_dis(GPIO_PD5 | GPIO_PD6 | GPIO_PD7);


    gpio_function_en(GPIO_PA1 | GPIO_PA2);
    gpio_output_en(GPIO_PA1 | GPIO_PA2);
    gpio_input_dis(GPIO_PA1 | GPIO_PA2);


	while(1) {
		public_loop();
		k_sleep(K_MSEC(3));
	}

	return 0;
}
