/*
 * Copyright (c) 2021 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT telink_b91_zb

#include "rf.h"
#include "stimer.h"

#define LOG_MODULE_NAME ieee802154_b91
#if defined(CONFIG_IEEE802154_DRIVER_LOG_LEVEL)
#define LOG_LEVEL CONFIG_IEEE802154_DRIVER_LOG_LEVEL
#else
#define LOG_LEVEL LOG_LEVEL_NONE
#endif

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#include <zephyr/random/rand32.h>
#include <zephyr/net/ieee802154_radio.h>
#if defined(CONFIG_NET_L2_OPENTHREAD)
#include <zephyr/net/openthread.h>
#endif

#include <zephyr/pm/device.h>
#include <zephyr/pm/policy.h>

#include "ieee802154_b91.h"

#include "ieee802154_b91_frame.c"


#ifdef CONFIG_OPENTHREAD_FTD
/* B91 radio source match table structure */
static struct b91_src_match_table src_match_table;
#endif /* CONFIG_OPENTHREAD_FTD */

/* B91 data structure */
static struct  b91_data data = {
#ifdef CONFIG_OPENTHREAD_FTD
	.src_match_table = &src_match_table
#endif /* CONFIG_OPENTHREAD_FTD */
};

#ifdef CONFIG_OPENTHREAD_FTD

/* clean radio search match table */
static void b91_src_match_table_clean(struct b91_src_match_table *table)
{
	memset(table, 0, sizeof(struct b91_src_match_table));
}

/* Search in radio search match table */
static bool
inline b91_src_match_table_search(
	const struct b91_src_match_table *table, const uint8_t *addr, bool ext)
{
	bool result = false;

	for (size_t i = 0; i < CONFIG_OPENTHREAD_MAX_CHILDREN; i++) {
		if (table->item[i].valid && table->item[i].ext == ext &&
			!memcmp(table->item[i].addr, addr,
				ext ? IEEE802154_FRAME_LENGTH_ADDR_EXT :
				IEEE802154_FRAME_LENGTH_ADDR_SHORT)) {
			result = true;
			break;
		}
	}

	return result;
}

/* Add to radio search match table */
static void b91_src_match_table_add(
	struct b91_src_match_table *table, const uint8_t *addr, bool ext)
{
	if (!b91_src_match_table_search(table, addr, ext)) {
		for (size_t i = 0; i < CONFIG_OPENTHREAD_MAX_CHILDREN; i++) {
			if (!table->item[i].valid) {
				table->item[i].valid = true;
				table->item[i].ext = ext;
				memcpy(table->item[i].addr, addr,
					ext ? IEEE802154_FRAME_LENGTH_ADDR_EXT :
					IEEE802154_FRAME_LENGTH_ADDR_SHORT);
				break;
			}
		}
	}
}

/* Remove from radio search match table */
static void b91_src_match_table_remove(
	struct b91_src_match_table *table, const uint8_t *addr, bool ext)
{
	for (size_t i = 0; i < CONFIG_OPENTHREAD_MAX_CHILDREN; i++) {
		if (table->item[i].valid && table->item[i].ext == ext &&
			!memcmp(table->item[i].addr, addr,
				ext ? IEEE802154_FRAME_LENGTH_ADDR_EXT :
				IEEE802154_FRAME_LENGTH_ADDR_SHORT)) {
			table->item[i].valid = false;
			table->item[i].ext = false;
			memset(table->item[i].addr, 0,
				ext ? IEEE802154_FRAME_LENGTH_ADDR_EXT :
				IEEE802154_FRAME_LENGTH_ADDR_SHORT);
			break;
		}
	}
}

/* Remove all entries from radio search match table */
static void b91_src_match_table_remove_group(struct b91_src_match_table *table, bool ext)
{
	for (size_t i = 0; i < CONFIG_OPENTHREAD_MAX_CHILDREN; i++) {
		if (table->item[i].valid && table->item[i].ext == ext) {
			table->item[i].valid = false;
			table->item[i].ext = false;
			memset(table->item[i].addr, 0,
				ext ? IEEE802154_FRAME_LENGTH_ADDR_EXT :
				IEEE802154_FRAME_LENGTH_ADDR_SHORT);
		}
	}
}

/*
 * Check frame possible require to set pending bit
 * data request command or data
 * buffer should be valid
 */
static bool
inline b91_require_pending_bit(const struct ieee802154_frame *frame)
{
	bool result = false;

	if (frame->general.valid) {
		if (frame->general.type == IEEE802154_FRAME_FCF_TYPE_DATA) {
			result = true;
		} else if (frame->general.type == IEEE802154_FRAME_FCF_TYPE_CMD && frame->data) {
			if (frame->data[0] == B91_CMD_ID_DATA_REQ) {
				result = true;
			}
		}
	}
	return result;
}

#endif /* CONFIG_OPENTHREAD_FTD */

/* Disable power management by device */
static void b91_disable_pm(const struct device *dev)
{
#ifdef CONFIG_PM_DEVICE
	struct b91_data *b91 = dev->data;

	if (atomic_test_and_set_bit(&b91->current_pm_lock, 0) == 0) {
		pm_policy_state_lock_get(PM_STATE_SUSPEND_TO_IDLE, PM_ALL_SUBSTATES);
	}
#else
	ARG_UNUSED(dev);
#endif /* CONFIG_PM_DEVICE */
}

/* Enable power management by device */
static void b91_enable_pm(const struct device *dev)
{
#ifdef CONFIG_PM_DEVICE
	struct b91_data *b91 = dev->data;

	if (atomic_test_and_clear_bit(&b91->current_pm_lock, 0) == 1) {
		pm_policy_state_lock_put(PM_STATE_SUSPEND_TO_IDLE, PM_ALL_SUBSTATES);
	}
#else
	ARG_UNUSED(dev);
#endif /* CONFIG_PM_DEVICE */
}

/* Set filter PAN ID */
static int b91_set_pan_id(uint16_t pan_id)
{
	uint8_t pan_id_le[IEEE802154_FRAME_LENGTH_PANID];

	sys_put_le16(pan_id, pan_id_le);
	memcpy(data.filter_pan_id, pan_id_le, IEEE802154_FRAME_LENGTH_PANID);

	return 0;
}

/* Set filter short address */
static int b91_set_short_addr(uint16_t short_addr)
{
	uint8_t short_addr_le[IEEE802154_FRAME_LENGTH_ADDR_SHORT];

	sys_put_le16(short_addr, short_addr_le);
	memcpy(data.filter_short_addr, short_addr_le, IEEE802154_FRAME_LENGTH_ADDR_SHORT);

	return 0;
}

/* Set filter IEEE address */
static int b91_set_ieee_addr(const uint8_t *ieee_addr)
{
	memcpy(data.filter_ieee_addr, ieee_addr, IEEE802154_FRAME_LENGTH_ADDR_EXT);

	return 0;
}

/* Filter PAN ID, short address and IEEE address */
static bool
inline b91_run_filter(const struct ieee802154_frame *frame)
{
	bool result = false;

	do {
		if (frame->dst_panid != NULL) {
			if (memcmp(frame->dst_panid, data.filter_pan_id,
					IEEE802154_FRAME_LENGTH_PANID) != 0 &&
				memcmp(frame->dst_panid, B91_BROADCAST_ADDRESS,
					IEEE802154_FRAME_LENGTH_PANID) != 0) {
				break;
			}
		}
		if (frame->dst_addr != NULL) {
			if (frame->dst_addr_ext) {
				if ((net_if_get_link_addr(data.iface)->len !=
						IEEE802154_FRAME_LENGTH_ADDR_EXT) ||
					memcmp(frame->dst_addr, data.filter_ieee_addr,
						IEEE802154_FRAME_LENGTH_ADDR_EXT) != 0) {
					break;
				}
			} else {
				if (memcmp(frame->dst_addr, B91_BROADCAST_ADDRESS,
						IEEE802154_FRAME_LENGTH_ADDR_SHORT) != 0 &&
					memcmp(frame->dst_addr, data.filter_short_addr,
						IEEE802154_FRAME_LENGTH_ADDR_SHORT) != 0) {
					break;
				}
			}
		}
		result = true;
	} while (0);

	return result;
}

/* Get MAC address */
static inline uint8_t *b91_get_mac(const struct device *dev)
{
	struct b91_data *b91 = dev->data;

#if defined(CONFIG_IEEE802154_B91_RANDOM_MAC)
	uint32_t *ptr = (uint32_t *)(b91->mac_addr);

	UNALIGNED_PUT(sys_rand32_get(), ptr);
	ptr = (uint32_t *)(b91->mac_addr + 4);
	UNALIGNED_PUT(sys_rand32_get(), ptr);

	/*
	 * Clear bit 0 to ensure it isn't a multicast address and set
	 * bit 1 to indicate address is locally administered and may
	 * not be globally unique.
	 */
	b91->mac_addr[0] = (b91->mac_addr[0] & ~0x01) | 0x02;
#else
	/* Vendor Unique Identifier */
	b91->mac_addr[0] = 0xC4;
	b91->mac_addr[1] = 0x19;
	b91->mac_addr[2] = 0xD1;
	b91->mac_addr[3] = 0x00;

	/* Extended Unique Identifier */
	b91->mac_addr[4] = CONFIG_IEEE802154_B91_MAC4;
	b91->mac_addr[5] = CONFIG_IEEE802154_B91_MAC5;
	b91->mac_addr[6] = CONFIG_IEEE802154_B91_MAC6;
	b91->mac_addr[7] = CONFIG_IEEE802154_B91_MAC7;
#endif

	return b91->mac_addr;
}

/* Convert RSSI to LQI */
static uint8_t
inline b91_convert_rssi_to_lqi(int8_t rssi)
{
	uint32_t lqi32 = 0;

	/* check for MIN value */
	if (rssi < B91_RSSI_TO_LQI_MIN) {
		return 0;
	}

	/* convert RSSI to LQI */
	lqi32 = B91_RSSI_TO_LQI_SCALE * (rssi - B91_RSSI_TO_LQI_MIN);

	/* check for MAX value */
	if (lqi32 > 0xFF) {
		lqi32 = 0xFF;
	}

	return (uint8_t)lqi32;
}

/* Update RSSI and LQI parameters */
static void
inline b91_update_rssi_and_lqi(struct net_pkt *pkt)
{
	int8_t rssi;
	uint8_t lqi;

	rssi = ((signed char)(data.rx_buffer
			      [data.rx_buffer[B91_LENGTH_OFFSET] + B91_RSSI_OFFSET])) - 110;
	lqi = b91_convert_rssi_to_lqi(rssi);

	net_pkt_set_ieee802154_lqi(pkt, lqi);
	net_pkt_set_ieee802154_rssi(pkt, rssi);
}

/* Prepare TX buffer */
static void
inline b91_set_tx_payload(uint8_t *payload, uint8_t payload_len)
{
	unsigned char rf_data_len;
	unsigned int rf_tx_dma_len;

	rf_data_len = payload_len + 1;
	rf_tx_dma_len = rf_tx_packet_dma_len(rf_data_len);
	data.tx_buffer[0] = rf_tx_dma_len & 0xff;
	data.tx_buffer[1] = (rf_tx_dma_len >> 8) & 0xff;
	data.tx_buffer[2] = (rf_tx_dma_len >> 16) & 0xff;
	data.tx_buffer[3] = (rf_tx_dma_len >> 24) & 0xff;
	data.tx_buffer[4] = payload_len + 2;
	memcpy(data.tx_buffer + B91_PAYLOAD_OFFSET, payload, payload_len);
}

/* Handle acknowledge packet */
static void
inline b91_handle_ack(const void *buf, size_t buf_len)
{
	struct net_pkt *ack_pkt = net_pkt_alloc_with_buffer(
		data.iface, buf_len, AF_UNSPEC, 0, K_NO_WAIT);

	do {
		if (!ack_pkt) {
			LOG_ERR("No free packet available.");
			break;
		}
		if (net_pkt_write(ack_pkt, buf, buf_len) < 0) {
			LOG_ERR("Failed to write to a packet.");
			break;
		}
		b91_update_rssi_and_lqi(ack_pkt);
		net_pkt_cursor_init(ack_pkt);
		if (ieee802154_radio_handle_ack(data.iface, ack_pkt) != NET_OK) {
			LOG_INF("ACK packet not handled - releasing.");
		}
		k_sem_give(&data.ack_wait);
	} while (0);

	if (ack_pkt) {
		net_pkt_unref(ack_pkt);
	}
}

/* Send acknowledge packet */
static void
inline b91_send_ack(const struct ieee802154_frame *frame)
{
	uint8_t ack_buf[64];
	size_t ack_len;

	if (b91_ieee802154_frame_build(frame, ack_buf, sizeof(ack_buf), &ack_len)) {
		data.ack_sending = true;
		k_sem_reset(&data.tx_wait);
		b91_set_tx_payload(ack_buf, ack_len);
		rf_set_txmode();
		delay_us(CONFIG_IEEE802154_B91_SET_TXRX_DELAY_US);
		rf_tx_pkt(data.tx_buffer);
	} else {
		LOG_ERR("Failed to create ACK.");
	}
}

/* RX IRQ handler */
static void
__attribute__((section(".ram_code")))
b91_rf_rx_isr(void)
{
	int status = -EINVAL;
	struct net_pkt *pkt = NULL;

	dma_chn_dis(DMA1);
	rf_clr_irq_status(FLD_RF_IRQ_RX);

	do {
		if (!rf_zigbee_packet_crc_ok(data.rx_buffer)) {
			break;
		}
		uint8_t length = data.rx_buffer[B91_LENGTH_OFFSET];

		if ((length < B91_PAYLOAD_MIN) || (length > B91_PAYLOAD_MAX)) {
			LOG_ERR("Invalid length.\n");
			break;
		}
		uint8_t *payload = (data.rx_buffer + B91_PAYLOAD_OFFSET);
		struct ieee802154_frame frame;

		if (IS_ENABLED(CONFIG_IEEE802154_RAW_MODE) ||
			IS_ENABLED(CONFIG_NET_L2_OPENTHREAD)) {
			b91_ieee802154_frame_parse(payload, length - B91_FCS_LENGTH, &frame);
		} else {
			length -= B91_FCS_LENGTH;
			b91_ieee802154_frame_parse(payload, length, &frame);
		}
		if (!frame.general.valid) {
			LOG_ERR("Invalid frame\n");
			break;
		}
		if (frame.general.type == IEEE802154_FRAME_FCF_TYPE_ACK) {
			if (data.ack_handler_en) {
				b91_handle_ack(payload, length);
			}
			break;
		}
		if (!b91_run_filter(&frame)) {
			LOG_DBG("Packet received is not addressed to me.");
			break;
		}
		bool frame_pending = false;

		if (frame.general.ack_req) {
#ifdef CONFIG_OPENTHREAD_FTD
			if (b91_require_pending_bit(&frame)) {
				if (frame.src_addr) {
					if (!data.src_match_table->enabled ||
						b91_src_match_table_search(data.src_match_table,
							frame.src_addr, frame.src_addr_ext)) {
						frame_pending = true;
					}
				}
			}
#endif
			const struct ieee802154_frame ack_frame = {
				.general = {
					.valid = true,
					.ver = IEEE802154_FRAME_FCF_VER_2003,
					.type = IEEE802154_FRAME_FCF_TYPE_ACK,
					.fp_bit = frame_pending
				},
				.sn = frame.sn
			};
			b91_send_ack(&ack_frame);
		}
		pkt = net_pkt_alloc_with_buffer(data.iface, length, AF_UNSPEC, 0, K_NO_WAIT);
		if (!pkt) {
			LOG_ERR("No pkt available.");
			break;
		}
		net_pkt_set_ieee802154_ack_fpb(pkt, frame_pending);
		if (net_pkt_write(pkt, payload, length)) {
			LOG_ERR("Failed to write to a packet.");
			break;
		}
		b91_update_rssi_and_lqi(pkt);
		status = net_recv_data(data.iface, pkt);
		if (status < 0) {
			LOG_ERR("RCV Packet dropped by NET stack: %d", status);
		}
	} while (0);

	if (status < 0 && pkt != NULL) {
		net_pkt_unref(pkt);
	}
	dma_chn_en(DMA1);
}

/* TX IRQ handler */
static void b91_rf_tx_isr(void)
{
	/* clear irq status */
	rf_clr_irq_status(FLD_RF_IRQ_TX);

	/* ack sent */
	data.ack_sending = false;

	/* release tx semaphore */
	k_sem_give(&data.tx_wait);

	/* set to rx mode */
	rf_set_rxmode();
}

/* IRQ handler */
static void b91_rf_isr(void)
{
	if (rf_get_irq_status(FLD_RF_IRQ_RX)) {
		b91_rf_rx_isr();
	} else if (rf_get_irq_status(FLD_RF_IRQ_TX)) {
		b91_rf_tx_isr();
	} else {
		rf_clr_irq_status(FLD_RF_IRQ_ALL);
	}
}

/* Driver initialization */
static int b91_init(const struct device *dev)
{
	struct b91_data *b91 = dev->data;

	/* init semaphores */
	k_sem_init(&b91->tx_wait, 0, 1);
	k_sem_init(&b91->ack_wait, 0, 1);

	/* init rf module */
	rf_mode_init();
	rf_set_zigbee_250K_mode();
	rf_set_tx_dma(1, B91_TRX_LENGTH);
	rf_set_rx_dma(data.rx_buffer, 0, B91_TRX_LENGTH);
	rf_set_txmode();
	rf_set_rxmode();

	/* init IRQs */
	IRQ_CONNECT(DT_INST_IRQN(0), DT_INST_IRQ(0, priority), b91_rf_isr, 0, 0);
	riscv_plic_irq_enable(DT_INST_IRQN(0) - CONFIG_2ND_LVL_ISR_TBL_OFFSET);
	riscv_plic_set_priority(DT_INST_IRQN(0) - CONFIG_2ND_LVL_ISR_TBL_OFFSET,
				DT_INST_IRQ(0, priority));
	rf_set_irq_mask(FLD_RF_IRQ_RX | FLD_RF_IRQ_TX);

	/* init data variables */
	data.is_started = true;
	data.ack_handler_en = false;
	data.ack_sending = false;
	data.current_channel = 0xFFFF;
	data.current_dbm = 0x7FFF;
#ifdef CONFIG_OPENTHREAD_FTD
	b91_src_match_table_clean(b91->src_match_table);
#endif /* CONFIG_OPENTHREAD_FTD */

	return 0;
}

/* API implementation: iface_init */
static void b91_iface_init(struct net_if *iface)
{
	const struct device *dev = net_if_get_device(iface);
	struct b91_data *b91 = dev->data;
	uint8_t *mac = b91_get_mac(dev);

	net_if_set_link_addr(iface, mac, IEEE802154_FRAME_LENGTH_ADDR_EXT, NET_LINK_IEEE802154);

	b91->iface = iface;

	ieee802154_init(iface);
}

/* API implementation: get_capabilities */
static enum ieee802154_hw_caps b91_get_capabilities(const struct device *dev)
{
	ARG_UNUSED(dev);

	return IEEE802154_HW_FCS | IEEE802154_HW_2_4_GHZ |
	       IEEE802154_HW_FILTER | IEEE802154_HW_TX_RX_ACK;
}

/* API implementation: cca */
static int b91_cca(const struct device *dev)
{
	ARG_UNUSED(dev);

	unsigned int t1 = stimer_get_tick();

	while (!clock_time_exceed(t1, B91_CCA_TIME_MAX_US)) {
		if (rf_get_rssi() < CONFIG_IEEE802154_B91_CCA_RSSI_THRESHOLD) {
			return 0;
		}
	}

	return -EBUSY;
}

/* API implementation: set_channel */
static int b91_set_channel(const struct device *dev, uint16_t channel)
{
	ARG_UNUSED(dev);

	if (channel < 11 || channel > 26) {
		return -EINVAL;
	}

	if (data.current_channel != channel) {
		data.current_channel = channel;
		rf_set_chn(B91_LOGIC_CHANNEL_TO_PHYSICAL(channel));
		rf_set_txmode();
		rf_set_rxmode();
	}

	return 0;
}

/* API implementation: filter */
static int b91_filter(const struct device *dev,
		      bool set,
		      enum ieee802154_filter_type type,
		      const struct ieee802154_filter *filter)
{
	if (!set) {
		return -ENOTSUP;
	}

	if (type == IEEE802154_FILTER_TYPE_IEEE_ADDR) {
		return b91_set_ieee_addr(filter->ieee_addr);
	} else if (type == IEEE802154_FILTER_TYPE_SHORT_ADDR) {
		return b91_set_short_addr(filter->short_addr);
	} else if (type == IEEE802154_FILTER_TYPE_PAN_ID) {
		return b91_set_pan_id(filter->pan_id);
	}

	return -ENOTSUP;
}

/* API implementation: set_txpower */
static int b91_set_txpower(const struct device *dev, int16_t dbm)
{
	ARG_UNUSED(dev);

	/* check for supported Min/Max range */
	if (dbm < B91_TX_POWER_MIN) {
		dbm = B91_TX_POWER_MIN;
	} else if (dbm > B91_TX_POWER_MAX) {
		dbm = B91_TX_POWER_MAX;
	}

	if (data.current_dbm != dbm) {
		data.current_dbm = dbm;
		/* set TX power */
		rf_set_power_level(b91_tx_pwr_lt[dbm - B91_TX_POWER_MIN]);
	}

	return 0;
}

/* API implementation: start */
static int b91_start(const struct device *dev)
{
	b91_disable_pm(dev);
	/* check if RF is already started */
	if (!data.is_started) {
		rf_set_txmode();
		rf_set_rxmode();
		delay_us(CONFIG_IEEE802154_B91_SET_TXRX_DELAY_US);
		riscv_plic_irq_enable(DT_INST_IRQN(0) - CONFIG_2ND_LVL_ISR_TBL_OFFSET);
		data.is_started = true;
	}

	return 0;
}

/* API implementation: stop */
static int b91_stop(const struct device *dev)
{
	/* check if RF is already stopped */
	if (data.is_started) {
		if (data.ack_sending) {
			if (k_sem_take(&data.tx_wait, K_MSEC(B91_TX_WAIT_TIME_MS)) != 0) {
				data.ack_sending = false;
			}
		}
		riscv_plic_irq_disable(DT_INST_IRQN(0) - CONFIG_2ND_LVL_ISR_TBL_OFFSET);
		rf_set_tx_rx_off();
		delay_us(CONFIG_IEEE802154_B91_SET_TXRX_DELAY_US);
		data.is_started = false;
	}
	b91_enable_pm(dev);

	return 0;
}

/* API implementation: tx */
static int b91_tx(const struct device *dev,
		  enum ieee802154_tx_mode mode,
		  struct net_pkt *pkt,
		  struct net_buf *frag)
{
	ARG_UNUSED(pkt);

	int status = 0;
	struct b91_data *b91 = dev->data;

	/* check for supported mode */
	if (mode != IEEE802154_TX_MODE_DIRECT) {
		LOG_WRN("TX mode %d not supported", mode);
		return -ENOTSUP;
	}

	if (data.ack_sending) {
		if (k_sem_take(&data.tx_wait, K_MSEC(B91_TX_WAIT_TIME_MS)) != 0) {
			data.ack_sending = false;
			rf_set_rxmode();
		}
	}

	/* prepare tx buffer */
	b91_set_tx_payload(frag->data, frag->len);

	/* reset semaphores */
	k_sem_reset(&b91->tx_wait);
	k_sem_reset(&b91->ack_wait);

	/* start transmission */
	rf_set_txmode();
	delay_us(CONFIG_IEEE802154_B91_SET_TXRX_DELAY_US);
	rf_tx_pkt(data.tx_buffer);

	/* wait for tx done */
	if (k_sem_take(&data.tx_wait, K_MSEC(B91_TX_WAIT_TIME_MS)) != 0) {
		rf_set_rxmode();
		status = -EIO;
	}

	/* wait for ACK if requested */
	if (!status && (frag->data[0] & IEEE802154_FRAME_FCF_ACK_REQ_MASK) ==
		IEEE802154_FRAME_FCF_ACK_REQ_ON) {
		data.ack_handler_en = true;
		if (k_sem_take(&b91->ack_wait, K_MSEC(B91_ACK_WAIT_TIME_MS)) != 0) {
			status = -ENOMSG;
		}
		data.ack_handler_en = false;
	}

	return status;
}

/* API implementation: ed_scan */
static int b91_ed_scan(const struct device *dev, uint16_t duration,
		       energy_scan_done_cb_t done_cb)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(duration);
	ARG_UNUSED(done_cb);

	/* ed_scan not supported */

	return -ENOTSUP;
}

/* API implementation: configure */
static int b91_configure(const struct device *dev,
			 enum ieee802154_config_type type,
			 const struct ieee802154_config *config)
{
#ifdef CONFIG_OPENTHREAD_FTD
	struct b91_data *b91 = dev->data;
#else
	ARG_UNUSED(dev);
	ARG_UNUSED(config);
#endif /* CONFIG_OPENTHREAD_FTD */
	int result = 0;

	switch (type) {
#ifdef CONFIG_OPENTHREAD_FTD
	case IEEE802154_CONFIG_AUTO_ACK_FPB:
		if (config->auto_ack_fpb.mode == IEEE802154_FPB_ADDR_MATCH_THREAD) {
			b91->src_match_table->enabled = config->auto_ack_fpb.enabled;
		} else {
			result = -ENOTSUP;
		}
		break;
	case IEEE802154_CONFIG_ACK_FPB:
		riscv_plic_irq_disable(DT_INST_IRQN(0) - CONFIG_2ND_LVL_ISR_TBL_OFFSET);
		if (config->ack_fpb.addr) {
			if (config->ack_fpb.enabled) {
				b91_src_match_table_add(b91->src_match_table,
					config->ack_fpb.addr, config->ack_fpb.extended);
			} else {
				b91_src_match_table_remove(b91->src_match_table,
					config->ack_fpb.addr, config->ack_fpb.extended);
			}
		} else if (!config->ack_fpb.enabled) {
			b91_src_match_table_remove_group(b91->src_match_table,
				config->ack_fpb.extended);
		} else {
			result = -ENOTSUP;
		}
		riscv_plic_irq_enable(DT_INST_IRQN(0) - CONFIG_2ND_LVL_ISR_TBL_OFFSET);
		break;
#endif /* CONFIG_OPENTHREAD_FTD */
	default:
		result = -ENOTSUP;
		break;
	}

	return result;
}

/* API implementation: get_sch_acc */
static uint8_t b91_get_sch_acc(const struct device *dev)
{
	ARG_UNUSED(dev);

	return CONFIG_IEEE802154_B91_DELAY_TRX_ACC;
}

/* IEEE802154 driver APIs structure */
static struct ieee802154_radio_api b91_radio_api = {
	.iface_api.init = b91_iface_init,
	.get_capabilities = b91_get_capabilities,
	.cca = b91_cca,
	.set_channel = b91_set_channel,
	.filter = b91_filter,
	.set_txpower = b91_set_txpower,
	.start = b91_start,
	.stop = b91_stop,
	.tx = b91_tx,
	.ed_scan = b91_ed_scan,
	.configure = b91_configure,
	.get_sch_acc = b91_get_sch_acc,
};


#if defined(CONFIG_NET_L2_IEEE802154)
#define L2 IEEE802154_L2
#define L2_CTX_TYPE NET_L2_GET_CTX_TYPE(IEEE802154_L2)
#define MTU 125
#elif defined(CONFIG_NET_L2_OPENTHREAD)
#define L2 OPENTHREAD_L2
#define L2_CTX_TYPE NET_L2_GET_CTX_TYPE(OPENTHREAD_L2)
#define MTU 1280
#endif

#ifdef CONFIG_PM_DEVICE
static int ieee802154_b91_pm_action(const struct device *dev, enum pm_device_action action)
{
	ARG_UNUSED(dev);

	switch (action) {
	case PM_DEVICE_ACTION_RESUME:
		/* restart radio */
		rf_mode_init();
		rf_set_zigbee_250K_mode();
		rf_set_chn(B91_LOGIC_CHANNEL_TO_PHYSICAL(data.current_channel));
		rf_set_power_level(b91_tx_pwr_lt[data.current_dbm - B91_TX_POWER_MIN]);
		rf_set_txmode();
		rf_set_rxmode();
		break;

	case PM_DEVICE_ACTION_SUSPEND:
		break;

	default:
		return -ENOTSUP;
	}

	return 0;
}
PM_DEVICE_DEFINE(ieee802154_b91_pm, ieee802154_b91_pm_action);
#define ieee802154_b91_pm_device	PM_DEVICE_GET(ieee802154_b91_pm)
#else
#define ieee802154_b91_pm_device	NULL
#endif

/* IEEE802154 driver registration */
#if defined(CONFIG_NET_L2_IEEE802154) || defined(CONFIG_NET_L2_OPENTHREAD)
NET_DEVICE_DT_INST_DEFINE(0, b91_init, ieee802154_b91_pm_device, &data, NULL,
			  CONFIG_IEEE802154_B91_INIT_PRIO,
			  &b91_radio_api, L2, L2_CTX_TYPE, MTU);
#else
DEVICE_DT_INST_DEFINE(0, b91_init, ieee802154_b91_pm_device, &data, NULL,
		      POST_KERNEL, CONFIG_IEEE802154_B91_INIT_PRIO,
		      &b91_radio_api);
#endif
