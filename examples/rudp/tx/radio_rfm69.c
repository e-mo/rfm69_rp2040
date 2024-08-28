#include <string.h>

#include "radio_interface.h"
#include "rfm69_rp2040.h"

static rfm69_context_t *_rfm = NULL;
static rudp_context_t *_rudp = NULL;
static bool _radio_init = false;

bool radio_init(void) {
	_radio_init  = false;
	uint init_stage = 0;
	
	_rfm = rfm69_create();
	if (_rfm == NULL) {
		radio_error_set(RADIO_MALLOC_FAILURE);
		goto END_INIT;
	}

	init_stage = 1;
	rfm69_config_t config = {
		.spi      = spi0,
		.pin_miso = RFM69_PIN_MISO,
		.pin_cs   = RFM69_PIN_CS,
		.pin_sck  = RFM69_PIN_SCK,
		.pin_mosi = RFM69_PIN_MOSI,
		.pin_rst  = RFM69_PIN_RST
	};
	if (rfm69_init(_rfm, &config) == false) {
		radio_error_set(RADIO_HW_FAILURE);

		rfm69_destroy(_rfm);
		_rfm = NULL;

		goto END_INIT;
	}

	_rudp = rfm69_rudp_create();
	if (_rudp == NULL) {
		radio_error_set(RADIO_MALLOC_FAILURE);

		rfm69_destroy(_rfm);
		_rfm = NULL;

		goto END_INIT;
	}

	init_stage = 2;
	if (rfm69_rudp_init(_rudp, _rfm) == false) {
		radio_error_set(RADIO_HW_FAILURE);
		goto END_INIT;
	}

	_radio_init = true;
	radio_error_set(RADIO_OK);
END_INIT:

	// init cleanup
	if (_radio_init == false) {
		switch (init_stage) {
		case 2:
			rfm69_rudp_destroy(_rudp);
			_rudp = NULL;
		case 1:
			rfm69_destroy(_rfm);
			_rfm = NULL;
		}
	}

	return _radio_init;
}

bool radio_address_set(uint8_t address) {
	if (_radio_init == false) {
		radio_error_set(RADIO_UNINITIALIZED);
		return false;
	}

	if (!rfm69_rudp_address_set(_rudp, address)) {
		radio_error_set(RADIO_HW_FAILURE);
		return false;
	}

	return true;
}

RADIO_ERROR_T radio_status(char dst[ERROR_STR_MAX]) {	

	strncpy(dst, radio_error_str(), ERROR_STR_MAX);
	return radio_error_get();

}

// TODO: make this viable for any payload size
bool radio_send(void *payload, uint size, uint8_t address) {
	if (_radio_init == false) {
		radio_error_set(RADIO_UNINITIALIZED);
		return false;
	}

	if (!rfm69_rudp_payload_set(_rudp, payload, size)) {
		radio_error_set(RADIO_HW_FAILURE);
		return false;
	}

	trx_report_t *report = rfm69_rudp_report_get(_rudp);
	if (!rfm69_rudp_transmit(_rudp, address)) {
		
		// Error depends on return status
		switch (report->return_status) {
		case RUDP_TIMEOUT:
			radio_error_set(RADIO_TX_TIMEOUT);
			break;
		default:
			radio_error_set(RADIO_TX_FAILURE);
		}

		return false;
	}

	if (report->return_status == RUDP_OK_UNCONFIRMED) {
		// TODO: Add data logging if transmissions was unconfirmed
	}

	return true;
}

bool radio_recv(void *buffer, uint size, uint *received) {
	if (_radio_init == false) {
		radio_error_set(RADIO_UNINITIALIZED);
		return false;
	}
	
	if (!rfm69_rudp_rx_buffer_set(_rudp, buffer, size)) {
		radio_error_set(RADIO_HW_FAILURE);
		return false;
	}

	trx_report_t *report = rfm69_rudp_report_get(_rudp);
	if (!rfm69_rudp_receive(_rudp)) {
		
		// Error depends on return status
		switch (report->return_status) {
		case RUDP_TIMEOUT:
			radio_error_set(RADIO_RX_TIMEOUT);
			break;
		default:
			radio_error_set(RADIO_RX_FAILURE);
		}

		return false;
	}

	*received = report->bytes_received;

	return true;
}
