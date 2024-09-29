/*
 * Copyright (c) 2024 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/init.h>

/* List of supported CCLK frequencies */
#define CLK_160MHZ                        160000000u

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
	return 0;
}

/**
 * @brief Reset the system.
 */
void sys_arch_reboot(int type)
{
	ARG_UNUSED(type);

	sys_write32(D25_START_ADDR, D25_RESET_VECTOR);
	sys_write32(D25_MASK, HART_RESET_CTRL);
}

SYS_INIT(soc_w91_init, PRE_KERNEL_1, 0);
