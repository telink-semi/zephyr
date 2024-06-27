/*
 * Copyright (c) 2024 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef TELINK_W91_MBEDTLS_USER_CONFIG_H
#define TELINK_W91_MBEDTLS_USER_CONFIG_H

#include <zephyr/sys/printk.h>

#define MBEDTLS_PLATFORM_PRINTF_MACRO     printk

#ifdef MBEDTLS_PLATFORM_MEMORY
#undef MBEDTLS_PLATFORM_MEMORY
#endif

#ifdef MBEDTLS_MEMORY_BUFFER_ALLOC_C
#undef MBEDTLS_MEMORY_BUFFER_ALLOC_C
#endif

#endif /* TELINK_W91_MBEDTLS_USER_CONFIG_H */
