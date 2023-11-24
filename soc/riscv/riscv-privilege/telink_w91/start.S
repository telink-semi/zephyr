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

SECTION_FUNC(init, init)
	.org 0x0

	.word (0x31707848)	      /* magic */
	.word (0x33e90d91)        /* header crc32 */
	.org 0x14
	.word (0xa0000080)        /* start address */
	.org 0xa0

	/* Enable I/D-Cache */
	csrr   t0, NDS_MCACHE_CTL
	ori    t0, t0, 1                #/ I-Cache
	ori    t0, t0, 2                #/ D-Cache
	csrw   NDS_MCACHE_CTL, t0
	fence.i

	/* Enable misaligned access and non-blocking load */
	li     t0, (1 << 8) | (1 << 6)
	csrs   NDS_MMISC_CTL, t0



_START:
	j      __start

	.option pop
