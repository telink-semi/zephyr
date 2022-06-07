/*
 * Copyright (c) 2022 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <pm/pm.h>

#include <stimer.h>
#include <ext_driver/ext_pm.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(soc, CONFIG_SOC_LOG_LEVEL);

static uint32_t tl_sleep_tick = 0;

#define mticks_to_systicks(mticks)      (((mticks) * SYSTEM_TIMER_TICK_1S) / (sys_clock_hw_cycles_per_sec() >> CONFIG_RISCV_MACHINE_TIMER_SYSTEM_CLOCK_DIVIDER))
#define systicks_to_mticks(sticks)      (((uint64_t)(sticks) * (sys_clock_hw_cycles_per_sec() >> CONFIG_RISCV_MACHINE_TIMER_SYSTEM_CLOCK_DIVIDER)) / SYSTEM_TIMER_TICK_1S)

#define SYSTICKS_MAX_SLEEP              0xe0000000
#define SYSTICKS_MIN_SLEEP              18352


/**
 * @brief This define converts Machine Timer ticks to B91 System Timer ticks.
 */
#define MTIME_TO_STIME_SCALE   (SYSTEM_TIMER_TICK_1S / CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC)


/**
 * @brief Get Machine Timer Compare value.
 */
static uint64_t get_mtime_compare(void)
{
	return *(volatile uint64_t *)(RISCV_MTIMECMP_BASE + (_current_cpu->id * sizeof(uint64_t)));
}

/**
 * @brief Get Machine Timer value.
 */
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

/**
 * @brief Set Machine Timer value.
 */
static void set_mtime(uint64_t time)
{
	volatile uint32_t *const rl = (volatile uint32_t *const)RISCV_MTIME_BASE;
	volatile uint32_t *const rh = (volatile uint32_t *const)(RISCV_MTIME_BASE + sizeof(uint32_t));

	*rl = 0;
	*rh = (uint32_t)(time >> 32);
	*rl = (uint32_t)time;
}

/**
 * @brief Set Machine Timer Compare Register value.
 */
static void set_mtime_compare(uint64_t time_cmp)
{
	volatile uint32_t *const rl = (volatile uint32_t *const)(RISCV_MTIMECMP_BASE + (_current_cpu->id * sizeof(uint64_t)));
	volatile uint32_t *const rh = (volatile uint32_t *const)(RISCV_MTIMECMP_BASE + (_current_cpu->id * sizeof(uint64_t)) + sizeof(uint32_t));

	*rh = (uint32_t)-1;
	*rl = (uint32_t)(time_cmp);
	*rh = (uint32_t)(time_cmp >> 32);
}

/**
 * @brief PM state set API implementation.
 */
__weak void pm_state_set(enum pm_state state, uint8_t substate_id)
{
	ARG_UNUSED(substate_id);
	tl_sleep_tick = stimer_get_tick();
	uint64_t current_time = get_mtime();
	uint64_t wakeup_time = get_mtime_compare();
	uint64_t stimer_sleep_ticks = (wakeup_time - current_time) * MTIME_TO_STIME_SCALE;

	switch (state) {
	case PM_STATE_SUSPEND_TO_IDLE:
		/* 
		 * Check for endless sleep.
		 * This limit is refferring to z_get_next_timeout_expiry() function, which cab set
		 * the max timeout (K_FOREVER (-1)) to INT_MAX
		 */
		if (wakeup_time >= INT_MAX) {
			/* 
			 * Currently the PIN Wakeup is not implemented.
			 * The k_cpu_idle is used for endless loop.
			 * The folllowing function must be enabled after PIN WAKEUP is implemented:
			 * cpu_sleep_wakeup_32k_rc(SUSPEND_MODE, PM_WAKEUP_PAD, 0);
			 */
			k_cpu_idle();
		} else if (wakeup_time > current_time) {
			/* Enter suspend mode */
			cpu_sleep_wakeup_32k_rc(SUSPEND_MODE, PM_WAKEUP_TIMER,
									stimer_get_tick() + stimer_sleep_ticks);

			/* Update Machine Timer value after resume since the timer does not tick during suspend */
		} else {
			LOG_DBG("Sleep Time = 0 or less\n");
		}
		break;

	case PM_STATE_SOFT_OFF:
		if (wakeup_time > current_time) {
			cpu_sleep_wakeup_32k_rc(DEEPSLEEP_MODE, PM_WAKEUP_TIMER | PM_WAKEUP_PAD,
									stimer_get_tick() + stimer_sleep_ticks);
		} else {
			LOG_DBG("Sleep Time = 0 or less\n");
		}
		break;

	default:
		LOG_DBG("Unsupported power state %u", state);
		break;
	}
	current_time += systicks_to_mticks(stimer_get_tick() - tl_sleep_tick);
	set_mtime(current_time);
}

/**
 * @brief PM state exit post operations API implementation.
 */
__weak void pm_state_exit_post_ops(enum pm_state state, uint8_t substate_id)
{
	ARG_UNUSED(substate_id);

	switch (state) {
	case PM_STATE_SUSPEND_TO_IDLE:
		/* Nothing to do. */
		break;

	case PM_STATE_SOFT_OFF:
		/* Nothing to do. */
		break;

	default:
		LOG_DBG("Unsupported power substate-id %u", state);
		break;
	}

	/*
	 * System is now in active mode. Reenable interrupts which were
	 * disabled when OS started idling code.
	 */
	arch_irq_unlock(MSTATUS_IEN);
}
