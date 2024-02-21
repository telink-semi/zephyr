/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

static void led_init(void)
{
	__asm__("lui     t0, 0x80140\n\t"
		"li      t2, 0xef\n\t"
		"li      t1, 0x10\n\t"
		"addi    t0, t0, 0x600\n\t"
		"addi    t0, t0, 0x600\n\t"
		"sb      t2, 0x12(t0)\n\t" // [0x80140c12] PB OEN = 0xef
		"sb      t1, 0x1c(t0)"     // [0x80140c1c] PB Output Set = 0x10
	);
}

static void led_toggle(void)
{
	__asm__("lui     t0, 0x80140\n\t"
		"li      t1, 0x10\n\t"
		"addi    t0, t0, 0x600\n\t"
		"addi    t0, t0, 0x600\n\t"
		"sb      t1, 0x1e(t0)" // [0x80140c1e]PB Output Toggle = 0x10
	);
}

int main(void)
{
	uint32_t i;

	led_init();

	for (;;) {
		for (i = 0; i < 5000000; i++) {
			__asm__("nop");
		}
		led_toggle();
	}

	return 0;
}
