/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "driver1_example.h"
#include "ipc_dispatcher.h"

#define RESPONCE_TIMEOUT_IN_MS	K_MSEC(100)

static struct k_mutex mutex;
static K_SEM_DEFINE(responce_sem, 0, 1);

static size_t pack_set_time_test_func(struct time_value *data, uint8_t *buff)
{
	size_t len = sizeof(ipc_dispatcher_id_t) + sizeof(uint32_t) + sizeof(uint8_t) +
			sizeof(uint16_t) + sizeof(uint16_t);

	if (buff != NULL) {
		ipc_dispatcher_id_t id = IPC_DISPATCHER_SYS_SET_TIME_TEST1_FUNC;

		PACK_FIELD(buff, id);
		PACK_FIELD(buff, data->mSec);
		PACK_FIELD(buff, data->hour);
		PACK_FIELD(buff, data->day);
		PACK_FIELD(buff, data->year);
	}

	return len;	
}

static void unpack_set_time_test_func(struct time_value_resp *data, const uint8_t *buff, size_t lenBuff)
{
	size_t len = sizeof(ipc_dispatcher_id_t) + sizeof(int) + sizeof(uint32_t) +
			sizeof(uint8_t) + sizeof(uint16_t) + sizeof(uint16_t);

	if (len != lenBuff) {
		return;
	}

	buff += sizeof(ipc_dispatcher_id_t);
	UNPACK_FIELD(buff, data->error);
	UNPACK_FIELD(buff, data->mSecSet);
	UNPACK_FIELD(buff, data->hourSet);
	UNPACK_FIELD(buff, data->daySet);
	UNPACK_FIELD(buff, data->yearSet);
}

struct time_value_resp driver1_set_time_test_func(struct time_value *time)
{
	struct time_value_resp time_value_resp;

	/* Nested callback */
	void set_time_test_func_cb(const void *data, size_t len) {
		unpack_set_time_test_func(&time_value_resp, (uint8_t *)data, len);
		k_sem_give(&responce_sem);
	}

	/* Packing ipc id (unit_offset + api_offset) and packed arguments */
	uint8_t packed_data[pack_set_time_test_func(NULL, NULL)];
	size_t len = pack_set_time_test_func(time, packed_data);

	/* Set IPC dispatcher element (id + callback) */
	ipc_dispatcher_elem_t elem = {
	    .id = *(ipc_dispatcher_id_t*)packed_data,
		.cb = set_time_test_func_cb
	};

	k_mutex_lock(&mutex, K_FOREVER);
	k_sem_reset(&responce_sem);
	ipc_dispatcher_add_elem(&elem);

	if (ipc_dispatcher_send(packed_data, len)) {
		k_mutex_unlock(&mutex);
		return time_value_resp;
	}

	k_sem_take(&responce_sem, RESPONCE_TIMEOUT_IN_MS);
	ipc_dispatcher_rm_elem(&elem);
	k_mutex_unlock(&mutex);

	return time_value_resp;
}

void driver1_examp_init(void)
{
	k_mutex_init(&mutex);
}
