/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef DRIVER2_EXAMPLE_H
#define DRIVER2_EXAMPLE_H

#include <stdint.h>

#include "driver1_example.h"

void driver2_examp_init(void);
struct time_value_resp driver2_set_time_test_func(struct time_value *time);

#endif /* DRIVER2_EXAMPLE_H */
