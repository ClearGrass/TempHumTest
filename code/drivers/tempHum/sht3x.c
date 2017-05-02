/* SHT Driver Version: 0.9.3
 *
 * Copyright (c) 2016, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef Bran_R8
#include "sht3x.h"



/**
 * \file
 *
 * This module provides functionality that is common to all SHT drivers
 */


static const uint8_t CRC_POLYNOMIAL    = 0x31;
static const uint8_t CRC_INIT          = 0xff;

static uint8_t sht_common_generate_crc(uint8_t *data, uint16_t count)
{
    uint8_t crc = CRC_INIT;
    uint8_t current_byte;
    uint8_t crc_bit;

    /* calculates 8-Bit checksum with given polynomial */
    for (current_byte = 0; current_byte < count; ++current_byte) {
        crc ^= (data[current_byte]);
        for (crc_bit = 8; crc_bit > 0; --crc_bit) {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

static short sht_common_check_crc(uint8_t *data, uint16_t count, uint8_t checksum)
{
    if (sht_common_generate_crc(data, count) != checksum)
        return STATUS_CRC_FAIL;
    return STATUS_OK;
}

static short sht_common_read_ticks(uint8_t address, int *temperature_ticks, int *humidity_ticks)
{
    uint8_t data[6];
    short ret = sensirion_i2c_read(address, data, sizeof(data));
    if (ret)
        return ret;
    if (sht_common_check_crc(data, 2, data[2]) || sht_common_check_crc(data + 3, 2, data[5]))
        return STATUS_CRC_FAIL;

    *temperature_ticks = (data[1] & 0xff) | ((int)data[0] << 8);
    *humidity_ticks = (data[4] & 0xff) | ((int)data[3] << 8);

    return STATUS_OK;
}

static short sht_common_read_measurement(uint8_t address, int *temperature, int *humidity)
{
    short ret = sht_common_read_ticks(address, temperature, humidity);
     /**
     * formulas for conversion of the sensor signals, optimized for fixed point algebra:
     * Temperature       = 175 * S_T / 2^16 - 45
     * Relative Humidity = 100 * S_RH / 2^16
     */
    *temperature = ((21875 * *temperature) >> 13) - 45000;
    *humidity = ((12500 * *humidity) >> 13);

    return ret;
}



/**
 * \file
 *
 * \brief Sensirion SHT3x driver implementation
 *
 * This module provides access to the SHT3x functionality over a generic I2C
 * interface. It supports measurements without clock stretching only.
 */


/* all measurement commands return T (CRC) RH (CRC) */
#if USE_SENSIRION_CLOCK_STRETCHING
static const uint8_t CMD_MEASURE_HPM[]     = { 0x2C, 0x06 };
#else
static const uint8_t CMD_MEASURE_HPM[]     = { 0x24, 0x00 };
#endif /* USE_SENSIRION_CLOCK_STRETCHING */
static const uint8_t CMD_READ_STATUS_REG[] = { 0xF3, 0x2D };
static const uint8_t COMMAND_SIZE = sizeof(CMD_MEASURE_HPM);
#ifdef SHT_ADDRESS
static const uint8_t SHT3X_ADDRESS = SHT_ADDRESS;
#else
static const uint8_t SHT3X_ADDRESS = 0x44;
#endif

static const uint16_t MEASUREMENT_DURATION_USEC = 15000;

short sht_measure_blocking_read(int *temperature, int *humidity)
{
    short ret = sht_measure();
    if (ret == STATUS_OK) {
        sensirion_sleep_usec(MEASUREMENT_DURATION_USEC);
        ret = sht_read(temperature, humidity);
    }
    return ret;
}

short sht_measure()
{
    return sensirion_i2c_write(SHT3X_ADDRESS, CMD_MEASURE_HPM, COMMAND_SIZE);
}

short sht_read(int *temperature, int *humidity)
{
    return sht_common_read_measurement(SHT3X_ADDRESS, temperature, humidity);
}

short sht_probe()
{
    uint8_t data[3];
    sensirion_i2c_init();
    short ret = sensirion_i2c_write(SHT3X_ADDRESS, CMD_READ_STATUS_REG, COMMAND_SIZE);
    if (ret)
        return ret;

    ret = sensirion_i2c_read(SHT3X_ADDRESS, data, sizeof(data));
    if (ret)
        return ret;

    ret = sht_common_check_crc(data, 2, data[2]);
    if (ret)
        return ret;
    return STATUS_OK;
}

uint8_t sht_get_configured_sht_address()
{
    return SHT3X_ADDRESS;
}

#endif
