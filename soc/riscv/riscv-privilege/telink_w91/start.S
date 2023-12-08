/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define NDS_MCACHE_CTL               0x7ca
#define NDS_MMISC_CTL                0x7d0

#include <zephyr/toolchain.h>

	.option push
	.option norelax

/* exports */
GTEXT(entry)

SECTION_FUNC(init, init)
	.org 0x0
entry:

	j start

	.org 0x20
	.word ('T' << 24 | 'L' << 16 | 'N' << 8 | 'K')

#if CONFIG_SOC_RISCV_TELINK_W91
	.org 0x26
#if defined(CONFIG_TELINK_W91_2_WIRE_SPI_ENABLE) && CONFIG_TELINK_W91_2_WIRE_SPI_ENABLE
	.short (0x173b)                 #/ 2 - wire SPI interface
#else
	.short (0x65eb)                 #/ 4 - wire SPI interface
#endif

#endif

	.align 2

start:

#if defined(CONFIG_TELINK_W91_USB_SWI_DISABLE) && CONFIG_TELINK_W91_USB_SWI_DISABLE
	/* USB SWI disable: 0x80100c01 <- 0x40 */
	lui    t0, 0x80100
	addi   t0, t0, 0x700
	li     t1, 0x40
	sb     t1, 0x501(t0)
#endif

	/* Enable I/D-Cache */
	csrr   t0, NDS_MCACHE_CTL
	ori    t0, t0, 1                #/ I-Cache
	ori    t0, t0, 2                #/ D-Cache
	csrw   NDS_MCACHE_CTL, t0
	fence.i

	/* Enable misaligned access and non-blocking load */
	li     t0, (1 << 8) | (1 << 6)
	csrs   NDS_MMISC_CTL, t0

_RETENTION_RESET_INIT:
	la     t1, _RETENTION_RESET_LMA_START
	la     t2, _RETENTION_RESET_VMA_START
	la     t3, _RETENTION_RESET_VMA_END
_RETENTION_RESET_BEGIN:
	bleu   t3, t2, _ZERO_AES
	lw     t0, 0(t1)
	sw     t0, 0(t2)
	addi   t1, t1, 4
	addi   t2, t2, 4
	j      _RETENTION_RESET_BEGIN

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
	bleu   t3, t2, _RAMCODE_RET_INIT
	lw     t0, 0(t1)
	sw     t0, 0(t2)
	addi   t1, t1, 4
	addi   t2, t2, 4
	j      _RETENTION_DATA_INIT_BEGIN

_RAMCODE_RET_INIT:
	la     t1, _RAMCODE_RET_LMA_START
	la     t2, _RAMCODE_RET_VMA_START
	la     t3, _RAMCODE_RET_VMA_END
_RAMCODE_RET_INIT_BEGIN:
	bleu   t3, t2, _RAMCODE_INIT
	lw     t0, 0(t1)
	sw     t0, 0(t2)
	addi   t1, t1, 4
	addi   t2, t2, 4
	j      _RAMCODE_RET_INIT_BEGIN

_RAMCODE_INIT:
	la     t1, _RAMCODE_LMA_START
	la     t2, _RAMCODE_VMA_START
	la     t3, _RAMCODE_VMA_END
_RAMCODE_INIT_BEGIN:
	bleu   t3, t2, _START
	lw     t0, 0(t1)
	sw     t0, 0(t2)
	addi   t1, t1, 4
	addi   t2, t2, 4
	j      _RAMCODE_INIT_BEGIN

_START:
	j      __start

	.option pop
