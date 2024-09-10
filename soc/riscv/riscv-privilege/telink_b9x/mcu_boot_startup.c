/*
 * Copyright (c) 2024 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <bootutil/bootutil_log.h>
BOOT_LOG_MODULE_REGISTER(telink_b9x_mcuboot);

static bool telink_b9x_mcu_boot_startup(void);

void __wrap_main(void)
{
	if (telink_b9x_mcu_boot_startup()) {
		extern void __real_main(void);

		__real_main();
	}
}

/* Vendor specific code during MCUBoot startup */
bool telink_b9x_mcu_boot_startup(void)
{
	bool result = true; /* run MCUBoot main */

	BOOT_LOG_INF("Telink B9x MCUBoot on early boot");

	return result;
}
