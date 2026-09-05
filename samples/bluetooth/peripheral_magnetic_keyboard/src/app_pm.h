/** @file app_pm.h
 *  @brief Power management (sleep/suspend) functions for USB and 2.4G modes.
 */

/*
 * Copyright (c) 2025 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __APP_PM_H__
#define __APP_PM_H__

#include <zephyr/types.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* 2.4G suspend / sleep                                               */
/* ------------------------------------------------------------------ */

/**
 * @brief Check whether 2.4G suspend is allowed by N22.
 *
 * @return true if D25F may enter suspend, false otherwise
 */
bool tl_app_suspend_state(void);

/**
 * @brief Check whether N22 delivered a not-yet-consumed wakeup tick.
 *
 * @return true - fresh tick, main may sleep once into idle;
 *         false - no new tick, keep running main_loop
 */
bool app_p24g_wakeup_tick_is_fresh(void);

/**
 * @brief Mark the current wakeup tick as consumed.
 */
void app_p24g_wakeup_tick_consume(void);

/**
 * @brief Sleep until the N22 wakeup tick delivered this period.
 *
 * Converts next_wakeup_tick into a relative k_sleep duration so main's
 * blocked window matches the N22 window. Actual suspend wake precision is
 * guaranteed by tl_app_suspend()'s min(kernel wake, N22 tick).
 *
 * @return true - k_sleep executed; false - no fresh tick or window exhausted
 */
bool app_p24g_sleep_until_n22_tick(void);

/**
 * @brief 2.4G suspend hook overriding the HAL weak symbol tl_app_suspend().
 *
 * HAL contract: only called after tl_app_suspend_state() returns true;
 * only dynamic time validity is checked here.
 *
 * @param[in] wake_stimer_tick - kernel-calculated next wake stimer tick
 * @return    true if suspend entered, otherwise false
 */
bool tl_app_suspend(uint32_t wake_stimer_tick);

/**
 * @brief USB mode main-thread sleep tick.
 *
 * USB mode does not involve the N22 core: key scan / report TX are
 * SOF-interrupt driven. The 3ms tick only yields the CPU so idle
 * enters WFI.
 */
void app_usb_sleep(void);

#endif /* __APP_PM_H__ */

#ifdef __cplusplus
}
#endif
