/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sys.h>

#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>

#include "ipc_dispatcher.h"

/* List of supported CCLK frequencies */
#define CLK_160MHZ                  160000000u

/* Check System Clock value. */
#if (DT_PROP(DT_PATH(cpus, cpu_0), clock_frequency) != CLK_160MHZ)
	#error "Unsupported clock-frequency. Supported values: 160 MHz"
#endif

/**
 * @brief Perform basic initialization at boot.
 *
 * @return 0
 */
static int soc_w91_init(void)
{
	/* Done by FreeRTOS clock_init() and clock_postinit() */
	/* hal/soc/scm2010/soc.c (hal/drivers/clk/clk-scm2010.c) */

	ipc_dispatcher_init();

	return 0;
}

/**
 * @brief Reset the system.
 */
void sys_arch_reboot(int type)
{
	ARG_UNUSED(type);

	/* TBD reset D25 core or reset via FreeRTOS WDG */
	/* reg_reset = SOFT_RESET; */
	/* writel(0x01, SYS(CORE_RESET_CTRL)); */
}

SYS_INIT(soc_w91_init, PRE_KERNEL_1, 0);
SYS_INIT(ipc_dispatcher_start, APPLICATION, 0);
/* SYS_INIT(soc_w91_check_flash, POST_KERNEL, 0); */
