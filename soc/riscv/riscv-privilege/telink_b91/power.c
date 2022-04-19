/*
 * Copyright (c) 2021 Telink Semiconductor
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

#define CYC_PER_TICK ((uint32_t)((uint64_t) (sys_clock_hw_cycles_per_sec()			\
					     >> CONFIG_RISCV_MACHINE_TIMER_SYSTEM_CLOCK_DIVIDER)		\
				/ (uint64_t)CONFIG_SYS_CLOCK_TICKS_PER_SEC))

#define SYS_TICK_TO_OS_TICK(tick) (((uint64_t) tick / (SYSTEM_TIMER_TICK_1S			\
				/ CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC)))


/* Invoke Low Power/System Off specific Tasks */
static uint64_t get_hart_mtimecmp(void)
{
	return RISCV_MTIMECMP_BASE + (_current_cpu->id * 8);
}

static uint64_t get_mtime_compare(void)
{
	volatile uint32_t *r = (uint32_t *)(uint32_t)get_hart_mtimecmp();
	uint64_t time = (((uint64_t)r[1]) << 32) | r[0];

	return time;
}

static uint64_t get_mtime(void)
{
	volatile uint32_t *r = (uint32_t *)RISCV_MTIME_BASE;
	uint64_t time = (((uint64_t)r[1]) << 32) | r[0];

	return time;
}

static void set_mtime(uint64_t time)
{
	volatile uint32_t *r = (uint32_t *)RISCV_MTIME_BASE;

	irq_disable(RISCV_MACHINE_TIMER_IRQ);
	r[0] = (uint32_t)time;
	r[1] = (uint32_t)(time >> 32);
	irq_enable(RISCV_MACHINE_TIMER_IRQ);
}

#define SYS_SLEEP_TIME (get_mtime_compare() - get_mtime()) / CYC_PER_TICK

__weak void pm_state_set(enum pm_state state, uint8_t substate_id)
{
	ARG_UNUSED(substate_id);

	uint64_t wakeup_time = get_mtime_compare();
	uint32_t stimer_sleep_entrance_tick = stimer_get_tick();

	switch (state) {
	case PM_STATE_SUSPEND_TO_IDLE:
		/* this limit is refferring to z_get_next_timeout_expiry() function, which is limiting */
		/* the max timeout (K_FOREVER (-1)) to INT_MAX */
		if (wakeup_time >= INT_MAX) {
			/* currently the PIN Wakeup is not implemented */
			/* The k_cpu_idle will be used for endless loop */
			/* The folllowing function must be enabled after PIN WAKEUP is implemented */
			/* cpu_sleep_wakeup_32k_rc(SUSPEND_MODE, PM_WAKEUP_PAD, 0); */
			k_cpu_idle();
		} else if (SYS_SLEEP_TIME > 0 && wakeup_time < INT_MAX) {
			cpu_sleep_wakeup_32k_rc(SUSPEND_MODE, PM_WAKEUP_TIMER | PM_WAKEUP_PAD,
			stimer_get_tick() + SYS_SLEEP_TIME * SYSTEM_TIMER_TICK_1MS); /* SUSPEND */
			set_mtime(get_mtime() + SYS_TICK_TO_OS_TICK((stimer_get_tick()
			- stimer_sleep_entrance_tick)));
			arch_irq_unlock(MSTATUS_IEN);
		} else {
			LOG_DBG("Sleep Time = 0 or less\n");
		}
		break;
	case PM_STATE_SOFT_OFF:
		if (SYS_SLEEP_TIME > 0) {
			cpu_sleep_wakeup_32k_rc(DEEPSLEEP_MODE, PM_WAKEUP_TIMER | PM_WAKEUP_PAD,
			stimer_get_tick() + SYS_SLEEP_TIME * SYSTEM_TIMER_TICK_1MS); /* Deep Sleep */
		}
		break;
	default:
		LOG_DBG("Unsupported power state %u", state);
		break;
	}
}

/* Handle SOC specific activity after Low Power Mode Exit */
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
	irq_unlock(0);
}
