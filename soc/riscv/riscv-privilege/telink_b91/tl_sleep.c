/*
 * Copyright (c) 2021 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include "tl_sleep.h"
#include <stdbool.h>

#include "pm.h"
#include "stimer.h"

const uint32_t tl_sleep_tick_hz = SYSTEM_TIMER_TICK_1S;

static uint32_t tl_sleep_tick = 0;

#if defined(CONFIG_BOARD_TLSR9518ADK80D_RETENTION)
void tl_sleep_default_handler(void)
{
}

void tl_sleep_on_enter(void) __attribute__((weak, alias("tl_sleep_default_handler")));
void tl_sleep_on_leave(void) __attribute__((weak, alias("tl_sleep_default_handler")));
void tl_sleep_on_error(void) __attribute__((weak, alias("tl_sleep_default_handler")));


void tl_sleep_deep(uint32_t duration)
{
	static volatile bool tl_sleep_retention = false;

	extern void tl_sleep_context_save(void);

	tl_sleep_context_save();
	if (!tl_sleep_retention) {
		tl_sleep_on_enter();
		tl_sleep_retention = true;
		(void)pm_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW64K, PM_WAKEUP_TIMER, PM_TICK_STIMER_16M, tl_sleep_tick + duration);
		tl_sleep_on_error();
		tl_sleep_retention = false;
	} else {
		tl_sleep_on_leave();
		tl_sleep_retention = false;
	}
}
#endif

void tl_sleep_suspend(uint32_t duration) {

	(void)pm_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER, PM_TICK_STIMER_16M, tl_sleep_tick + duration);
}

void tl_sleep_reset(uint32_t duration) {

	(void)pm_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_TIMER, PM_TICK_STIMER_16M, tl_sleep_tick + duration);
}

void tl_sleep_save_tick(void)
{
	tl_sleep_tick = stimer_get_tick();
}

uint32_t tl_sleep_get_diff(void)
{
	return stimer_get_tick() - tl_sleep_tick;
}
