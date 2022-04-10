/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr.h>
#include <device.h>
#include <init.h>
#include <pm/pm.h>
#include <pm/device.h>
#include <pm/policy.h>
#include <soc.h>


#define SLEEP_S 2U

#define STACK_SIZE 500
#define PRIORITY 5

void entry_point(int unused1, int unused2, int unused3)
{
    while (1) {
		printk("System time: %lli ms \n", k_uptime_get());
		printk("Sleeping %u s\n", SLEEP_S);
 		k_sleep(K_SECONDS(SLEEP_S));
		printk("WakeUp system time: %lli ms\n", k_uptime_get());
    }

    /* thread terminates at end of entry point function */
}


K_THREAD_DEFINE(tid, STACK_SIZE,
                entry_point, NULL, NULL, NULL,
                PRIORITY, 0, 0);