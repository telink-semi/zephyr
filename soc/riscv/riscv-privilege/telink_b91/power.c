/*
 * Copyright (c) 2022 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>

#include <soc.h>
#include <init.h>
#include <pm.h>
#include <arch/riscv/arch.h>

#include <stimer.h>
#include <pm/pm.h>
#include <ext_driver/ext_pm.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(soc, CONFIG_SOC_LOG_LEVEL);


/**
 * @brief This define converts Machine Timer ticks to B91 System Timer ticks.
 */
#define MTIME_TO_STIME_SCALE   (SYSTEM_TIMER_TICK_1S / CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC)


/**
 * @brief Get Machine Timer Compare value.
 */
static uint64_t get_mtime_compare(void)
{
	volatile uint32_t *r = (uint32_t *)RISCV_MTIMECMP_BASE;
	uint64_t time = (((uint64_t)r[1]) << 32) | r[0];

	return time;
}

/**
 * @brief Get Machine Timer value.
 */
static uint64_t get_mtime(void)
{
	volatile uint32_t *r = (uint32_t *)RISCV_MTIME_BASE;
	uint64_t time = (((uint64_t)r[1]) << 32) | r[0];

	return time;
}

/**
 * @brief Set Machine Timer value.
 */
static void set_mtime(uint64_t time)
{
	volatile uint32_t *r = (uint32_t *)RISCV_MTIME_BASE;

	irq_disable(RISCV_MACHINE_TIMER_IRQ);
	r[0] = (uint32_t)time;
	r[1] = (uint32_t)(time >> 32);
	irq_enable(RISCV_MACHINE_TIMER_IRQ);
}

/**
 * @brief PM state set API implementation.
 */
__weak void pm_state_set(enum pm_state state, uint8_t substate_id)
{
	ARG_UNUSED(substate_id);

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
			cpu_sleep_wakeup_32k_rc(SUSPEND_MODE, PM_WAKEUP_TIMER | PM_WAKEUP_PAD,
									stimer_get_tick() + stimer_sleep_ticks);

			/* Update Machine Timer value after resume since the timer does not tick during suspend */
			set_mtime(get_mtime_compare() + 1);
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
