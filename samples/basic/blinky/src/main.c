/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if defined(CONFIG_BOARD_TLSR9118BDK40D)

#include "reg_include/register.h"
#include "driver_w91.h"
#include "debug.h"

#define CORE_NAME "D25 core -> "

static uint32_t request_sw_irq_cntr;
static uint32_t response_sw_irq_cntr;

static bool led_state;

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

static void irq_sw_handler(uint32_t id)
{
	if (id == 1) {
		led_state = !led_state;
		control_gpio_out(20, led_state);
	}
	debug_printf(CORE_NAME "sw irq handler id = %u: cnt = %u\n", id, ++response_sw_irq_cntr);
}

static void except_handler(uint32_t mcause, uint32_t mepc)
{
	(void)mcause;
	(void)mepc;

	for (;;) {
		__asm__ volatile("nop");
	}
}

static void irq_trap_handler(const void *unused)
{
	uint32_t mcause, mepc;

	__asm__ volatile("csrr %0, mcause" : "=r"(mcause));
	__asm__ volatile("csrr %0, mepc"   : "=r"(mepc));

	if (mcause & (1u << 31)) {
		switch (mcause & 0xfff) {
		case 3: {
			uint32_t id = plic_sw_claim_interrupt();

			irq_sw_handler(id);
			plic_sw_complete_interrupt(id);
		}
		break;
		default:
			except_handler(mcause, mepc);
			break;
		}
	} else {
		except_handler(mcause, mepc);
	}
}

int main(void)
{
	unsigned long hartid, vendor, arch;

	read_csr(hartid, NDS_MHARTID);
	read_csr(vendor, NDS_MVENDORID);
	read_csr(arch, NDS_MARCHID);
	debug_printf(CORE_NAME " [%u] vendor %08x, arch %08x\n", hartid, vendor, arch);

	/* blink red LED */
	enable_gpio_out(20);

	/* enable sw interrupt */
	core_interrupt_enable();
	IRQ_CONNECT(RISCV_MACHINE_SOFT_IRQ, 0, irq_trap_handler, NULL, 0);
	irq_enable(RISCV_MACHINE_SOFT_IRQ);
	plic_sw_interrupt_enable(1);

	/* Waiting init N22 core */
	k_msleep(SLEEP_TIME_MS);

	for (;;) {
		debug_printf(CORE_NAME "sw irq request: cnt = %u\n", ++request_sw_irq_cntr);
		plic_sw_set_pending(1);
		plic_sw_set_pending(2);
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

#else

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	int ret;

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

#endif
