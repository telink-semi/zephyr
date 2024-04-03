/*
 * Copyright (c) 2024 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <zephyr/sys/atomic.h>
#include <ipc/ipc_based_driver.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(blocking_core_w91);

enum {
	IPC_DISPATCHER_BLOCKING_SET_STATE_ADDR = IPC_DISPATCHER_BLOCKING,
	IPC_DISPATCHER_BLOCKING_STOP_CORE_REQ,
};

enum {
	BLOCKING_INVALID_STATE,
	BLOCKING_INITIATED_STATE,
	BLOCKING_CORE_ACTIVE_STATE,
	BLOCKING_CORE_STOP_REQ_STATE,
	BLOCKING_CORE_STOPPED_STATE,
	BLOCKING_CORE_ACTIVE_REQ_STATE,
};

static atomic_t blocking_state = BLOCKING_INVALID_STATE;
static struct ipc_based_driver ipc_data;    /* ipc driver data part */

static struct k_thread blocking_thread_data;
K_THREAD_STACK_DEFINE(blocking_thread_stack, CONFIG_TELINK_W91_BLOCKING_CORE_THREAD_STACK_SIZE);
K_SEM_DEFINE(blocking_sem, 0, 1);

/* APIs implementation: set address of blocking state */
static size_t pack_blocking_w91_set_state_addr(uint8_t inst, void *unpack_data, uint8_t *pack_data)
{
	uint32_t *p_set_state_addr_req = unpack_data;
	size_t pack_data_len = sizeof(uint32_t) + sizeof(uint32_t);

	if (pack_data != NULL) {
		uint32_t id = IPC_DISPATCHER_MK_ID(IPC_DISPATCHER_BLOCKING_SET_STATE_ADDR, inst);

		IPC_DISPATCHER_PACK_FIELD(pack_data, id);
		IPC_DISPATCHER_PACK_FIELD(pack_data, *p_set_state_addr_req);
	}

	return pack_data_len;
}

IPC_DISPATCHER_UNPACK_FUNC_ONLY_WITH_ERROR_PARAM(blocking_w91_set_state_addr);

static int blocking_w91_set_state_addr(uint32_t addr)
{
	int err;

	IPC_DISPATCHER_HOST_SEND_DATA(&ipc_data, 0,
			blocking_w91_set_state_addr, &addr, &err,
			CONFIG_BLOCKING_CORE_TELINK_W91_IPC_RESPONSE_TIMEOUT_MS);

	return err;
}

static void blocking_w91_stop_core_req(const void *data, size_t len, void *param)
{
	if (atomic_get(&blocking_state) == BLOCKING_CORE_STOP_REQ_STATE) {
		k_sem_give(&blocking_sem);
	}
}

/* APIs implementation: core stop request event */
static void __GENERIC_SECTION(.ram_code) __attribute__((noinline)) blocking_w91_stop_core(void)
{
	unsigned int key;

	/* Convert the check state period to approximate number of "nop" instructions */
	uint32_t nop_count = CONFIG_BLOCKING_CORE_TELINK_W91_CHECK_STATE_PERIOD_US *
			(CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC / USEC_PER_SEC);

	uint32_t steps_timeout = CONFIG_BLOCKING_CORE_TELINK_W91_CORE_STOP_TIMEOUT_US /
			CONFIG_BLOCKING_CORE_TELINK_W91_CHECK_STATE_PERIOD_US;
	uint32_t steps_count = 0;

	key = irq_lock();

	atomic_set(&blocking_state, BLOCKING_CORE_STOPPED_STATE);

	while (atomic_get(&blocking_state) != BLOCKING_CORE_ACTIVE_REQ_STATE) {
		for (uint32_t i = 0; i < nop_count; i++) {
			__asm volatile("nop");
		}

		steps_count++;

		if (steps_count == steps_timeout) {
			break;
		}
	}

	atomic_set(&blocking_state, BLOCKING_CORE_ACTIVE_STATE);

	irq_unlock(key);

	if (steps_count == steps_timeout) {
		LOG_ERR("Blocking core timeout");
	}
}

static void blocking_w91_thread(void)
{
	while (1) {
		k_sem_take(&blocking_sem, K_FOREVER);

		/* Exclude the operation that causes the blocking */
		while (ipc_based_driver_get_exch_act_cnt() - 1) {
		}

		blocking_w91_stop_core();
	}
}

static int blocking_w91_init(void)
{
	int err;

	ipc_based_driver_init(&ipc_data);

	k_thread_create(&blocking_thread_data,
		blocking_thread_stack, K_THREAD_STACK_SIZEOF(blocking_thread_stack),
		blocking_w91_thread, NULL, NULL, NULL,
		CONFIG_TELINK_W91_BLOCKING_CORE_THREAD_PRIORITY, 0, K_NO_WAIT);

	ipc_dispatcher_add(IPC_DISPATCHER_MK_ID(IPC_DISPATCHER_BLOCKING_STOP_CORE_REQ, 0),
		blocking_w91_stop_core_req, NULL);

	err = blocking_w91_set_state_addr((uint32_t)&blocking_state);
	if (err < 0) {
		return err;
	}

	if (atomic_get(&blocking_state) != BLOCKING_INITIATED_STATE) {
		LOG_ERR("Incorrect state of blocking_state");
		return -EINVAL;
	}

	atomic_set(&blocking_state, BLOCKING_CORE_ACTIVE_STATE);

	return 0;
}

SYS_INIT(blocking_w91_init, POST_KERNEL, CONFIG_TELINK_W91_IPC_PRE_DRIVERS_INIT_PRIORITY);
