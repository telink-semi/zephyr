/*
 * Copyright (c) 2022-2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/drivers/pinctrl.h>
#include <zephyr/dt-bindings/pinctrl/w91-pinctrl.h>
#include <zephyr/pm/device.h>

#include <ipc/ipc_based_driver.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/gpio/gpio_utils.h>

enum {
	IPC_DISPATCHER_GPIO_PIN_CONFIG = IPC_DISPATCHER_GPIO,
	IPC_DISPATCHER_GPIO_PORT_GET_RAW,
	IPC_DISPATCHER_GPIO_PORT_SET_MASKED_RAW,
	IPC_DISPATCHER_GPIO_PORT_SET_BITS_RAW,
	IPC_DISPATCHER_GPIO_PORT_CLEAR_BITS_RAW,
	IPC_DISPATCHER_GPIO_PORT_TOGGLE_BITS,
	IPC_DISPATCHER_GPIO_PIN_IRQ_CONFIG,
	IPC_DISPATCHER_GPIO_PIN_IRQ_EVENT,
	IPC_DISPATCHER_PINCTRL_INIT_EVENT,
};

struct pinctrl_w91_config {
	// uint8_t pins_num;            /* pins number */
	// uint8_t pins_func;            /* pins function */
    const struct pinctrl_dev_config *pcfg;
	uint8_t instance_id;            /* instance id */
};

struct pinctrl_w91_data {
	/* gpio_driver_data needs to be first */
	// struct gpio_driver_data common;
	struct ipc_based_driver ipc;    /* ipc driver part */
	// sys_slist_t callbacks;          /* list of callbacks */
};

struct pinctrl_pin_map {
	unsigned pin;
	const char *func;
	const char *id;
	int status;
	struct pm_conf *pmconf; /* 0 : not alternated, others : set to GPIO */
};

#define debug_msg(...) printk("[%s %d %s()]: ", "PINCTRL_W91", __LINE__, __func__); printk(__VA_ARGS__); printk("\n")

#define DT_DRV_COMPAT telink_w91_pinctrl

/* Sanity checker: to keep instance in in uint8_t */
#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) > UINT8_MAX
	#error Number of driver instances is more than UINT8_MAX
#endif

/* APIs implementation: pin configure */
static size_t pack_pinctrl_w91_pin_configure(void *unpack_data, uint8_t *pack_data)
{
	debug_msg("");
	struct pinctrl_pin_map *p_pinctrl_pin_map = unpack_data;

	size_t pack_data_len = sizeof(enum ipc_dispatcher_id) + 
		sizeof(p_pinctrl_pin_map->pin) + sizeof(p_pinctrl_pin_map->func) +
		sizeof(p_pinctrl_pin_map->id) + sizeof(p_pinctrl_pin_map->status);
		// sizeof(p_pinctrl_pin_map->pmconf);
	if (pack_data != NULL) {
		enum ipc_dispatcher_id id = IPC_DISPATCHER_PINCTRL_INIT_EVENT;

	debug_msg("");
		IPC_DISPATCHER_PACK_FIELD(pack_data, id);
	debug_msg("");
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_pinctrl_pin_map->pin);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_pinctrl_pin_map->func);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_pinctrl_pin_map->id);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_pinctrl_pin_map->status);
		// IPC_DISPATCHER_PACK_FIELD(pack_data, p_pinctrl_pin_map->pmconf);
	debug_msg("END");
	}

	return pack_data_len;
}

IPC_DISPATCHER_UNPACK_FUNC_ONLY_WITH_ERROR_PARAM(pinctrl_w91_pin_configure);

static int pinctrl_w91_pin_configure(int pin, int flags)
{
	debug_msg("");
	int err = 0;
	struct pinctrl_pin_map pin_config;

	pin_config.pin = 1;

	/* Check input parameters: simultaneous in/out mode */
	pin_config.func = 1;
	/* Set GPIO init state if defined to avoid glitches */
	pin_config.id = "gpio0"; // OUTPUT_LOW

	pin_config.status = 1; // default
	// pin_config.pmconf = 0; // default

	debug_msg("");
	// struct ipc_based_driver *ipc_data = &((struct gpio_w91_data *)dev->data)->ipc;
	// uint8_t inst = ((struct gpio_w91_config *)dev->config)->instance_id;

	// debug_msg("");
	// IPC_DISPATCHER_HOST_SEND_DATA(ipc_data, inst,
	// 	pinctrl_w91_pin_configure, &pin_config, &err,
	// 	CONFIG_GPIO_TELINK_W91_IPC_RESPONSE_TIMEOUT_MS);
	debug_msg("END");

	return err;
}

/* Pinctrl driver initialization */
static int pinctrl_w91_init(const struct device *dev)
{
	debug_msg("");
	struct pinctrl_w91_data *data = dev->data;
	ipc_based_driver_init(&data->ipc);
	debug_msg("END");
	return 0;
}

/* Act as GPIO function disable */
static inline void pinctrl_b9x_gpio_function_disable(uint32_t pin)
{
	debug_msg("pin(%d)", pin);
	// pinctrl_free_pin();
}

/* Set pin's function */
static int pinctrl_configure_pin(const pinctrl_soc_pin_t *pinctrl)
{
	debug_msg("");
	int status;
	uint8_t mask;
	uint8_t offset = 0;
	uint8_t func = W91_PINMUX_GET_FUNC(*pinctrl);
	uint32_t pin = W91_PINMUX_GET_PIN(*pinctrl);
	debug_msg("pin(%d) \t func(%d) \t", pin, func);
	uint8_t pull_up_en_addr = 0;
	// setup struct, pack it to N22
	// ...

	/* disable GPIO function (can be enabled back by GPIO init using GPIO driver) */
	// ...

	debug_msg("END");
	return status;
}

/* API implementation: configure_pins */
int pinctrl_configure_pins(const pinctrl_soc_pin_t *pins, uint8_t pin_cnt, uintptr_t reg)
{
	debug_msg("");
	ARG_UNUSED(reg);

	int status = 0;
	for (uint8_t i = 0; i < pin_cnt; i++) {
		status = pinctrl_configure_pin(pins++);
		if (status < 0) {
			break;
		}
	}
	debug_msg("END");
	return status;
}

/* GPIO driver registration */
#define PINCTRL_W91_INIT(n)                                                \
    PINCTRL_DT_INST_DEFINE(n);                                             \
	static const struct pinctrl_w91_config pinctrl_w91_config_##n = {         \
        .pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(n),                         \
		.instance_id = n                                               \
	};                                                                  \
                                                                        \
	static struct pinctrl_w91_data pinctrl_w91_data_##n;                      \
                                                                        \
	DEVICE_DT_INST_DEFINE(n, pinctrl_w91_init,                             \
			      NULL,                                                 \
			      &pinctrl_w91_data_##n,                                   \
			      &pinctrl_w91_config_##n,                                 \
			      POST_KERNEL,                                          \
			      CONFIG_TELINK_W91_IPC_DRIVERS_INIT_PRIORITY,          \
			      NULL);

DT_INST_FOREACH_STATUS_OKAY(PINCTRL_W91_INIT)
