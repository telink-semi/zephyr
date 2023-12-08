#include "debug.h"
#include <reg_include/register.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#ifndef readl
#define readl(a) \
	({uint32_t __v = *((volatile uint32_t *)(a)); __v; })
#endif /* readl */

#ifndef writel
#define writel(v, a) \
	({uint32_t __v = v; *((volatile uint32_t *)(a)) = __v; })
#endif /* writel */

#define DEBUG_BUFFER_SIZE			512

#define F_REQ                       40000000

#define OFT_ATCUART_HWC				0x10
#define OFT_ATCUART_OSC				0x14
#define OFT_ATCUART_RBR				0x20
#define OFT_ATCUART_THR 			0x20
#define OFT_ATCUART_DLL 			0x20
#define OFT_ATCUART_IER  			0x24
#define OFT_ATCUART_DLM				0x24
#define OFT_ATCUART_IIR				0x28
#define OFT_ATCUART_FCR				0x28
#define OFT_ATCUART_LCR				0x2C
#define OFT_ATCUART_MCR 			0x30
#define OFT_ATCUART_LSR				0x34
#define OFT_ATCUART_MSR 			0x38
#define OFT_ATCUART_SCR 			0x3C

static void debug_init(void)
{
	static volatile bool debug_inited = false;

	if (!debug_inited) {
		debug_inited = true;

		writel(readl(GPIO_BASE_ADDR + 0x28) & ~0x0ff00000, GPIO_BASE_ADDR + 0x28);

		const uint32_t os = readl(UART0_BASE_ADDR + OFT_ATCUART_OSC) & 0x1F;
		const uint32_t div = F_REQ / (DEBUG_BAUDRATE * os);

		uint32_t lc = readl(UART0_BASE_ADDR + OFT_ATCUART_LCR);

		lc |= 0x80;
		writel(lc, UART0_BASE_ADDR + OFT_ATCUART_LCR);

		writel((uint8_t)(div >> 8), UART0_BASE_ADDR + OFT_ATCUART_DLM);
		writel((uint8_t)div, UART0_BASE_ADDR + OFT_ATCUART_DLL);

		lc &= ~(0x80);
		writel(lc, UART0_BASE_ADDR + OFT_ATCUART_LCR);

		writel(3, UART0_BASE_ADDR + OFT_ATCUART_LCR);
		writel(7, UART0_BASE_ADDR + OFT_ATCUART_FCR);
	}
}

void debug_putch(char ch)
{
	debug_init();
	while (!(readl(UART0_BASE_ADDR + OFT_ATCUART_LSR) & (1 << 5)));
	writel((uint32_t)ch, UART0_BASE_ADDR + OFT_ATCUART_THR);
}

void debug_puts(const char *str)
{
	while (*str) {
		debug_putch(*str);
		str++;
	}
}

void debug_printf(const char *format, ...)
{
	char buf[DEBUG_BUFFER_SIZE];
	buf[0] = 0;
	va_list args;
	va_start(args, format);
	(void)vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);
	debug_puts(buf);
}
