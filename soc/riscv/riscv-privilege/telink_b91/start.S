/*
 * Copyright (c) 2021 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define NDS_MCACHE_CTL               0x7CA
#define NDS_MMISC_CTL                0x7D0

#include <zephyr/toolchain.h>

	.option push
	.option norelax
	.org 0x0

/* exports */
GTEXT(entry)

SECTION_FUNC(init, init)
entry:

	j start

	.org 0x20
	.word ('T'<<24 | 'L'<<16 | 'N'<<8 | 'K')

	.org 0x26
	.short (0x173B)

	.align 2

start:
#if defined(CONFIG_MCUBOOT)
	/* get reset reason */
    lui     t0, 0x80140
    li      t1, 0xffbbffff
    sw      t1, 0x1e0(t0)       // 0xffbbffff->0x801401e0 (reset register)

    lui     t0, 0x80140
    li      t1, 0xffbbffff
    sw      t1, 0x1e4(t0)       // 0xffbbffff->0x801401e4 (clock enable register)

    lui     t0, 0x80140
    li      t1, 0x64
    sb      t1, 0x180(t0)       // an_rst_reason->reg_ana_addr

    lui     t0, 0x80140
    li      t1, 0x01
    sb      t1, 0x183(t0)       // 1 byte->reg_ana_len

    lui     t0, 0x80140
    li      t1, 0x40
    sb      t1, 0x182(t0)       // FLD_ANA_CYC->reg_ana_ctrl

wait_rst_reason:
    lui     t0, 0x80140
    lb      t1, 0x182(t0)
    and     t1, t1, 0x80
    bnez    t1, wait_rst_reason // while FLD_ANA_BUSY in reg_ana_ctrl

    lui     t0, 0x80140
    lb      t1, 0x184(t0)       // reg_ana_data->t1 (so reset reason in t1)

    and     t1, t1, 0x02
    bnez    t1, retention_area
#endif
	/* Enable I/D-Cache */
	csrr   t0,  NDS_MCACHE_CTL
	ori    t0,  t0,  1        #/I-Cache
	ori    t0,  t0,  2        #/D-Cache
	csrw   NDS_MCACHE_CTL,  t0
	fence.i

	/* Enable misaligned access and non-blocking load */
	li t0, (1 << 8) | (1 << 6)
	csrs NDS_MMISC_CTL, t0

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
	j __start

#if defined(CONFIG_MCUBOOT)
retention_area:
	tail (CONFIG_FLASH_BASE_ADDRESS + 0x10200)
#endif

	.option pop
