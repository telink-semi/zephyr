/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define DEBUG_BAUDRATE                     115200

void debug_putch(char ch);
void debug_puts(const char *str);
void debug_printf(const char *format, ...);

#endif /* __DEBUG_H */
