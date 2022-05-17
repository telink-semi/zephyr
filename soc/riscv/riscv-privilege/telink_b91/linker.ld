/*
 * Copyright (c) 2021 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @brief Linker script for the Telink B91 SoC
 */

#include <zephyr/devicetree.h>
#include <zephyr/linker/linker-tool.h>

MEMORY
{
	ROM_INIT (rx)  : ORIGIN = DT_REG_ADDR(DT_CHOSEN(zephyr_flash)), LENGTH = DT_REG_SIZE(DT_CHOSEN(zephyr_flash))
#if defined(CONFIG_BOARD_TLSR9518ADK80D)
	RAM_ILM  (rwx) : ORIGIN = DT_REG_ADDR(DT_NODELABEL(ram_ilm)), LENGTH = DT_REG_SIZE(DT_NODELABEL(ram_ilm))
#endif
#if defined(CONFIG_BOARD_TLSR9518ADK80D_RETENTION)
	RAM_ILM_BOOT (rwx) : ORIGIN = DT_REG_ADDR(DT_NODELABEL(ram_ilm_boot)), LENGTH = DT_REG_SIZE(DT_NODELABEL(ram_ilm_boot))
	RAM_ILM_N (rwx) : ORIGIN = DT_REG_ADDR(DT_NODELABEL(ram_ilm_nonretention)), LENGTH = DT_REG_SIZE(DT_NODELABEL(ram_ilm_nonretention))
#endif
}

SECTIONS
{
	SECTION_PROLOGUE(vector,,)
	{
		. = CONFIG_ROM_START_OFFSET;
		KEEP(*(.init.*))
	} GROUP_LINK_IN(ROM_INIT)
}

#include <zephyr/arch/riscv/common/linker.ld>

SECTIONS
{
#if defined(CONFIG_BOARD_TLSR9518ADK80D)
	SECTION_DATA_PROLOGUE(aes_data,,)
	{
		. = ALIGN(4);
		*(.aes_data)
		*(".aes_data.*")

		PROVIDE (_AES_DATA_VMA_END = .);
		PROVIDE (_AES_DATA_VMA_START = ADDR(aes_data));
	} GROUP_DATA_LINK_IN(RAM_ILM, ROMABLE_REGION)

	SECTION_DATA_PROLOGUE(retention_data,,)
	{
		. = ALIGN(4);
		*(.retention_data)
		*(".retention_data.*")

		PROVIDE (_RETENTION_DATA_VMA_END = .);
		PROVIDE (_RETENTION_DATA_VMA_START = ADDR(retention_data));
		PROVIDE (_RETENTION_DATA_LMA_START = LOADADDR(retention_data));
	} GROUP_DATA_LINK_IN(RAM_ILM, ROMABLE_REGION)

	SECTION_DATA_PROLOGUE(ram_code,,)
	{
		. = ALIGN(4);
		*(.ram_code)
		*(".ram_code.*")

		PROVIDE (_RAMCODE_VMA_END = .);
		PROVIDE (_RAMCODE_VMA_START = ADDR(ram_code));
		PROVIDE (_RAMCODE_LMA_START = LOADADDR(ram_code));
	} GROUP_DATA_LINK_IN(RAM_ILM, ROMABLE_REGION)
#endif
#if defined(CONFIG_BOARD_TLSR9518ADK80D_RETENTION)
	SECTION_DATA_PROLOGUE(retention_reset,,)
	{
		. = ALIGN(4);
		KEEP(*(.retention_reset.*))

		PROVIDE (_RETENTION_RESET_VMA_END = .);
		PROVIDE (_RETENTION_RESET_VMA_START = ADDR(retention_reset));
		PROVIDE (_RETENTION_RESET_LMA_START = LOADADDR(retention_reset));
	} GROUP_DATA_LINK_IN(RAM_ILM_BOOT, ROMABLE_REGION)

	SECTION_DATA_PROLOGUE(ram_code,,)
	{
		. = ALIGN(4);
		*(.ram_code)
		*(".ram_code.*")

		PROVIDE (_RAMCODE_VMA_END = .);
		PROVIDE (_RAMCODE_VMA_START = ADDR(ram_code));
		PROVIDE (_RAMCODE_LMA_START = LOADADDR(ram_code));
	} GROUP_DATA_LINK_IN(RAM_ILM_N, ROMABLE_REGION)
#endif
}
