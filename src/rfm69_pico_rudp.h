// rfm69_pico_rudp.h
// Definitions and interface for rfm69_pico Reliable UDP

//	Copyright (C) 2024 
//	Evan Morse
//	Amelia Vlahogiannis

//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.

//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.

//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef RFM69_PICO_RUDP_H
#define RFM69_PICO_RUDP_H

#include "rfm69_pico_interface.h"

typedef struct rudp_context_ rudp_context_t;

typedef enum _RUDP_RETURN {
    RUDP_OK,
    RUDP_OK_UNCONFIRMED,
    RUDP_TIMEOUT,
    RUDP_BUFFER_OVERFLOW,
    RUDP_PAYLOAD_OVERFLOW
} RUDP_RETURN;

// BAUD rates available to user of library
typedef enum RUDP_BAUD {
	RUDP_BAUD_57_6,
	RUDP_BAUD_NUM
} rudp_baud_t;

typedef struct _trx_report {
    uint payload_size;
	uint bytes_sent;
	uint bytes_received; 
	uint data_packets_sent;
	uint data_packets_received;
	uint data_packets_retransmitted;
	uint rbt_sent;
	uint rbt_received;
	uint acks_sent;
	uint acks_received;
	uint racks_sent;
	uint racks_received;
	uint rack_requests_sent;
	uint rack_requests_received;
	RUDP_RETURN return_status;
	uint8_t tx_address;
	uint8_t rx_address;
} trx_report_t;

enum HEADER {
    HEADER_PACKET_SIZE,
    HEADER_RX_ADDRESS,
    HEADER_TX_ADDRESS,
    HEADER_FLAGS,
    HEADER_SEQ_NUMBER,
    HEADER_SIZE // Keep this at end
};

#define PAYLOAD_BEGIN (HEADER_SIZE)
#define HEADER_EFFECTIVE_SIZE (HEADER_SIZE - 1) // HEADER_SIZE - length byte (it isn't part of its own count)
#define PAYLOAD_MAX (65 - HEADER_EFFECTIVE_SIZE)
#define SEQ_NUM_RAND_LIMIT 25 
// 256 (byte packet num max) - potential range for starting seq num - 1 ack packet
#define TX_PACKETS_MAX (256 - SEQ_NUM_RAND_LIMIT - 1) 

enum FLAG {
    HEADER_FLAG_RBT  = 0x80,
    HEADER_FLAG_DATA = 0x40,
    HEADER_FLAG_ACK  = 0x20,
    HEADER_FLAG_RACK = 0x10,
    HEADER_FLAG_OK   = 0x08,
};

rudp_context_t *rfm69_rudp_create(void);

// This is a convenience function which initializes and 
// configures Rfm69 module to work properly with
// RUDP protocol transmit and receive functions.
//
// Rfm69 should be initialized before being passed to RUDP
bool rfm69_rudp_init(rudp_context_t *context, rfm69_context_t *rfm);

// Set transmission BAUD rate
// RX and TX must have same BAUD settings
bool rfm69_rudp_baud_set(rudp_context_t *context, rudp_baud_t baud);

// RX/TX timeout settings
// TODO: Add documentation on how each timeout setting works
bool rfm69_rudp_tx_timeout_set(rudp_context_t *context, uint timeout);
int rfm69_rudp_tx_timeout_get(const rudp_context_t *context);
bool rfm69_rudp_rx_timeout_set(rudp_context_t *context, uint timeout);
int rfm69_rudp_rx_timeout_get(const rudp_context_t *context);

bool rfm69_rudp_rx_buffer_set(
		rudp_context_t *context, 
		void *buffer,
		uint buffer_size
); 

void * rfm69_rudp_rx_buffer_get(rudp_context_t *context, uint *size);

bool rfm69_rudp_payload_set(
		rudp_context_t *context,
		void *payload,
		uint payload_size
);

bool rfm69_rudp_address_set(rudp_context_t *context, uint8_t address);

// Returns a copy of last TRX report struct
trx_report_t * rfm69_rudp_report_get(rudp_context_t *context);
void rfm69_rudp_report_print(trx_report_t *report);

// Attempts to send payload to provided radio address
bool rfm69_rudp_transmit(rudp_context_t *context, uint8_t address);

static inline void _rudp_block_until_packet_sent(rfm69_context_t *rfm);

bool rfm69_rudp_receive(rudp_context_t *context);


// Internal ack rx logic
static RUDP_RETURN _rudp_rx_ack(
        rfm69_context_t *rfm,
        uint8_t seq_num,
        uint timeout
);

// Internal rack rx logic
static RUDP_RETURN _rudp_rx_rack(
        rfm69_context_t *rfm,
        uint8_t seq_num,
        uint timeout,
        uint8_t *header
);

// Internal polling block on payload ready flag
static inline bool _rudp_is_payload_ready(rfm69_context_t *rfm);

#endif // RFM60_PICO_RUDP_H
