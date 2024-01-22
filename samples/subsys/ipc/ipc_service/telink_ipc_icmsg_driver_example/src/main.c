/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

#include "driver1_example.h"
#include "driver2_example.h"
#include "debug.h"

#define SET_MILLI_SEC			1234567
#define SET_HOUR				101
#define SET_DAY					103
#define SET_YEAR				2023

K_THREAD_STACK_DEFINE(test_thread1_stack, 2048);
static struct k_thread test_thread1_data;

K_THREAD_STACK_DEFINE(test_thread2_stack, 2048);
static struct k_thread test_thread2_data;

volatile uint32_t thread1_comm_failed_cnt;
volatile uint32_t thread2_comm_failed_cnt;

static void test_thread1_func(void *p1, void *p2, void *p3)
{
	debug_printf("D25-> %s: started\n", __func__);

	int i = 0;

	for (;;) {
		struct time_value req_time = {
			.mSec = SET_MILLI_SEC+i,
			.hour = SET_HOUR+i,
			.day = SET_DAY+i,
			.year = SET_YEAR+i
		};

		struct time_value_resp resp_time = driver1_set_time_test_func(&req_time);

		if (resp_time.error || (resp_time.mSecSet != req_time.mSec) ||
				(resp_time.hourSet != req_time.hour) ||
				(resp_time.daySet != req_time.day) ||
				(resp_time.yearSet != req_time.year)) {
			thread1_comm_failed_cnt++;
		}

		if (i == 100) {
			i = 0;
		} else {
			i++;
		}

		k_msleep(2);
	}
}

static void test_thread2_func(void *p1, void *p2, void *p3)
{
	debug_printf("D25-> %s: started\n", __func__);

	int i = 0;

	for (;;) {
		struct time_value req_time = {
			.mSec = SET_MILLI_SEC-i,
			.hour = SET_HOUR-i,
			.day = SET_DAY-i,
			.year = SET_YEAR-i
		};

		struct time_value_resp resp_time = driver2_set_time_test_func(&req_time);

		if (resp_time.error || (resp_time.mSecSet != req_time.mSec) ||
				(resp_time.hourSet != req_time.hour) ||
				(resp_time.daySet != req_time.day) ||
				(resp_time.yearSet != req_time.year)) {
			thread2_comm_failed_cnt++;
		}

		if (i == 100) {
			i = 0;
		} else {
			i++;
		}

		k_msleep(6);
	}
}

int main(void)
{
	debug_printf("D25-> IPC-service HOST demo started (bonded)\n");

	(void)k_thread_create(&test_thread1_data,
		test_thread1_stack, K_THREAD_STACK_SIZEOF(test_thread1_stack),
		test_thread1_func, NULL, NULL, NULL, K_PRIO_PREEMPT(2), 0, K_NO_WAIT);
	(void)k_thread_name_set(&test_thread1_data, "test_thread1");

	(void)k_thread_create(&test_thread2_data,
		test_thread2_stack, K_THREAD_STACK_SIZEOF(test_thread2_stack),
		test_thread2_func, NULL, NULL, NULL, K_PRIO_PREEMPT(1), 0, K_NO_WAIT);
	(void)k_thread_name_set(&test_thread2_data, "test_thread2");

	driver1_examp_init();
	driver2_examp_init();

	debug_printf("D25-> Threads starting...\n");

	k_thread_start(&test_thread1_data);
	k_thread_start(&test_thread2_data);

	for (;;) {
		k_msleep(1000);
		debug_printf("D25-> t1_fail_cnt = %lu, t2_fail_cnt = %lu\n",
				thread1_comm_failed_cnt, thread2_comm_failed_cnt);
	}

	return 0;
}
