#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "rfm69_rp2040.h"
#include "tusb.h"

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

	// To make execution wait for usb serial connection
	//while (!tud_cdc_connected()) { sleep_ms(100); };

	// INITIALIZATION

	// SPI init
    spi_init(SPI_INST, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

	// Drive CS pin high
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

	rfm69_context_t rfm;
	struct rfm69_config_s config = {
		.spi = SPI_INST,
		.pin_cs = PIN_CS,
		.pin_rst = PIN_RST
	};

	if (!rfm69_init(&rfm, &config)) {
		// radio init fail (SPI communication failure)
		goto FAIL;
	}

	// TX CONFIG
	
    // 250kb/s baud rate
    rfm69_bitrate_set(&rfm, RFM69_MODEM_BITRATE_57_6);
    // ~2 beta 
    rfm69_fdev_set(&rfm, 70000);
    // 915MHz 
    rfm69_frequency_set(&rfm, 915);
    // RXBW >= fdev + br/2
    rfm69_rxbw_set(&rfm, RFM69_RXBW_MANTISSA_20, 2);
    rfm69_dcfree_set(&rfm, RFM69_DCFREE_WHITENING);
	
	// Set addresses

	const uint8_t rx_address = 0x01; // Destination radio address
    rfm69_node_address_set(&rfm, rx_address); 

	// Build payload
	rfm69_packet_format_set(&rfm, RFM69_PACKET_VARIABLE);
	// Packet size > payload length will be refused
	rfm69_payload_length_set(&rfm, 100);

	uint8_t buf[100] = {0};
	rfm69_mode_set(&rfm, RFM69_OP_MODE_RX);
	
	int i = 0;
	// Wait for packets
	for(ever) {

		// Poll payload ready flag
		bool state = false;
		while (!state) {
            rfm69_irq2_flag_state(&rfm, RFM69_IRQ2_FLAG_PAYLOAD_READY, &state);
			sleep_ms(10);
        }
        printf("%i: Packet received!\n", i);

		// Read payload size byte from FIFO
		rfm69_read(&rfm, RFM69_REG_FIFO, buf, 1);
		printf("%u bytes received.\n", buf[0]);
		// Read remaining bytes from FIFO 
		rfm69_read(&rfm, RFM69_REG_FIFO, buf+1, buf[0]);

		// Second byte is address
		printf("Address byte: 0x%02X\n", buf[1]);

		// Print out payload (in this case a string)
		printf("Payload: ");
		for (int i = 2; i <= buf[0]-1; i++)
			printf("%c", buf[i]);
		printf("\n\n");

		i++;
	}

FAIL:
	for(ever) {
		printf("Radio failed\n");
		sleep_ms(1000);
	}
}
