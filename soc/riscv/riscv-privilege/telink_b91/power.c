/*
 * Copyright (c) 2021 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <pm/pm.h>
#include "tl_sleep.h"
#include "soc_hw.h"

#define mticks_to_systicks(mticks)      (((uint64_t)(mticks) * tl_sleep_tick_hz) / CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC)
#define systicks_to_mticks(sticks)      (((uint64_t)(sticks) * CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC) / tl_sleep_tick_hz)
#define us_to_systicks(us)              (((uint64_t)(us) * tl_sleep_tick_hz) / 1000000)

#define SYSTICKS_MAX_SLEEP              0xe0000000
#define SYSTICKS_MIN_SLEEP              18352


static uint64_t get_mtime_compare(void)
{
	return *(volatile uint64_t *)(RISCV_MTIMECMP_BASE + (_current_cpu->id * sizeof(uint64_t)));
}

#if defined(CONFIG_BOARD_TLSR9518ADK80D_RETENTION)
static void set_mtime_compare(uint64_t time_cmp)
{
	volatile uint32_t *const rl = (volatile uint32_t *const)(RISCV_MTIMECMP_BASE + (_current_cpu->id * sizeof(uint64_t)));
	volatile uint32_t *const rh = (volatile uint32_t *const)(RISCV_MTIMECMP_BASE + (_current_cpu->id * sizeof(uint64_t)) + sizeof(uint32_t));

	*rh = (uint32_t)-1;
	*rl = (uint32_t)(time_cmp);
	*rh = (uint32_t)(time_cmp >> 32);
}
#endif

static uint64_t get_mtime(void)
{
	const volatile uint32_t *const rl = (const volatile uint32_t *const)RISCV_MTIME_BASE;
	const volatile uint32_t *const rh = (const volatile uint32_t *const)(RISCV_MTIME_BASE + sizeof(uint32_t));
	uint32_t mtime_l, mtime_h;

	do{
		mtime_h = *rh;
		mtime_l = *rl;
	} while(mtime_h != *rh);
	return (((uint64_t)mtime_h) << 32) | mtime_l;
}

static void set_mtime(uint64_t time)
{
	volatile uint32_t *const rl = (volatile uint32_t *const)RISCV_MTIME_BASE;
	volatile uint32_t *const rh = (volatile uint32_t *const)(RISCV_MTIME_BASE + sizeof(uint32_t));

	*rl = 0;
	*rh = (uint32_t)(time >> 32);
	*rl = (uint32_t)time;
}

#if defined(CONFIG_BOARD_TLSR9518ADK80D_RETENTION) && defined(CONFIG_PM_DEVICE)
bool b91_deep_sleep_retention = false;
#endif

__weak void pm_state_set(enum pm_state state, uint8_t substate_id)
{
	ARG_UNUSED(substate_id);

	tl_sleep_save_tick();

	uint64_t mtick = get_mtime();
	uint64_t mcompare = get_mtime_compare();
	uint64_t systicks_sleep_timeout = mticks_to_systicks(mcompare - mtick);

	if (systicks_sleep_timeout > SYSTICKS_MAX_SLEEP) {
		systicks_sleep_timeout = SYSTICKS_MAX_SLEEP;
	}

	if (systicks_sleep_timeout >= SYSTICKS_MIN_SLEEP) {
		switch (state) {
		case PM_STATE_SUSPEND_TO_IDLE:
#if defined(CONFIG_BOARD_TLSR9518ADK80D_RETENTION)
			if (CONFIG_B91_RETENTION_THRESHOLD_US && systicks_sleep_timeout > us_to_systicks(CONFIG_B91_RETENTION_THRESHOLD_US)) {
				tl_sleep_deep(systicks_sleep_timeout);
				set_mtime_compare(mcompare);
			} else {
				tl_sleep_suspend(systicks_sleep_timeout);
			}
#else
			tl_sleep_suspend(systicks_sleep_timeout);
#endif
			break;
		case PM_STATE_SOFT_OFF:
			tl_sleep_reset(systicks_sleep_timeout);
			break;
		default:
			break;
		}

		mtick += systicks_to_mticks(tl_sleep_get_diff());
		set_mtime(mtick);
	}
}


__weak void pm_state_exit_post_ops(enum pm_state state, uint8_t substate_id)
{
	ARG_UNUSED(state);
	ARG_UNUSED(substate_id);

#if defined(CONFIG_BOARD_TLSR9518ADK80D_RETENTION) && defined(CONFIG_PM_DEVICE)
	b91_deep_sleep_retention = false;
#endif
	irq_unlock(MSTATUS_IEN);
}

#if defined(CONFIG_BOARD_TLSR9518ADK80D_RETENTION)
void tl_sleep_on_leave(void)
{
#if	defined(CONFIG_PM_DEVICE)
	b91_deep_sleep_retention = true;
#endif
	soc_b91_up();
}
#endif
