/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __ST25DV_H_
#define __ST25DV_H_

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/types.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/nfc/nfc_tag.h>

/******************* DEVICE STRUCTURE *******************/

struct st25dvxxkc_cfg {
    /* i2c parameters */
    struct i2c_dt_spec i2c;
    /* irq DTS settings */
    const struct gpio_dt_spec irq_gpio;
    uint8_t irq_pin;
    /* internal (1) or external driver (0) */
    uint8_t internal;
};

struct st25dvxxkc_data {
    const struct device *parent;
    const struct device *dev_i2c;
    struct k_work worker_irq;
    /* NFC subsys data */
    nfc_tag_cb_t nfc_tag_cb;
    enum nfc_tag_type tag_type;
};

#endif /* __ST25DV_H_ */
