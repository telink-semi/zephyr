#
# Copyright (c) 2023 Sendrato
#
# SPDX-License-Identifier: Apache-2.0
#

if (CONFIG_NT3H2X11)
    zephyr_sources(nt3h2x11.c)
endif()

if (CONFIG_ST25DVXXKC)
    zephyr_sources(st25dv.c)
    add_subdirectory(st25dvxxkc)
endif()
