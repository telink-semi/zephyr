/*
 * Copyright (c) 2021 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc_hw.h"
#include "sys.h"
#include <zephyr/device.h>

/* Software reset defines */
#define reg_reset                   REG_ADDR8(0x1401ef)
#define SOFT_RESET                  0x20u


/**
 * @brief Perform basic initialization at boot.
 *
 * @return 0
 */
static int soc_b91_init(const struct device *arg)
{
	ARG_UNUSED(arg);

	soc_b91_up();

	return 0;
}

/**
 * @brief Reset the system.
 */
void sys_arch_reboot(int type)
{
	ARG_UNUSED(type);

	reg_reset = SOFT_RESET;
}

SYS_INIT(soc_b91_init, PRE_KERNEL_1, 0);
