### rfm69_create
**description:** Allocates and returns pointer to rfm69_context_t  context object.  
**return:** Pointer to rfm69_context_t object.  
**error:** Returns NULL if malloc fails.  
```c
rfm69_context_t * rfm69_create(void);
```

---
### rfm69_destroy
**description:** Deallocates rfm69_context_t context object.  
**return:** None  
**error:** None  
```c
void *rfm69_destroy(rfm69_context_t *rfm);
```

---
### rfm69_init
**description:** Initializes rfm69_context_t object. Must be performed prior to calling  
any library functions with context object. Ensures that the hardware wakes up in a known state  
**return:** `true` if init was successful.  
**error:** Returns `false` if init fails.  
```c
bool rfm69_init(rfm69_context_t *rfm, const rfm69_config_t *config)
```
**usage**: Initialization function performes all necessary GPIO configuration, including SPI initializtion. Can be recalled to reset state of already initialized device.
```c
// Rfm69 config object declaration (rfm69_rp2040_definitions.h)
typedef struct _rfm69_config {
	spi_inst_t *spi;
	uint pin_miso;
	uint pin_mosi;
	uint pin_cs;
	uint pin_sck;
	uint pin_rst;
} rfm69_config_t;
```
```c
// Initialization example
#include "rfm69_rp2040.h"
int main() {
    rfm69_context_t *rfm = rfm69_create(); // Check for NULL here
    rfm69_config_t config = {
        .spi = spi0,
        .pin_miso = 16,
        .pin_mosi = 19,
        .pin_cs = 17,
        .pin_sck = 18,
        .pin_rst = 20
    };
    bool success = rfm69_init(rfm, &config);
}
```

---
### rfm69_reset
**description:** Physically resets Rfm69 device by toggling the reset pin in accordance with datasheet  
**return:** None  
**error:** None  
```c
bool rfm69_reset(rfm69_context_t *rfm);
```
**usage notes:** Reset leaves the hardware in an uncertain state. To ensure correct operation of library,  
prefer recalling `rfm69_init` which internally calls `rfm69_reset` but also ensures the context object
reflects the proper state of the hardware.

---
### rfm69_write
**description:** Writes `len` bytes from `src` buffer to register starting at `address`.  
**return:** `true` if SPI write was successful.  
**error:**  `false` if SPI write fails.  
```c
bool rfm69_write(rfm69_context_t *rfm, uint8_t address, const uint8_t *src, size_t len);
```
**usage notes:** For writing directly to device registers using SPI. For easier register manipulation,  
user should prefer using one of the register specific interface functions.  

---
### rfm69_write_masked
**description:**  Writes `src` value to register `address`. Applies `mask` to current register value  
before writing. Used to write to specific fields within a buffer. Unlike `rfm69_write`, cannot be used to burst   
write to registers.   
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_write_masked(rfm69_context_t *rfm, uint8_t address, const uint8_t *src, uint8_t mask);
```
**usage notes:** For writing masked values directly to device registers using SPI. For easier   
register manipulation, user should prefer using one of the register specific interface functions.  

---
### rfm69_read
**description:** Reads `len` bytes starting from register `address` to `dst` buffer.  
**return:** `true` if SPI read was successful.  
**error:**  `false` if SPI read fails.  
```c
bool rfm69_read(rfm69_context_t *rfm, uint8_t address, const uint8_t *dst, size_t len);
```
**usage notes:** For reading directly from device registers using SPI. For easier register manipulation,  
user should prefer using one of the register specific interface functions.  

---
### rfm69_read_masked
**description:**  Reads value from register `address` into `dst`. Applies `mask` to current register   
value after reading. Used to read specific fields within a buffer. Unlike `rfm69_read`,  
cannot be used to burst read from registers.   
**return:** `true` if SPI read was successful.  
**error:** `false` if SPI read fails.  
```c
bool rfm69_read_masked(rfm69_context_t *rfm, uint8_t address, const uint8_t *dst, uint8_t mask);
```
**usage notes:** For reading masked values directly from device registers using SPI.   
For easier register manipulation, user should prefer using one of the register specific interface   
functions.  

---
### rfm69_irq1_flag_state
**description:** Sets value of `state` to match `flag` in IRQ register 1.  
**return:** `true` if SPI read was successful.  
**error:** `false` if SPI read fails.  
```c
bool rfm69_irq1_flag_state(rfm69_context_t *rfm, RFM69_IRQ1_FLAG flag, bool *state);
```
**usage notes:** Used to check one of the boolean state flags in the IRQ1 register.
```c
// rfm69_rp2040_definitions.h
// legal flag values (rfm69_rp2040_definitions.h)
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
```

---
### rfm69_irq2_flag_state
**description:** Sets value of `state` to match `flag` in IRQ register 2.  
**return:** `true` if SPI read was successful.  
**error:** `false` if SPI read fails.
```c
bool rfm69_irq2_flag_state(rfm69_context_t *rfm, RFM69_IRQ2_FLAG flag, bool *state);
```
**usage notes:** Used to check one of the boolean state flags in the IRQ2 register.
```c
// rfm69_rp2040_definitions.h
// legal flag values (rfm69_rp2040_definitions.h)
typedef enum _IRQ_2_FLAG {
    RFM69_IRQ2_FLAG_CRC_OK         = 0x02,
    RFM69_IRQ2_FLAG_PAYLOAD_READY  = 0x04,
    RFM69_IRQ2_FLAG_PACKET_SENT    = 0x08,
    RFM69_IRQ2_FLAG_FIFO_OVERRUN   = 0x10,
    RFM69_IRQ2_FLAG_FIFO_LEVEL     = 0x20,
    RFM69_IRQ2_FLAG_FIFO_NOT_EMPTY = 0x40,
    RFM69_IRQ2_FLAG_FIFO_FULL      = 0x80
} RFM69_IRQ2_FLAG;
```

---
### rfm69_frequency_set
**description:** Sets device frequency to `frequency`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_frequency_set(rfm69_context_t *rfm, uint23_t frequency);
```
**usage notes:** `frequency` argument must be given in MHz.

---
### rfm69_frequency_get
**description:** Returns `frequency` set to current device frequency in MHz.  
**return:** `true` if SPI read was successful.  
**error:** `false` if SPI read fails.  
```c
bool rfm69_frequency_set(rfm69_context_t *rfm, uint23_t *frequency);
```
**usage notes:** frequency returned is in MHz.

---
### rfm69_fdev_set
**description:** Sets device frequency deviation to `fdev`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_fdev_set(rfm69_context_t *rfm, uint32_t fdev);
```
**usage notes:** Proper frequency deviation values are tightly coupled with bitrate.  
```
B = 2 * Fdev/Bitrate
0.5 <= B <= 10
```
See [notes on configuring the Rfm69](docs/configuration.md) for more information.

---
### rfm69_rxbw_set
**description:** Sets device receiver bandwidth `mantissa` and `exponent` register values.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_rxbw_set(rfm69_context_t *rfm, RFM69_RXBW_MANTISSA mantissa, uint8_t exponent);
```
**usage notes:** Proper RXBW values are tightly coupled with frequency deviation and bitrate.  
```
RxBw >= Fdev + BR/2
```
Ideally, RXBW is set as close to Beta (Fdev + BR/2) as possible. 
```c
// rfm69_rp2040_definitions.h
// Legal mantissa values
#define _RXBW_MANTISSA_OFFSET 3
typedef enum _RXBW_MANTISSA {
    RFM69_RXBW_MANTISSA_16   = 0x0 << _RXBW_MANTISSA_OFFSET, 
    RFM69_RXBW_MANTISSA_20   = 0x1 << _RXBW_MANTISSA_OFFSET, 
    RFM69_RXBW_MANTISSA_24   = 0x2 << _RXBW_MANTISSA_OFFSET, 
} RFM69_RXBW_MANTISSA ;
```
[Available RxBw settings from RFM69HCW datasheet](https://cdn.sparkfun.com/datasheets/Wireless/General/RFM69HCW-V1.1.pdf#page=26)
[RegRxBw register info](https://cdn.sparkfun.com/datasheets/Wireless/General/RFM69HCW-V1.1.pdf#page=67)

---
### rfm69_bitrate_set
**description:** Sets device modem bitrate register to `bit_rate`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_bitrate_set(rfm69_context_t *rfm, RFM69_MODEM_BITRATE bit_rate);
```
**usage notes:** For simplicity and compatibility, bitrates should be chosen from values defined below.  
```c
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
```
[Suggested bitrates in datasheet](https://cdn.sparkfun.com/datasheets/Wireless/General/RFM69HCW-V1.1.pdf#page=26)

---
### rfm69_bitrate_get
**description:** Sets `bit_rate` to current modem bitrate.  
**return:** `true` if SPI read was successful.  
**error:** `false` if SPI read fails.  
```c
bool rfm69_bitrate_set(rfm69_context_t *rfm, uint16_t *bit_rate);
```

---
### rfm69_mode_set
**description:** Sets device operating mode to `mode`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_mode_set(rfm69_context_t *rfm, RFM69_OP_MODE mode);
```
**usage notes:** Mode should be chosen from values defined below.  
```c
// rfm69_rp2040_definitions.h
#define _OP_MODE_OFFSET 2
typedef enum _OP_MODE {
    RFM69_OP_MODE_DEFAULT = 0x01,
    RFM69_OP_MODE_SLEEP   = 0x00,
    RFM69_OP_MODE_STDBY   = 0x01 << _OP_MODE_OFFSET,
    RFM69_OP_MODE_FS      = 0x02 << _OP_MODE_OFFSET,
    RFM69_OP_MODE_TX      = 0x03 << _OP_MODE_OFFSET,
    RFM69_OP_MODE_RX      = 0x04 << _OP_MODE_OFFSET,
    RFM69_OP_MODE_MASK    = 0x07 << _OP_MODE_OFFSET // Not a valid mode input
} RFM69_OP_MODE;

```

---
### rfm69_mode_get
**description:** Sets `mode` to current device operating mode value.  
**return:** `true` if SPI read was successful.  
**error:** `false` if SPI read fails.  
```c
bool rfm69_mode_get(rfm69_context_t *rfm, uint8_t *mode);
```

---
### rfm69_data_mode_set
**description:** Sets device data mode to `mode`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.
```c
bool rfm69_data_mode_set(rfm69_context_t *rfm, RFM69_DATA_MODE mode);
```
**usage notes:** Data mode must be chosen from values defined below.
```c
// rfm69_rp2040_definitions.h
#define _DATA_MODE_OFFSET 5
typedef enum _DATA_MODE {
    RFM69_DATA_MODE_PACKET,
    RFM69_DATA_MODE_CONTINUOUS_BIT_SYNC = 0x02 << _DATA_MODE_OFFSET,
    RFM69_DATA_MODE_CONTINUOUS          = 0x03 << _DATA_MODE_OFFSET,
    RFM69_DATA_MODE_MASK                = 0x60
} RFM69_DATA_MODE;
```

---
### rfm69_data_mode_get
**description:** Sets `mode` to reflect current device data mode.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_data_mode_get(rfm69_context_t *rfm, uint8_t *mode);
```

---
### rfm69_modulation_type_set
**description:** Sets device modulation type to `type`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.
```c
bool rfm69_modulation_type_set(rfm69_context_t *rfm, RFM69_MODULATION_TYPE type);

```
**usage notes:** Modulation type must be choson from values defined below.
```c
// rfm69_rp2040_definitions.h
#define _MODULATION_TYPE_OFFSET 3
typedef enum _MODULATION_TYPE {
    RFM69_MODULATION_FSK       = 0x00,
    RFM69_MODULATION_OOK       = 0x01 << _MODULATION_TYPE_OFFSET,
    RFM69_MODULATION_TYPE_MASK = 0x18
} RFM69_MODULATION_TYPE;
```

---
### rfm69_modulation_type_get
**description:** Sets `type` to reflect current modulation type.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.
```c
bool rfm69_modulation_type_get(rfm69_context_t *rfm, uint8_t *type);
```

---
### rfm69_modulation_shaping_set
**description:** Sets device modulation shaping mode to `shaping`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_modulation_shaping_set(rfm69_context_t *rfm, RFM69_MODULATION_SHAPING shaping);
```
**usage notes:** Modulation shaping mode must be chosen from values defined below.
```c
// rfm69_rp2040_definitions.h
typedef enum _MODULATION_SHAPING {
    RFM69_NO_SHAPING,
    RFM69_FSK_GAUSSIAN_1_0        = 0x01, RFM69_OOK_FCUTOFF_BR   = 0x01,
    RFM69_FSK_GAUSSIAN_0_5        = 0x02, RFM69_OOK_FCUTOFF_2XBR = 0x02,
    RFM69_FSK_GAUSSIAN_0_3        = 0x03,
    RFM69_MODULATION_SHAPING_MASK = 0x03
} RFM69_MODULATION_SHAPING;
```

---
### rfm69_modulation_shaping_get
**description:** sets `shaping` to reflect current device modulation shaping mode.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_modulation_shaping_get(rfm69_context_t *rfm, uint8_t *shaping);
```

---
### rfm69_rssi_measurment_get
**description:** Sets `rssi` to last RSSI value measured by device.
**return:** `true` if SPI write was successful.
**error:** `false` if SPI write fails.
```c
bool rfm69_rssi_measurment_get(rfm69_context_t *rfm, int16_t *rssi);
```

---
### rfm69_rssi_measurment_start
**description:** Trigger device to begin RSSI measurement.
**return:** `true` if SPI write was successful.
**error:** `false` if SPI write fails.
```c
bool rfm69_rssi_measurment_start(rfm69_context_t *rfm);
```

---
### rfm69_rssi_threshold_set
**description:** Sets device RSSI threshold to `threshold`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_rssi_threshold_set(rfm69_context_t *rfm, uint8_t threshold);
```

---
### rfm69_power_level_set
**description:** Sets device power level to `pa_level`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_power_level_set(rfm69_context_t *rfm, int8_t pa_level);
```
**usage notes:** Valid power levels vary by device model. See [notes on Rfm69 configuration](docs/configuration.md)

---
### rfm69_power_level_get
**description:** Sets `pa_level` to reflect current device power level.   
**return:** none.  
**error:** none. 
```c
void rfm69_power_level_get(rfm69_context_t *rfm, uint8_t *pa_level);
```
**usage notes:** Power level is cached so no SPI read/write is necessary.

---
### rfm69_tx_start_condition_set
**description:** Sets device transmit start condition to `condition`.  
**return:** `true` if SPI write was successful.
**error:** `false` if SPI write fails.
```c
bool rfm69_tx_start_condition_set(rfm69_context_t *rfm, RFM69_TX_START_CONDITION condition);
```
**usage notes:** `RFM69_TX_START_CONDITION` can be chosen from values defined below.
```c
// rfm69_rp2040_definitions.h
#define _TX_START_CONDITION_OFFSET 7
typedef enum _TX_START_CONDITION {
    RFM69_TX_FIFO_LEVEL     = 0x00,
    RFM69_TX_FIFO_NOT_EMPTY = 0x01 << _TX_START_CONDITION_OFFSET,
} RFM69_TX_START_CONDITION;
```

---
### rfm69_payload_length_set
**description:** Sets device payload length to `length`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_payload_length_set(rfm69_context_t *rfm, uint8_t length);
```

---
### rfm69_packet_format_set
**description:** Sets device packet format to `format`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_packet_format_set(rfm69_context_t *rfm, RFM69_PACKET_FORMAT format);
```
**usage notes:** `RFM69_PACKET_FORMAT` can be chosen from values defined below.
```c
// rfm69_rp2040_definitions.h
typedef enum _PACKET_FORMAT {
	RFM69_PACKET_FIXED    = 0x00,
	RFM69_PACKET_VARIABLE = 0x80
} RFM69_PACKET_FORMAT;
```

---
### rfm69_address_filter_set
**description:** Sets device address filter type to `filter`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_address_filter_set(rfm69_context_t *rfm, RFM69_ADDRESS_FILTER filter);
```
**usage notes:** `RFM69_ADDRESS_FILTER` can be chosen from values defined below.
```c
// rfm69_rp2040_definitions.h
#define _ADDRESS_FILTER_OFFSET 1
typedef enum _ADDRESS_FILTER {
    RFM69_FILTER_NONE           = 0x00,
    RFM69_FILTER_NODE           = 0x01 << _ADDRESS_FILTER_OFFSET,
    RFM69_FILTER_NODE_BROADCAST = 0x02 << _ADDRESS_FILTER_OFFSET
} RFM69_ADDRESS_FILTER;
```

---
### rfm69_node_address_set
**description:** Sets device node address to `address`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_node_address_set(rfm69_context_t *rfm, uint8_t address);
```
**usage notes:** 

---
### rfm69_node_address_get
**description:** Sets `address` to current device node address.
**return:** none.  
**error:** none.  
```c
void rfm69_node_address_get(rfm69_context_t *rfm, uint8_t *address);
```
**usage notes:** Node address is cached so no SPI read/write required.

---
### rfm69_broadcast_address_set
**description:** Sets device broadcast address to `address`.  
**return:** `true` if SPI write was successful.
**error:** `false` if SPI write fails.
```c
bool rfm69_broadcast_address_set(rfm69_context_t *rfm, uint8_t address);
```
**usage notes:** 

---
### rfm69_sync_value_set
**description:** Sets device sync preamble to first `size` bytes of `value`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_sync_value_set(rfm69_context_t *rfm, uint8_t *value, uint8_t size);
```

---
### rfm69_crc_autoclear_set
**description:** Turn device CRC autoclear on (`set == true`) or off (`set == false`)  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_crc_autoclear_set(rfm69_context_t *rfm, bool set);
```
**usage notes:** 

---
### rfm69_dcfree_set
**description:** Sets device DC free mode to `setting`.  
**return:** `true` if SPI write was successful.  
**error:** `false` if SPI write fails.  
```c
bool rfm69_dcfree_set(rfm69_context_t *rfm, RFM69_DCFREE_SETTING setting);
```
**usage notes:** `RFM69_DCFREE_SETTING` can be chosen from values defined below.
```c
// rfm69_rp2040_definitions.h
#define _DCFREE_SETTING_OFFSET 5
typedef enum _DCFREE_SETTING {
    RFM69_DCFREE_OFF         = 0x00,
    RFM69_DCFREE_MANCHESTER  = 0x01 << _DCFREE_SETTING_OFFSET,
    RFM69_DCFREE_WHITENING   = 0x02 << _DCFREE_SETTING_OFFSET
} RFM69_DCFREE_SETTING;
```

---
### rfm69_dagc_set
**description:** Sets device digital AGC mode to `setting`.
**return:** `true` if SPI write was successful.
**error:** `false` if SPI write fails.
```c
bool rfm69_dagc_set(rfm69_context_t *rfm, RFM69_DAGC_SETTING setting);
```
**usage notes:** Having this set incorrectly was a huge pain point for me which is why the init function does set this to a sane default. Make sure you read your device datasheet to understand the importance of digital AGC.  
`RFM69_DAGC_SETTING` can be chosen from values defined below.
```c
// rfm69_rp2040_definitions.h
typedef enum _DAGC_SETTING {
	RFM69_DAGC_NORMAL,
	RFM69_DAGC_IMPROVED_1 = 0x20,
	RFM69_DAGC_IMPROVED_0 = 0x30
} RFM69_DAGC_SETTING;
```