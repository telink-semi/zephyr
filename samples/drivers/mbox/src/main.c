/*
 * Copyright (c) 2021 Carlo Caione <ccaione@baylibre.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <ipc/ipc_dispatcher.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ot_main, LOG_LEVEL_DBG);

enum ipc_dispatcher_id_sys {
	IPC_DISPATCHER_SYS_FUNC_0,
	IPC_DISPATCHER_SYS_FUNC_1,
} __attribute__((__packed__));


void ipc_dispatcher_sys_func_0(const void *data, size_t len)
{
	LOG_DBG("ipc_dispatcher_sys_func_0");
}

void ipc_dispatcher_sys_func_1(const void *data, size_t len)
{
	LOG_DBG("ipc_dispatcher_sys_func_1");
}

int main(void)
{
	LOG_INF("main process started");

	ipc_dispatcher_add(IPC_DISPATCHER_SYS + IPC_DISPATCHER_SYS_FUNC_0, ipc_dispatcher_sys_func_0);
	ipc_dispatcher_add(IPC_DISPATCHER_SYS + IPC_DISPATCHER_SYS_FUNC_1, ipc_dispatcher_sys_func_1);

	LOG_INF("ipc_dispatcher_add_elem done");

	return 0;
}
