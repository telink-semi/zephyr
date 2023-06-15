#include "tl_radio.h"

#include <rf.h>
#include <stimer.h>

#include <string.h>
#include <zephyr/kernel.h>


/*****************************************************************************
 * Radio unit configuration
 *****************************************************************************/

#define TL_RADIO_TRX_BUF_SIZE       ( 256 )

#define TL_RADIO_DEFAULT_CHANNEL    ( 16 )
#define TL_RADIO_DEFAULT_POWER_DBM  ( 4 )

#define TL_RADIO_TX_GUARD_US        ( 120 )


/*****************************************************************************
 * Radio unit configuration checker
 *****************************************************************************/

#if (TL_RADIO_TRX_BUF_SIZE  > 0xfff)
#error TL_RADIO_TRX_BUF_SIZE should be not greater than 0xfff
#endif

#if (TL_RADIO_TRX_BUF_SIZE > 0xfff)
#error TL_RADIO_TRX_BUF_SIZE should be not greater than 0xfff
#endif

#if ((TL_RADIO_DEFAULT_CHANNEL < TL_RADIO_MIN_CHANNEL) || (TL_RADIO_DEFAULT_CHANNEL > TL_RADIO_MAX_CHANNEL))
#error TL_RADIO_DEFAULT_CHANNEL should be in range TL_RADIO_MIN_CHANNEL .. TL_RADIO_MAX_CHANNEL
#endif

#if ((TL_RADIO_DEFAULT_POWER_DBM < TL_RADIO_MIN_POWER_DBM) || (TL_RADIO_DEFAULT_POWER_DBM > TL_RADIO_MAX_POWER_DBM))
#error TL_RADIO_DEFAULT_POWER_DBM should be in range TL_RADIO_MIN_POWER_DBM .. TL_RADIO_MAX_POWER_DBM
#endif


/*****************************************************************************
 * Radio unit data
 *****************************************************************************/

static struct {
    bool                inited;
    uint8_t             rx_buffer[TL_RADIO_TRX_BUF_SIZE] __attribute__( ( aligned( 4 ) ) );
    uint8_t             tx_buffer[TL_RADIO_TRX_BUF_SIZE] __attribute__( ( aligned( 4 ) ) );
    uint8_t             channel;
    int8_t              power_dbm;
    volatile bool       tx_process;
    tl_radio_ondata_t   ondata;
    void *              context;
} tl_radio = {
    .inited             = false,
    .channel            = TL_RADIO_DEFAULT_CHANNEL,
    .power_dbm          = TL_RADIO_DEFAULT_POWER_DBM,
    .tx_process         = false,
    .ondata             = NULL,
    .context            = NULL
};


/*****************************************************************************
 * Radio unit private functions declarations
 *****************************************************************************/

static void tl_radio_baseband_reset( void );


/*****************************************************************************
 * Radio ISR
 *****************************************************************************/

__attribute__( ( section( ".ram_code" ) ) ) void rf_irq_handler( void ) {

    if ( rf_get_irq_status( FLD_RF_IRQ_RX ) ) {
        dma_chn_dis( DMA1 );
        if ( rf_zigbee_packet_crc_ok( tl_radio.rx_buffer ) ) {
            if ( tl_radio.ondata ) {
                tl_radio.ondata( &tl_radio.rx_buffer[5], rf_zigbee_get_payload_len( tl_radio.rx_buffer ) - 2, tl_radio.context );
            }
        }
        dma_chn_en( DMA1 );
        rf_clr_irq_status( FLD_RF_IRQ_RX );
    } else if ( rf_get_irq_status( FLD_RF_IRQ_TX ) ) {
        tl_radio.tx_process = false;
        rf_set_rxmode();
        rf_clr_irq_status( FLD_RF_IRQ_TX );
    } else {
        rf_clr_irq_status( FLD_RF_IRQ_ALL );
    }
}


/*****************************************************************************
 * Radio unit public functions
 *****************************************************************************/

void tl_radio_start( uint8_t channel, int8_t power_dbm ) {

    if ( !tl_radio.inited ) {
        tl_radio_baseband_reset();
        CLEAR_ALL_RFIRQ_STATUS;
        rf_clr_irq_mask( FLD_RF_IRQ_ALL );

        rf_mode_init();
        rf_set_zigbee_250K_mode();
        dma_chn_dis( DMA1 );
        rf_set_tx_dma( 1, TL_RADIO_TRX_BUF_SIZE );
        rf_set_rx_dma( ( unsigned char * ) tl_radio.rx_buffer, 0, TL_RADIO_TRX_BUF_SIZE );
        dma_chn_en( DMA1 );

        if ( !tl_radio_set_channel( channel ) ) {
            ( void ) tl_radio_set_channel( TL_RADIO_DEFAULT_CHANNEL );
        }

        if ( !tl_radio_set_power_dbm( power_dbm ) ) {
            ( void ) tl_radio_set_power_dbm( TL_RADIO_DEFAULT_POWER_DBM );
        }

        rf_set_txmode();
        rf_set_rxmode();
        rf_set_irq_mask( FLD_RF_IRQ_RX | FLD_RF_IRQ_TX );


        IRQ_CONNECT(IRQ15_ZB_RT+CONFIG_2ND_LVL_ISR_TBL_OFFSET, 2, rf_irq_handler, NULL, 0);

   	    riscv_plic_irq_enable(IRQ15_ZB_RT);
	    riscv_plic_set_priority(IRQ15_ZB_RT, 2);

        tl_radio.tx_process = false;
        tl_radio.inited = true;
    }
}

bool tl_radio_set_channel( uint8_t channel ) {

    bool result = false;

    if ( channel >= TL_RADIO_MIN_CHANNEL && channel <= TL_RADIO_MAX_CHANNEL ) {
        tl_radio.channel = channel;
        result = true;
        rf_set_chn( ( tl_radio.channel - TL_RADIO_MIN_CHANNEL  + 1 ) * 5 );
    }

    return result;
}

uint8_t tl_radio_get_channel( void ) {

    return tl_radio.channel;
}

bool tl_radio_set_power_dbm( int8_t power_dbm ) {

    bool result = false;

    if ( power_dbm >= TL_RADIO_MIN_POWER_DBM && power_dbm <= TL_RADIO_MAX_POWER_DBM ) {
        tl_radio.power_dbm = power_dbm;
        result = true;

        static const rf_power_level_e chip_rf_power_dbm[] =  {
           	RF_POWER_N30dBm,        /* -30.0 dBm: -30 */
            RF_POWER_N30dBm,        /* -30.0 dBm: -29 */
            RF_POWER_N30dBm,        /* -30.0 dBm: -28 */
            RF_POWER_N30dBm,        /* -30.0 dBm: -27 */
            RF_POWER_N30dBm,        /* -30.0 dBm: -26 */
            RF_POWER_N22p67dBm,     /* -23.5 dBm: -25 */
            RF_POWER_N22p67dBm,     /* -23.5 dBm: -24 */
            RF_POWER_N22p67dBm,     /* -23.5 dBm: -23 */
            RF_POWER_N22p67dBm,     /* -23.5 dBm: -22 */
            RF_POWER_N22p67dBm,     /* -23.5 dBm: -21 */
            RF_POWER_N16p80dBm,     /* -17.8 dBm: -20 */
            RF_POWER_N16p80dBm,     /* -17.8 dBm: -19 */
            RF_POWER_N16p80dBm,     /* -17.8 dBm: -18 */
            RF_POWER_N16p80dBm,     /* -17.8 dBm: -17 */
            RF_POWER_N16p80dBm,     /* -17.8 dBm: -16 */
            RF_POWER_N13p40dBm,     /* -12.0 dBm: -15 */
            RF_POWER_N13p40dBm,     /* -12.0 dBm: -14 */
            RF_POWER_N13p40dBm,     /* -12.0 dBm: -13 */
            RF_POWER_N13p40dBm,     /* -12.0 dBm: -12 */
            RF_POWER_N13p40dBm,     /* -12.0 dBm: -11 */
            RF_POWER_N9p03dBm,      /*  -8.7 dBm: -10 */
            RF_POWER_N9p03dBm,      /*  -8.7 dBm:  -9 */
            RF_POWER_N9p03dBm,      /*  -8.7 dBm:  -8 */
            RF_POWER_N5p85dBm,      /*  -6.5 dBm:  -7 */
            RF_POWER_N5p85dBm,      /*  -6.5 dBm:  -6 */
            RF_POWER_N3p96dBm,      /*  -4.7 dBm:  -5 */
            RF_POWER_N3p96dBm,      /*  -4.7 dBm:  -4 */
            RF_POWER_N2p58dBm,      /*  -3.3 dBm:  -3 */
            RF_POWER_N1p38dBm,      /*  -2.0 dBm:  -2 */
            RF_POWER_N0p87dBm,      /*  -1.3 dBm:  -1 */
            RF_POWER_P0p04dBm,      /*   0.0 dBm:   0 */
            RF_POWER_P0p95dBm,      /*   0.8 dBm:   1 */
            RF_POWER_P1p57dBm,      /*   2.3 dBm:   2 */
            RF_POWER_P2p51dBm,      /*   3.2 dBm:   3 */
            RF_POWER_P3p52dBm,      /*   4.3 dBm:   4 */
            RF_POWER_P4p45dBm,      /*   5.6 dBm:   5 */
            RF_POWER_P5p21dBm,      /*   5.6 dBm:   6 */
            RF_POWER_P7p00dBm,      /*   6.9 dBm:   7 */
            RF_POWER_P8p25dBm,      /*   8.0 dBm:   8 */
            RF_POWER_P10p20dBm,      /*   9.1 dBm:   9 */
        };

        rf_set_power_level( chip_rf_power_dbm[ tl_radio.power_dbm - TL_RADIO_MIN_POWER_DBM ] );
    }

    return result;
}

int8_t tl_radio_get_power_dbm( void ) {

    return tl_radio.power_dbm;
}

bool tl_radio_transmitt( const void * data, uint8_t length ) {

    bool result = false;

    if ( tl_radio.inited && data && length ) {

        tl_radio_sync();
        tl_radio.tx_process = true;

        uint32_t rf_tx_dma_len = rf_tx_packet_dma_len( ( uint32_t )( length + 1 ) );

        tl_radio.tx_buffer[0] = rf_tx_dma_len & 0xff;
        tl_radio.tx_buffer[1] = ( rf_tx_dma_len >> 8 ) & 0xff;
        tl_radio.tx_buffer[2] = ( rf_tx_dma_len >> 16 ) & 0xff;
        tl_radio.tx_buffer[3] = ( rf_tx_dma_len >> 24 ) & 0xff;
        tl_radio.tx_buffer[4] = length + 2;
        memcpy( &tl_radio.tx_buffer[5], data, length );
        rf_set_txmode();
        delay_us( TL_RADIO_TX_GUARD_US );
        rf_tx_pkt( tl_radio.tx_buffer );

        result = true;
    }

    return result;
}

void tl_radio_sync( void ) {

    while ( tl_radio.inited && tl_radio.tx_process );
}

void tl_radio_set_ondata( tl_radio_ondata_t callback, void * ctx ) {

    tl_radio.ondata = callback;
    tl_radio.context = ctx;
}

void tl_radio_stop( void ) {

    if ( tl_radio.inited ) {

        tl_radio_sync();

        riscv_plic_irq_disable( IRQ15_ZB_RT );

        rf_set_tx_rx_off();

        tl_radio_baseband_reset();
        CLEAR_ALL_RFIRQ_STATUS;
        rf_clr_irq_mask( FLD_RF_IRQ_ALL );

        tl_radio.inited = false;
    }
}

void tl_radio_suspend_restore( void ) {

    if ( tl_radio.inited ) {
        rf_mode_init();
        rf_set_zigbee_250K_mode();
        tl_radio_set_channel( tl_radio.channel );
        tl_radio_set_power_dbm( tl_radio.power_dbm );
        // rf_set_rxmode(); /* newer do it with PM!!! fail after that */
    }
}


/*****************************************************************************
 * Radio unit private functions
 *****************************************************************************/

static void tl_radio_baseband_reset( void ) {
    reg_rst3 &= ( ~FLD_RST3_ZB );             /* reset baseband */
    reg_rst3 |= ( FLD_RST3_ZB );              /* clr baseband */
}
