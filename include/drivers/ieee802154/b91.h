/*
 * Copyright (c) 2022 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_DRIVERS_IEEE802154_B91_H_
#define ZEPHYR_INCLUDE_DRIVERS_IEEE802154_B91_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief init b91 ieee802154 radio driver
 *
 * @param [in] pointer to ieee802154 device
 */
int b91_init(const struct device *dev);

/**
 * @brief deinit b91 ieee802154 radio driver
 *
 * @param [in] pointer to ieee802154 device
 */
void b91_deinit(const struct device *dev);

#ifdef __cplusplus
}
#endif

#endif