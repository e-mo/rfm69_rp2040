// rfm69_rp2040_interface.c
// Interface implementation for controlling the RFM69 with a RPi Pico

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

#include "rfm69_rp2040_interface.h"
#include "stdlib.h"

// DEPRECATED
//rfm69_context_t *rfm69_create() {
//	return malloc(sizeof(rfm69_context_t));
//}
//
//void rfm69_destroy(rfm69_context_t *rfm) {
//	free(rfm);
//}

bool rfm69_init(
        rfm69_context_t *rfm,
		const struct rfm69_config_s *config
)
{
	bool success = false;

	rfm->spi = config->spi;
	rfm->pin_cs = config->pin_cs;
	rfm->pin_rst = config->pin_rst;
	rfm->op_mode = RFM69_OP_MODE_STDBY;
	rfm->pa_level = 0xFF;
	rfm->pa_mode = RFM69_PA_MODE_PA0;
	rfm->ocp_trim = RFM69_OCP_TRIM_DEFAULT;
	rfm->address = 0;

    // Per documentation we leave RST pin floating for at least
    // 10 ms on startup. No harm in waiting 10ms here to
    // guarantee.
    sleep_ms(10); 
    gpio_init(config->pin_rst);
    gpio_set_dir(config->pin_rst, GPIO_OUT);
    gpio_put(config->pin_rst, 0);

    // Try to read version register
    // As long as this returns anything other than 0 or 255, this passes.
    // The most common return is 0x24, but I can't guarantee that future
    // modules will return the same value.
    rfm69_reset(rfm);
    uint8_t buf;
	if (!rfm69_read(rfm, RFM69_REG_VERSION, &buf, 1)) goto RETURN; 
		
	if (buf == 0x00 || buf == 0xFF) { 
		rfm->return_status = RFM69_REGISTER_TEST_FAIL;
		goto RETURN;
	}

	rfm69_data_mode_set(rfm, RFM69_DATA_MODE_PACKET);

	// You have no idea how important this is and how odd
	// the radio can behave with it off
	rfm69_dagc_set(rfm, RFM69_DAGC_IMPROVED_0);

	rfm69_power_level_set(rfm, 13);
	rfm69_rssi_threshold_set(rfm, 0xE4);
	rfm69_tx_start_condition_set(rfm, RFM69_TX_FIFO_NOT_EMPTY);
	rfm69_broadcast_address_set(rfm, 0xFF); 
	rfm69_address_filter_set(rfm, RFM69_FILTER_NODE_BROADCAST);

	//Set sync value (essentially functions as subnet)
	uint8_t sync[3] = {0x01, 0x01, 0x01};
	rfm69_sync_value_set(rfm, sync, 3);

	success = true;
RETURN:
    return success;
}

// Have you tried turning it off and on again?
void rfm69_reset(rfm69_context_t *rfm) {
    gpio_put(rfm->pin_rst, 1);
    sleep_us(100);
    gpio_put(rfm->pin_rst, 0);
    sleep_ms(5);
}

// 3x NOP delay added before and after spi CS pin level change
// to allow pin to settle.
static inline void cs_select(uint pin_cs) {
    asm volatile("nop \n nop \n nop");
    gpio_put(pin_cs, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect(uint pin_cs) {
    asm volatile("nop \n nop \n nop");
    gpio_put(pin_cs, 1);
    asm volatile("nop \n nop \n nop");
}

bool rfm69_write(
        rfm69_context_t *rfm, 
        uint8_t address, 
        const uint8_t *src,
        size_t len)
{
    address |= 0x80; // Set rw bit
    cs_select(rfm->pin_cs); 

    int rval = spi_write_blocking(rfm->spi, &address, 1);
    rval += spi_write_blocking(rfm->spi, src, len);

    cs_deselect(rfm->pin_cs);

    if (rval != len + 1) {
        rfm->return_status = RFM69_SPI_UNEXPECTED_RETURN;
		return false;
	}

	rfm->return_status = RFM69_OK;
    return true;
}

bool rfm69_write_masked(
        rfm69_context_t *rfm, 
        uint8_t address, 
        const uint8_t src,
        const uint8_t mask)
{
    uint8_t reg;
	if (!rfm69_read(rfm, address, &reg, 1)) return false;

    reg &= ~mask;
    reg |= src & mask;

    return rfm69_write(rfm, address, &reg, 1);
}

bool rfm69_read(
        rfm69_context_t *rfm, 
        uint8_t address, 
        uint8_t *dst,
        size_t len)
{
    address &= 0x7F; // Clear rw bit

    cs_select(rfm->pin_cs);

    int rval = spi_write_blocking(rfm->spi, &address, 1);
    rval += spi_read_blocking(rfm->spi, 0, dst, len);

    cs_deselect(rfm->pin_cs);

    if (rval != len + 1) {
        rfm->return_status = RFM69_SPI_UNEXPECTED_RETURN;
		return false;
	}

	rfm->return_status = RFM69_OK;
	return true;
}

bool rfm69_read_masked(
        rfm69_context_t *rfm,
        uint8_t address,
        uint8_t *dst,
        const uint8_t mask)
{
    if(!rfm69_read(rfm, address, dst, 1)) return false;
    
    *dst &= mask;

    return true;
}


bool rfm69_irq1_flag_state(rfm69_context_t *rfm, RFM69_IRQ1_FLAG flag, bool *state) {
    uint8_t reg;
    if (!rfm69_read_masked(rfm, RFM69_REG_IRQ_FLAGS_1, &reg, flag))
		return false;

    if (reg) *state = true;
    else *state = false;
    
    return true;
}

bool rfm69_irq2_flag_state(rfm69_context_t *rfm, RFM69_IRQ2_FLAG flag, bool *state) {
    uint8_t reg;
    if (!rfm69_read_masked(rfm, RFM69_REG_IRQ_FLAGS_2, &reg, flag))
		return false;

    if (reg) *state = true;
    else *state = false;

    return true;
}

bool rfm69_frequency_set(rfm69_context_t *rfm, uint32_t frequency) {
    // Frf = Fstep * Frf(23,0) frequency *= 1000000; // MHz to Hz
    frequency *= 1000000;
    frequency = (frequency / RFM69_FSTEP) + 0.5; // Gives needed register value
												 //
    // Split into three bytes.
    uint8_t buf[3] = {
        (frequency >> 16) & 0xFF,
        (frequency >> 8) & 0xFF,
        frequency & 0xFF
    };

    return rfm69_write(rfm, RFM69_REG_FRF_MSB, buf, 3);
}

bool rfm69_frequency_get(rfm69_context_t *rfm, uint32_t *frequency) {
    uint8_t buf[3] = {0};
    if (!rfm69_read(rfm, RFM69_REG_FRF_MSB, buf, 3)) return false;

    *frequency = (uint32_t) buf[0] << 16;
    *frequency |= (uint32_t) buf[1] << 8;
    *frequency |= (uint32_t) buf[2];
    *frequency *= RFM69_FSTEP;

    return true;
}

bool rfm69_fdev_set(rfm69_context_t *rfm, uint32_t fdev) {
    fdev = (fdev / RFM69_FSTEP) + 0.5;

    uint8_t buf[2] = {
        (fdev >> 8) & 0x3F, 
        fdev & 0xFF 
    };

    return rfm69_write(rfm, RFM69_REG_FDEV_MSB, buf, 2);
}

bool rfm69_rxbw_set(rfm69_context_t *rfm, RFM69_RXBW_MANTISSA mantissa, uint8_t exponent) {
    // Mask all inputs to prevent invalid input
    exponent &= RFM69_RXBW_EXPONENT_MASK;
    mantissa &= RFM69_RXBW_MANTISSA_MASK;

    uint8_t buf = exponent | mantissa;

    return rfm69_write_masked(
            rfm,
            RFM69_REG_RXBW,
            buf,
            RFM69_RXBW_EXPONENT_MASK | RFM69_RXBW_MANTISSA_MASK
    );
}

bool rfm69_bitrate_set(rfm69_context_t *rfm, uint16_t bit_rate) {
    uint8_t bytes[2] = {
        (bit_rate & 0xFF00) >> 8,
        bit_rate & 0xFF
    }; 

    return rfm69_write(rfm, RFM69_REG_BITRATE_MSB, bytes, 2);
}

bool rfm69_bitrate_get(rfm69_context_t *rfm, uint16_t *bit_rate) {
    uint8_t buf[2] = {0}; 
    if (!rfm69_read(rfm, RFM69_REG_BITRATE_MSB, buf, 2)) return false;

    *bit_rate = (uint16_t) buf[0] << 8;
    *bit_rate |= (uint16_t) buf[1];

    return true;
}

bool rfm69_mode_set(rfm69_context_t *rfm, RFM69_OP_MODE mode) {
	bool success = false;

	// Just return true/OK if we are already in requested mode
	if (rfm->op_mode == mode) {
		rfm->return_status = RFM69_OK;
		return true;
	}

	// Switch off high power if switching into RX
	if (mode == RFM69_OP_MODE_RX && rfm->pa_level >= 17) {
		if(!_hp_set(rfm, RFM69_HP_DISABLE)) goto RETURN;
	}
	// Enable high power if necessary if switching into TX
	else if (mode == RFM69_OP_MODE_TX && rfm->pa_level >= 17) {
		if(!_hp_set(rfm, RFM69_HP_ENABLE)) goto RETURN;
	}

	if (!rfm69_write_masked(rfm, RFM69_REG_OP_MODE, mode, RFM69_OP_MODE_MASK))
		goto RETURN;

	if (!_mode_wait_until_ready(rfm)) goto RETURN;
	rfm->op_mode = mode;
	
	success = true;
RETURN:
    return success;
}

void rfm69_mode_get(rfm69_context_t *rfm, uint8_t *mode) { *mode = rfm->op_mode; }

bool _mode_ready(rfm69_context_t *rfm, bool *ready) {
    return rfm69_irq1_flag_state(rfm, RFM69_IRQ1_FLAG_MODE_READY, ready);
}

bool _mode_wait_until_ready(rfm69_context_t *rfm) {
    bool ready = false;
    while (!ready) {
        // Return immediately if there is an spi error
        if (!_mode_ready(rfm, &ready)) return false;
    }

    return true;
}

bool rfm69_data_mode_set(rfm69_context_t *rfm, RFM69_DATA_MODE mode) {
    return rfm69_write_masked(
            rfm, 
            RFM69_REG_DATA_MODUL,
            mode,
            RFM69_DATA_MODE_MASK
    );
}

bool rfm69_data_mode_get(rfm69_context_t *rfm, uint8_t *mode) {
    return rfm69_read_masked(
            rfm,
            RFM69_REG_DATA_MODUL,
            mode,
            RFM69_DATA_MODE_MASK
    );
}

bool rfm69_modulation_type_set(rfm69_context_t *rfm, RFM69_MODULATION_TYPE type) {
    return rfm69_write_masked(
            rfm,
            RFM69_REG_DATA_MODUL,
            type,
            RFM69_MODULATION_TYPE_MASK
    );
}

bool rfm69_modulation_type_get(rfm69_context_t *rfm, uint8_t *type) {
    return rfm69_read_masked(
            rfm,
            RFM69_REG_DATA_MODUL,
            type,
            RFM69_MODULATION_TYPE_MASK
    );
}

bool rfm69_modulation_shaping_set(rfm69_context_t *rfm, RFM69_MODULATION_SHAPING shaping) {
    return rfm69_write_masked(
            rfm,
            RFM69_REG_DATA_MODUL,
            shaping,
            RFM69_MODULATION_SHAPING_MASK
    );
}

bool rfm69_modulation_shaping_get(rfm69_context_t *rfm, uint8_t *shaping) {
    return rfm69_read_masked(
            rfm,
            RFM69_REG_DATA_MODUL,
            shaping,
            RFM69_MODULATION_SHAPING_MASK
    );
}

//reads rssi - see p.68 of rfm69 datasheet
bool rfm69_rssi_measurment_get(rfm69_context_t *rfm, int16_t *rssi) {
	uint8_t reg;

	if (!rfm69_read(rfm, RFM69_REG_RSSI_CONFIG, &reg, 1)) return false;

	//checks RssiDone flag - all other bits should be 0
	if(reg & RFM69_RSSI_MEASURMENT_DONE) {
		rfm->return_status = RFM69_RSSI_BUSY; 
		return false;
	}

	if(!rfm69_read(rfm, RFM69_REG_RSSI_VALUE, &reg, 1)) return false;

	*rssi = -((int16_t)(reg >> 1));

	return true;
}

//triggers the rfm69 to check rssi
//probably best to run this function before calling rfm69_rssi_get
bool rfm69_rssi_measurment_start(rfm69_context_t *rfm) {
	uint8_t reg;

	if (!rfm69_read(rfm, RFM69_REG_RSSI_CONFIG, &reg, 1)) return false;

	reg |= RFM69_RSSI_MEASURMENT_START;

	return rfm69_write(rfm, RFM69_REG_RSSI_CONFIG, &reg, 1);
}

bool rfm69_rssi_threshold_set(rfm69_context_t *rfm, uint8_t threshold) {
    return rfm69_write(
            rfm,
            RFM69_REG_RSSI_THRESH,
            &threshold,
            1
    );
}

bool rfm69_power_level_set(rfm69_context_t *rfm, int8_t pa_level) {
	bool success = false;

    if (rfm->pa_level == pa_level) {
        rfm->return_status = RFM69_OK;
		goto RETURN;
	}

    uint8_t buf;
    RFM69_PA_MODE pa_mode;
    int8_t pout;

#ifdef RFM69_HIGH_POWER
    bool high_power = true;
#else
    bool high_power = false;
#endif

    // High power modules have to follow slightly different bounds
    // regarding PA_LEVEL. -2 -> 20 Dbm. 
    //
    // HW and HCW modules use only the PA1 and PA2 pins
    // 
    if (high_power) {
        // Pull pa_level within acceptible bounds 
        if (pa_level < RFM69_PA_HIGH_MIN)
            pa_level = RFM69_PA_HIGH_MIN;
        else if (pa_level > RFM69_PA_HIGH_MAX)
            pa_level = RFM69_PA_HIGH_MAX;

        // PA1 on only
        if (pa_level <= 13) {
            pa_mode = RFM69_PA_MODE_PA1;
            pout = pa_level + 18; 
        }
        // PA1 + PA2
        else if (pa_level < 18) {
            pa_mode = RFM69_PA_MODE_PA1_PA2;
            pout = pa_level + 14; 
        }
        // PA1 + PA2 + High Power Enabled
        else {
            pa_mode = RFM69_PA_MODE_HIGH;
            pout = pa_level + 11; 
        }
    }
    else {
        // Low power modules only use PA0
        pa_mode = RFM69_PA_MODE_PA0;
        // Pull pa_level within acceptible bounds 
        if (pa_level < RFM69_PA_LOW_MIN)
            pa_level = RFM69_PA_LOW_MIN;
        else if (pa_level > RFM69_PA_LOW_MAX)
            pa_level = RFM69_PA_LOW_MAX;

        pout = pa_level + 18;
    }

    if(!_power_mode_set(rfm, pa_mode)) goto RETURN;

	rfm->pa_mode = pa_mode;
	if (!rfm69_write_masked(rfm, RFM69_REG_PA_LEVEL, pout, RFM69_PA_OUTPUT_MASK))
		goto RETURN;

	// If power level was successfully set, cache value
	rfm->pa_level = pa_level; 
	success = true;
RETURN:
    return false;
}

void rfm69_power_level_get(rfm69_context_t *rfm, uint8_t *pa_level) {
    *pa_level = rfm->pa_level;
};

bool _power_mode_set(rfm69_context_t *rfm, RFM69_PA_MODE pa_mode) {
    // Skip if we are already in this mode
    if (rfm->pa_mode == pa_mode) {
		rfm->return_status = RFM69_OK;
		return true;
	}

    uint8_t buf = 0x00;
	switch (pa_mode) {
		case RFM69_PA_MODE_PA0:
			buf |= RFM69_PA0_ON;
			break;
		case RFM69_PA_MODE_PA1:
			buf |= RFM69_PA1_ON;
			break;
		case RFM69_PA_MODE_PA1_PA2:
			buf |= RFM69_PA1_ON | RFM69_PA2_ON;
			break;
		case RFM69_PA_MODE_HIGH:
			buf |= RFM69_PA1_ON | RFM69_PA2_ON;
			break;
	} 

	// Set needed PA pins
	if (!rfm69_write_masked(rfm, RFM69_REG_PA_LEVEL, buf, RFM69_PA_PINS_MASK))
		return false;

	bool success;
	if (pa_mode == RFM69_PA_MODE_HIGH)
		success = _hp_set(rfm, RFM69_HP_ENABLE);
	else 
		success = _hp_set(rfm, RFM69_HP_DISABLE);

    return success;
}

bool _ocp_set(rfm69_context_t *rfm, RFM69_OCP state) {
    return rfm69_write_masked(
            rfm,
            RFM69_REG_OCP,
            state,
            RFM69_OCP_ENABLED
    );
}

bool _hp_set(rfm69_context_t *rfm, RFM69_HP_CONFIG config) {
    RFM69_HP_CONFIG buf[2];
    RFM69_OCP ocp;
    RFM69_OCP_TRIM ocp_trim;

    
    if (config == RFM69_HP_ENABLE) {
        buf[0] = RFM69_HP_PA1_HIGH;
        buf[1] = RFM69_HP_PA2_HIGH;
        ocp = RFM69_OCP_DISABLED;
        ocp_trim = RFM69_OCP_TRIM_HIGH;

    }
    else {
        buf[0] = RFM69_HP_PA1_LOW;
        buf[1] = RFM69_HP_PA2_LOW;
        ocp = RFM69_OCP_ENABLED;
        ocp_trim = rfm->ocp_trim;
    }

    if (!rfm69_write(rfm, RFM69_REG_TEST_PA1, &buf[0], 1))
		return false;

    if (!rfm69_write(rfm, RFM69_REG_TEST_PA2, &buf[1], 1))
		return false;


	if (!_ocp_set(rfm, ocp)) return false;

	if (!rfm69_write_masked(rfm, RFM69_REG_OCP, ocp_trim, _OCP_TRIM_MASK))
		return false;

    return true;
}

bool rfm69_tx_start_condition_set(rfm69_context_t *rfm, RFM69_TX_START_CONDITION condition) {
    return rfm69_write_masked(
            rfm,
            RFM69_REG_FIFO_THRESH,
            condition,
            _TX_START_CONDITION_MASK
    );
}

bool rfm69_payload_length_set(rfm69_context_t *rfm, uint8_t length) {
    return rfm69_write(
            rfm,
            RFM69_REG_PAYLOAD_LENGTH,
            &length,
            1
    );
}

bool rfm69_packet_format_set(rfm69_context_t *rfm, RFM69_PACKET_FORMAT format) {
	return rfm69_write_masked(
			rfm,
			RFM69_REG_PACKET_CONFIG_1,
			format,
			0x80
	);
}

bool rfm69_address_filter_set(rfm69_context_t *rfm, RFM69_ADDRESS_FILTER filter) {
    return rfm69_write_masked(
            rfm,
            RFM69_REG_PACKET_CONFIG_1,
            filter,
            _ADDRESS_FILTER_MASK
    );
}

bool rfm69_node_address_set(rfm69_context_t *rfm, uint8_t address) {
    if (!rfm69_write( rfm, RFM69_REG_NODE_ADRS, &address, 1))
		return false;

	rfm->address = address;
	return true;
}

void rfm69_node_address_get(rfm69_context_t *rfm, uint8_t *address) {
    *address = rfm->address;
}

bool rfm69_broadcast_address_set(rfm69_context_t *rfm, uint8_t address) {
    return rfm69_write(
            rfm,
            RFM69_REG_BROADCAST_ADRS,
            &address,
            1
    );
}

bool rfm69_sync_value_set(rfm69_context_t *rfm, uint8_t *value, uint8_t size) {
    if (!rfm69_write(rfm, RFM69_REG_SYNC_VALUE_1, value, size))
		return false;

	size = (size - 1) << _SYNC_SIZE_OFFSET;
	return rfm69_write_masked(rfm, RFM69_REG_SYNC_CONFIG, size, _SYNC_SIZE_MASK);
}

bool rfm69_crc_autoclear_set(rfm69_context_t *rfm, bool set) {
    return rfm69_write_masked(
            rfm,
            RFM69_REG_PACKET_CONFIG_1,
            !set << 7,
            0x08
    );
}

bool rfm69_dcfree_set(rfm69_context_t *rfm, RFM69_DCFREE_SETTING setting) {
    return rfm69_write_masked(
            rfm,
            RFM69_REG_PACKET_CONFIG_1,
            setting,
            _DCFREE_SETTING_MASK
    );
}

bool rfm69_dagc_set(rfm69_context_t *rfm, RFM69_DAGC_SETTING setting) {
    return rfm69_write(
            rfm,
            RFM69_REG_TEST_DAGC,
            &setting,
            1
    );
}
