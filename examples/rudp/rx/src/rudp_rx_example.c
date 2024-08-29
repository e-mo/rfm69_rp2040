#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "rfm69_rp2040.h"

#define ever ;;

// Change these defines to fit your build
#define SPI_INST spi0
#define PIN_MISO (16)
#define PIN_CS   (17)
#define PIN_SCK  (18)
#define PIN_MOSI (19)
#define PIN_RST  (20)

int main() {
    stdio_init_all(); // To be able to use printf

	// INITIALIZATION

	// SPI init
    spi_init(SPI_INST, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

	rfm69_context_t *rfm = (rfm69_context_t *){0};
	struct rfm69_config_s config = {
		.spi = SPI_INST,
		.pin_miso = PIN_MISO,
		.pin_cs = PIN_CS,
		.pin_sck = PIN_SCK,
		.pin_mosi = PIN_MOSI,
		.pin_rst = PIN_RST
	};

	if (!rfm69_init(rfm, &config)) {
		// radio init fail (SPI communication failure)
		goto FAIL;
	}

	rudp_context_t *rudp = (rudp_context_t *){0};
	if (!rfm69_rudp_init(rudp, rfm)) {
		// rudp init fail (SPI communication failure)
		goto FAIL;
	}

	// RX CONFIG
	
	// Set addresses
	const uint8_t rx_address = 0x01; // Our address
	rfm69_rudp_address_set(rudp, rx_address);

	const uint8_t tx_address = 0x00; // Destination radio address
	
	// Set buffer to receive payload
	uint8_t buffer[100] = {0};
	const uint size = sizeof buffer;

	rfm69_rudp_rx_buffer_set(rudp, buffer, size);
	
	// Grab a pointer to the rudp context object's trx report.
	// This report is overwritten during any tx or rx.
	// See rfm69_rp2040_rudp.h for struct definition.
	const struct trx_report_s *report = rfm69_rudp_report_get(rudp);

	// Send payload every 1 sec
	for(ever) {

		// If transmit returns false, check trx report for return status
		if(!rfm69_rudp_transmit(rudp, rx_address)) {
			switch (report->return_status) {
			case RUDP_TIMEOUT:
				printf("TX timed out!\n");
				break;
			default:
				printf("Tx failed due to an error!\n");
			}
		} 

		// If receive returns false, check return_status
		// in report.
		if (!rfm69_rudp_receive(_rudp)) {
			switch (report->return_status) {
			case RUDP_TIMEOUT:
				printf("Rx timed out!\n");
				break;
			default:
				printf("Rx failed due to an error!\n");
			}
		}

		printf("Rx success!\n");
		printf("Payload: %.*s\n", report->bytes_received, buffer);

		sleep_ms(1000);
	}

FAIL:
	for(ever) {
		printf("Radio failed\n");
		sleep_ms(1000);
	}
}
