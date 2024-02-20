/*
 * Copyright (c) 2024 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT telink_w91_trng

#include <zephyr/drivers/entropy.h>
#include <zephyr/logging/log.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
LOG_MODULE_REGISTER(entropy_w91);

/* API implementation: driver initialization */
static int entropy_w91_trng_init(const struct device *dev)
{
	LOG_ERR("not supported");
	return -ENOTSUP;
}

/* API implementation: get_entropy */
static int entropy_w91_trng_get_entropy(const struct device *dev, uint8_t *buffer, uint16_t length)
{
	LOG_ERR("not supported");
	return -ENOTSUP;
}

/* API implementation: get_entropy_isr */
static int entropy_w91_trng_get_entropy_isr(const struct device *dev, uint8_t *buffer,
					    uint16_t length, uint32_t flags)
{
	LOG_ERR("not supported");
	return -ENOTSUP;
}

/* Entropy driver APIs structure */
static const struct entropy_driver_api entropy_w91_trng_api = {
	.get_entropy = entropy_w91_trng_get_entropy,
	.get_entropy_isr = entropy_w91_trng_get_entropy_isr};

/* Entropy driver registration */
DEVICE_DT_INST_DEFINE(0, entropy_w91_trng_init, NULL, NULL, NULL, POST_KERNEL,
		      CONFIG_TELINK_W91_IPC_DRIVERS_INIT_PRIORITY, &entropy_w91_trng_api);
