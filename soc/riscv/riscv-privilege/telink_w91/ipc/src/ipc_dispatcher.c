/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/ipc/ipc_service.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include "ipc_dispatcher.h"

#define LOG_LEVEL LOG_LEVEL_INFO
#define LOG_MODULE_NAME ipc_dispatcher
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#define IPC_BOUND_TIMEOUT_IN_MS 		K_MSEC(1000)

static struct ipc_ept ept;
static struct k_mutex ipc_mutex;
static sys_slist_t ipc_elem_list;
static K_SEM_DEFINE(edp_bound_sem, 0, 1);

static void endpoint_bound(void *priv)
{
	k_sem_give(&edp_bound_sem);
}

static void endpoint_received(const void *data, size_t len, void *priv)
{
	bool is_elem_added = false;
	ipc_dispatcher_elem_t *p_elem;
	ipc_dispatcher_id_t *p_id = (ipc_dispatcher_id_t *)data;

	k_mutex_lock(&ipc_mutex, K_FOREVER);

	SYS_SLIST_FOR_EACH_CONTAINER(&ipc_elem_list, p_elem, node) {
		if ((p_elem->id == *p_id) && (p_elem->cb != NULL)) {
			is_elem_added = true;
			break;
		}
	}

	k_mutex_unlock(&ipc_mutex);

	if (is_elem_added) {
		p_elem->cb(data, len);
	}
	else {
		LOG_ERR("IPC element is not added (id = %u)", *p_id);
	}
}

static struct ipc_ept_cfg ept_cfg = {
	.cb = {
		.bound = endpoint_bound,
		.received = endpoint_received
	},
};

void ipc_dispatcher_init(void)
{
	k_mutex_init(&ipc_mutex);
	sys_slist_init(&ipc_elem_list);
}

int ipc_dispatcher_start(void)
{
	const struct device *const ipc_instance = DEVICE_DT_GET(DT_NODELABEL(ipc0));

	int ret = ipc_service_register_endpoint(ipc_instance, &ept, &ept_cfg);
	if (ret < 0) {
		LOG_ERR("Failed to register IPC endpoint: %d \n", ret);
		return ret;
	}

	ret = k_sem_take(&edp_bound_sem, IPC_BOUND_TIMEOUT_IN_MS);
	if (ret < 0) {
		LOG_ERR("IPC endpoint bind timed out: %d \n", ret);
		return ret;
	}

	return 0;
}

void ipc_dispatcher_add_elem(ipc_dispatcher_elem_t *p_elem)
{
	k_mutex_lock(&ipc_mutex, K_FOREVER);
	sys_slist_append(&ipc_elem_list, &p_elem->node);
	k_mutex_unlock(&ipc_mutex);
}

void ipc_dispatcher_rm_elem(ipc_dispatcher_elem_t *p_elem)
{
	k_mutex_lock(&ipc_mutex, K_FOREVER);
	sys_slist_remove(&ipc_elem_list, NULL, &p_elem->node);
	k_mutex_unlock(&ipc_mutex);
}

int ipc_dispatcher_send(const void *data, size_t len)
{
	int ret = ipc_service_send(&ept, data, len);
	if (ret != len) {
		LOG_ERR("IPC send error");
		return -EPERM;
	}

	return 0;
}
