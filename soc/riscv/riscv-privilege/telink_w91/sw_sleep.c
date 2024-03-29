/*
 * Copyright (c) 2024 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

#define ASM_NOP_CLOCK_PER_CYCLE           10u
#define ASM_NOP_CYCLES_PER_SEC            (CLK_160MHZ / ASM_NOP_CLOCK_PER_CYCLE)

void __GENERIC_SECTION(.ram_code) __attribute__((noinline)) w91_sw_delay(uint32_t ms)
{
	/* Convert the check state period to approximate number of "nop" instructions */
	uint32_t nop_count = ms * (ASM_NOP_CYCLES_PER_SEC / MSEC_PER_SEC);
	
	for (uint32_t i = 0; i < nop_count; i++) {
		__asm volatile("nop");
	}
}


