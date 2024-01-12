/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @brief Linker script for the Telink W91 SoCs
 */

#include <zephyr/devicetree.h>
#include <zephyr/linker/linker-tool.h>

MEMORY
{
#if CONFIG_BOARD_TLSR9118BDK40D
	RAMILM       (rwx) : ORIGIN = DT_REG_ADDR(DT_NODELABEL(ram_ilm)),              LENGTH = DT_REG_SIZE(DT_NODELABEL(ram_ilm))
#endif /* CONFIG_BOARD_TLSR9518ADK80D */
}

#include <zephyr/arch/riscv/common/linker.ld>

#ifdef CONFIG_MCUBOOT
	ASSERT(__rom_region_size <= DT_REG_SIZE(DT_NODELABEL(boot_partition)), "boot_partition overflows")
#endif
