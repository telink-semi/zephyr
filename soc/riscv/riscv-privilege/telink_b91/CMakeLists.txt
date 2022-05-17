# Copyright (c) 2021 Telink Semiconductor
# SPDX-License-Identifier: Apache-2.0

zephyr_sources(
	start.S
	soc_irq.S
	soc.c
	soc_hw.c
)

zephyr_library_sources_ifdef(CONFIG_PM
	power.c
	tl_sleep.c
)

zephyr_library_sources_ifdef(CONFIG_BOARD_TLSR9518ADK80D_RETENTION
	tl_sleep_context.S
)

# Force using BFD-LD
zephyr_ld_options(-fuse-ld=bfd)

# Set compile options
zephyr_compile_options_ifdef(CONFIG_TELINK_B91_HWDSP -mext-dsp)
zephyr_compile_options_ifndef(CONFIG_RISCV_GP -mno-relax)
