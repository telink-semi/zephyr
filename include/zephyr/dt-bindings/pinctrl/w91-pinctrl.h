/*
 * Copyright (c) 2024 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_W91_PINCTRL_COMMON_H_
#define ZEPHYR_W91_PINCTRL_COMMON_H_

/* IDs for GPIO Pins */
// reference: DS-TLSR9118-E4, Table 1-3 GPIO Pin Mux of TLSR9118A

#define W91_PIN_0        0
#define W91_PIN_1        1
#define W91_PIN_2        2
#define W91_PIN_3        3
#define W91_PIN_4        4
#define W91_PIN_5        5
#define W91_PIN_6        6
#define W91_PIN_7        7
#define W91_PIN_8        8
#define W91_PIN_9        9
#define W91_PIN_10       10
#define W91_PIN_11       11
#define W91_PIN_12       12
#define W91_PIN_13       13
#define W91_PIN_14       14
#define W91_PIN_15       15
#define W91_PIN_16       16
#define W91_PIN_17       17
#define W91_PIN_18       18
#define W91_PIN_19       19
#define W91_PIN_20       20
#define W91_PIN_21       21
#define W91_PIN_22       22
#define W91_PIN_23       23
#define W91_PIN_24       24

/* IDs for W91 GPIO functions */

#define W91_FUNC_DEFAULT    0
#define W91_FUNC_UART_CTS   1
#define W91_FUNC_UART_RTS   2
#define W91_FUNC_UART_TXD   3
#define W91_FUNC_UART_RXD   4
#define W91_FUNC_PWM_0      5
#define W91_FUNC_PWM_1      6
#define W91_FUNC_PWM_2      7
#define W91_FUNC_PWM_3      8
#define W91_FUNC_SPI_CLK    9
#define W91_FUNC_SPI_CS     10
#define W91_FUNC_SPI_MOSI   11
#define W91_FUNC_SPI_MISO   12
#define W91_FUNC_SPI_WP     13
#define W91_FUNC_SPI_HOLD   14
#define W91_FUNC_SDIO_DATA0 15
#define W91_FUNC_SDIO_DATA1 16
#define W91_FUNC_SDIO_DATA2 17
#define W91_FUNC_SDIO_DATA3 18
#define W91_FUNC_SDIO_CLK   19
#define W91_FUNC_SDIO_CMD   20
#define W91_FUNC_I2C_SCL    21
#define W91_FUNC_I2C_SDA    22

/* W91 pinctrl pull-up/down */

#define W91_PULL_NONE    0
#define W91_PULL_DOWN    2
#define W91_PULL_UP      3

/* W91 pin configuration bit field positions and masks */

#define W91_PULL_POS     21
#define W91_PULL_MSK     0x3
#define W91_FUNC_POS     16
#define W91_FUNC_MSK     0x1F
#define W91_PORT_POS     8
#define W91_PORT_MSK     0xFF

#define W91_PIN_POS      0
#define W91_PIN_MSK      0xFFFF
#define W91_PIN_ID_MSK   0xFF

/* Setters and getters */
#define W91_PINMUX_SET(pin, func)   ((func << W91_FUNC_POS) | \
					   (pin << W91_PIN_POS))
#define W91_PINMUX_GET_PULL(pinmux)       ((pinmux >> W91_PULL_POS) & W91_PULL_MSK)
#define W91_PINMUX_GET_FUNC(pinmux)       ((pinmux >> W91_FUNC_POS) & W91_FUNC_MSK)
#define W91_PINMUX_GET_PIN(pinmux)        ((pinmux >> W91_PIN_POS) & W91_PIN_MSK)
#define W91_PINMUX_GET_PIN_ID(pinmux)     ((pinmux >> W91_PIN_POS) & W91_PIN_ID_MSK)

#endif  /* ZEPHYR_W91_PINCTRL_COMMON_H_ */
