// rfm69_pico_rudp.c
// Interface implementation for rfm69_pico Reliable UDP

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

#include <stdlib.h>
#include "rfm69_rp2040_rudp.h"
#include "pico/rand.h"
#include "string.h"

// Context struct that handles internal data state of RUDP protocol
// and Rfm69 hardware



// BAUD SETTINGS
// These delay settings correspond with a BAUD rate set by user 
// Only one BAUD available currently: 57.6 KB/s

typedef struct baud_settings {
	uint fdev;
	RFM69_MODEM_BITRATE bitrate;	
	uint pp_delay; // per-packet delay us
	RFM69_RXBW_MANTISSA rxbw_mantissa;
	uint rxbw_exp;
} baud_settings_t;

const baud_settings_t BAUD_SETTINGS_LOOKUP[RUDP_BAUD_NUM] = {
	// RUDP_BAUD_57_6, ~2 beta, 12000 us PPD
	{70000, RFM69_MODEM_BITRATE_57_6, 12000, RFM69_RXBW_MANTISSA_20, 2} 
};


// FUNCS

//rudp_context_t *rfm69_rudp_create() {
//	return malloc(sizeof (rudp_context_t));
//}
//
//void rfm69_rudp_destroy(rudp_context_t *context) {
//	free(context->report);
//	free(context);
//}

bool rfm69_rudp_init(rudp_context_t *context, rfm69_context_t *rfm) {
	context->rfm = rfm;

	// Default settings for rudp
	context->buffer = NULL;
	context->buffer_size = 0;
	context->payload = NULL;
	context->payload_size = 0;

	context->tx_timeout = 100; // 100ms tx retry timeout default
	context->rx_timeout = 3000; // 3s rx timeout

	context->tx_retries = 5;
	
	if (!rfm69_rudp_baud_set(context, RUDP_BAUD_57_6)) return false;

	// some rfm69 sane default settings
	// address and power level should be set directly through radio
	// todo: add power level negotiation to protocol for first
	// communication with external radio
    rfm69_dcfree_set(rfm, RFM69_DCFREE_WHITENING);
	rfm69_packet_format_set(rfm, RFM69_PACKET_VARIABLE);
	rfm69_payload_length_set(rfm, PAYLOAD_MAX);

	rfm69_mode_set(rfm, RFM69_OP_MODE_SLEEP);
	return true;
}

bool rfm69_rudp_baud_set(rudp_context_t *context, rudp_baud_t baud) {
	if (baud < 0 || baud >= RUDP_BAUD_NUM) return false;

	baud_settings_t bs = BAUD_SETTINGS_LOOKUP[baud];

    if (!rfm69_fdev_set(context->rfm, bs.fdev)) return false;
    if (!rfm69_bitrate_set(context->rfm, bs.bitrate)) return false;
    if (!rfm69_rxbw_set(context->rfm, bs.rxbw_mantissa, bs.rxbw_exp));

	context->baud = baud;

	return true;
}

bool rfm69_rudp_tx_timeout_set(rudp_context_t *context, uint timeout) {
	context->tx_timeout = timeout;
	return true;
}

int rfm69_rudp_tx_timeout_get(const rudp_context_t *context) {
	return context->tx_timeout;
}

bool rfm69_rudp_rx_timeout_set(rudp_context_t *context, uint timeout) {
	context->rx_timeout = timeout;
	return true;
}

int rfm69_rudp_rx_timeout_get(const rudp_context_t *context) {
	return context->rx_timeout;
}

bool rfm69_rudp_rx_buffer_set(
		rudp_context_t *context, 
		void *buffer,
		uint buffer_size
) 
{
	context->buffer = (uint8_t *) buffer;
	context->buffer_size = buffer_size;

	return true;
}

void * rfm69_rudp_rx_buffer_get(rudp_context_t *context, uint *size) {
	*size = context->buffer_size;
	return context->buffer;
}

bool rfm69_rudp_payload_set(
		rudp_context_t *context,
		void *payload,
		uint payload_size
)
{
	context->payload = (uint8_t *) payload;
	context->payload_size = payload_size;

	return true;
}

bool rfm69_rudp_address_set(rudp_context_t *context, uint8_t address) {
	return rfm69_node_address_set(context->rfm, address);
}


struct trx_report_s * rfm69_rudp_report_get(rudp_context_t *context) {
	return context->report;
}

void rfm69_rudp_report_print(struct trx_report_s *report) {
	if (report == NULL) return;

	printf("payload_size: %u\n", report->payload_size);
	printf("bytes_sent: %u\n", report->bytes_sent);
	printf("bytes_received: %u\n", report->bytes_received);
	printf("data_packets_sent: %u\n", report->data_packets_sent);
	printf("data_packets_retransmitted: %u\n", report->data_packets_retransmitted);
	printf("rbt_sent: %u\n", report->rbt_sent);
	printf("rbt_received: %u\n", report->rbt_received);
	printf("acks_sent: %u\n", report->acks_sent);
	printf("acks_received: %u\n", report->acks_received);
	printf("racks_sent: %u\n", report->racks_sent);
	printf("racks_received: %u\n", report->racks_received);
	printf("rack_requests_sent: %u\n", report->rack_requests_sent);
	printf("rack_requests_received: %u\n", report->rack_requests_received);
	printf("return_status: ");
	switch (report->return_status) {
		case RUDP_OK:
			printf("RUDP_OK\n");
			break;
		case RUDP_OK_UNCONFIRMED:
			printf("RUDP_OK_UNCONFIRMED\n");
			break;
		case RUDP_TIMEOUT:
			printf("RUDP_TIMEOUT\n");
			break;
		case RUDP_BUFFER_OVERFLOW:
			printf("RUDP_BUFFER_OVERFLOW\n");
			break;
		case RUDP_PAYLOAD_OVERFLOW:
			printf("RUDP_BUFFER_OVERFLOW\n");
			break;
		default:
			printf("UNKNOWN\n");
	}
	printf("tx_address: %02X\n", report->tx_address); 
	printf("rx_address: %02X\n", report->rx_address); 

}

bool rfm69_rudp_transmit(rudp_context_t *context, uint8_t address) {

	// Set locals with context
	rfm69_context_t *rfm = context->rfm;
	struct trx_report_s *report = context->report;
	uint8_t *payload = context->payload;
	uint payload_size = context->payload_size;
	uint timeout = context->tx_timeout;
	uint8_t retries = context->tx_retries;

    // Cache previous op mode so it can be restored
    // after transmit.
    uint8_t previous_mode;
    rfm69_mode_get(rfm, &previous_mode);

    rfm69_mode_set(rfm, RFM69_OP_MODE_STDBY);

	uint8_t seq_num = get_rand_32() % SEQ_NUM_RAND_LIMIT;

    uint8_t size_bytes[sizeof(payload_size)];
    for (int i = 0; i < sizeof(payload_size); i++)
        size_bytes[i] = (payload_size >> (((sizeof(payload_size) - 1) * 8) - (i * 8))) & 0xFF;

    // Get our tx_address;
    uint8_t tx_address;
    rfm69_node_address_get(rfm, &tx_address);

	// Build header
	uint8_t header[HEADER_SIZE];
	header[HEADER_PACKET_SIZE] = HEADER_EFFECTIVE_SIZE + sizeof(payload_size);
	header[HEADER_RX_ADDRESS]  = address;
	header[HEADER_TX_ADDRESS]  = tx_address;
	header[HEADER_FLAGS]       = HEADER_FLAG_RBT;
	header[HEADER_SEQ_NUMBER]  = seq_num;
	// This count does not include the RBT packet
	uint8_t num_packets = payload_size/PAYLOAD_MAX;
    if (payload_size % PAYLOAD_MAX) num_packets++;

	// zero report struct
	memset(report, 0x00, (sizeof *report));
	report->tx_address = tx_address;
	report->rx_address = address;
	report->payload_size = payload_size;
	report->return_status = RUDP_TIMEOUT;

    // This payload is too large and should be fplit into multiple transmissions
    if (num_packets > TX_PACKETS_MAX) {
        report->return_status = RUDP_PAYLOAD_OVERFLOW; 
        return false;
    }

    // Buffer for receiving ACK/RACK
    // Max possible size for ACK/RACK packets
    uint8_t ack_packet[HEADER_SIZE + num_packets];
    bool success = false;
    bool ack_received = false;
    for (uint retry = 0; retry <= retries; retry++) {
        
        rfm69_mode_set(rfm, RFM69_OP_MODE_STDBY);

        // Write header to fifo
        rfm69_write(
                rfm,
                RFM69_REG_FIFO,
                header,	
                HEADER_SIZE
        );

        // Write payload size as payload
        rfm69_write(
                rfm,
                RFM69_REG_FIFO,
                size_bytes,	
                sizeof(payload_size)
        );
        
        rfm69_mode_set(rfm, RFM69_OP_MODE_TX);
        _rudp_block_until_packet_sent(rfm);

		report->rbt_sent++;

        // I employ "backoff" where the timout increases with each retry plus "jitter"
        // This allows you to have a quick retry followed by successively slower retries
        // with some random deviation to avoid a certain class of timing bugs
        uint next_timeout = timeout + (retry * timeout) + (get_rand_32() % 100);
        // Retry if ACK was not received within timeout
        if (_rudp_rx_ack(rfm, seq_num + 1, next_timeout) == RUDP_TIMEOUT) continue;

        // Ack received
        ack_received = true;
		report->acks_received++;
        break;
    }
    if (!ack_received) goto CLEANUP; // Do not pass go

    seq_num += 2; // Set to first data packet seq num

    uint8_t seq_num_max = seq_num + num_packets - 1;

    uint8_t size;
    uint8_t offset;
    for (int i = 0; i < num_packets; i++) {
        rfm69_mode_set(rfm, RFM69_OP_MODE_STDBY);

        // Not graceful
        if (seq_num + i == seq_num_max && payload_size % PAYLOAD_MAX)
            size = payload_size % PAYLOAD_MAX;
        else
            size = PAYLOAD_MAX;

        header[HEADER_PACKET_SIZE] = HEADER_EFFECTIVE_SIZE + size;
        header[HEADER_FLAGS]       = HEADER_FLAG_DATA;
        header[HEADER_SEQ_NUMBER]  = seq_num + i;

        uint offset = PAYLOAD_MAX * i;

        // Write header to fifo
        rfm69_write(
                rfm,
                RFM69_REG_FIFO,
                header,	
                HEADER_SIZE
        );

        // Write slice of payload
        rfm69_write(
                rfm,
                RFM69_REG_FIFO,
                &payload[offset],
                size
        );

        rfm69_mode_set(rfm, RFM69_OP_MODE_TX);
        _rudp_block_until_packet_sent(rfm);

		report->bytes_sent += size;
        report->data_packets_sent++;
    }

    uint8_t message_size = num_packets;
    uint8_t packet_num;
    uint8_t is_ok;
    bool rack_timeout;
    for (;;) {

        is_ok = false;
        rack_timeout = true;
        while (retries) {
            retries--;
            if (_rudp_rx_rack(rfm, seq_num_max, timeout, ack_packet) == RUDP_TIMEOUT) {
                rfm69_mode_set(rfm, RFM69_OP_MODE_STDBY);
                
                header[HEADER_PACKET_SIZE] = HEADER_EFFECTIVE_SIZE; 
                header[HEADER_FLAGS]       = HEADER_FLAG_DATA | HEADER_FLAG_RACK;
                header[HEADER_SEQ_NUMBER]  = seq_num;

                rfm69_write(
                        rfm,
                        RFM69_REG_FIFO,
                        header,
                        HEADER_SIZE
                );

                rfm69_mode_set(rfm, RFM69_OP_MODE_TX);
                _rudp_block_until_packet_sent(rfm);

                report->rack_requests_sent++;

                continue;
            }
            is_ok = ack_packet[HEADER_FLAGS] & HEADER_FLAG_OK;
            rack_timeout = false;
            break;
        }
        if (is_ok || rack_timeout) break;
        
        report->racks_received++;

        message_size = ack_packet[HEADER_PACKET_SIZE] - HEADER_EFFECTIVE_SIZE; 
        for (int i = 0; i < message_size; i++) {
            rfm69_mode_set(rfm, RFM69_OP_MODE_STDBY);

            packet_num = ack_packet[PAYLOAD_BEGIN + i]; 

            // Not graceful still
            if (packet_num == seq_num_max && payload_size % PAYLOAD_MAX)
                size = payload_size % PAYLOAD_MAX;
            else
                size = PAYLOAD_MAX;

            header[HEADER_PACKET_SIZE] = HEADER_EFFECTIVE_SIZE + size;
            header[HEADER_SEQ_NUMBER]  = packet_num;

            uint offset = PAYLOAD_MAX * (packet_num - seq_num);

            // Write header to fifo
            rfm69_write(
                    rfm,
                    RFM69_REG_FIFO,
                    header,	
                    HEADER_SIZE
            );

            // Write slice of payload
            rfm69_write(
                    rfm,
                    RFM69_REG_FIFO,
                    &payload[offset],
                    size
            );

            rfm69_mode_set(rfm, RFM69_OP_MODE_TX);

            _rudp_block_until_packet_sent(rfm);
			
			report->data_packets_retransmitted++;
			report->data_packets_sent++;
        }
    }

	if (is_ok) report->return_status = RUDP_OK;
	else report->return_status = RUDP_OK_UNCONFIRMED;

    success = true;
CLEANUP:
    rfm69_mode_set(rfm, previous_mode);
    return success;
}


bool rfm69_rudp_receive(rudp_context_t *context) {
	// Local variables to avoid refactoring
	rfm69_context_t *rfm = context->rfm; 
	struct trx_report_s *report = context->report;
	uint8_t *payload = context->buffer;
    uint payload_buffer_size = context->buffer_size;
	uint per_packet_delay = BAUD_SETTINGS_LOOKUP[context->baud].pp_delay;
	uint timeout = context->rx_timeout;

    // Cache previous op mode so it can be restored
    // after RX
    uint8_t previous_mode;
    rfm69_mode_get(rfm, &previous_mode);

    uint8_t rx_address;
    rfm69_node_address_get(rfm, &rx_address);

    // Max size packet buffer
    uint8_t packet[RFM69_FIFO_SIZE];
    // Header buffer
    uint8_t header[HEADER_SIZE];

    uint8_t is_rbt;
    uint8_t seq_num;

	// Zero that report meow
	memset(report, 0x00, (sizeof *report));
	report->rx_address = rx_address;
	report->return_status = RUDP_TIMEOUT;

    bool success = false;

    absolute_time_t timeout_time = make_timeout_time_ms(timeout);
    uint8_t tx_started;
RESTART_RBT_LOOP: // This is to return to the RBT loop in case of a false
                  // start receiving the transmission
    tx_started = false;
	uint payload_size = 0;
	uint8_t tx_address;
    for (;;) {
        if (get_absolute_time() >= timeout_time) break;

        rfm69_mode_set(rfm, RFM69_OP_MODE_RX);

        if (!_rudp_is_payload_ready(rfm)) {
            sleep_us(1);
            continue;
        }
		
        rfm69_mode_set(rfm, RFM69_OP_MODE_STDBY);

        rfm69_read(
                rfm,
                RFM69_REG_FIFO,
                packet,
                HEADER_SIZE
        );

        is_rbt = packet[HEADER_FLAGS] & HEADER_FLAG_RBT;

        if (!is_rbt) {
            // Empty the FIFO
            rfm69_read(
                    rfm,
                    RFM69_REG_FIFO,
                    packet,
                    packet[HEADER_PACKET_SIZE] - HEADER_EFFECTIVE_SIZE
            );
            continue;
        } 

		report->rbt_received++;

        // Read expected payload size
        uint8_t size_bytes[sizeof(payload_size)];
        rfm69_read(
                rfm,
                RFM69_REG_FIFO,
                size_bytes,
                sizeof(payload_size) 
        );

        for (int i = 0; i < sizeof(payload_size); i++) 
            payload_size |= size_bytes[i] << (((sizeof(payload_size) - 1) * 8) - (i * 8));


        // Get the sender's node address
        tx_address = packet[HEADER_TX_ADDRESS];

        
        // Increment the sequence
        seq_num = packet[HEADER_SEQ_NUMBER] + 1;

        // Build ACK packet header
        header[HEADER_PACKET_SIZE] = HEADER_EFFECTIVE_SIZE;
        header[HEADER_RX_ADDRESS]  = tx_address;
        header[HEADER_TX_ADDRESS]  = rx_address;
        header[HEADER_FLAGS]       = HEADER_FLAG_RBT | HEADER_FLAG_ACK;
        header[HEADER_SEQ_NUMBER]  = seq_num;

        rfm69_write( 
                rfm,
                RFM69_REG_FIFO,
                header,	
                HEADER_SIZE
        );

        rfm69_mode_set(rfm, RFM69_OP_MODE_TX);
        _rudp_block_until_packet_sent(rfm);

		report->payload_size = payload_size;
		report->tx_address = tx_address;
		report->acks_sent++;

        tx_started = true;
        break;
    }
    // If we have broken from the loop but tx hasn't started,
    // we have timed out
    if (!tx_started) goto CLEANUP;


	uint8_t num_packets_expected = payload_size/PAYLOAD_MAX;
    if (payload_size % PAYLOAD_MAX) num_packets_expected++;

    // We have our first data packet waiting in the FIFO now
    // Set our data packet seq num bounds
    uint8_t seq_num_max = seq_num + num_packets_expected;
    seq_num++;

    bool packets_received[TX_PACKETS_MAX] = {false}; // Keep track of what packets we have received
    uint8_t num_packets_missing = num_packets_expected;

    uint payload_bytes_received = 0; 

    uint8_t is_data;
    uint8_t packet_num;
    uint8_t is_req_rack;

    absolute_time_t rack_timeout = make_timeout_time_us(per_packet_delay * num_packets_missing);
    absolute_time_t now;
    while (num_packets_missing) {
        now = get_absolute_time();
        if (now >= timeout_time) goto CLEANUP;

        if (now >= rack_timeout) {
            rfm69_mode_set(rfm, RFM69_OP_MODE_STDBY);
            // Time to send a RACK
            uint8_t size = (num_packets_missing > PAYLOAD_MAX) ? PAYLOAD_MAX : num_packets_missing;

            header[HEADER_PACKET_SIZE] = HEADER_EFFECTIVE_SIZE + size;
            header[HEADER_FLAGS] = HEADER_FLAG_RACK;
            header[HEADER_SEQ_NUMBER] = seq_num_max;

            bool state;
            rfm69_irq2_flag_state(rfm, RFM69_IRQ2_FLAG_FIFO_NOT_EMPTY, &state);

            rfm69_write( 
                    rfm,
                    RFM69_REG_FIFO,
                    header,	
                    HEADER_SIZE
            );

            // We are actually limited by packet size how many
            // missing packets we can report. Hopefully we aren't losing
            // 61+ packets in a single TX, but worst case scenario is
            // we have to send another RACK later
            uint8_t missing_packet;
            for (int i = 0; size; i++) {
                if (packets_received[i]) continue;
                missing_packet = i + seq_num;
                rfm69_write( 
                        rfm,
                        RFM69_REG_FIFO,
                        &missing_packet,	
                        1
                );
                size--;
            }

            rfm69_mode_set(rfm, RFM69_OP_MODE_TX);

            rack_timeout = make_timeout_time_us(per_packet_delay * num_packets_missing);
            _rudp_block_until_packet_sent(rfm);

            report->racks_sent++;
        }

        // Make sure packet is sent before leaving TX
        rfm69_mode_set(rfm, RFM69_OP_MODE_RX);
        
        if (!_rudp_is_payload_ready(rfm)) {
            sleep_us(1);
            continue;
        }

        rfm69_read(
                rfm,
                RFM69_REG_FIFO,
                packet,
                HEADER_SIZE
        );

        uint message_size = packet[HEADER_PACKET_SIZE] - HEADER_EFFECTIVE_SIZE;
        rfm69_read(
            rfm,
            RFM69_REG_FIFO,
            &packet[PAYLOAD_BEGIN],
            message_size
        );


        if (tx_address != packet[HEADER_TX_ADDRESS]) continue;

        is_rbt = packet[HEADER_FLAGS] & HEADER_FLAG_RBT;
        if (is_rbt) goto RESTART_RBT_LOOP;

        is_data = packet[HEADER_FLAGS] & HEADER_FLAG_DATA;
        if (!is_data) continue;

        packet_num = packet[HEADER_SEQ_NUMBER];
        if (packet_num < seq_num || packet_num > seq_num_max) continue;

        // Check if this is a request Rack
        is_req_rack = packet[HEADER_FLAGS] & HEADER_FLAG_RACK;
        if (is_req_rack && packet_num == seq_num) {
            report->rack_requests_received++;
            rack_timeout = 0;
            continue;
        }

        // Account for packet only if it is a new packet
        if (packets_received[packet_num - seq_num]) continue;
        packets_received[packet_num - seq_num] = true;

        num_packets_missing--;

        payload_bytes_received += message_size;

		report->data_packets_received++;
		report->bytes_received = payload_bytes_received;

        if (payload_bytes_received > payload_buffer_size) {
            report->return_status = RUDP_BUFFER_OVERFLOW;
            goto CLEANUP;
        }


        // Copy the payload data into the payload buffer
        uint payload_offset = PAYLOAD_MAX * (packet_num - seq_num);
        for (int i = 0; i < message_size; i++) {
            payload[payload_offset + i] = packet[PAYLOAD_BEGIN + i];    
        }
    }

    rfm69_mode_set(rfm, RFM69_OP_MODE_STDBY);
    header[HEADER_PACKET_SIZE] = HEADER_EFFECTIVE_SIZE;
    header[HEADER_FLAGS] = HEADER_FLAG_RACK | HEADER_FLAG_OK;
    header[HEADER_SEQ_NUMBER] = seq_num_max;

    // Send a non-guaranteed success packet
    rfm69_write(
            rfm,
            RFM69_REG_FIFO,
            header,
            HEADER_SIZE
    );

    rfm69_mode_set(rfm, RFM69_OP_MODE_TX);
    _rudp_block_until_packet_sent(rfm);

    report->return_status = RUDP_OK;
    success = true;

CLEANUP:
    rfm69_mode_set(rfm, previous_mode);
    return success;
}

static RUDP_RETURN _rudp_rx_rack(
        rfm69_context_t *rfm,
        uint8_t seq_num,
        uint timeout,
        uint8_t *packet
)
{
    RUDP_RETURN rval = RUDP_TIMEOUT;
    uint8_t is_rack;
    bool is_seq;
    uint8_t is_ok;

    rfm69_mode_set(rfm, RFM69_OP_MODE_RX);

    absolute_time_t timeout_time = make_timeout_time_ms(timeout);
    for (;;) {
        if (get_absolute_time() > timeout_time) break;

        if (!_rudp_is_payload_ready(rfm)) {
            continue;
        }

        rfm69_read(
                rfm,
                RFM69_REG_FIFO,
                packet,
                HEADER_SIZE
        );

        uint8_t message_size = packet[HEADER_PACKET_SIZE] - HEADER_EFFECTIVE_SIZE; 
        rfm69_read(
                rfm,
                RFM69_REG_FIFO,
                &packet[PAYLOAD_BEGIN],
                message_size 
        );

        // This is a RACK packet, which is what we wanted
        is_rack = (packet[HEADER_FLAGS] & HEADER_FLAG_RACK);
        // is it the correct sequence num?
        is_seq = packet[HEADER_SEQ_NUMBER] == seq_num;

        if (!is_rack || !is_seq) continue;

        rval = RUDP_OK; 
        break;
    }
    return rval;
}

static RUDP_RETURN _rudp_rx_ack(
        rfm69_context_t *rfm,
        uint8_t seq_num,
        uint timeout
)
{
    RUDP_RETURN rval = RUDP_TIMEOUT;
    uint8_t packet[HEADER_SIZE];
    bool state;
    bool is_ack;
    bool is_seq;

    rfm69_mode_set(rfm, RFM69_OP_MODE_RX);

    absolute_time_t timeout_time = make_timeout_time_ms(timeout);
    for (;;) {
        if (get_absolute_time() > timeout_time) break;

        if (!_rudp_is_payload_ready(rfm)) {
            continue;
        }
        // No need to check length byte, an ack packet is only a header
        // with some flags set
        rfm69_read(
                rfm,
                RFM69_REG_FIFO,
                packet,
                HEADER_SIZE
        );

        // This is an RBT/ACK packet, which is what we wanted
        is_ack = (packet[HEADER_FLAGS] & (HEADER_FLAG_ACK | HEADER_FLAG_RBT)) > 0;
        // is it the correct sequence num?
        is_seq = packet[HEADER_SEQ_NUMBER] == seq_num;
        if (!is_ack || !is_seq) continue;

        // ACK RECEIVED
        rval = RUDP_OK; 
        break;
    }
    return rval;
}

static inline bool _rudp_is_payload_ready(rfm69_context_t *rfm) {
    bool state;
    rfm69_irq2_flag_state(rfm, RFM69_IRQ2_FLAG_PAYLOAD_READY, &state);
    return state;
}

static inline void _rudp_block_until_packet_sent(rfm69_context_t *rfm) {
    bool state = false;
    while (!state) {
        rfm69_irq2_flag_state(rfm, RFM69_IRQ2_FLAG_PACKET_SENT, &state);
        sleep_us(1);
    }
}
