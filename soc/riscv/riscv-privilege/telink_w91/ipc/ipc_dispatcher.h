/*
 * Copyright (c) 2024 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IPC_DISPATCHER_H
#define IPC_DISPATCHER_H

#include <zephyr/kernel.h>

#include <stddef.h>
#include <string.h>

/* Data types */

enum ipc_dispatcher_id {
	IPC_DISPATCHER_SYS                      = 0x0,
	IPC_DISPATCHER_UART                     = 0x100,
	IPC_DISPATCHER_GPIO                     = 0x200,
	IPC_DISPATCHER_PWM                      = 0x300,
} __attribute__((__packed__));

typedef void (*ipc_dispatcher_cb_t)(const void *data, size_t len, void *param);

/* Public APIs */

void ipc_dispatcher_add(enum ipc_dispatcher_id id, ipc_dispatcher_cb_t cb, void *param);
void ipc_dispatcher_rm(enum ipc_dispatcher_id id);
int ipc_dispatcher_send(const void *data, size_t len);

/* Macros to pack/unpack the different types */

#define IPC_DISPATCHER_PACK_FIELD(buff, field)                                 \
do {                                                                           \
	memcpy(buff, &field, sizeof(field));                                       \
	buff += sizeof(field);                                                     \
} while (0)

#define IPC_DISPATCHER_UNPACK_FIELD(buff, field)                               \
do {                                                                           \
	memcpy(&field, buff, sizeof(field));                                       \
	buff += sizeof(field);                                                     \
} while (0)

/* Macros for init ipc data in driver */

#define IPC_DISPATCHER_HOST_INIT                                               \
static K_MUTEX_DEFINE(ipc_mutex);                                              \
static K_SEM_DEFINE(ipc_resp_sem, 0, 1)

/* Macros for sending data from host device to remote */

#define IPC_DISPATCHER_HOST_SEND_DATA(name, tx_buff, rx_buff, timeout_ms)      \
do {                                                                           \
	/* Nested callback */                                                      \
	void resp_cb(const void *data, size_t len, void *param)                    \
	{                                                                          \
		unpack_##name(param, data, len);                                       \
		k_sem_give(&ipc_resp_sem);                                             \
	}                                                                          \
                                                                               \
	uint8_t packed_data[pack_##name(NULL, NULL)];                              \
	size_t packed_len = pack_##name(tx_buff, packed_data);                     \
	enum ipc_dispatcher_id id = *(enum ipc_dispatcher_id *)packed_data;        \
                                                                               \
	k_mutex_lock(&ipc_mutex, K_FOREVER);                                       \
	k_sem_reset(&ipc_resp_sem);                                                \
	ipc_dispatcher_add(id, resp_cb, rx_buff);                                  \
                                                                               \
	if (ipc_dispatcher_send(packed_data, packed_len) != packed_len) {          \
		ipc_dispatcher_rm(id);                                                 \
		k_mutex_unlock(&ipc_mutex);                                            \
		LOG_ERR("IPC data send error (id=%x)", id);                            \
		return -ECANCELED;                                                     \
	}                                                                          \
                                                                               \
	if (k_sem_take(&ipc_resp_sem,                                              \
			K_MSEC(timeout_ms))) {                                             \
		ipc_dispatcher_rm(id);                                                 \
		k_mutex_unlock(&ipc_mutex);                                            \
		LOG_ERR("IPC response timeout (id=%x)", id);                           \
		return -ETIMEDOUT;                                                     \
	}                                                                          \
                                                                               \
	ipc_dispatcher_rm(id);                                                     \
	k_mutex_unlock(&ipc_mutex);                                                \
} while (0)

/* Macros for packing data without param (only id) */

#define IPC_DISPATCHER_PACK_FUNC_WITHOUT_PARAM(name, ipc_id)                   \
static size_t pack_##name(void *unpack_data, uint8_t *pack_data)               \
{                                                                              \
	if (pack_data != NULL) {                                                   \
		enum ipc_dispatcher_id id = ipc_id;                                    \
                                                                               \
		IPC_DISPATCHER_PACK_FIELD(pack_data, id);                              \
	}                                                                          \
                                                                               \
	return sizeof(enum ipc_dispatcher_id);                                     \
}

/* Macros for unpacking data only with error param */

#define IPC_DISPATCHER_UNPACK_FUNC_ONLY_WITH_ERROR_PARAM(name)                 \
static void unpack_##name(void *unpack_data,                                   \
		const uint8_t *pack_data, size_t pack_data_len)                        \
{                                                                              \
	int *p_err = unpack_data;                                                  \
	size_t expect_len = sizeof(enum ipc_dispatcher_id) + sizeof(*p_err);       \
                                                                               \
	if (expect_len != pack_data_len) {                                         \
		*p_err = -EINVAL;                                                      \
		return;                                                                \
	}                                                                          \
                                                                               \
	pack_data += sizeof(enum ipc_dispatcher_id);                               \
	IPC_DISPATCHER_UNPACK_FIELD(pack_data, *p_err);                            \
}

#endif /* IPC_DISPATCHER_H */
