/*
 * Copyright (c) 2024 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "zephyr_key_pool.h"

static void on_button_change(size_t button, bool pressed, void *context)
{
	const char *context_name = "isr";

	if (!k_is_in_isr()) {
		context_name = k_thread_name_get(k_current_get());
	}
	printk("[%s] button %u %s '%s'\n",
		context_name, button, pressed ? "pressed" : "released", (const char *)context);
}

static KEY_POOL_DEFINE(key_pool);

int main(void)
{
	if (!key_pool_init(&key_pool)) {
		printk("key_pool_init failed\n");
		return 0;
	}

	static const char test_context[] = "test context";

	key_pool_set_callback(&key_pool, on_button_change, (void *)test_context);

	printk("press any key\n");

	return 0;
}
