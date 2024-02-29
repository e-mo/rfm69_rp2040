// rfm69_pico_definitions.h
// Definitions necessary for working with rfm69_pico interface

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

    RFM69_PA0_ON           = 0X01 << 7,
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

typedef struct _rfm69_config {
	spi_inst_t *spi;
	uint pin_miso;
	uint pin_mosi;
	uint pin_cs;
	uint pin_sck;
	uint pin_rst;
} rfm69_config_t;
