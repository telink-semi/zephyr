/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IPC_DISPATCHER_H
#define IPC_DISPATCHER_H

#include <string.h>
#include <zephyr/kernel.h>

/* Data types */
enum __attribute__((__packed__)) ipc_dispatcher_id {
    IPC_DISPATCHER_SYS_1                    = 0,
    IPC_DISPATCHER_SYS_SET_TIME_TEST1_FUNC,

    IPC_DISPATCHER_SYS_2                    = 0x100,
    IPC_DISPATCHER_SYS_SET_TIME_TEST2_FUNC,

    IPC_DISPATCHER_UART                     = 0x200,

    IPC_DISPATCHER_GPIO                     = 0x300,

    IPC_DISPATCHER_PWM                      = 0x400,
};

typedef void (*ipc_dispatcher_cb_t)(const void *data, size_t len);

int ipc_dispatcher_start(void);
void ipc_dispatcher_add_elem(enum ipc_dispatcher_id id, ipc_dispatcher_cb_t cb);
void ipc_dispatcher_rm_elem(enum ipc_dispatcher_id id);
int ipc_dispatcher_send(const void *data, size_t len);

/* Macros for packaging the different types */
#define PACK_FIELD(buff, field)                 \
    do {                                        \
        memcpy(buff, &field, sizeof(field));    \
        buff += sizeof(field);                  \
    } while (0)

#define UNPACK_FIELD(buff, field)               \
    do {                                        \
        memcpy(&field, buff, sizeof(field));    \
        buff += sizeof(field);                  \
    } while (0)

#endif /* IPC_DISPATCHER_H */
