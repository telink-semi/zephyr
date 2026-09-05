/** @file app_pm.c
 *  @brief Power management (sleep/suspend) for USB and 2.4G modes.
 */

/*
 * Copyright (c) 2025 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <stdbool.h>
#include <zephyr/kernel.h>
#include <zephyr/usb/usbd.h>
#include <ext_driver/ext_pm.h>

#include "app_pm.h"
#include "app_public.h"
#include "app_kb_matrix.h"

#define LOG_LEVEL LOG_LEVEL_DBG
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app_pm);

/* ------------------------------------------------------------------ */
/* 2.4G constants                                                      */
/* ------------------------------------------------------------------ */

/*
 * Minimum sleep duration (us) for suspend entry. Shorter sleeps save no
 * power (pm_sleep_wakeup() busy-waits on too-short intervals), so fall
 * back to WFI instead.
 */
#define APP_SUSPEND_MIN_TIME_US    (1500)

/* Margin added to k_sleep duration: covers wake/schedule latency and tick
 * rounding. Waking slightly late is harmless; waking early wastes an idle
 * PM attempt. */
#define APP_SLEEP_MARGIN_US     (50)

/* Upper bound of one dynamic k_sleep. If N22 grants a longer window
 * (keep-alive), sleep continues in clamped segments. Must stay below
 * WDT_INTV_MS when APP_WDT_ENABLE is turned on. */
#define APP_SLEEP_MAX_US        (300 * 1000)

/* Fixed main-loop sleep tick when N22 grants no suspend window.
 * No suspend here; idle falls through to WFI. */
#define APP_IDLE_SLEEP_MS       (3)

/* ------------------------------------------------------------------ */
/* USB constant                                                        */
/* ------------------------------------------------------------------ */

#define APP_USB_SLEEP_MS    (3)

/* ------------------------------------------------------------------ */
/* 2.4G local state                                                    */
/* ------------------------------------------------------------------ */

/**
 * @brief Last consumed N22 wakeup tick (D25F local, invisible to N22).
 *
 * next_wakeup_tick is rewritten by N22 every period, so it doubles as a
 * new-period edge signal. main only sleeps once per new tick, preventing
 * repeated idle entries with a stale tick after a failed suspend.
 */
static uint32_t s_consumed_wakeup_tick;

/* ------------------------------------------------------------------ */
/* 2.4G helper                                                         */
/* ------------------------------------------------------------------ */

/**
 * @brief Wraparound-safe tick compare: true if tick_a is later than tick_b.
 */
static inline bool app_tick_after(uint32_t tick_a, uint32_t tick_b)
{
	return (uint32_t)(tick_a - tick_b) < (1u << 30);
}

/* ------------------------------------------------------------------ */
/* 2.4G suspend / sleep functions (moved from app_d24g.c)              */
/* ------------------------------------------------------------------ */

_attribute_ram_code_sec_ bool tl_app_suspend_state(void)
{
    return g_p24g_pm_info.suspend_allowed;
}

/**
 * @brief Check whether N22 delivered a not-yet-consumed wakeup tick.
 */
_attribute_ram_code_sec_ bool app_p24g_wakeup_tick_is_fresh(void)
{
	return g_p24g_pm_info.suspend_allowed &&
	       (g_p24g_pm_info.next_wakeup_tick != s_consumed_wakeup_tick);
}

/**
 * @brief Mark the current wakeup tick as consumed.
 */
_attribute_ram_code_sec_ void app_p24g_wakeup_tick_consume(void)
{
	s_consumed_wakeup_tick = g_p24g_pm_info.next_wakeup_tick;
}

/**
 * @brief 2.4G main-thread sleep policy (2.4G mode only, entered via
 *        public_sleep()).
 *
 * 1. Fresh N22 window: sleep dynamically until next_wakeup_tick (covers
 *    125/250 and future keep-alive). Actual suspend wake precision is
 *    guaranteed by tl_app_suspend()'s min(kernel wake, N22 tick).
 * 2. No window: fixed 3ms tick; idle enters WFI. If N22 grants a window
 *    mid-sleep, idle performs the suspend on its own (main is not needed).
 *
 * @return true - slept dynamically until the N22 tick; false - fixed tick
 *         or no sleep.
 */
_attribute_ram_code_sec_ bool app_p24g_sleep_until_n22_tick(void)
{
	uint32_t now;
	uint32_t remaining_us;

	if (!app_p24g_wakeup_tick_is_fresh()) {
		/* No N22 window: fixed 3ms tick, idle falls through to WFI */
		k_sleep(K_MSEC(APP_IDLE_SLEEP_MS));
		return false;
	}

	now = stimer_get_tick();

	/* Wraparound-safe remaining time (same stimer base as next_wakeup_tick) */
	if ((uint32_t)(g_p24g_pm_info.next_wakeup_tick - now) <
	    (uint32_t)APP_SUSPEND_MIN_TIME_US * SYSTEM_TIMER_TICK_1US) {
		/* Window exhausted / too close: not worth sleeping, N22 sends a
		 * new tick shortly; spin and wait */
		app_p24g_wakeup_tick_consume();
		return false;
	}

	remaining_us = (g_p24g_pm_info.next_wakeup_tick - now) / SYSTEM_TIMER_TICK_1US;
	if (remaining_us > APP_SLEEP_MAX_US - APP_SLEEP_MARGIN_US) {
		remaining_us = APP_SLEEP_MAX_US - APP_SLEEP_MARGIN_US;
	}

	/* Consume before sleeping: no re-entry into idle with a stale tick
	 * after a failed suspend or early wake */
	app_p24g_wakeup_tick_consume();
	k_sleep(K_USEC(remaining_us - APP_SLEEP_MARGIN_US));

	return true;
}

/**
 * @brief     2.4G suspend hook overriding the HAL weak symbol tl_app_suspend().
 *
 * HAL contract: only called after tl_app_suspend_state() returns true;
 * only dynamic time validity is checked here.
 *
 * Enters suspend (2.4G path, BLE controller stopped). Short suspend: keeps
 * the baseband (ZB) power domain powered.
 *
 * Actual sleep duration is min(kernel wake tick, N22 next wake tick) so the
 * device wakes before N22's next TX/event. Returns false for an expired or
 * too-short window so the kernel falls back to WFI.
 *
 * @param[in] wake_stimer_tick - kernel-calculated next wake stimer tick
 * @return    true if suspend entered, otherwise false
 */
_attribute_ram_code_sec_ bool tl_app_suspend(uint32_t wake_stimer_tick)
{
	bool result = false;
    static uint32_t now;
    static uint32_t actual_wakeup_tick;

    now = stimer_get_tick();
    actual_wakeup_tick = wake_stimer_tick;

	/* Take the earlier of kernel wake and N22 next wake (wraparound-safe) */
	if (app_tick_after(actual_wakeup_tick, g_p24g_pm_info.next_wakeup_tick)) {
		actual_wakeup_tick = g_p24g_pm_info.next_wakeup_tick;
	}

	/* Wake tick already expired: cannot sleep */
	if (!app_tick_after(actual_wakeup_tick, now)) {
		return false;
	}

	/* Remaining time too short to be worth suspend: fall back to WFI */
	if ((uint32_t)(actual_wakeup_tick - now) <
	    (uint32_t)APP_SUSPEND_MIN_TIME_US * SYSTEM_TIMER_TICK_1US) {
		return false;
	}

	/* Short suspend: keep ZB (baseband) powered */
	pm_set_suspend_power_cfg(FLD_PD_ZB_EN, 1);

    #if ALG_KEYSCAN_APP_FUN_ENABLE
    ks_pwm_mode_disable();
    #endif

	if (cpu_sleep_wakeup_32k_rc(SUSPEND_MODE, PM_WAKEUP_TIMER | PM_WAKEUP_PAD,
		             actual_wakeup_tick) != STATUS_GPIO_ERR_NO_ENTER_PM) {
		result = true;
	}

    g_p24g_pm_info.suspend_allowed = false;

    #if ALG_KEYSCAN_APP_FUN_ENABLE
    ks_pwm_mode_enable();
    key_scan();
    #endif

	return result;
}

_attribute_ram_code_sec_ void app_usb_sleep(void)
{
    k_sleep(K_MSEC(APP_USB_SLEEP_MS));
}
