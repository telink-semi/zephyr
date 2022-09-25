/*
 * Copyright (c) 2021 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <zephyr/toolchain/common.h>


/* frame control field byte 0 */
#define IEEE802154_FRAME_FCF_TYPE_MASK             (0x07)
#define IEEE802154_FRAME_FCF_TYPE_BEACON           (0x00)
#define IEEE802154_FRAME_FCF_TYPE_DATA             (0x01)
#define IEEE802154_FRAME_FCF_TYPE_ACK              (0x02)
#define IEEE802154_FRAME_FCF_TYPE_CMD              (0x03)
#define IEEE802154_FRAME_FCF_SECURITY_EN_MASK      (0x08)
#define IEEE802154_FRAME_FCF_SECURITY_EN_ON        (0x08)
#define IEEE802154_FRAME_FCF_SECURITY_EN_OFF       (0x00)
#define IEEE802154_FRAME_FCF_PENDING_MASK          (0x10)
#define IEEE802154_FRAME_FCF_PENDING_ON            (0x10)
#define IEEE802154_FRAME_FCF_PENDING_OFF           (0x10)
#define IEEE802154_FRAME_FCF_ACK_REQ_MASK          (0x20)
#define IEEE802154_FRAME_FCF_ACK_REQ_ON            (0x20)
#define IEEE802154_FRAME_FCF_ACK_REQ_OFF           (0x20)
#define IEEE802154_FRAME_FCF_PANID_COMP_MASK       (0x40)
#define IEEE802154_FRAME_FCF_PANID_COMP_ON         (0x40)
#define IEEE802154_FRAME_FCF_PANID_COMP_OFF        (0x00)

/* frame control field byte 1 */
#define IEEE802154_FRAME_FCF_SN_SUP_MASK           (0x01)
#define IEEE802154_FRAME_FCF_SN_SUP_ON             (0x01)
#define IEEE802154_FRAME_FCF_SN_SUP_OFF            (0x00)
#define IEEE802154_FRAME_FCF_IE_MASK               (0x02)
#define IEEE802154_FRAME_FCF_IE_ON                 (0x02)
#define IEEE802154_FRAME_FCF_IE_OFF                (0x00)
#define IEEE802154_FRAME_FCF_DST_ADDR_T_MASK       (0x0c)
#define IEEE802154_FRAME_FCF_DST_ADDR_T_NA         (0x00)
#define IEEE802154_FRAME_FCF_DST_ADDR_T_SHORT      (0x08)
#define IEEE802154_FRAME_FCF_DST_ADDR_T_EXT        (0x0c)
#define IEEE802154_FRAME_FCF_VER_MASK              (0x30)
#define IEEE802154_FRAME_FCF_VER_OFS               (4)
#define IEEE802154_FRAME_FCF_VER_2003              (0x00)
#define IEEE802154_FRAME_FCF_VER_2006              (0x01)
#define IEEE802154_FRAME_FCF_VER_2015              (0x02)
#define IEEE802154_FRAME_FCF_SRC_ADDR_T_MASK       (0xc0)
#define IEEE802154_FRAME_FCF_SRC_ADDR_T_NA         (0x00)
#define IEEE802154_FRAME_FCF_SRC_ADDR_T_SHORT      (0x80)
#define IEEE802154_FRAME_FCF_SRC_ADDR_T_EXT        (0xc0)

/* security control byte */
#define IEEE802154_FRAME_SECCTRL_KEY_ID_MODE_MASK  (0x18)
#define IEEE802154_FRAME_SECCTRL_KEY_ID_MODE_0     (0x00)
#define IEEE802154_FRAME_SECCTRL_KEY_ID_MODE_1     (0x08)
#define IEEE802154_FRAME_SECCTRL_KEY_ID_MODE_2     (0x10)
#define IEEE802154_FRAME_SECCTRL_KEY_ID_MODE_3     (0x18)

/* IE header byte 0 */
#define IEEE802154_FRAME_IE_HEADER_LEN_MASK        (0x7f)
#define IEEE802154_FRAME_IE_HEADER_TYPE_L_MASK     (0x80)
#define IEEE802154_FRAME_IE_HEADER_TYPE_L_OFS      (7)

/* IE header byte 1 */
#define IEEE802154_FRAME_IE_HEADER_TYPE_H_MASK     (0x7f)
#define IEEE802154_FRAME_IE_HEADER_TYPE_H_OFS      (1)

/* IE header types */
#define IEEE802154_FRAME_IE_HEADER_TYPE_TERM       (0x7f)

/* elements lengths */
#define IEEE802154_FRAME_LENGTH_FCF                (2)
#define IEEE802154_FRAME_LENGTH_SN                 (1)
#ifndef IEEE802154_FRAME_LENGTH_PANID
#define IEEE802154_FRAME_LENGTH_PANID              (2)
#endif
#ifndef IEEE802154_FRAME_LENGTH_ADDR_SHORT
#define IEEE802154_FRAME_LENGTH_ADDR_SHORT         (2)
#endif
#ifndef IEEE802154_FRAME_LENGTH_ADDR_EXT
#define IEEE802154_FRAME_LENGTH_ADDR_EXT           (8)
#endif
#define IEEE802154_FRAME_LENGTH_SEC_HEADER         (1)
#define IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_0  (4)
#define IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_1  (5)
#define IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_2  (9)
#define IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_3  (13)
#define IEEE802154_FRAME_LENGTH_IE_HEADER          (2)


/* ieee802154_frame structure */
struct ieee802154_frame {
	struct {
		bool valid;
		uint8_t ver;
		uint8_t type;
		bool ack_req;
		bool fp_bit;
	} general;
	const uint8_t *sn;
	const uint8_t *dst_panid;
	const uint8_t *dst_addr;
	bool dst_addr_ext;
	const uint8_t *src_panid;
	const uint8_t *src_addr;
	bool src_addr_ext;
	const uint8_t *sec_header;
	size_t sec_header_len;
	const uint8_t *ie_header;
	size_t ie_header_len;
	const uint8_t *data;
	size_t data_len;
};


/*
 * Check if FCF describes destination PANID
 * fcf should be valid and contains at lest 2 bytes
 */
static bool
ALWAYS_INLINE ieee802154_frame_has_dest_panid(const uint8_t fcf[2])
{
	bool result = true;
	const uint8_t frame_ver_t = (fcf[1] & IEEE802154_FRAME_FCF_VER_MASK) >>
		IEEE802154_FRAME_FCF_VER_OFS;
	const uint8_t dst_addr_t = fcf[1] & IEEE802154_FRAME_FCF_DST_ADDR_T_MASK;
	const uint8_t src_addr_t = fcf[1] & IEEE802154_FRAME_FCF_SRC_ADDR_T_MASK;
	const uint8_t panid_compr_t = fcf[0] & IEEE802154_FRAME_FCF_PANID_COMP_MASK;

	if (frame_ver_t == IEEE802154_FRAME_FCF_VER_2015) {
		if ((dst_addr_t == IEEE802154_FRAME_FCF_DST_ADDR_T_NA &&
				src_addr_t == IEEE802154_FRAME_FCF_SRC_ADDR_T_NA &&
				panid_compr_t == IEEE802154_FRAME_FCF_PANID_COMP_OFF) ||
			(dst_addr_t == IEEE802154_FRAME_FCF_DST_ADDR_T_EXT &&
				src_addr_t == IEEE802154_FRAME_FCF_SRC_ADDR_T_NA &&
				panid_compr_t == IEEE802154_FRAME_FCF_PANID_COMP_ON) ||
			(dst_addr_t == IEEE802154_FRAME_FCF_DST_ADDR_T_SHORT &&
				src_addr_t == IEEE802154_FRAME_FCF_SRC_ADDR_T_NA &&
				panid_compr_t == IEEE802154_FRAME_FCF_PANID_COMP_ON) ||
			(dst_addr_t == IEEE802154_FRAME_FCF_DST_ADDR_T_NA &&
				src_addr_t == IEEE802154_FRAME_FCF_SRC_ADDR_T_EXT &&
				panid_compr_t == IEEE802154_FRAME_FCF_PANID_COMP_OFF) ||
			(dst_addr_t == IEEE802154_FRAME_FCF_DST_ADDR_T_NA &&
				src_addr_t == IEEE802154_FRAME_FCF_SRC_ADDR_T_SHORT &&
				panid_compr_t == IEEE802154_FRAME_FCF_PANID_COMP_OFF) ||
			(dst_addr_t == IEEE802154_FRAME_FCF_DST_ADDR_T_NA &&
				src_addr_t == IEEE802154_FRAME_FCF_SRC_ADDR_T_EXT &&
				panid_compr_t == IEEE802154_FRAME_FCF_PANID_COMP_ON) ||
			(dst_addr_t == IEEE802154_FRAME_FCF_DST_ADDR_T_NA &&
				src_addr_t == IEEE802154_FRAME_FCF_SRC_ADDR_T_SHORT &&
				panid_compr_t == IEEE802154_FRAME_FCF_PANID_COMP_ON) ||
			(dst_addr_t == IEEE802154_FRAME_FCF_DST_ADDR_T_EXT &&
				src_addr_t == IEEE802154_FRAME_FCF_SRC_ADDR_T_EXT &&
				panid_compr_t == IEEE802154_FRAME_FCF_PANID_COMP_ON)) {
			result = false;
		}
	} else {
		if (dst_addr_t == IEEE802154_FRAME_FCF_DST_ADDR_T_NA) {
			result = false;
		}
	}

	return result;
}

/*
 * return PANID compression bit
 * frame should be valid
 */
static bool
ALWAYS_INLINE ieee802154_frame_panid_compression(const struct ieee802154_frame *frame)
{
	bool result = false;

	if (frame->general.ver == IEEE802154_FRAME_FCF_VER_2015) {
		if ((frame->dst_addr == NULL && frame->src_addr == NULL &&
				frame->dst_panid != NULL && frame->src_panid == NULL) ||
			(frame->dst_addr != NULL && frame->src_addr == NULL &&
				frame->dst_panid == NULL && frame->src_panid == NULL) ||
			(frame->dst_addr == NULL && frame->src_addr != NULL &&
				frame->dst_panid == NULL && frame->src_panid == NULL) ||
			(frame->dst_addr != NULL && frame->dst_addr_ext &&
				frame->src_addr != NULL && frame->src_addr_ext &&
				frame->dst_panid == NULL && frame->src_panid == NULL) ||
			(frame->dst_addr != NULL && !frame->dst_addr_ext &&
				frame->src_addr != NULL && frame->src_addr_ext &&
				frame->dst_panid != NULL && frame->src_panid == NULL) ||
			(frame->dst_addr != NULL && frame->dst_addr_ext &&
				frame->src_addr != NULL && !frame->src_addr_ext &&
				frame->dst_panid != NULL && frame->src_panid == NULL) ||
			(frame->dst_addr != NULL && !frame->dst_addr_ext &&
				frame->src_addr != NULL && !frame->src_addr_ext &&
				frame->dst_panid != NULL && frame->src_panid == NULL)) {
			result = true;
		}
	} else {
		if ((frame->src_panid == NULL && frame->src_addr != NULL) ||
			(frame->dst_panid != NULL &&
				frame->dst_addr != NULL && frame->dst_addr_ext &&
				frame->src_addr != NULL && frame->src_addr_ext)) {
			result = true;
		}
	}

	return result;
}

/*
 * Parse IEEE802154 buffer
 * buf & frame should be valid
 */
static void
ALWAYS_INLINE b91_ieee802154_frame_parse(const uint8_t *buf, size_t bul_len,
	struct ieee802154_frame *frame)
{
	size_t pos = 0; /* current buffer position */

	if (bul_len >= pos + IEEE802154_FRAME_LENGTH_FCF) {
		frame->general.ver = (buf[1] & IEEE802154_FRAME_FCF_VER_MASK) >>
			IEEE802154_FRAME_FCF_VER_OFS;
		frame->general.type = buf[0] & IEEE802154_FRAME_FCF_TYPE_MASK;
		if ((buf[0] & IEEE802154_FRAME_FCF_ACK_REQ_MASK) ==
			IEEE802154_FRAME_FCF_ACK_REQ_ON) {
			frame->general.ack_req = true;
		} else {
			frame->general.ack_req = false;
		}
		if ((buf[0] & IEEE802154_FRAME_FCF_PENDING_MASK) ==
			IEEE802154_FRAME_FCF_PENDING_ON) {
			frame->general.fp_bit = true;
		} else {
			frame->general.fp_bit = false;
		}
		frame->general.valid = true;
	} else {
		frame->general.valid = false;
		frame->general.ver = IEEE802154_FRAME_FCF_VER_2003;
		frame->general.type = IEEE802154_FRAME_FCF_TYPE_BEACON;
		frame->general.ack_req = false;
		frame->general.fp_bit = false;
	}
	pos += IEEE802154_FRAME_LENGTH_FCF;	/* FCF[2] */
	/* at sequence number */
	if (bul_len >= pos + IEEE802154_FRAME_LENGTH_SN) {
		frame->sn = &buf[IEEE802154_FRAME_LENGTH_FCF];
	} else {
		frame->sn = NULL;
	}
	pos += IEEE802154_FRAME_LENGTH_SN;  /* SN[1] */
	/* at destination PANID */
	if (frame->general.valid && ieee802154_frame_has_dest_panid(buf)) {
		if (bul_len >= pos + IEEE802154_FRAME_LENGTH_PANID) {
			frame->dst_panid = &buf[pos];
		} else {
			frame->dst_panid = NULL;
		}
		pos += IEEE802154_FRAME_LENGTH_PANID; /* PANID[2] */
	} else {
		frame->dst_panid = NULL;
	}
	/* at destination address */
	if (frame->general.valid) {
		switch (buf[1] & IEEE802154_FRAME_FCF_DST_ADDR_T_MASK) {
		case IEEE802154_FRAME_FCF_DST_ADDR_T_NA:
			frame->dst_addr = NULL;
			frame->dst_addr_ext = false;
			break;
		case IEEE802154_FRAME_FCF_DST_ADDR_T_SHORT:
			if (bul_len >= pos + IEEE802154_FRAME_LENGTH_ADDR_SHORT) {
				frame->dst_addr = &buf[pos];
			} else {
				frame->dst_addr = NULL;
			}
			frame->dst_addr_ext = false;
			pos += IEEE802154_FRAME_LENGTH_ADDR_SHORT; /* ADDR[2] */
			break;
		case IEEE802154_FRAME_FCF_DST_ADDR_T_EXT:
			if (bul_len >= pos + IEEE802154_FRAME_LENGTH_ADDR_EXT) {
				frame->dst_addr = &buf[pos];
			} else {
				frame->dst_addr = NULL;
			}
			frame->dst_addr_ext = true;
			pos += IEEE802154_FRAME_LENGTH_ADDR_EXT; /* ADDR[8] */
			break;
		default:
			frame->dst_addr = NULL;
			frame->dst_addr_ext = false;
			break;
		}
	} else {
		frame->dst_addr = NULL;
		frame->dst_addr_ext = false;
	}
	/* at source PAN ID */
	if (frame->general.valid &&
		(buf[1] & IEEE802154_FRAME_FCF_SRC_ADDR_T_MASK) !=
			IEEE802154_FRAME_FCF_SRC_ADDR_T_NA &&
		(buf[0] & IEEE802154_FRAME_FCF_PANID_COMP_MASK) ==
			IEEE802154_FRAME_FCF_PANID_COMP_OFF) {
		if (frame->general.ver  != IEEE802154_FRAME_FCF_VER_2015 ||
			(buf[1] & IEEE802154_FRAME_FCF_DST_ADDR_T_MASK) !=
				IEEE802154_FRAME_FCF_DST_ADDR_T_EXT ||
			(buf[1] & IEEE802154_FRAME_FCF_SRC_ADDR_T_MASK) !=
				IEEE802154_FRAME_FCF_SRC_ADDR_T_EXT) {
			if (bul_len >= pos + IEEE802154_FRAME_LENGTH_PANID) {
				frame->src_panid = &buf[pos];
			} else {
				frame->src_panid = NULL;
			}
			pos += IEEE802154_FRAME_LENGTH_PANID; /* PANID[2] */
		} else {
			frame->src_panid = NULL;
		}
	} else {
		frame->src_panid = NULL;
	}
	/* at source address */
	if (frame->general.valid) {
		switch (buf[1] & IEEE802154_FRAME_FCF_SRC_ADDR_T_MASK) {
		case IEEE802154_FRAME_FCF_SRC_ADDR_T_NA:
			frame->src_addr = NULL;
			frame->src_addr_ext = false;
			break;
		case IEEE802154_FRAME_FCF_SRC_ADDR_T_SHORT:
			if (bul_len >= pos + IEEE802154_FRAME_LENGTH_ADDR_SHORT) {
				frame->src_addr = &buf[pos];
			} else {
				frame->src_addr = NULL;
			}
			frame->src_addr_ext = false;
			pos += IEEE802154_FRAME_LENGTH_ADDR_SHORT; /* ADDR[2] */
			break;
		case IEEE802154_FRAME_FCF_SRC_ADDR_T_EXT:
			if (bul_len >= pos + IEEE802154_FRAME_LENGTH_ADDR_EXT) {
				frame->src_addr = &buf[pos];
			} else {
				frame->src_addr = NULL;
			}
			frame->src_addr_ext = true;
			pos += IEEE802154_FRAME_LENGTH_ADDR_EXT; /* ADDR[8] */
			break;
		default:
			frame->src_addr = NULL;
			frame->src_addr_ext = false;
			break;
		}
	} else {
		frame->src_addr = NULL;
		frame->src_addr_ext = false;
	}
	/* at security header */
	if (frame->general.valid &&
		(buf[0] & IEEE802154_FRAME_FCF_SECURITY_EN_MASK) ==
			IEEE802154_FRAME_FCF_SECURITY_EN_ON) {
		if (bul_len >= pos + IEEE802154_FRAME_LENGTH_SEC_HEADER) {
			switch (buf[pos] & IEEE802154_FRAME_SECCTRL_KEY_ID_MODE_MASK) {
			case IEEE802154_FRAME_SECCTRL_KEY_ID_MODE_0:
				if (bul_len >= pos + IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_0) {
					frame->sec_header = &buf[pos];
					frame->sec_header_len = IEEE802154_FRAME_LENGTH_SEC_HEADER
						+ IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_0;
				} else {
					frame->sec_header = NULL;
				}
				/* FC[4] */
				pos += IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_0;
				break;
			case IEEE802154_FRAME_SECCTRL_KEY_ID_MODE_1:
				if (bul_len >= pos + IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_1) {
					frame->sec_header = &buf[pos];
					frame->sec_header_len = IEEE802154_FRAME_LENGTH_SEC_HEADER
						+ IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_1;
				} else {
					frame->sec_header = NULL;
				}
				/* FC[4], KEYID[1] */
				pos += IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_1;
				break;
			case IEEE802154_FRAME_SECCTRL_KEY_ID_MODE_2:
				if (bul_len >= pos + IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_2) {
					frame->sec_header = &buf[pos];
					frame->sec_header_len = IEEE802154_FRAME_LENGTH_SEC_HEADER
						+ IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_2;
				} else {
					frame->sec_header = NULL;
				}
				/* FC[4], KEY[4], KEYID[1] */
				pos += IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_2;
				break;
			case IEEE802154_FRAME_SECCTRL_KEY_ID_MODE_3:
				if (bul_len >= pos + IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_3) {
					frame->sec_header = &buf[pos];
					frame->sec_header_len = IEEE802154_FRAME_LENGTH_SEC_HEADER
						+ IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_3;
				} else {
					frame->sec_header = NULL;
				}
				/* FC[4], KEY[8], KEYID[1] */
				pos += IEEE802154_FRAME_LENGTH_SEC_HEADER_MODE_3;
				break;
			default:
				frame->sec_header = NULL;
				break;
			}
		} else {
			frame->sec_header = NULL;
		}
		pos += IEEE802154_FRAME_LENGTH_SEC_HEADER;
	} else {
		frame->sec_header = NULL;
	}
	/* at IE header */
	bool ie_error = false;

	if (frame->general.valid &&
		(buf[1] & IEEE802154_FRAME_FCF_IE_MASK) == IEEE802154_FRAME_FCF_IE_ON) {
		frame->ie_header = NULL;
		frame->ie_header_len = 0;
		ie_error = true;
		while (bul_len >= pos + frame->ie_header_len) {
			if (frame->ie_header == NULL) {
				frame->ie_header = &buf[pos];
			}
			uint8_t ie_type =
				((buf[pos + frame->ie_header_len + 1] &
					IEEE802154_FRAME_IE_HEADER_TYPE_H_MASK)
					<< IEEE802154_FRAME_IE_HEADER_TYPE_H_OFS) |
				((buf[pos + frame->ie_header_len] &
					IEEE802154_FRAME_IE_HEADER_TYPE_L_MASK)
					>> IEEE802154_FRAME_IE_HEADER_TYPE_L_OFS);

			frame->ie_header_len += IEEE802154_FRAME_LENGTH_IE_HEADER +
				(buf[pos + frame->ie_header_len] &
				IEEE802154_FRAME_IE_HEADER_LEN_MASK);
			if (ie_type == IEEE802154_FRAME_IE_HEADER_TYPE_TERM) {
				ie_error = false;
				break;
			}
		}
	} else {
		frame->ie_header = NULL;
	}
	if (ie_error) {
		frame->ie_header_len = bul_len - pos;
		pos = bul_len;
	} else if (frame->ie_header) {
		pos += frame->ie_header_len;
	}
	/* at payload */
	if (pos < bul_len) {
		frame->data = &buf[pos];
		frame->data_len = bul_len - pos;
	} else {
		frame->data = NULL;
		frame->data_len = 0;
	}
}

/*
 * Create ACK buffer
 * frame & buf should be valid
 */
static bool
ALWAYS_INLINE b91_ieee802154_frame_build(const struct ieee802154_frame *frame,
	uint8_t *buf, size_t bul_len, size_t *o_len)
{
	bool result = false;

	do {
		if (!frame->general.valid) {
			break;
		}
		*o_len = 0;
		/* at frame control */
		if (bul_len < *o_len + IEEE802154_FRAME_LENGTH_FCF) {
			break;
		}
		buf[1] = (frame->general.ver << IEEE802154_FRAME_FCF_VER_OFS) &
			IEEE802154_FRAME_FCF_VER_MASK;
		buf[0] = frame->general.type & IEEE802154_FRAME_FCF_TYPE_MASK;
		if (frame->general.fp_bit) {
			buf[0] |= IEEE802154_FRAME_FCF_PENDING_ON;
		}
		if (frame->general.ack_req) {
			buf[0] |= IEEE802154_FRAME_FCF_ACK_REQ_ON;
		}
		if (ieee802154_frame_panid_compression(frame)) {
			buf[0] |= IEEE802154_FRAME_FCF_PANID_COMP_ON;
		}
		*o_len += IEEE802154_FRAME_LENGTH_FCF;	/* FCF[2] */

		if (bul_len < *o_len + IEEE802154_FRAME_LENGTH_SN) {
			break;
		}
		buf[*o_len] = *frame->sn;
		*o_len += IEEE802154_FRAME_LENGTH_SN; /* SN[1] */

		if (frame->dst_panid != NULL) {
			if (bul_len < *o_len + IEEE802154_FRAME_LENGTH_PANID) {
				break;
			}
			memcpy(&buf[*o_len], frame->dst_panid, IEEE802154_FRAME_LENGTH_PANID);
			*o_len += IEEE802154_FRAME_LENGTH_PANID; /* PANID[2] */
		}

		if (frame->dst_addr != NULL) {
			if (frame->dst_addr_ext) {

				if (bul_len < *o_len + IEEE802154_FRAME_LENGTH_ADDR_EXT) {
					break;
				}
				buf[1] |= IEEE802154_FRAME_FCF_DST_ADDR_T_EXT;
				memcpy(&buf[*o_len], frame->dst_addr,
					IEEE802154_FRAME_LENGTH_ADDR_EXT);
				*o_len += IEEE802154_FRAME_LENGTH_ADDR_EXT; /* ADDR[8] */
			} else {
				if (bul_len < *o_len + IEEE802154_FRAME_LENGTH_ADDR_SHORT) {
					break;
				}
				buf[1] |= IEEE802154_FRAME_FCF_DST_ADDR_T_SHORT;
				memcpy(&buf[*o_len], frame->dst_addr,
					IEEE802154_FRAME_LENGTH_ADDR_SHORT);
				*o_len += IEEE802154_FRAME_LENGTH_ADDR_SHORT; /* ADDR[2] */
			}
		}

		if (frame->src_panid != NULL) {
			if (bul_len < *o_len + IEEE802154_FRAME_LENGTH_PANID) {
				break;
			}
			memcpy(&buf[*o_len], frame->src_panid, IEEE802154_FRAME_LENGTH_PANID);
			*o_len += IEEE802154_FRAME_LENGTH_PANID; /* PANID[2] */
		}

		if (frame->src_addr != NULL) {
			if (frame->src_addr_ext) {
				if (bul_len < *o_len + IEEE802154_FRAME_LENGTH_ADDR_EXT) {
					break;
				}
				buf[1] |= IEEE802154_FRAME_FCF_SRC_ADDR_T_EXT;
				memcpy(&buf[*o_len], frame->src_addr,
					IEEE802154_FRAME_LENGTH_ADDR_EXT);
				*o_len += IEEE802154_FRAME_LENGTH_ADDR_EXT; /* ADDR[8] */
			} else {
				if (bul_len < *o_len + IEEE802154_FRAME_LENGTH_ADDR_SHORT) {
					break;
				}
				buf[1] |= IEEE802154_FRAME_FCF_SRC_ADDR_T_SHORT;
				memcpy(&buf[*o_len], frame->src_addr,
					IEEE802154_FRAME_LENGTH_ADDR_SHORT);
				*o_len += IEEE802154_FRAME_LENGTH_ADDR_SHORT; /* ADDR[2] */
			}
		}

		if (frame->sec_header != NULL) {

			if (bul_len < *o_len + frame->sec_header_len) {
				break;
			}
			buf[0] |= IEEE802154_FRAME_FCF_SECURITY_EN_ON;
			memcpy(&buf[*o_len], frame->sec_header, frame->sec_header_len);
			*o_len += frame->sec_header_len;
		}

		if (frame->ie_header != NULL) {

			if (bul_len < *o_len + frame->ie_header_len) {
				break;
			}
			buf[1] |= IEEE802154_FRAME_FCF_IE_ON;
			memcpy(&buf[*o_len], frame->ie_header, frame->ie_header_len);
			*o_len += frame->ie_header_len;
		}
		if (frame->data != NULL) {
			if (bul_len < *o_len + frame->data_len) {
				break;
			}
			memcpy(&buf[*o_len], frame->data, frame->data_len);
			*o_len += frame->data_len;
		}
		result = true;
	} while (0);

	return result;
}
