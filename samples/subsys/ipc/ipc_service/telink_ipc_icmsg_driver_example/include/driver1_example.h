/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef DRIVER1_EXAMPLE_H
#define DRIVER1_EXAMPLE_H

#include <stdint.h>

struct time_value {
	uint32_t mSec;
	uint8_t hour;
	uint16_t day;
	uint16_t year;
};

struct time_value_resp {
	int error;
	uint32_t mSecSet;
	uint8_t hourSet;
	uint16_t daySet;
	uint16_t yearSet;
};

void driver1_examp_init(void);
struct time_value_resp driver1_set_time_test_func(struct time_value *time);

#endif /* DRIVER1_EXAMPLE_H */
