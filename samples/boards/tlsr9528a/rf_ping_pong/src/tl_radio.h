#ifndef __TL_RADIO_H
#define __TL_RADIO_H

#include <stdint.h>
#include <stdbool.h>


#define TL_RADIO_MIN_CHANNEL    ( 11 )
#define TL_RADIO_MAX_CHANNEL    ( 26 )

#define TL_RADIO_MIN_POWER_DBM  ( -30 )
#define TL_RADIO_MAX_POWER_DBM  ( 9 )


typedef void ( *tl_radio_ondata_t )( const void * data, uint8_t len, void * ctx );


void tl_radio_start( uint8_t channel, int8_t power_dbm );
bool tl_radio_set_channel( uint8_t channel );
uint8_t tl_radio_get_channel( void );
bool tl_radio_set_power_dbm( int8_t power_dbm );
int8_t tl_radio_get_power_dbm( void );
bool tl_radio_transmitt( const void * data, uint8_t length );
void tl_radio_sync( void );
void tl_radio_set_ondata( tl_radio_ondata_t callback, void * ctx );
void tl_radio_stop( void );
void tl_radio_suspend_restore( void );


#endif
