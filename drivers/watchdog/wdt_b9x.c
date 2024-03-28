/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT telink_b9x_watchdog

#include <clock.h>
#include <watchdog.h>
#include <zephyr/drivers/watchdog.h>

#define LOG_MODULE_NAME watchdog_b9x
#if defined(CONFIG_WDT_LOG_LEVEL)
#define LOG_LEVEL CONFIG_WDT_LOG_LEVEL
#else
#define LOG_LEVEL LOG_LEVEL_NONE
#endif

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(LOG_MODULE_NAME);


static int wdt_b9x_setup(const struct device *dev, uint8_t options)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(options);

	wd_start();

	LOG_INF("HW watchdog started");

	return 0;
}

static int wdt_b9x_disable(const struct device *dev)
{
	ARG_UNUSED(dev);

	wd_stop();

	LOG_INF("HW watchdog stopped");

	return 0;
}

static int wdt_b9x_install_timeout(const struct device *dev,
				    const struct wdt_timeout_cfg *cfg)
{
	ARG_UNUSED(dev);

	if (cfg->window.min != 0) {
		LOG_WRN("Window watchdog not supported");
		return -EINVAL;
	}

	const uint32_t max_timeout_ms = UINT32_MAX / (sys_clk.pclk * 1000);

	if (cfg->window.max > max_timeout_ms) {
		LOG_WRN("Timeout overflows %umS max is %umS", cfg->window.max, max_timeout_ms);
		return -EINVAL;
	}

	if (cfg->flags != WDT_FLAG_RESET_SOC) {
		LOG_WRN("Not supported flag");
		return -EINVAL;
	}

	wd_set_interval_ms(cfg->window.max);

	/* HW restriction */
	LOG_INF("HW watchdog set to %u mS", cfg->window.max & 0xffffff00);

	return 0;
}

static int wdt_b9x_feed(const struct device *dev, int channel_id)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(channel_id);

	wd_clear_cnt();
	return 0;
}


static const struct wdt_driver_api wdt_b9x_api = {
	.setup = wdt_b9x_setup,
	.disable = wdt_b9x_disable,
	.install_timeout = wdt_b9x_install_timeout,
	.feed = wdt_b9x_feed,
};


static int wdt_b9x_init(const struct device *dev)
{
	ARG_UNUSED(dev);

#if CONFIG_SOC_RISCV_TELINK_B95
	BM_SET(reg_rst2, FLD_RST2_TIMER);
	BM_SET(reg_clk_en2, FLD_CLK2_TIMER_EN);
#endif

	return 0;
}


DEVICE_DT_INST_DEFINE(0, wdt_b9x_init, NULL,
	NULL, NULL, PRE_KERNEL_1,
	CONFIG_KERNEL_INIT_PRIORITY_DEVICE, &wdt_b9x_api);
