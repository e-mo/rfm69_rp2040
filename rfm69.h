// rfm69hcw.h
// This library contains register definitions and a set of baseline functions for communicating
// with the RFM69HCW transceiver module (though it should work with any of the RFM69 modules)
// specifically for working with the RaspberryPi Pico C/C++ SDK.
//
// Evan Morse
// e.morse8686@gmail.com
#ifndef RFM69_DRIVER_H
#define RFM69_DRIVER_H

#include <stdio.h> // for testing. remove later

#include "pico/stdlib.h"
#include "hardware/spi.h"

#define RFM69_REG_FIFO            0x00 // FIFO read/write access

#define RFM69_REG_OP_MODE         0x01 // Operating modes of the transceiver

#define RFM69_REG_DATA_MODUL      0x02 // Data operation mode and Modulation settings

#define RFM69_REG_BITRATE_MSB     0x03 // Bit Rate setting, Most Significant Bits
#define RFN69_REG_BITRATE_LSB     0x04 // Bit Rate setting, Least Significant Bits

#define RFM69_REG_FDEV_MSB        0x05 // Frequency Deviation setting, Most Significant Bits
#define RFM69_REG_FDEV_LSB        0x06 // Frequency Deviation setting, Least Significant Bits

#define RFM69_REG_FRF_MSB         0x07 // RF Carrier Frequency, Most Significant Bits
#define RFM69_REG_FRF_MID         0x08 // RF Carrier Frequency, Intermediate Bits
#define RFM69_REG_FRF_LSB         0x09 // RF Carrier Frequency, Least Significant Bits

#define RFM69_REG_OSC_1           0x0A // RC Oscillators Settings

#define RFM69_REG_AFC_CTRL        0x0B // AFC control in low modulation index situations

#define RFM69_REG_LISTEN_1        0x0D // Listen Mode settings
#define RFM69_REG_LISTEN_2        0x0E // Listen Mode Idle duration
#define RFM69_REG_LISTEN_3        0x0F // Listen Mode Rx duration

#define RFM69_REG_VERSION         0x10 // Version

#define RFM69_REG_PA_LEVEL        0x11 // PA selection and Output Power control
#define RFM69_REG_PA_RAMP         0x12 // Control of the PA ramp time in FSK mode

#define RFM69_REG_OCP             0x13 // Over Current Protection control

#define RFM69_REG_LNA             0x18 // LNA settings

#define RFM69_REG_RXBW            0x19 // Channel Filter BW Control
#define RFM69_REG_AFCBW           0x1A // Channel Filter BW control during the AFC routine

#define RFM69_REG_OOK_PEAK        0x1B // OOK demodulator selection and control in peak mode
#define RFM69_REG_OOK_AVG         0x1C // Average threshold control of the OOK demodulator
#define RFM69_REG_OOK_FIX         0x1D // Fixed threshold control of the OOK demodulato

#define RFM69_REG_AFC_FEI         0x1E // AFC and FEI control and status
#define RFM69_REG_AFC_MSB         0x1F // MSB of the frequency correction of the AFC
#define RFM69_REG_AFC_LSB         0x20 // LSB of the frequency correction of the AFC

#define RFM69_REG_FEI_MSB         0x21 // MSB of the calculated frequency error
#define RFM69_REG_FEI_LSB         0x22 // LSB of the calculated frequency error

#define RFM69_REG_RSSI_CONFIG     0x23 // RSSI-related settings
#define RFM69_REG_RSSI_VALUE      0x24 // RSSI value in dBm

#define RFM69_REG_DIO_MAPPING_1   0x25 // Mapping of pins DIO0 to DIO3
#define RFM69_REG_DIO_MAPPING_2   0x26 // Mapping of pins DIO4 and DIO5, ClkOut frequency

#define RFM69_REG_IRQ_FLAGS_1     0x27 // Status register: PLL Lock state, Timeout, RSSI > Threshold...
#define RFM69_REG_IRQ_FLAGS_2     0x28 // Status register: FIFO handling flags...

#define RFM69_REG_RSSI_THRESH     0x29 // RSSI Threshold control

#define RFM69_REG_RX_TIMEOUT_1    0x2A // Timeout duration between Rx request and RSSI detection
#define RFM69_REG_RX_TIMEOUT_2    0x2B // Timeout duration between RSSI detection and PayloadReady
#define RFM69_REG_PREAMBLE_MSB    0x2C // Preamble length, MSB
#define RFM69_REG_PREAMBLE_LSB    0x2D // Preamble length, LSB

#define RFM69_REG_SYNC_CONFIG     0x2E // Sync Word Recognition control
#define RFM69_REG_SYNC_VALUE_1    0x2F // Sync Word bytes, 1 through 8
#define RFM69_REG_SYNC_VALUE_2    0x30 
#define RFM69_REG_SYNC_VALUE_3    0x31 
#define RFM69_REG_SYNC_VALUE_4    0x32 
#define RFM69_REG_SYNC_VALUE_5    0x33 
#define RFM69_REG_SYNC_VALUE_6    0x34 
#define RFM69_REG_SYNC_VALUE_7    0x35 
#define RFM69_REG_SYNC_VALUE_8    0x36 

#define RFM69_REG_PACKET_CONFIG_1 0x37 // Packet mode settings

#define RFM69_REG_PAYLOAD_LENGTH  0x38 // Payload length setting

#define RFM69_REG_NODE_ADRS       0x39 // Node address
#define RFM69_REG_BROADCAST_ADRS  0x3A // Broadcast address

#define RFM69_REG_AUTO_MODES      0x3B // Auto modes settings

#define RFM69_REG_FIFO_THRESH     0x3C // Fifo threshold, Tx start condition

#define RFM69_REG_PACKET_CONFIG_2 0x3D // Packet mode settings

#define RFM69_REG_AES_KEY_1       0x3E // 16 bytes of the cypher key
#define RFM69_REG_AES_KEY_2       0x3F
#define RFM69_REG_AES_KEY_3       0x40
#define RFM69_REG_AES_KEY_4       0x41
#define RFM69_REG_AES_KEY_5       0x42
#define RFM69_REG_AES_KEY_6       0x43
#define RFM69_REG_AES_KEY_7       0x44
#define RFM69_REG_AES_KEY_8       0x45
#define RFM69_REG_AES_KEY_9       0x46
#define RFM69_REG_AES_KEY_10      0x47
#define RFM69_REG_AES_KEY_11      0x48
#define RFM69_REG_AES_KEY_12      0x49
#define RFM69_REG_AES_KEY_13      0x4A
#define RFM69_REG_AES_KEY_14      0x4B
#define RFM69_REG_AES_KEY_15      0x4C
#define RFM69_REG_AES_KEY_16      0x4D

#define RFM69_REG_TEMP_1          0x4E // Temperature Sensor control
#define RFM69_REG_TEMP_2          0x4F // Temperature readout

#define RFM69_REG_TEST_LNA        0x58 // Sensitivity boost
#define RFM69_REG_TEST_PA1        0x5A // High Power PA settings
#define RFM69_REG_TEST_PA2        0x5C // High Power PA settings
#define RFM69_REG_TEST_DAGC       0x6F // Fading Margin Improvement
#define RFM69_REG_TEST_AFC        0x71 // AFC offset for low modulation index AFC

#define RFM69_FIFO_SIZE             66 // The FIFO size is fixed to 66 bytes 
#define RFM69_FSTEP                 61

typedef enum _RETURN {
    RFM69_OK                      =  0,
    RFM69_REGISTER_TEST_FAIL      = -2,
    RFM69_SPI_UNEXPECTED_RETURN   = -3,
    RFM69_RSSI_BUSY               = -5,
} RFM69_RETURN;

#define _OP_MODE_OFFSET 2
typedef enum _OP_MODE {
    RFM69_OP_MODE_DEFAULT = 0x01,
    RFM69_OP_MODE_SLEEP   = 0x00,
    RFM69_OP_MODE_STDBY   = 0x01 << _OP_MODE_OFFSET,
    RFM69_OP_MODE_FS      = 0x02 << _OP_MODE_OFFSET,
    RFM69_OP_MODE_TX      = 0x03 << _OP_MODE_OFFSET,
    RFM69_OP_MODE_RX      = 0x04 << _OP_MODE_OFFSET,
    RFM69_OP_MODE_MASK    = 0x07 << _OP_MODE_OFFSET
} RFM69_OP_MODE;

#define _DATA_MODE_OFFSET 5
typedef enum _DATA_MODE {
    RFM69_DATA_MODE_PACKET,
    RFM69_DATA_MODE_CONTINUOUS_BIT_SYNC = 0x02 << _DATA_MODE_OFFSET,
    RFM69_DATA_MODE_CONTINUOUS          = 0x03 << _DATA_MODE_OFFSET,
    RFM69_DATA_MODE_MASK                = 0x60
} RFM69_DATA_MODE;

#define _MODULATION_TYPE_OFFSET 3
typedef enum _MODULATION_TYPE {
    RFM69_MODULATION_FSK,
    RFM69_MODULATION_OOK       = 0x01 << _MODULATION_TYPE_OFFSET,
    RFM69_MODULATION_TYPE_MASK = 0x18
} RFM69_MODULATION_TYPE;

#define _MODULATION_SHAPING_OFFSET 0
typedef enum _MODULATION_SHAPING {
    RFM69_NO_SHAPING,
    RFM69_FSK_GAUSSIAN_1_0        = 0x01, RFM69_OOK_FCUTOFF_BR   = 0x01,
    RFM69_FSK_GAUSSIAN_0_5        = 0x02, RFM69_OOK_FCUTOFF_2XBR = 0x02,
    RFM69_FSK_GAUSSIAN_0_3        = 0x03,
    RFM69_MODULATION_SHAPING_MASK = 0x03
} RFM69_MODULATION_SHAPING;

typedef enum _MODEM_BITRATE {
// Classic modem baud rates (multiples of 1.2 kbps)
    RFM69_MODEM_BITRATE_1_2        = 0x682B, // 1.2 kbps
    RFM69_MODEM_BITRATE_2_4        = 0x3415, // 2.4 kbps
    RFM69_MODEM_BITRATE_4_8        = 0x1A0B, // 4.8 kbps
    RFM69_MODEM_BITRATE_9_6        = 0x0D05, // 9.6 kbps
    RFM69_MODEM_BITRATE_19_2       = 0x0683, // 19.2 kbps
    RFM69_MODEM_BITRATE_38_4       = 0x0341, // 38.4 kbps
    RFM69_MODEM_BITRATE_76_8       = 0x01A1, // 76.8 kbps
    RFM69_MODEM_BITRATE_153_6      = 0x00D0, // 153.6 kbps

// Classic modem baud rates (multiples of 0.9 kbps)
    RFM69_MODEM_BITRATE_57_6       = 0x022C, // 57.6 kbps
    RFM69_MODEM_BITRATE_115_2      = 0x0116, // 115.2 kbps

// Round bit rates (multiples of 12.5, 25, and 50 kbps)
    RFM69_MODEM_BITRATE_12_5       = 0x0A00, // 12.5 kbps
    RFM69_MODEM_BITRATE_25         = 0x0500, // 25 kbps
    RFM69_MODEM_BITRATE_50         = 0x0280, // 50 kbps
    RFM69_MODEM_BITRATE_100        = 0x0140, // 100 kbps
    RFM69_MODEM_BITRATE_150        = 0x00D5, // 150 kbps
    RFM69_MODEM_BITRATE_200        = 0x00A0, // 200 kbps
    RFM69_MODEM_BITRATE_250        = 0x0080, // 250 kbps
    RFM69_MODEM_BITRATE_300        = 0x006B, // 300 kbps

    RFM69_MODEM_BITRATE_WATCH_XTAL = 0x03D1, // 32.768 kbps
} RFM69_MODEM_BITRATE;

typedef enum _IRQ1_FLAG {
    RFM69_IRQ1_FLAG_SYNC_ADDRESS_MATCH = 0x01,
    RFM69_IRQ1_FLAG_AUTO_MODE          = 0x02,
    RFM69_IRQ1_FLAG_TIMEOUT            = 0x04,
    RFM69_IRQ1_FLAG_RSSI               = 0x08,
    RFM69_IRQ1_FLAG_PLL_LOCK           = 0x10,
    RFM69_IRQ1_FLAG_TX_READY           = 0x20,
    RFM69_IRQ1_FLAG_RX_READY           = 0x40,
    RFM69_IRQ1_FLAG_MODE_READY         = 0x80
} RFM69_IRQ1_FLAG;

typedef enum _IRQ_2_FLAG {
    RFM69_IRQ2_FLAG_CRC_OK         = 0x02,
    RFM69_IRQ2_FLAG_PAYLOAD_READY  = 0x04,
    RFM69_IRQ2_FLAG_PACKET_SENT    = 0x08,
    RFM69_IRQ2_FLAG_FIFO_OVERRUN   = 0x10,
    RFM69_IRQ2_FLAG_FIFO_LEVEL     = 0x20,
    RFM69_IRQ2_FLAG_FIFO_NOT_EMPTY = 0x40,
    RFM69_IRQ2_FLAG_FIFO_FULL      = 0x80
} RFM69_IRQ2_FLAG;

typedef enum _RSSI_CONFIG {
    RFM69_RSSI_MEASURMENT_START = 0x01,
    RFM69_RSSI_MEASURMENT_DONE  = 0x02
} RFM69_RSSI_CONFIG;

typedef enum _PA_MODE {
    RFM69_PA_MODE_UNKNOWN,
    RFM69_PA_MODE_PA0,
    RFM69_PA_MODE_PA1,
    RFM69_PA_MODE_PA1_PA2,
    RFM69_PA_MODE_HIGH
} RFM69_PA_MODE;

#define _SYNC_SIZE_OFFSET 3
#define _SYNC_SIZE_MASK 0x38

enum _PA_LEVEL {
    RFM69_PA_LEVEL_DEFAULT = 0x1F,

    RFM69_PA_HIGH_MIN      = -2,
    RFM69_PA_HIGH_MAX      =  20,

    RFM69_PA_LOW_MIN       = -18,
    RFM69_PA_LOW_MAX       =  13,

    RFM69_PA0_ON           = 0x01 << 7,
    RFM69_PA1_ON           = 0x01 << 6,
    RFM69_PA2_ON           = 0x01 << 5,
    RFM69_PA_PINS_MASK     = 0x07 << 5,
    RFM69_PA_OUTPUT_MASK   = 0x1F
};

typedef enum _HP_CONFIG {
    RFM69_HP_ENABLE,
    RFM69_HP_DISABLE,
    RFM69_HP_PA1_HIGH = 0x5D,
    RFM69_HP_PA1_LOW  = 0x55,
    RFM69_HP_PA2_HIGH = 0x7C,
    RFM69_HP_PA2_LOW  = 0x70,
} RFM69_HP_CONFIG;

#define _OCP_OFFSET 4
typedef enum _OCP {
    RFM69_OCP_DISABLED     = 0x00,
    RFM69_OCP_ENABLED      = 0x01 << _OCP_OFFSET
} RFM69_OCP;

typedef enum _OCP_TRIM {
    RFM69_OCP_TRIM_HIGH    = 0x0F, 
    RFM69_OCP_TRIM_DEFAULT = 0x0A
} RFM69_OCP_TRIM;
#define _OCP_TRIM_MASK 0x0F

#define _RXBW_MANTISSA_OFFSET 3
typedef enum _RXBW_MANTISSA {
    RFM69_RXBW_MANTISSA_16   = 0x0 << _RXBW_MANTISSA_OFFSET, 
    RFM69_RXBW_MANTISSA_20   = 0x1 << _RXBW_MANTISSA_OFFSET, 
    RFM69_RXBW_MANTISSA_24   = 0x2 << _RXBW_MANTISSA_OFFSET, 
} RFM69_RXBW_MANTISSA ;
#define RFM69_RXBW_EXPONENT_MASK 0x07
#define RFM69_RXBW_MANTISSA_MASK 0x18

#define _TX_START_CONDITION_OFFSET 7
typedef enum _TX_START_CONDITION {
    RFM69_TX_FIFO_LEVEL     = 0x00,
    RFM69_TX_FIFO_NOT_EMPTY = 0x01 << _TX_START_CONDITION_OFFSET,
} RFM69_TX_START_CONDITION;
#define _TX_START_CONDITION_MASK 0x80

#define _ADDRESS_FILTER_OFFSET 1
typedef enum _ADDRESS_FILTER {
    RFM69_FILTER_NONE           = 0x00,
    RFM69_FILTER_NODE           = 0x01 << _ADDRESS_FILTER_OFFSET,
    RFM69_FILTER_NODE_BROADCAST = 0x02 << _ADDRESS_FILTER_OFFSET
} RFM69_ADDRESS_FILTER;
#define _ADDRESS_FILTER_MASK 0x06

#define _DCFREE_SETTING_OFFSET 5
typedef enum _DCFREE_SETTING {
    RFM69_DCFREE_OFF         = 0x00,
    RFM69_DCFREE_MANCHESTER  = 0x01 << _DCFREE_SETTING_OFFSET,
    RFM69_DCFREE_WHITENING   = 0x02 << _DCFREE_SETTING_OFFSET
} RFM69_DCFREE_SETTING;
#define _DCFREE_SETTING_MASK 0x60

typedef enum _PACKET_FORMAT {
	RFM69_PACKET_FIXED    = 0x00,
	RFM69_PACKET_VARIABLE = 0x80
} RFM69_PACKET_FORMAT;

typedef enum _DAGC_SETTING {
	RFM69_DAGC_NORMAL,
	RFM69_DAGC_IMPROVED_1 = 0x20,
	RFM69_DAGC_IMPROVED_0 = 0x30
} RFM69_DAGC_SETTING;

// Incomplete type representing Rfm69 radio module.
typedef struct _rfm69 {
    spi_inst_t *spi; // Initialized SPI instance
    uint pin_cs;
    uint pin_rst;
    RFM69_OP_MODE op_mode;
    int8_t pa_level;
    RFM69_PA_MODE pa_mode;
	RFM69_RETURN return_status;
    uint8_t ocp_trim;
	uint8_t address;
} Rfm69;

typedef struct _rfm69_pin_config {
	spi_inst_t *spi;
	uint pin_miso;
	uint pin_mosi;
	uint pin_cs;
	uint pin_sck;
	uint pin_rst;
	uint pin_irq0;
	uint pin_irq1;
} Rfm69Config;

Rfm69 *rfm69_create();
void rfm69_destroy(Rfm69 *rfm);

// Initializes passed in Rfm69 pointer and sets pins to proper
// mode for spi communication. Passed pins must match the passed in
// spi instane (e.g. spi0 pins for spi0 instance).
//
// This function assumes spi_inst_t *spi has already been initialized. 
bool rfm69_init(
    Rfm69 *rfm,
	Rfm69Config *config
);

// Resets the module by setting the reset pin for 100ms
// and then waiting an additional 5ms after clearing as per the
// RFM69HCW datasheet: https://cdn.sparkfun.com/datasheets/Wireless/General/RFM69HCW-V1.1.pdf
void rfm69_reset(Rfm69 *rfm);

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
        Rfm69 *rfm, 
        uint8_t address, 
        const uint8_t *src, 
        size_t len
);

// For writing to a specific bit field within a register.
// Only writes one byte of data.
//
// address - register address
// src     - properly aligned value to be written
// mask    - mask for clearing bit field
bool rfm69_write_masked(
        Rfm69 *rfm, 
        uint8_t address, 
        const uint8_t src,
        const uint8_t mask
);

// Reads <len> bytes into <dst> from RFM69 registers/FIFO over SPI.
// SPI instance must be initialized before calling.
// If src len > 1, address will be incremented between each byte (burst write).
//
// rfm     - initialized Rfm69 *
// address - uint8_t register/FIFO address.
// dst     - an array of uint8_t to be read into.
// len     - dst array length.
// Returns number of bytes written (not including address byte).
bool rfm69_read(
        Rfm69 *rfm, 
        uint8_t address, 
        uint8_t *dst, 
        size_t len
);

// For writing to a specific bit field within a register.
// Only writes one byte of data.
//
// address - register address
// src     - buffer to read field into
// mask    - mask for isolating bit field
bool rfm69_read_masked(
        Rfm69 *rfm,
        uint8_t address,
        uint8_t *dst,
        const uint8_t mask
);

// Reads state of IRQ flags. Each function corresponds with one
// of the flag registers.
// flag    - IRQ flag constant you want to check.
// state   - stores flag state.
//
// Returns number of bytes written. 
bool rfm69_irq1_flag_state(Rfm69 *rfm, RFM69_IRQ1_FLAG flag, bool *state);
bool rfm69_irq2_flag_state(Rfm69 *rfm, RFM69_IRQ2_FLAG flag, bool *state);

// Sets the opterating frequency of the module.
// frequency - desired frequency in MHz.
//
// Returns number of bytes written. 
bool rfm69_frequency_set(Rfm69 *rfm, uint32_t frequency);

// Reads operating frequency from module.
// Note - might not reflect set freqency until a mode change.
// frequency - stores frequency in Hz.
//
// Returns number of bytes written. 
bool rfm69_frequency_get(Rfm69 *rfm, uint32_t *frequency);

// Sets frequency deviation. 
// Note: 0.5 <= 2* Fdev/Bitrate <= 10
// Beta value should stay within this range per specification.
bool rfm69_fdev_set(Rfm69 *rfm, uint32_t fdev);

bool rfm69_rxbw_set(Rfm69 *rfm, RFM69_RXBW_MANTISSA mantissa, uint8_t exponent);

// Sets modem bitrate.
bool rfm69_bitrate_set(Rfm69 *rfm,
                      RFM69_MODEM_BITRATE bit_rate);

// Reads modem bitrate.
bool rfm69_bitrate_get(Rfm69 *rfm, uint16_t *bit_rate);

// Sets module into a new mode.
// Blocks until mode is ready.
bool rfm69_mode_set(Rfm69 *rfm, RFM69_OP_MODE mode);

// Gets current mode.
void rfm69_mode_get(Rfm69 *rfm, uint8_t *mode);

// Checks if current mode is ready.
bool _mode_ready(Rfm69 *rfm, bool *ready);

// Blocks until mode ready IRQ flag is set. 
bool _mode_wait_until_ready(Rfm69 *rfm);

// Sets module into packet or continuous mode. 
bool rfm69_data_mode_set(Rfm69 *rfm, RFM69_DATA_MODE mode);
// Read data mode register. For testing. 
bool rfm69_data_mode_get(Rfm69 *rfm, uint8_t *mode);

// Sets modulation scheme (FSK or OOK)
bool rfm69_modulation_type_set(Rfm69 *rfm, RFM69_MODULATION_TYPE type);
bool rfm69_modulation_type_get(Rfm69 *rfm, uint8_t *type);

bool rfm69_modulation_shaping_set(Rfm69 *rfm, RFM69_MODULATION_SHAPING shaping);
bool rfm69_modulation_shaping_get(Rfm69 *rfm, uint8_t *shaping);

// Read value of last RSSI measurment
bool rfm69_rssi_measurment_get(Rfm69 *rfm, int16_t *rssi);

// Trigger a new RSSI reading
bool rfm69_rssi_measurment_start(Rfm69 *rfm);
bool rfm69_rssi_threshold_set(Rfm69 *rfm, uint8_t threshold);

// Sets power level of module.
// Low power modules accept power levels -18 -> 13 
// High power modules accept power levels -2 -> 20
//
// Define RFM69_HIGH_POWER for high power modules. 
// Also sets appropriate PA* and power flags based
// on desired power level. 
bool rfm69_power_level_set(Rfm69 *rfm, int8_t pa_level);
void rfm69_power_level_get(Rfm69 *rfm, uint8_t *pa_level);
bool _power_mode_set(Rfm69 *rfm, RFM69_PA_MODE mode);

// Enable or disable overcurent protection
bool _ocp_set(Rfm69 *rfm, RFM69_OCP state);
// Enable or disable high power settings
bool _hp_set(Rfm69 *rfm, RFM69_HP_CONFIG enable);

bool rfm69_tx_start_condition_set(Rfm69 *rfm, RFM69_TX_START_CONDITION condition);

bool rfm69_payload_length_set(Rfm69 *rfm, uint8_t length);
bool rfm69_packet_format_set(Rfm69 *rfm, RFM69_PACKET_FORMAT format);

bool rfm69_address_filter_set(Rfm69 *rfm, RFM69_ADDRESS_FILTER filter);
bool rfm69_node_address_set(Rfm69 *rfm, uint8_t address);
void rfm69_node_address_get(Rfm69 *rfm, uint8_t *address);
bool rfm69_broadcast_address_set(Rfm69 *rfm, uint8_t address);

bool rfm69_sync_value_set(Rfm69 *rfm, uint8_t *value, uint8_t size);

bool rfm69_crc_autoclear_set(Rfm69 *rfm, bool set);

bool rfm69_dcfree_set(Rfm69 *rfm, RFM69_DCFREE_SETTING setting);
bool rfm69_dagc_set(Rfm69 *rfm, RFM69_DAGC_SETTING setting);
#endif // RFM69_DRIVER_H
