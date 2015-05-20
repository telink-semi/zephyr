/* conn.c - Bluetooth connection handling */

/*
 * Copyright (c) 2015 Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1) Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3) Neither the name of Intel Corporation nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <nanokernel.h>
#include <toolchain.h>
#include <string.h>
#include <errno.h>
#include <misc/byteorder.h>
#include <misc/util.h>

#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>

#include "hci_core.h"
#include "conn.h"
#include "l2cap.h"

#if !defined(CONFIG_BLUETOOTH_DEBUG_CONN)
#undef BT_DBG
#define BT_DBG(fmt, ...)
#endif

#define MAX_CONN_COUNT 1
static struct bt_conn conns[MAX_CONN_COUNT];

static void bt_conn_reset_rx_state(struct bt_conn *conn)
{
	if (!conn->rx_len) {
		return;
	}

	bt_buf_put(conn->rx);
	conn->rx = NULL;
	conn->rx_len = 0;
}

void bt_conn_recv(struct bt_conn *conn, struct bt_buf *buf, uint8_t flags)
{
	struct bt_l2cap_hdr *hdr;
	uint16_t len;

	BT_DBG("handle %u len %u flags %x\n", conn->handle, buf->len, flags);

	/* Check packet boundary flags */
	switch (flags) {
	case 0x02:
		/* First packet */
		hdr = (void *)buf->data;
		len = sys_le16_to_cpu(hdr->len);

		BT_DBG("First, len %u final %u\n", buf->len, len);

		if (conn->rx_len) {
			BT_ERR("Unexpected first L2CAP frame\n");
			bt_conn_reset_rx_state(conn);
		}

		conn->rx_len = (sizeof(*hdr) + len) - buf->len;
		BT_DBG("rx_len %u\n", conn->rx_len);
		if (conn->rx_len) {
			conn->rx = buf;
			return;
		}

		break;
	case 0x01:
		/* Continuation */
		if (!conn->rx_len) {
			BT_ERR("Unexpected L2CAP continuation\n");
			bt_conn_reset_rx_state(conn);
			bt_buf_put(buf);
			return;
		}

		if (buf->len > conn->rx_len) {
			BT_ERR("L2CAP data overflow\n");
			bt_conn_reset_rx_state(conn);
			bt_buf_put(buf);
			return;
		}

		BT_DBG("Cont, len %u rx_len %u\n", buf->len, conn->rx_len);

		if (buf->len > bt_buf_tailroom(conn->rx)) {
			BT_ERR("Not enough buffer space for L2CAP data\n");
			bt_conn_reset_rx_state(conn);
			bt_buf_put(buf);
			return;
		}

		memcpy(bt_buf_add(conn->rx, buf->len), buf->data, buf->len);
		conn->rx_len -= buf->len;
		bt_buf_put(buf);

		if (conn->rx_len) {
			return;
		}

		buf = conn->rx;
		conn->rx = NULL;
		conn->rx_len = 0;

		break;
	default:
		BT_ERR("Unexpected ACL flags (%u)\n", flags);
		bt_conn_reset_rx_state(conn);
		bt_buf_put(buf);
		return;
	}

	hdr = (void *)buf->data;
	len = sys_le16_to_cpu(hdr->len);

	if (sizeof(*hdr) + len != buf->len) {
		BT_ERR("ACL len mismatch (%u != %u)\n", len, buf->len);
		bt_buf_put(buf);
		return;
	}

	BT_DBG("Successfully parsed %u byte L2CAP packet\n", buf->len);

	nano_fiber_fifo_put(&conn->rx_queue, buf);
}

void bt_conn_send(struct bt_conn *conn, struct bt_buf *buf)
{
	uint16_t len, remaining = buf->len;
	struct bt_dev *dev = conn->dev;
	struct bt_hci_acl_hdr *hdr;
	struct nano_fifo frags;
	uint8_t *ptr;

	BT_DBG("conn handle %u buf len %u\n", conn->handle, buf->len);

	nano_fifo_init(&frags);

	len = min(remaining, dev->le_mtu);

	hdr = bt_buf_push(buf, sizeof(*hdr));
	hdr->handle = sys_cpu_to_le16(conn->handle);
	hdr->len = sys_cpu_to_le16(len);

	buf->len -= remaining - len;
	ptr = bt_buf_tail(buf);

	nano_fifo_put(&frags, buf);
	remaining -= len;

	while (remaining) {
		buf = bt_conn_create_pdu(conn);

		len = min(remaining, dev->le_mtu);

		/* Copy from original buffer */
		memcpy(bt_buf_add(buf, len), ptr, len);
		ptr += len;

		hdr = bt_buf_push(buf, sizeof(*hdr));
		hdr->handle = sys_cpu_to_le16(conn->handle | (1 << 12));
		hdr->len = sys_cpu_to_le16(len);

		nano_fifo_put(&frags, buf);
		remaining -= len;
	}

	while ((buf = nano_fifo_get(&frags))) {
		nano_fifo_put(&conn->tx_queue, buf);
	}
}

static void conn_rx_fiber(int arg1, int arg2)
{
	struct bt_conn *conn = (struct bt_conn *)arg1;
	struct bt_buf *buf;

	BT_DBG("Started for handle %u\n", conn->handle);

	while (conn->state == BT_CONN_CONNECTED) {
		BT_DBG("calling fifo_get_wait\n");
		buf = nano_fifo_get_wait(&conn->rx_queue);

		/* check for disconnection */
		if (conn->state != BT_CONN_CONNECTED) {
			bt_buf_put(buf);
			break;
		}

		BT_DBG("passing buf %p len %u to L2CAP\n", buf, buf->len);
		bt_l2cap_recv(conn, buf);
	}

	BT_DBG("handle %u disconnected - cleaning up\n", conn->handle);

	/* Give back any allocated buffers */
	while ((buf = nano_fifo_get(&conn->rx_queue))) {
		bt_buf_put(buf);
	}

	bt_conn_reset_rx_state(conn);

	BT_DBG("handle %u exiting\n", conn->handle);
	bt_conn_put(conn);
}

static void conn_tx_fiber(int arg1, int arg2)
{
	struct bt_conn *conn = (struct bt_conn *)arg1;
	struct bt_dev *dev = conn->dev;
	struct bt_buf *buf;

	BT_DBG("Started for handle %u\n", conn->handle);

	while (conn->state == BT_CONN_CONNECTED) {
		/* Wait until the controller can accept ACL packets */
		BT_DBG("calling sem_take_wait\n");
		nano_fiber_sem_take_wait(&dev->le_pkts_sem);

		/* check for disconnection */
		if (conn->state != BT_CONN_CONNECTED) {
			nano_fiber_sem_give(&dev->le_pkts_sem);
			break;
		}

		/* Get next ACL packet for connection */
		buf = nano_fifo_get_wait(&conn->tx_queue);
		if (conn->state != BT_CONN_CONNECTED) {
			nano_fiber_sem_give(&dev->le_pkts_sem);
			bt_buf_put(buf);
			break;
		}

		BT_DBG("passing buf %p len %u to driver\n", buf, buf->len);
		dev->drv->send(buf);
		bt_buf_put(buf);
	}

	BT_DBG("handle %u disconnected - cleaning up\n", conn->handle);

	/* Give back any allocated buffers */
	while ((buf = nano_fifo_get(&conn->tx_queue))) {
		bt_buf_put(buf);
	}

	BT_DBG("handle %u exiting\n", conn->handle);
	bt_conn_put(conn);
}

struct bt_conn *bt_conn_add(struct bt_dev *dev, uint16_t handle)
{
	struct bt_conn *conn = NULL;
	int i;

	for (i = 0; i < MAX_CONN_COUNT; i++) {
		if (!conns[i].handle) {
			conn = &conns[i];
			break;
		}
	}

	if (!conn) {
		return NULL;
	}

	memset(conn, 0, sizeof(*conn));

	conn->ref	= 1;
	conn->state	= BT_CONN_CONNECTED;
	conn->handle	= handle;
	conn->dev	= dev;

	nano_fifo_init(&conn->tx_queue);
	nano_fifo_init(&conn->rx_queue);

	fiber_start(conn->rx_stack, BT_CONN_RX_STACK_SIZE, conn_rx_fiber,
		    (int)bt_conn_get(conn), 0, 7, 0);

	fiber_start(conn->tx_stack, BT_CONN_TX_STACK_SIZE, conn_tx_fiber,
		    (int)bt_conn_get(conn), 0, 7, 0);

	bt_l2cap_update_conn_param(conn);

	return conn;
}

void bt_conn_del(struct bt_conn *conn)
{
	BT_DBG("handle %u\n", conn->handle);

	if (conn->state != BT_CONN_CONNECTED) {
		return;
	}

	conn->state = BT_CONN_DISCONNECTED;

	/* Send dummy buffers to wake up and kill the fibers */
	nano_fifo_put(&conn->tx_queue, bt_buf_get(BT_DUMMY, 0));
	nano_fifo_put(&conn->rx_queue, bt_buf_get(BT_DUMMY, 0));

	bt_conn_put(conn);
}

struct bt_conn *bt_conn_lookup(uint16_t handle)
{
	int i;

	for (i = 0; i < MAX_CONN_COUNT; i++) {
		if (conns[i].state != BT_CONN_CONNECTED) {
			continue;
		}

		if (conns[i].handle == handle) {
			return &conns[i];
		}
	}

	return NULL;
}

struct bt_conn *bt_conn_get(struct bt_conn *conn)
{
	conn->ref++;

	BT_DBG("handle %u ref %u\n", conn->handle, conn->ref);

	return conn;
}

void bt_conn_put(struct bt_conn *conn)
{
	conn->ref--;

	BT_DBG("handle %u ref %u\n", conn->handle, conn->ref);

	if (conn->ref) {
		return;
	}

	conn->handle = 0;
}

struct bt_buf *bt_conn_create_pdu(struct bt_conn *conn)
{
	size_t reserve = conn->dev->drv->head_reserve;

	return bt_buf_get(BT_ACL_OUT, reserve + sizeof(struct bt_hci_acl_hdr));
}
