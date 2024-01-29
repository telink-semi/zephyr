/*
 * Copyright (c) 2024 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IPC_DISPATCHER_H
#define IPC_DISPATCHER_H

#include <stddef.h>
#include <string.h>

/* Data types */

enum ipc_dispatcher_id {
	IPC_DISPATCHER_SYS                      = 0x0,
	IPC_DISPATCHER_UART                     = 0x100,
	IPC_DISPATCHER_GPIO                     = 0x200,
	IPC_DISPATCHER_PWM                      = 0x300,
} __attribute__((__packed__));

typedef void (*ipc_dispatcher_cb_t)(const void *data, size_t len);

/* Public APIs */

void ipc_dispatcher_add(enum ipc_dispatcher_id id, ipc_dispatcher_cb_t cb);
void ipc_dispatcher_rm(enum ipc_dispatcher_id id);
int ipc_dispatcher_send(const void *data, size_t len);

/* Macros to pack/unpack the different types */

#define IPC_DISPATCHER_PACK_FIELD(buff, field)             \
do {                                                       \
	memcpy(buff, &field, sizeof(field));                   \
	buff += sizeof(field);                                 \
} while (0)

#define IPC_DISPATCHER_UNPACK_FIELD(buff, field)           \
do {                                                       \
	memcpy(&field, buff, sizeof(field));                   \
	buff += sizeof(field);                                 \
} while (0)

#endif /* IPC_DISPATCHER_H */
