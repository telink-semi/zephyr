/*
 * Copyright (c) 2023 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include "tl_radio.h"
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(TLSR9528a_pp, LOG_LEVEL_DBG);

#define APP_CLIENT

#define SLEEP_TIME_MS   1000

#ifdef APP_CLIENT

void data_rx( const void * data, uint8_t len, void * ctx ) {

    ( void ) ctx;

    LOG_HEXDUMP_INF( data, len, "Received: ");
}

#else

typedef struct {
    volatile uint8_t len;
    uint8_t buf[256];
} data_t;

void data_rx( const void * data, uint8_t len, void * ctx ) {

    data_t *p_data = ( data_t* ) ctx;

    p_data->len = len;
    memcpy( p_data->buf, data, len );

     LOG_HEXDUMP_INF( data, len, "Received: ");
}

#endif



int main(void)
{
	LOG_INF("Radio Ping Pong demo start");

#ifdef APP_CLIENT
    LOG_INF( "Client" );

    uint32_t cnt = 0;
    tl_radio_set_ondata( data_rx, NULL );
#else
    LOG_INF( "Server" );

    data_t data  = {
        .len = 0
    };

    tl_radio_start( 12, 4 );
    tl_radio_set_ondata( data_rx, &data );
#endif

	while (1) {
#ifdef APP_CLIENT
        tl_radio_start( 12, 4 );
        tl_radio_transmitt( &cnt, sizeof( cnt ) );
        LOG_HEXDUMP_INF(&cnt, sizeof( cnt ) , "Transmitted: ");
        cnt ++;
        k_msleep( 10 );/* time to get response */
        tl_radio_stop();

        k_msleep( SLEEP_TIME_MS );
#else
        if ( data.len ) {
            ( void ) tl_radio_transmitt( data.buf, data.len );
            LOG_HEXDUMP_INF( data.buf, data.len , "Transmitted: ");
            data.len = 0;
        }
#endif
		k_msleep(1);
	}
	return 0;
}
