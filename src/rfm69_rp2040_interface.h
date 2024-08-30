// rfm69_rp2040_interface.h
// Interface for controlling the RFM69 with a RPi Pico

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

#ifndef RFM69_RP2040_INTERFACE_H
#define RFM69_RP2040_INTERFACE_H

#include <stdio.h> // for testing. remove later

#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "rfm69_rp2040_definitions.h"

typedef struct _rfm69_context {
    spi_inst_t *spi; // Initialized SPI instance
    uint pin_cs;
    uint pin_rst;
    RFM69_OP_MODE op_mode;
    int8_t pa_level;
    RFM69_PA_MODE pa_mode;
	RFM69_RETURN return_status;
    uint8_t ocp_trim;
	uint8_t address;
} rfm69_context_t;

struct rfm69_config_s {
	spi_inst_t *spi;
	uint pin_cs;
	uint pin_rst;
};

// DEPRECATED
// rfm69_context_t *rfm69_create();
// void rfm69_destroy(rfm69_context_t *rfm);

// Initializes passed in Rfm69 pointer and sets pins to proper
// mode for spi communication. Passed pins must match the passed in
// spi instane (e.g. spi0 pins for spi0 instance).
//
// This function assumes spi_inst_t *spi has already been initialized. 
// This function returns heap allocated memory. Since this kind of
// module typically stays active for the lifetime of the process, I
// see no reason to provide an rfm69 specific free function.
bool rfm69_init(
    rfm69_context_t *rfm,
	const struct rfm69_config_s *config
);

// Resets the module by setting the reset pin for 100ms
// and then waiting an additional 5ms after clearing as per the
// RFM69HCW datasheet: https://cdn.sparkfun.com/datasheets/Wireless/General/RFM69HCW-V1.1.pdf
void rfm69_reset(rfm69_context_t *rfm);

// Writes <len> bytes from <src> to RFM69 registers/FIFO over SPI.
// SPI instance must be initialized before calling.
// If src len > 1, address will be incremented between each byte (burst write).
//
// rfm     - initialized Rfm69 *
// address - uint8_t register/FIFO address.
// src     - an array of uint8_t to be written.
// len     - src array length.
// Returns number of bytes written (not including address byte).
bool rfm69_write(
        rfm69_context_t *rfm, 
        uint8_t address, 
        const uint8_t *src, 
        size_t len);

// For writing to a specific bit field within a register.
// Only writes one byte of data.
//
// address - register address
// src     - properly aligned value to be written
// mask    - mask for clearing bit field
bool rfm69_write_masked(
        rfm69_context_t *rfm, 
        uint8_t address, 
        const uint8_t src,
        const uint8_t mask);

// Reads <len> bytes into <dst> from RFM69 registers/FIFO over SPI.
// SPI instance must be initialized before calling.
// If src len > 1, address will be incremented between each byte (burst write).
//
// rfm     - initialized rfm69_context_t *
// address - uint8_t register/FIFO address.
// dst     - an array of uint8_t to be read into.
// len     - dst array length.
// Returns number of bytes written (not including address byte).
bool rfm69_read(
        rfm69_context_t *rfm, 
        uint8_t address, 
        uint8_t *dst, 
        size_t len);

// For writing to a specific bit field within a register.
// Only writes one byte of data.
//
// address - register address
// src     - buffer to read field into
// mask    - mask for isolating bit field
bool rfm69_read_masked(
        rfm69_context_t *rfm,
        uint8_t address,
        uint8_t *dst,
        const uint8_t mask);

// Reads state of IRQ flags. Each function corresponds with one
// of the flag registers.
// flag    - IRQ flag constant you want to check.
// state   - stores flag state.
//
// Returns number of bytes written. 
bool rfm69_irq1_flag_state(rfm69_context_t *rfm, RFM69_IRQ1_FLAG flag, bool *state);
bool rfm69_irq2_flag_state(rfm69_context_t *rfm, RFM69_IRQ2_FLAG flag, bool *state);

// Sets the opterating frequency of the module.
// frequency - desired frequency in MHz.
//
// Returns number of bytes written. 
bool rfm69_frequency_set(rfm69_context_t *rfm, uint32_t frequency);

// Reads operating frequency from module.
// Note - might not reflect set freqency until a mode change.
// frequency - stores frequency in Hz.
//
// Returns number of bytes written. 
bool rfm69_frequency_get(rfm69_context_t *rfm, uint32_t *frequency);

// Sets frequency deviation. 
// Note: 0.5 <= 2* Fdev/Bitrate <= 10
// Beta value should stay within this range per specification.
bool rfm69_fdev_set(rfm69_context_t *rfm, uint32_t fdev);

bool rfm69_rxbw_set(rfm69_context_t *rfm, RFM69_RXBW_MANTISSA mantissa, uint8_t exponent);

// Sets modem bitrate.
bool rfm69_bitrate_set(rfm69_context_t *rfm,
                      RFM69_MODEM_BITRATE bit_rate);

// Reads modem bitrate.
bool rfm69_bitrate_get(rfm69_context_t *rfm, uint16_t *bit_rate);

// Sets module into a new mode.
// Blocks until mode is ready.
bool rfm69_mode_set(rfm69_context_t *rfm, RFM69_OP_MODE mode);

// Gets current mode.
void rfm69_mode_get(rfm69_context_t *rfm, uint8_t *mode);

// Checks if current mode is ready.
bool _mode_ready(rfm69_context_t *rfm, bool *ready);

// Blocks until mode ready IRQ flag is set. 
bool _mode_wait_until_ready(rfm69_context_t *rfm);

// Sets module into packet or continuous mode. 
bool rfm69_data_mode_set(rfm69_context_t *rfm, RFM69_DATA_MODE mode);
// Read data mode register. For testing. 
bool rfm69_data_mode_get(rfm69_context_t *rfm, uint8_t *mode);

// Sets modulation scheme (FSK or OOK)
bool rfm69_modulation_type_set(rfm69_context_t *rfm, RFM69_MODULATION_TYPE type);
bool rfm69_modulation_type_get(rfm69_context_t *rfm, uint8_t *type);

bool rfm69_modulation_shaping_set(rfm69_context_t *rfm, RFM69_MODULATION_SHAPING shaping);
bool rfm69_modulation_shaping_get(rfm69_context_t *rfm, uint8_t *shaping);

// Read value of last RSSI measurment
bool rfm69_rssi_measurment_get(rfm69_context_t *rfm, int16_t *rssi);

// Trigger a new RSSI reading
bool rfm69_rssi_measurment_start(rfm69_context_t *rfm);
bool rfm69_rssi_threshold_set(rfm69_context_t *rfm, uint8_t threshold);

// Sets power level of module.
// Low power modules accept power levels -18 -> 13 
// High power modules accept power levels -2 -> 20
//
// Define RFM69_HIGH_POWER for high power modules. 
// Also sets appropriate PA* and power flags based
// on desired power level. 
bool rfm69_power_level_set(rfm69_context_t *rfm, int8_t pa_level);
void rfm69_power_level_get(rfm69_context_t *rfm, uint8_t *pa_level);
bool _power_mode_set(rfm69_context_t *rfm, RFM69_PA_MODE mode);

// Enable or disable overcurent protection
bool _ocp_set(rfm69_context_t *rfm, RFM69_OCP state);
// Enable or disable high power settings
bool _hp_set(rfm69_context_t *rfm, RFM69_HP_CONFIG enable);

bool rfm69_tx_start_condition_set(rfm69_context_t *rfm, RFM69_TX_START_CONDITION condition);

bool rfm69_payload_length_set(rfm69_context_t *rfm, uint8_t length);
bool rfm69_packet_format_set(rfm69_context_t *rfm, RFM69_PACKET_FORMAT format);

bool rfm69_address_filter_set(rfm69_context_t *rfm, RFM69_ADDRESS_FILTER filter);
bool rfm69_node_address_set(rfm69_context_t *rfm, uint8_t address);
void rfm69_node_address_get(rfm69_context_t *rfm, uint8_t *address);
bool rfm69_broadcast_address_set(rfm69_context_t *rfm, uint8_t address);

bool rfm69_sync_value_set(rfm69_context_t *rfm, uint8_t *value, uint8_t size);

bool rfm69_crc_autoclear_set(rfm69_context_t *rfm, bool set);

bool rfm69_dcfree_set(rfm69_context_t *rfm, RFM69_DCFREE_SETTING setting);
bool rfm69_dagc_set(rfm69_context_t *rfm, RFM69_DAGC_SETTING setting);

#endif // RFM69_RP2040_INTERFACE_H
