/*
 * Copyright (c) 2021 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#ifndef __TL_SLEEP_H
#define __TL_SLEEP_H

#include <stdint.h>

extern const uint32_t tl_sleep_tick_hz;

#if defined(CONFIG_BOARD_TLSR9518ADK80D_RETENTION)
void tl_sleep_deep(uint32_t duration);
#endif
void tl_sleep_suspend(uint32_t duration);
void tl_sleep_reset(uint32_t duration);
void tl_sleep_save_tick(void);
uint32_t tl_sleep_get_diff(void);

#endif
