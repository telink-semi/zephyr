/*
 * Copyright (c) 2022-2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT telink_w91_adc

/* Local driver headers */
#define ADC_CONTEXT_USES_KERNEL_TIMER
#include "adc_context.h"

/* Zephyr Device Tree headers */
#include <zephyr/dt-bindings/adc/b9x-adc.h>

/* Zephyr Logging headers */
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(adc_w91, CONFIG_ADC_LOG_LEVEL);

/* Telink HAL headers */
// #include <adc.h>
// #include "/home/matter/repos/zephyrproject/modules/hal/telink/tlsr9/drivers/B92/adc.h"
// #include "/home/matter/repos/zephyrproject/modules/hal/telink/tlsr9/drivers/B91/adc.h"
#include <zephyr/drivers/pinctrl.h>

/* ADC b9x defines */
#define SIGN_BIT_POSITION          (13)
#define AREG_ADC_DATA_STATUS       (0xf6)
#define ADC_DATA_READY             BIT(0)

#define debug_msg(...) printk("[%s %d %s()]: ", "ADC_W91", __LINE__, __func__); printk(__VA_ARGS__); printk("\n")
/* b9x ADC driver data */
struct b9x_adc_data {
	struct adc_context ctx;
	int16_t *buffer;
	int16_t *repeat_buffer;
	uint8_t differential;
	uint8_t resolution_divider;
	struct k_sem acq_sem;
	struct k_thread thread;

	K_THREAD_STACK_MEMBER(stack, CONFIG_ADC_B9X_ACQUISITION_THREAD_STACK_SIZE);
};

struct b9x_adc_cfg {
	uint32_t sample_freq;
	uint16_t vref_internal_mv;
	const struct pinctrl_dev_config *pcfg;
};

/* Validate ADC data buffer size */
static int adc_b9x_validate_buffer_size(const struct adc_sequence *sequence)
{
	size_t needed = sizeof(int16_t);

	if (sequence->options) {
		needed *= (1 + sequence->options->extra_samplings);
	}

	if (sequence->buffer_size < needed) {
		return -ENOMEM;
	}

	return 0;
}

/* Validate ADC read API input parameters */
static int adc_b9x_validate_sequence(const struct adc_sequence *sequence)
{
	int status;

	if (sequence->channels != BIT(0)) {
		LOG_ERR("Only channel 0 is supported.");
		return -ENOTSUP;
	}

	if (sequence->oversampling) {
		LOG_ERR("Oversampling is not supported.");
		return -ENOTSUP;
	}

	status = adc_b9x_validate_buffer_size(sequence);
	if (status) {
		LOG_ERR("Buffer size too small.");
		return status;
	}

	return 0;
}

/* ADC Context API implementation: start sampling */
static void adc_context_start_sampling(struct adc_context *ctx)
{
	struct b9x_adc_data *data =
		CONTAINER_OF(ctx, struct b9x_adc_data, ctx);

	data->repeat_buffer = data->buffer;

	// adc_power_on();

	k_sem_give(&data->acq_sem);
}

/* ADC Context API implementation: buffer pointer */
static void adc_context_update_buffer_pointer(struct adc_context *ctx, bool repeat_sampling)
{

}

/* Start ADC measurements */
static int adc_b9x_adc_start_read(const struct device *dev, const struct adc_sequence *sequence)
{
	debug_msg("");
	return 0;
}

/* Main ADC Acquisition thread */
static void adc_b9x_acquisition_thread(const struct device *dev)
{
	debug_msg("");
}

/* ADC Driver initialization */
static int adc_b9x_init(const struct device *dev)
{
	struct b9x_adc_data *data = dev->data;

	const struct b9x_adc_cfg *config = dev->config;
	int err;
	debug_msg("");
	/* Configure dt provided device signals when available */
	err = pinctrl_apply_state(config->pcfg, PINCTRL_STATE_DEFAULT);
	if (err < 0) {
		LOG_ERR("ADC pinctrl setup failed (%d)", err);
		return err;
	}
	debug_msg("");

	k_sem_init(&data->acq_sem, 0, 1);

	k_thread_create(&data->thread, data->stack,
			CONFIG_ADC_B9X_ACQUISITION_THREAD_STACK_SIZE,
			(k_thread_entry_t)adc_b9x_acquisition_thread,
			(void *)dev, NULL, NULL,
			CONFIG_ADC_B9X_ACQUISITION_THREAD_PRIO,
			0, K_NO_WAIT);

	adc_context_unlock_unconditionally(&data->ctx);

	return 0;
}

/* API implementation: channel_setup */
static int adc_b9x_channel_setup(const struct device *dev,
				 const struct adc_channel_cfg *channel_cfg)
{
	debug_msg("");
	return 0;
}

/* API implementation: read */
static int adc_b9x_read(const struct device *dev,
			const struct adc_sequence *sequence)
{
	debug_msg("");
	return 0;
}

static struct b9x_adc_data data_0 = {
	ADC_CONTEXT_INIT_TIMER(data_0, ctx),
	ADC_CONTEXT_INIT_LOCK(data_0, ctx),
	ADC_CONTEXT_INIT_SYNC(data_0, ctx),
};

PINCTRL_DT_INST_DEFINE(0);

static const struct b9x_adc_cfg cfg_0 = {
	.sample_freq = DT_INST_PROP(0, sample_freq),
	.vref_internal_mv = DT_INST_PROP(0, vref_internal_mv),
	.pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(0),
};

static const struct adc_driver_api adc_b9x_driver_api = {
	.channel_setup = adc_b9x_channel_setup,
	.read = adc_b9x_read,
#ifdef CONFIG_ADC_ASYNC
	.read_async = adc_b9x_read_async,
#endif
	.ref_internal = cfg_0.vref_internal_mv,
};

DEVICE_DT_INST_DEFINE(0, adc_b9x_init, NULL,
		      &data_0,  &cfg_0,
		      POST_KERNEL,
		      CONFIG_TELINK_W91_IPC_DRIVERS_INIT_PRIORITY,
		      &adc_b9x_driver_api);
