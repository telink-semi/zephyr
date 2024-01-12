/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

#include "driver1_example.h"
#include "driver2_example.h"
#include "debug.h"

#define CORE_NAME "D25 core -> "

#define RESPONCE_TIMEOUT_IN_MS	K_MSEC(100)

#define SET_MILLI_SEC			1234567
#define SET_HOUR				101
#define SET_DAY 				103
#define SET_YEAR 				2023

K_THREAD_STACK_DEFINE(test_thread1_stack, 2048);
static struct k_thread test_thread1_data;

K_THREAD_STACK_DEFINE(test_thread2_stack, 2048);
static struct k_thread test_thread2_data;

volatile bool test_thread1_status = true;
volatile bool test_thread2_status = true;

static void test_thread1_func()
{
	debug_printf(CORE_NAME "test_thread1_func: started \n");

	int i = 0;

	for(;;) {
		struct time_value req_time = {
			.mSec = SET_MILLI_SEC+i,
			.hour = SET_HOUR+i,
			.day = SET_DAY+i,
			.year = SET_YEAR+i
		};

		struct time_value_resp resp_time = driver1_set_time_test_func(&req_time);

		if (resp_time.error || (resp_time.mSecSet != req_time.mSec) ||
				(resp_time.hourSet != req_time.hour) || (resp_time.daySet != req_time.day) ||
				(resp_time.yearSet != req_time.year)) {
			debug_printf(CORE_NAME "test_thread1_func: update time INCORRECT (step = %d) \n", i);
			test_thread1_status = false;
			return;
		}

		if (i == 100) {
			i = 0;
		}
		else {
			i++;
		}

		k_usleep(2);
	}
}

static void test_thread2_func()
{
	debug_printf(CORE_NAME "test_thread2_func: started \n");

	int i = 0;

	for(;;) {
		struct time_value req_time = {
			.mSec = SET_MILLI_SEC-i,
			.hour = SET_HOUR-i,
			.day = SET_DAY-i,
			.year = SET_YEAR-i
		};

		struct time_value_resp resp_time = driver2_set_time_test_func(&req_time);

		if (resp_time.error || (resp_time.mSecSet != req_time.mSec) ||
				(resp_time.hourSet != req_time.hour) || (resp_time.daySet != req_time.day) ||
				(resp_time.yearSet != req_time.year)) {
			debug_printf(CORE_NAME "test_thread2_func: update time INCORRECT (step = %d) \n", i);
			test_thread2_status = false;
			return;
		}

		if (i == 100) {
			i = 0;
		}
		else {
			i++;
		}

		k_usleep(3);
	}
}

int main(void)
{
	debug_printf(CORE_NAME "IPC-service HOST demo started (bonded)\n");

	(void)k_thread_create(&test_thread1_data,
		test_thread1_stack, K_THREAD_STACK_SIZEOF(test_thread1_stack),
		test_thread1_func, NULL, NULL, NULL, K_PRIO_PREEMPT(9), 0, K_NO_WAIT);
	(void)k_thread_name_set(&test_thread1_data, "test_thread1");

	(void)k_thread_create(&test_thread2_data,
		test_thread2_stack, K_THREAD_STACK_SIZEOF(test_thread2_stack),
		test_thread2_func, NULL, NULL, NULL, K_PRIO_PREEMPT(10), 0, K_NO_WAIT);
	(void)k_thread_name_set(&test_thread2_data, "test_thread2");
	
	driver1_examp_init();
	driver2_examp_init();

	debug_printf(CORE_NAME "Threads starting...\n");

	k_thread_start(&test_thread1_data);
	k_thread_start(&test_thread2_data);

	for(;;) {
		k_msleep(1000);
		debug_printf(CORE_NAME "status updates: thread1 = %d, thread2 = %d\n", test_thread1_status, test_thread2_status);
	}

	return 0;
}
