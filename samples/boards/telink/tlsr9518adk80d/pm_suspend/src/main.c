/*
 * Copyright (c) 2022 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr.h>

#define SLEEP_S     (2u)
#define PRIORITY    (5u)
#define STACK_SIZE  (500u)

void entry_point(int unused1, int unused2, int unused3)
{
	while (1) {
		printk("System time: %lli ms\n", k_uptime_get());
		printk("Sleeping %u s\n", SLEEP_S);
		k_sleep(K_SECONDS(SLEEP_S));
		printk("WakeUp system time: %lli ms\n", k_uptime_get());
	}
}

K_THREAD_DEFINE(tid, STACK_SIZE,
		entry_point, NULL, NULL, NULL,
		PRIORITY, 0, 0);
