/*
 * Copyright (c) 2021 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr/toolchain.h>


#define NDS_MCACHE_CTL               0x7ca
#define NDS_MMISC_CTL                0x7d0


#if defined(CONFIG_RISCV_GP)
.option push
.option norelax
#endif

/* exports */
GTEXT(entry)

SECTION_FUNC(init, init)
.org 0x0
entry:

	j start

.org 0x20
	.word('T' << 24 | 'L' << 16 | 'N' << 8 | 'K')

.org 0x26
	.short (0x173B)

.align 2

start:

	/* Enable I/D-Cache */
	csrr t0,  NDS_MCACHE_CTL
	ori t0,  t0,  1        #/ I - Cache
	ori t0,  t0,  2        #/ D - Cache
	csrw NDS_MCACHE_CTL,  t0
	fence.i

	/* Enable misaligned access and non-blocking load */
	li t0, (1 << 8) | (1 << 6)
	csrs NDS_MMISC_CTL, t0

#if defined(CONFIG_BOARD_TLSR9518ADK80D)
_ZERO_AES:
	lui    t0, 0
	la     t2, _AES_DATA_VMA_START
	la     t3, _AES_DATA_VMA_END
_ZERO_AES_BEGIN:
	bleu   t3, t2, _RETENTION_DATA_INIT
	sw     t0, 0(t2)
	addi   t2, t2, 4
	j      _ZERO_AES_BEGIN

_RETENTION_DATA_INIT:
	la     t1, _RETENTION_DATA_LMA_START
	la     t2, _RETENTION_DATA_VMA_START
	la     t3, _RETENTION_DATA_VMA_END
_RETENTION_DATA_INIT_BEGIN:
	bleu   t3, t2, _RAMCODE_INIT
	lw     t0, 0(t1)
	sw     t0, 0(t2)
	addi   t1, t1, 4
	addi   t2, t2, 4
	j      _RETENTION_DATA_INIT_BEGIN
#endif

#if defined(CONFIG_BOARD_TLSR9518ADK80D_RETENTION) && defined(CONFIG_PM)
_RETENTION_RESET_INIT:
	la t1, _RETENTION_RESET_LMA_START
	la t2, _RETENTION_RESET_VMA_START
	la t3, _RETENTION_RESET_VMA_END
_RETENTION_RESET_BEGIN:
	bleu t3, t2, _RAMCODE_INIT
	lw t0, 0 (t1)
	sw t0, 0 (t2)
	addi t1, t1, 4
	addi t2, t2, 4
	j _RETENTION_RESET_BEGIN
#endif

_RAMCODE_INIT:
	la t1, _RAMCODE_LMA_START
	la t2, _RAMCODE_VMA_START
	la t3, _RAMCODE_VMA_END
_RAMCODE_INIT_BEGIN:
	bleu t3, t2, _START
	lw t0, 0 (t1)
	sw t0, 0 (t2)
	addi t1, t1, 4
	addi t2, t2, 4
	j _RAMCODE_INIT_BEGIN

_START:
	j __start

#if defined(CONFIG_RISCV_GP)
.option pop
#endif


#if defined(CONFIG_BOARD_TLSR9518ADK80D_RETENTION) && defined(CONFIG_PM)

#if defined(CONFIG_RISCV_GP)
.option push
.option norelax
#endif

/* exports */
GTEXT(retention_entry)

SECTION_FUNC(retention_reset, retention_reset)
.org 0x0
retention_entry:
/* Decide whether this is an NMI or cold reset */
	j retention_start

.org 0x22
retention_start:
#if defined(CONFIG_RISCV_GP)
	/* Initialize global pointer */
	la gp, __global_pointer$
	.option pop
#endif

	/* Enable I/D-Cache */
	csrr t0,  NDS_MCACHE_CTL
	ori t0,  t0,  1        #/ I - Cache
	ori t0,  t0,  2        #/ D - Cache
	csrw NDS_MCACHE_CTL,  t0
	fence.i

	/* Enable misaligned access and non-blocking load */
	li t0, (1 << 8) | (1 << 6)
	csrs NDS_MMISC_CTL, t0

	/* flash wakeup */
_WAKEUP_FLASH:
	lui t0, 0x80140
	li t1, 0xff
	li t2, 0x0
	li t3, 0xab
	sb t1, 0x329 (t0)           #/ mspi ie enable   : 0x140329 : 0x1f
	sb t2, 0x101 (t0)           #/ cs_low           : 0x140101 : 0x00
	sb t3, 0x100 (t0)           #/ wakeup_cmd       : 0x140100 : 0xab
_MSPI_WAIT:
	lui t0, 0x80140
	lb t2, 0x102 (t0)           #/ read reg_mspi_status FLD_MSPI_BUSY(bit0)
	li t3, 0x1
	li t4, 0x10
	beq t3, t2, _MSPI_WAIT
	sb t4, 0x101 (t0)          #/ cs_high           : 0x140101 : 0x10
	/* efuse load need delay about 18us */
	li t0, 0
	li t1, 226
_WAIT_EFUSE_LOAD_FINISH:
	addi t0, t0, 1
	bgeu t1, t0, _WAIT_EFUSE_LOAD_FINISH

_MULTI_ADDRESS_BEGIN:
	lui t0, 0x80140
	la t1, g_pm_multi_addr
	lw t2, 0 (t1)
	sw t2, 0x104 (t0)           #/ g_pm_multi_addr->0x80140104

_IRAMCODE_INIT:
	la t1, _RAMCODE_LMA_START
	la t2, _RAMCODE_VMA_START
	la t3, _RAMCODE_VMA_END
_IRAMCODE_INIT_BEGIN:
	bleu t3, t2, _RESTORE
	lw t0, 0 (t1)
	sw t0, 0 (t2)
	addi t1, t1, 4
	addi t2, t2, 4
	j _IRAMCODE_INIT_BEGIN

_RESTORE:
	tail tl_sleep_context_restore

#endif
