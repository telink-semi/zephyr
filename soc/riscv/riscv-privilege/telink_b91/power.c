/*
 * Copyright (c) 2021 Fabio Baltieri
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr.h>
#include <pm/pm.h>
#include <soc.h>
#include <init.h>
#include <sys/printk.h>
#include <pm.h>
#include <ext_driver/ext_pm.h>
#include <stimer.h>

#include <logging/log.h>
LOG_MODULE_DECLARE(soc, CONFIG_SOC_LOG_LEVEL);

#define CYC_PER_TICK ((uint32_t)((uint64_t) (sys_clock_hw_cycles_per_sec()			 \
					     >> CONFIG_RISCV_MACHINE_TIMER_SYSTEM_CLOCK_DIVIDER) \
				 / (uint64_t)CONFIG_SYS_CLOCK_TICKS_PER_SEC))

/* Invoke Low Power/System Off specific Tasks */
static uint64_t get_hart_mtimecmp(void)
{
	return RISCV_MTIMECMP_BASE + (_current_cpu->id * 8);
}

static uint64_t get_mtime_compare(void)
{
    volatile uint32_t *r = (uint32_t *)(uint32_t)get_hart_mtimecmp();
    uint64_t time = (((uint64_t)r[1])<<32) | r[0];
    return time;
}

static uint64_t get_mtime(void)
{
    volatile uint32_t *r = (uint32_t *)RISCV_MTIME_BASE;
    uint64_t time = (((uint64_t)r[1])<<32) | r[0];
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

__weak void pm_state_set(enum pm_state state, uint8_t substate_id)
{
	ARG_UNUSED(substate_id);

    uint32_t sleep_time = (get_mtime_compare() - get_mtime()) / CYC_PER_TICK;
    uint64_t new_mtime = get_mtime_compare();

	switch (state) {
	case PM_STATE_SUSPEND_TO_IDLE:        
        if (sleep_time > 0)
        {
            cpu_sleep_wakeup_32k_rc(SUSPEND_MODE, PM_WAKEUP_TIMER | PM_WAKEUP_PAD, stimer_get_tick() + sleep_time*SYSTEM_TIMER_TICK_1MS);  //SUSPEND
            set_mtime(new_mtime);
        }
		break;
	case PM_STATE_SOFT_OFF:
		if (sleep_time > 0)
        {
        	cpu_sleep_wakeup_32k_rc(DEEPSLEEP_MODE_RET_SRAM_LOW64K, PM_WAKEUP_TIMER | PM_WAKEUP_PAD, stimer_get_tick() + sleep_time*SYSTEM_TIMER_TICK_1MS);  //Deep Sleep
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
   // printk("PMState Exit\n");
	switch (state) {
	case PM_STATE_SUSPEND_TO_IDLE:

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

