#ifndef WTP_1_0_H
#define WTP_1_0_H
//| This file contains definitions for implementing the WTP 
//| (Wisdom Transmission Protocol) 1.0 specification.
//| See WTP_specification.txt included with these drivers.

// Max value of packet size field.
// Actual packet size is always 1 byte large due to the packet size byte not
// being counted in the calculation. 
#define WTP_PKT_SIZE_MAX     (255)
#define WTP_PKT_SIZE_MAX_AES (65)

// Size of packet header in bytes
#define WTP_HEADER_SIZE (12) // Does not include first byte of header

// Header offsets from beginning of packet
// uint8_t packet_size = header[WTP_HEADER_OFFSET_PKT_SIZE];
#define WTP_HEADER_PKT_SIZE_OFFSET (0) // 1 byte
#define WTP_HEADER_RX_ADDR_OFFSET  (1) // 1 byte
#define WTP_HEADER_TX_ADDR_OFFSET  (2) // 1 byte
#define WTP_HEADER_FLAGS_OFFSET    (3) // 1 byte
#define WTP_HEADER_SEQ_NUM_OFFSET  (4) // 4 bytes
#define WTP_HEADER_ACK_NUM_OFFSET  (8) // 4 bytes

// Flag masks
#define WTP_FLAG_SYN (0x01)
#define WTP_FLAG_ACK (0x02)
#define WTP_FLAG_FIN (0x04)
#define WTP_FLAG_RTR (0x08)
#define WTP_FLAG_SEG (0x10)

// Data segment is directly after header
#define WTP_DATA_SEGMENT_OFFSET (WTP_HEADER_SIZE)

// +1 here comes from the fact that the first byte of the header (packet size)
// is not included included in the packet size. (i.e. packet size byte does not
// count itself).
#define WTP_PKT_DATA_MAX (WTP_PKT_SIZE_MAX + 1 - WTP_PKT_HEADER_SIZE)
#define WTP_PKT_DATA_MAX_AES (WTP_PKT_SIZE_MAX_AES + 1 - WTP_PKT_HEADER_SIZE)

// Special broadcast address
#define WTP_BROADCAST_ADDRESS (0xFF)

#endif // WTP_1_0_H
