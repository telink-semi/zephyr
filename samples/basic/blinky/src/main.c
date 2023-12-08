/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "reg_include/register.h"
#include "debug.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/***************************************************************************************
 * GPIO test functionality
 ***************************************************************************************/
#ifndef readl
#define readl(a) \
	({uint32_t __v = *((volatile uint32_t *)(a)); __v; })
#endif /* readl */

#ifndef writel
#define writel(v, a) \
	({uint32_t __v = v; *((volatile uint32_t *)(a)) = __v; })
#endif /* writel */

static void update_reg(uint32_t addr, uint32_t value, bool set)
{
	uint32_t reg = readl(addr);
	if (set) {
		reg |= value;
	} else {
		reg &= ~value;
	}
	writel(reg, addr);
}

static void enable_gpio_out(uint8_t gpio)
{
	uint8_t bank  = gpio / 8;
	uint32_t addr  = IOMUX_BASE_ADDR + bank * 4;
	uint8_t shift = (gpio - bank * 8) * 4;

	update_reg(addr, 0xf << shift, false);
	update_reg(addr, 0x8 << shift, true);

	update_reg(GPIO_BASE_ADDR + 0x0c, 1 << gpio, false);
	update_reg(GPIO_BASE_ADDR + 0x10, 1 << gpio, true);
}

static void control_gpio_out(uint8_t gpio, bool state)
{
	update_reg(GPIO_BASE_ADDR + 0x14, 1 << gpio, state);
}

int main(void)
{
	unsigned long hartid, vendor, arch;
	read_csr(hartid, NDS_MHARTID);
	read_csr(vendor, NDS_MVENDORID);
	read_csr(arch, NDS_MARCHID);
	debug_printf("main   [%u] vendor %08x, arch %08x\n", hartid, vendor, arch);

// *((volatile unsigned int *)(0xF0700028)) = *((volatile unsigned int *)(0xF0700028)) & ~(0xf << 16);
// *((volatile unsigned int *)(0xF0700028)) = *((volatile unsigned int *)(0xF0700028)) | (0x8 << 16);
// *((volatile unsigned int *)(0xf0700010)) = *((volatile unsigned int *)(0xf0700010)) | (1 << 20);
// *((volatile unsigned int *)(0xf0700014)) = *((volatile unsigned int *)(0xf0700014)) | (1 << 20);

	/* blink red LED */
	enable_gpio_out(20);
	for (;;) {
		control_gpio_out(20, true);
		k_msleep(SLEEP_TIME_MS);
		control_gpio_out(20, false);
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
