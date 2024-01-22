/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/ipc/ipc_service.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include "ipc_dispatcher.h"

#define LOG_LEVEL LOG_LEVEL_INFO
#define LOG_MODULE_NAME ipc_dispatcher
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#define IPC_BOUND_TIMEOUT_IN_MS 		K_MSEC(1000)

struct ipc_dispatcher_elem {
	sys_snode_t node;
	enum ipc_dispatcher_id id;
	ipc_dispatcher_cb_t cb;
};

static struct ipc_ept ept;
static struct k_mutex ipc_mutex;
static struct k_sem ipc_sem_bound;
static sys_slist_t ipc_elem_list;

static void endpoint_bound(void *priv)
{
	k_sem_give(&ipc_sem_bound);
}

static void endpoint_received(const void *data, size_t len, void *priv)
{
	bool is_elem_added = false;
	struct ipc_dispatcher_elem *p_elem;
	enum ipc_dispatcher_id *p_id = (enum ipc_dispatcher_id *)data;

	k_mutex_lock(&ipc_mutex, K_FOREVER);

	SYS_SLIST_FOR_EACH_CONTAINER(&ipc_elem_list, p_elem, node) {
		if ((p_elem->id == *p_id) && (p_elem->cb != NULL)) {
			is_elem_added = true;
			p_elem->cb(data, len);
			break;
		}
	}

	k_mutex_unlock(&ipc_mutex);

	if (!is_elem_added) {
		LOG_ERR("IPC element is not added (id = %u)", *p_id);
	}
}

static struct ipc_ept_cfg ept_cfg = {
	.cb = {
		.bound = endpoint_bound,
		.received = endpoint_received
	},
};

int ipc_dispatcher_start(void)
{
	const struct device *const ipc_instance = DEVICE_DT_GET(DT_NODELABEL(ipc0));

	k_mutex_init(&ipc_mutex);
	k_sem_init(&ipc_sem_bound, 0, 1);
	sys_slist_init(&ipc_elem_list);

	int ret = ipc_service_register_endpoint(ipc_instance, &ept, &ept_cfg);
	if (ret < 0) {
		LOG_ERR("Failed to register IPC endpoint: %d", ret);
		return ret;
	}

	ret = k_sem_take(&ipc_sem_bound, IPC_BOUND_TIMEOUT_IN_MS);
	if (ret < 0) {
		LOG_ERR("IPC endpoint bind timed out: %d", ret);
		return ret;
	}

	return 0;
}

void ipc_dispatcher_add_elem(enum ipc_dispatcher_id id, ipc_dispatcher_cb_t cb)
{
	struct ipc_dispatcher_elem *p_elem = malloc(sizeof(struct ipc_dispatcher_elem));

	if (p_elem == NULL) {
		LOG_ERR("IPC dispatcher add elem is failed");
		return;
	}

	p_elem->id = id;
	p_elem->cb = cb;

	k_mutex_lock(&ipc_mutex, K_FOREVER);
	sys_slist_append(&ipc_elem_list, &p_elem->node);
	k_mutex_unlock(&ipc_mutex);
}

void ipc_dispatcher_rm_elem(enum ipc_dispatcher_id id)
{
	bool rm_elem = false;
	struct ipc_dispatcher_elem *p_elem;

	k_mutex_lock(&ipc_mutex, K_FOREVER);

	SYS_SLIST_FOR_EACH_CONTAINER(&ipc_elem_list, p_elem, node) {
		if (p_elem->id == id) {
			sys_slist_remove(&ipc_elem_list, NULL, &p_elem->node);
			rm_elem = true;
			break;
		}
	}

	k_mutex_unlock(&ipc_mutex);

	if (rm_elem) {
		free(p_elem);
	}
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

SYS_INIT(ipc_dispatcher_start, POST_KERNEL, CONFIG_IPC_DISPATCHER_PRIORITY);
