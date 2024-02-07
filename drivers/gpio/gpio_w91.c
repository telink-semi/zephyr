/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2024 Telink Semiconductor (Shanghai) Co., Ltd.
 */

#include <zephyr/drivers/gpio/gpio_utils.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define LOG_LEVEL CONFIG_GPIO_LOG_LEVEL
LOG_MODULE_REGISTER(gpio_w91);

#include <ipc/ipc_dispatcher.h>
IPC_DISPATCHER_HOST_INIT;

/* Driver dts compatibility: telink,w91_gpio */
#define DT_DRV_COMPAT telink_w91_gpio

/* Max gpio pin number */
#define GPIO_PIN_NUM_MAX         ((uint8_t)25u)

/* GPIO interrupt types */
#define GPIO_PIN_OUTPUT_LOW      ((uint8_t)0u)
#define GPIO_PIN_OUTPUT_HIGH     ((uint8_t)1u)

/* Pull-up/down resistors */
#define GPIO_PIN_DEFAULT         ((uint8_t)0u)
#define GPIO_PIN_PULL_UP         ((uint8_t)1u)
#define GPIO_PIN_PULL_DOWN       ((uint8_t)2u)

/* GPIO interrupt types */
#define GPIO_PIN_IRQ_RISE_EDGE   ((uint8_t)0u)
#define GPIO_PIN_IRQ_FALL_EDGE   ((uint8_t)1u)
#define GPIO_PIN_IRQ_BOTH_EDGE   ((uint8_t)2u)

enum {
	IPC_DISPATCHER_GPIO_PIN_CONFIG = IPC_DISPATCHER_GPIO,
	IPC_DISPATCHER_GPIO_PORT_GET_RAW,
	IPC_DISPATCHER_GPIO_PORT_SET_MASKED_RAW,
	IPC_DISPATCHER_GPIO_PORT_SET_BITS_RAW,
	IPC_DISPATCHER_GPIO_PORT_CLEAR_BITS_RAW,
	IPC_DISPATCHER_GPIO_PORT_TOGGLE_BITS,
	IPC_DISPATCHER_GPIO_PIN_IRQ_CONFIG,
	IPC_DISPATCHER_GPIO_PIN_IRQ_EVENT,
};

struct gpio_w91_data {
	struct gpio_driver_data common; /* driver data */
	sys_slist_t callbacks;          /* list of callbacks */
};

struct gpio_w91_pin_config_req {
	uint8_t pin;
	bool output;
	uint8_t output_init;
	uint8_t bias;
};

struct gpio_w91_pin_irq_config_req {
	uint8_t pin;
	bool irq_enable;
	uint8_t type;
};

struct gpio_w91_port_get_raw_resp {
	int err;
	gpio_port_value_t value;
};

struct gpio_w91_port_set_masked_raw_req {
	int err;
	gpio_port_pins_t mask;
	gpio_port_value_t value;
};

/* APIs implementation: pin configure */
static size_t pack_gpio_w91_pin_configure(void *unpack_data, uint8_t *pack_data)
{
	struct gpio_w91_pin_config_req *p_pin_config_req = unpack_data;

	size_t pack_data_len = sizeof(enum ipc_dispatcher_id) +
			sizeof(p_pin_config_req->pin) + sizeof(p_pin_config_req->output) +
			sizeof(p_pin_config_req->output_init) + sizeof(p_pin_config_req->bias);

	if (pack_data != NULL) {
		enum ipc_dispatcher_id id = IPC_DISPATCHER_GPIO_PIN_CONFIG;

		IPC_DISPATCHER_PACK_FIELD(pack_data, id);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_pin_config_req->pin);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_pin_config_req->output);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_pin_config_req->output_init);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_pin_config_req->bias);
	}

	return pack_data_len;
}

IPC_DISPATCHER_UNPACK_FUNC_ONLY_WITH_ERROR_PARAM(gpio_w91_pin_configure);

static int gpio_w91_pin_configure(const struct device *dev,
		gpio_pin_t pin, gpio_flags_t flags)
{
	int err;
	struct gpio_w91_pin_config_req pin_config_req;

	/* Check input parameters: pin number */
	if (pin > GPIO_PIN_NUM_MAX) {
		return -ENOTSUP;
	}

	/* Check input parameters: open-source and open-drain */
	if ((flags & GPIO_SINGLE_ENDED) != 0) {
		return -ENOTSUP;
	}

	pin_config_req.pin = pin;

	/* Check input parameters: simultaneous in/out mode */
	if ((flags & GPIO_OUTPUT) && (flags & GPIO_INPUT)) {
		return -ENOTSUP;
	} else if (flags & GPIO_OUTPUT) {
		pin_config_req.output = true;
	} else if (flags & GPIO_INPUT) {
		pin_config_req.output = false;
	} else {
		return -EINVAL;
	}

	/* Set GPIO init state if defined to avoid glitches */
	if (flags & GPIO_OUTPUT_INIT_HIGH) {
		pin_config_req.output_init = GPIO_PIN_OUTPUT_HIGH;
	} else if (flags & GPIO_OUTPUT_INIT_LOW) {
		pin_config_req.output_init = GPIO_PIN_OUTPUT_LOW;
	}

	/* Config Pin pull-up / pull-down */
	if (flags & GPIO_PULL_UP) {
		pin_config_req.bias = GPIO_PIN_PULL_UP;
	} else if (flags & GPIO_PULL_DOWN) {
		pin_config_req.bias = GPIO_PIN_PULL_DOWN;
	} else {
		pin_config_req.bias = GPIO_PIN_DEFAULT;
	}

	IPC_DISPATCHER_HOST_SEND_DATA(gpio_w91_pin_configure, &pin_config_req, &err,
		CONFIG_GPIO_TELINK_W91_IPC_RESPONSE_TIMEOUT_MS);

	return err;
}

/* API implementation: get pins mask */
IPC_DISPATCHER_PACK_FUNC_WITHOUT_PARAM(gpio_w91_port_get_raw, IPC_DISPATCHER_GPIO_PORT_GET_RAW);

static void unpack_gpio_w91_port_get_raw(void *unpack_data,
		const uint8_t *pack_data, size_t pack_data_len)
{
	struct gpio_w91_port_get_raw_resp *p_port_get_raw_resp = unpack_data;
	size_t expect_len = sizeof(enum ipc_dispatcher_id) +
			sizeof(p_port_get_raw_resp->err) + sizeof(p_port_get_raw_resp->value);

	if (expect_len != pack_data_len) {
		p_port_get_raw_resp->err = -EINVAL;
		return;
	}

	pack_data += sizeof(enum ipc_dispatcher_id);
	IPC_DISPATCHER_UNPACK_FIELD(pack_data, p_port_get_raw_resp->err);
	IPC_DISPATCHER_UNPACK_FIELD(pack_data, p_port_get_raw_resp->value);
}

static int gpio_w91_port_get_raw(const struct device *dev, gpio_port_value_t *value)
{
	struct gpio_w91_port_get_raw_resp port_get_raw_resp;

	IPC_DISPATCHER_HOST_SEND_DATA(gpio_w91_port_get_raw, NULL, &port_get_raw_resp,
		CONFIG_GPIO_TELINK_W91_IPC_RESPONSE_TIMEOUT_MS);

	if (!port_get_raw_resp.err) {
		*value = port_get_raw_resp.value;
	}

	return port_get_raw_resp.err;
}

/* API implementation: set pins value */
static size_t pack_gpio_w91_port_set_masked_raw(void *unpack_data, uint8_t *pack_data)
{
	struct gpio_w91_port_set_masked_raw_req *p_port_set_masked_raw_req = unpack_data;

	size_t pack_data_len = sizeof(enum ipc_dispatcher_id) +
			sizeof(p_port_set_masked_raw_req->mask) +
			sizeof(p_port_set_masked_raw_req->value);

	if (pack_data != NULL) {
		enum ipc_dispatcher_id id = IPC_DISPATCHER_GPIO_PORT_SET_MASKED_RAW;

		IPC_DISPATCHER_PACK_FIELD(pack_data, id);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_port_set_masked_raw_req->mask);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_port_set_masked_raw_req->value);
	}

	return pack_data_len;
}

IPC_DISPATCHER_UNPACK_FUNC_ONLY_WITH_ERROR_PARAM(gpio_w91_port_set_masked_raw);

static int gpio_w91_port_set_masked_raw(const struct device *dev,
					gpio_port_pins_t mask,
					gpio_port_value_t value)
{
	int err;
	struct gpio_w91_port_set_masked_raw_req port_set_masked_raw_req;

	port_set_masked_raw_req.mask = mask;
	port_set_masked_raw_req.value = value;

	IPC_DISPATCHER_HOST_SEND_DATA(gpio_w91_port_set_masked_raw, &port_set_masked_raw_req, &err,
		CONFIG_GPIO_TELINK_W91_IPC_RESPONSE_TIMEOUT_MS);

	return err;
}

/* API implementation: set pins */
static size_t pack_gpio_w91_port_set_bits_raw(void *unpack_data, uint8_t *pack_data)
{
	gpio_port_pins_t *p_mask = unpack_data;

	size_t pack_data_len = sizeof(enum ipc_dispatcher_id) +  sizeof(*p_mask);

	if (pack_data != NULL) {
		enum ipc_dispatcher_id id = IPC_DISPATCHER_GPIO_PORT_SET_BITS_RAW;

		IPC_DISPATCHER_PACK_FIELD(pack_data, id);
		IPC_DISPATCHER_PACK_FIELD(pack_data, *p_mask);
	}

	return pack_data_len;
}

IPC_DISPATCHER_UNPACK_FUNC_ONLY_WITH_ERROR_PARAM(gpio_w91_port_set_bits_raw);

static int gpio_w91_port_set_bits_raw(const struct device *dev, gpio_port_pins_t mask)
{
	int err;

	IPC_DISPATCHER_HOST_SEND_DATA(gpio_w91_port_set_bits_raw, &mask, &err,
		CONFIG_GPIO_TELINK_W91_IPC_RESPONSE_TIMEOUT_MS);

	return err;
}

/* API implementation: clear pins */
static size_t pack_gpio_w91_port_clear_bits_raw(void *unpack_data, uint8_t *pack_data)
{
	gpio_port_pins_t *p_mask = unpack_data;

	size_t pack_data_len = sizeof(enum ipc_dispatcher_id) +  sizeof(*p_mask);

	if (pack_data != NULL) {
		enum ipc_dispatcher_id id = IPC_DISPATCHER_GPIO_PORT_CLEAR_BITS_RAW;

		IPC_DISPATCHER_PACK_FIELD(pack_data, id);
		IPC_DISPATCHER_PACK_FIELD(pack_data, *p_mask);
	}

	return pack_data_len;
}

IPC_DISPATCHER_UNPACK_FUNC_ONLY_WITH_ERROR_PARAM(gpio_w91_port_clear_bits_raw);

static int gpio_w91_port_clear_bits_raw(const struct device *dev, gpio_port_pins_t mask)
{
	int err;

	IPC_DISPATCHER_HOST_SEND_DATA(gpio_w91_port_clear_bits_raw, &mask, &err,
		CONFIG_GPIO_TELINK_W91_IPC_RESPONSE_TIMEOUT_MS);

	return err;
}

/* APIs implementation: toggle pins */
static size_t pack_gpio_w91_port_toggle_bits(void *unpack_data, uint8_t *pack_data)
{
	gpio_port_pins_t *p_mask = unpack_data;

	size_t pack_data_len = sizeof(enum ipc_dispatcher_id) +  sizeof(*p_mask);

	if (pack_data != NULL) {
		enum ipc_dispatcher_id id = IPC_DISPATCHER_GPIO_PORT_TOGGLE_BITS;

		IPC_DISPATCHER_PACK_FIELD(pack_data, id);
		IPC_DISPATCHER_PACK_FIELD(pack_data, *p_mask);
	}

	return pack_data_len;
}

IPC_DISPATCHER_UNPACK_FUNC_ONLY_WITH_ERROR_PARAM(gpio_w91_port_toggle_bits);

static int gpio_w91_port_toggle_bits(const struct device *dev, gpio_port_pins_t mask)
{
	int err;

	IPC_DISPATCHER_HOST_SEND_DATA(gpio_w91_port_toggle_bits, &mask, &err,
		CONFIG_GPIO_TELINK_W91_IPC_RESPONSE_TIMEOUT_MS);

	return err;
}

/* API implementation: pin interrupt configure */
static size_t pack_gpio_w91_pin_interrupt_configure(void *unpack_data, uint8_t *pack_data)
{
	struct gpio_w91_pin_irq_config_req *p_pin_irq_config = unpack_data;

	size_t pack_data_len = sizeof(enum ipc_dispatcher_id) + sizeof(p_pin_irq_config->pin)
			+ sizeof(p_pin_irq_config->irq_enable) + sizeof(p_pin_irq_config->type);

	if (pack_data != NULL) {
		enum ipc_dispatcher_id id = IPC_DISPATCHER_GPIO_PIN_IRQ_CONFIG;

		IPC_DISPATCHER_PACK_FIELD(pack_data, id);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_pin_irq_config->pin);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_pin_irq_config->irq_enable);
		IPC_DISPATCHER_PACK_FIELD(pack_data, p_pin_irq_config->type);
	}

	return pack_data_len;
}

IPC_DISPATCHER_UNPACK_FUNC_ONLY_WITH_ERROR_PARAM(gpio_w91_pin_interrupt_configure);

static int gpio_w91_pin_interrupt_configure(const struct device *dev,
					    gpio_pin_t pin,
					    enum gpio_int_mode mode,
					    enum gpio_int_trig trig)
{
	int err = 0;
	struct gpio_w91_pin_irq_config_req pin_irq_config_req;

	pin_irq_config_req.pin = pin;

	switch (mode) {
	case GPIO_INT_MODE_DISABLED:
		pin_irq_config_req.irq_enable = false;
		break;

	case GPIO_INT_MODE_EDGE:
		pin_irq_config_req.irq_enable = true;

		if (trig == GPIO_INT_TRIG_LOW) {
			pin_irq_config_req.type = GPIO_PIN_IRQ_FALL_EDGE;
		} else if (trig == GPIO_INT_TRIG_HIGH) {
			pin_irq_config_req.type = GPIO_PIN_IRQ_RISE_EDGE;
		} else if (trig == GPIO_INT_TRIG_BOTH) {
			pin_irq_config_req.type = GPIO_PIN_IRQ_BOTH_EDGE;
		} else {
			return -ENOTSUP;
		}
		break;

	default:
		return -ENOTSUP;
	}

	IPC_DISPATCHER_HOST_SEND_DATA(gpio_w91_pin_interrupt_configure, &pin_irq_config_req, &err,
		CONFIG_GPIO_TELINK_W91_IPC_RESPONSE_TIMEOUT_MS);

	return err;
}

/* API implementation: manage irq callback */
static int gpio_w91_manage_callback(const struct device *dev,
				    struct gpio_callback *callback,
				    bool set)
{
	struct gpio_w91_data *data = dev->data;

	return gpio_manage_callback(&data->callbacks, callback, set);
}

/* APIs implementation: irq callback */
static bool unpack_gpio_w91_irq_cb(void *unpack_data,
		const uint8_t *pack_data, size_t pack_data_len)
{
	uint8_t *p_pin = unpack_data;
	size_t expect_len = sizeof(enum ipc_dispatcher_id) + sizeof(*p_pin);

	if (expect_len != pack_data_len) {
		return false;
	}

	pack_data += sizeof(enum ipc_dispatcher_id);
	IPC_DISPATCHER_UNPACK_FIELD(pack_data, *p_pin);
	return true;
}

static void gpio_w91_irq_cb(const void *data, size_t len, void *param)
{
	uint8_t pin;
	const struct device *dev = param;
	struct gpio_w91_data *dev_data = dev->data;

	if (unpack_gpio_w91_irq_cb(&pin, data, len)) {
		gpio_fire_callbacks(&dev_data->callbacks, dev, BIT(pin));
	}
}

/* APIs implementation: gpio init */
static int gpio_w91_init(const struct device *dev)
{
	ipc_dispatcher_add(IPC_DISPATCHER_GPIO_PIN_IRQ_EVENT, gpio_w91_irq_cb, (void *)dev);
	return 0;
}

/* GPIO driver APIs structure */
static const struct gpio_driver_api gpio_w91_driver_api = {
	.pin_configure = gpio_w91_pin_configure,
	.port_get_raw = gpio_w91_port_get_raw,
	.port_set_masked_raw = gpio_w91_port_set_masked_raw,
	.port_set_bits_raw = gpio_w91_port_set_bits_raw,
	.port_clear_bits_raw = gpio_w91_port_clear_bits_raw,
	.port_toggle_bits = gpio_w91_port_toggle_bits,
	.pin_interrupt_configure = gpio_w91_pin_interrupt_configure,
	.manage_callback = gpio_w91_manage_callback
};

/* GPIO driver registration */
#define GPIO_W91_INIT(n)                                                \
	static const struct gpio_driver_config gpio_w91_config_##n = {      \
		.port_pin_mask = GPIO_PORT_PIN_MASK_FROM_DT_INST(n),            \
	};                                                                  \
                                                                        \
	static struct gpio_w91_data gpio_w91_data_##n;                      \
                                                                        \
	DEVICE_DT_INST_DEFINE(n, gpio_w91_init,                             \
			      NULL,                                                 \
			      &gpio_w91_data_##n,                                   \
			      &gpio_w91_config_##n,                                 \
			      POST_KERNEL,                                          \
			      CONFIG_TELINK_W91_IPC_DRIVERS_INIT_PRIORITY,          \
			      &gpio_w91_driver_api);

DT_INST_FOREACH_STATUS_OKAY(GPIO_W91_INIT)
