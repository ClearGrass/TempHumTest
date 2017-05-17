/* SHT Driver Version: 0.9.6
 *
 * Copyright (c) 2016 Sensirion AG.
 * All rights reserved.
 *
 * CONFIDENTIAL SOURCE CODE
 *
 * The software contained in this file is only available under the Sensirion
 * Special Software License Agreement (SLA) that has been accepted by Licensee.
 * A copy of the SLA is delivered along with this code for your reference. The
 * SLA regulates among others the use, incorporation into products,
 * distribution, etc. (collectively "Use") of this software, and the limitation
 * or exclusion of Sensirion's warranty and liability.
 * The SLA defines this software that it governs as The Software, and the
 * licensee as Licensee. See terms of the SLA. Licensee is legally bound to the
 * terms and conditions mentioned in the SLA. If in disagreement or if
 * compliance to the SLA cannot be guaranteed, the Use of The Software is
 * forbidden, The Software must be returned immediately to Sensirion, and any
 * and all copies of The Software shall be deleted. By using The Software
 * Licensee confirms its acceptance of the SLA and its compliance with the terms
 * of the SLA.
 * In particular (SLA terms apply, see SLA for details):
 * The Software contains confidential, proprietary information of Sensirion.
 * Disclosure to third parties is prohibited.
 * The Software source code must not be distributed and must not be reverse
 * engineered.
 * The Software source code shall only be used and adapted as stated in
 * writing in the application notes Sensirion delivered with The Software.
 * The Software shall only be used in conjunction with the Sensirion hardware
 * components it has been created for.
 *
 * The Software herein has been created for use with Sensirion SHT3x
 * sensor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES AND
 * REPRESENTATIONS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE OF THE SOFTWARE, ARE
 * HEREBY DISCLAIMED. EXCEPT FOR WILLFUL ACTS AND TO THE GREATEST EXTENT
 * PERMITTED BY APPLICABLE LAW, IN NO EVENT SHALL SENSIRION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, CLAIMS OF INTELLECTUAL PROPERTY
 * INFRINGEMENT, OR LOSS OF USE, DATA, OR PROFITS, OR BUSINESS INTERRUPTION,
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THE SOFTWARE OR PARTS THEREOF, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * For full SLA details please see SLA in Folder "5 Legal Documents" of this
 * Composer Kit.
 */

#ifdef Bran_R8
#include "sht3x.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


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

static int8_t sht_common_check_crc(uint8_t *data, uint16_t count, uint8_t checksum)
{
    if (sht_common_generate_crc(data, count) != checksum)
        return STATUS_CRC_FAIL;
    return STATUS_OK;
}

static int8_t sht_common_read_ticks(uint8_t address, int32_t *temperature_ticks, int32_t *humidity_ticks)
{
    uint8_t data[6];
    int8_t ret = sensirion_i2c_read(address, data, sizeof(data));
    if (ret)
        return ret;
    if (sht_common_check_crc(data, 2, data[2]) || sht_common_check_crc(data + 3, 2, data[5]))
        return STATUS_CRC_FAIL;

    *temperature_ticks = (data[1] & 0xff) | ((int32_t)data[0] << 8);
    *humidity_ticks = (data[4] & 0xff) | ((int32_t)data[3] << 8);

    return STATUS_OK;
}

static int8_t sht_common_read_measurement(uint8_t address, int32_t *temperature, int32_t *humidity)
{
    int8_t ret = sht_common_read_ticks(address, temperature, humidity);
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

int8_t sht_measure_blocking_read(int32_t *temperature, int32_t *humidity)
{
    int8_t ret = sht_measure();
    if (ret == STATUS_OK) {
        sensirion_sleep_usec(MEASUREMENT_DURATION_USEC);
        ret = sht_read(temperature, humidity);
    }
    return ret;
}

int8_t sht_measure()
{
    return sensirion_i2c_write(SHT3X_ADDRESS, CMD_MEASURE_HPM, COMMAND_SIZE);
}

int8_t sht_read(int32_t *temperature, int32_t *humidity)
{
    return sht_common_read_measurement(SHT3X_ADDRESS, temperature, humidity);
}

int8_t sht_probe()
{
    uint8_t data[3];
    sensirion_i2c_init();
    int8_t ret = sensirion_i2c_write(SHT3X_ADDRESS, CMD_READ_STATUS_REG, COMMAND_SIZE);
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



/*
 * Sensirion Temperature+Humidity Compensation
 * Version: cleargrass-iaq_v3
 */

/* Definition of global constants and variables */
static const int32_t c2[37] = {73727, 523692, 1610612736, 226892, 83610, -226492416, 523807, 1879048192, 219860, 81466,
                                -264241152, 73728, 523812, 181927, 95458, 523668, -78800, -72480, -61036, 220386,
                                -53012, -45906, 41856, 22937600, -23592960, 227092, 196653, -1610612736, 128922, -179304,
                                71745, 115475, 86886, -786432, 11726618, 18927601, 65536};
static const int16_t c1[24] = {1000, 25000, 500, -23156, -256, -31394, -26437, -30095, -16748, -26956,
                                -20282, -3476, 18432, 14894, -12888, -11160, -7316, 10176, -4071, -6553,
                                -4931, -845, 6554, 16384};
static const int8_t c0[12] = {19, 14, 5, 1, -21, -9, -20, -24, -55, 6,
                                8, -76};
static int64_t v[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0};
static int32_t t1[5] = {0, 0, 0, 0, 0};
static uint8_t t0[8] = {54, 130, 0, 0, 0, 0, 0, 0};

int8_t sht_measure_blocking_read_compensated_every_1_seconds(int32_t *temperature_ambient, int32_t *humidity_ambient, int32_t *raw_temperature, int32_t *raw_humidity) {
    int8_t ret = sht_measure();
    if (ret == STATUS_OK) {
        sensirion_sleep_usec(MEASUREMENT_DURATION_USEC);
        ret = sht_read_compensated_every_1_seconds(temperature_ambient, humidity_ambient, raw_temperature, raw_humidity);
 //       printf("-------------------------------------------------------------------- 原始值 t = %d  h = %d, rawt = %d, rawh =%d \r\n", *temperature_ambient, *humidity_ambient, *raw_temperature, *raw_humidity);
    }
    return ret;
}

int8_t sht_read_compensated_every_1_seconds(int32_t *temperature_ambient, int32_t *humidity_ambient, int32_t *raw_temperature, int32_t *raw_humidity) {
    int8_t ret; uint8_t i, j, k;
    ret = sht_common_read_ticks(SHT3X_ADDRESS, &t1[0], &t1[1]);
    ret |= sensirion_i2c_write(SHT3X_ADDRESS, t0, 2); sensirion_sleep_usec(c1[2]);
    ret |= sensirion_i2c_read(SHT3X_ADDRESS, t0+2, 6);

    *raw_temperature = ((21875 * t1[0]) >> 13) - 45000;
    *raw_humidity = ((12500 * t1[1]) >> 13);
//            printf("-------------------------------------------------------------------- 原始 rawt = %d, rawh =%d \r\n", *raw_temperature, *raw_humidity);
//        printf("-------------------------------------------------------------------- 原始值 t = %d  h = %d, rawt = %d, rawh =%d \r\n", *temperature_ambient, *humidity_ambient, *raw_temperature, *raw_humidity);
    get_status_CPU_f(&t1[2]); get_status_CPU_load(&t1[3]); get_status_LCD_bri(&t1[4]);
    v[1]=(int64_t)((((uint32_t)t0[2])<<24)|(((uint32_t)t0[3])<<16)|(((uint32_t)t0[5])<<8)|((uint32_t)t0[6]));
    v[2]=(int64_t)t1[0]; v[3]=(int64_t)t1[1]; v[4]=(int64_t)t1[2]; v[5]=(int64_t)t1[3]; v[6]=(int64_t)t1[4];
    if(v[0]==0) {
        v[0]=c0[3];
        v[10]=((c2[0])>>c0[2]);
        v[13]=((c2[20]*v[7])>>c0[0])+((c2[21]*v[8])>>c0[0])+((c1[7]*v[11])>>c0[0])+((c2[22]*v[12])>>c0[0])+((c2[23]*v[2])>>c0[0])+((c1[8]*v[4])>>c0[0])+((c1[9]*v[5])>>c0[0])+((c1[10]*v[6])>>c0[0])+((c1[11])>>c0[2])+((c2[24])>>c0[2]);
        v[14]=((c2[25]*v[7])>>c0[0])+((c2[26]*v[8])>>c0[0])+((c2[27]*v[9])>>c0[0])+((c2[28]*v[11])>>c0[0])+((c2[29]*v[12])>>c0[0])+((c1[12]*v[1])>>c0[0])+((c2[30]*v[4])>>c0[0])+((c2[31]*v[5])>>c0[0])+((c2[32]*v[6])>>c0[0])+((c1[13])>>c0[2])+((c2[33])>>c0[2]);
        v[15]=((c1[14]*v[7])>>c0[0])+((c1[15]*v[8])>>c0[0])+((c1[16]*v[11])>>c0[0])+((c1[17]*v[12])>>c0[0])+((c2[34]*v[2])>>c0[0])+((c1[18]*v[4])>>c0[0])+((c1[19]*v[5])>>c0[0])+((c1[20]*v[6])>>c0[0])+((c1[21])>>c0[2])+((c2[35])>>c0[2]);
        v[16]=c1[22]; for(i=0;i<c0[10];i++) {v[16]=v[16]+v[16]-((((v[16]*v[16])>>c0[0])*v[15])>>c0[1]);}
        v[17]=c1[23]+((v[14]*v[16])>>(c0[0]+c0[10])); for(j=0;j<c0[10];j++) {v[17]=((v[17]*v[17])>>c0[1]);} v[17]=((v[17]*v[3])>>c0[1]); if(v[17]>c2[36]) v[17]=c2[36];
    } else {
        int64_t vp[18]; for(k=0;k<18;k++) {vp[k]=v[k];}
        v[7]=((c2[1]*vp[7])>>c0[0])+((c0[4]*vp[8])>>c0[0])+((c2[2]*vp[10])>>c0[0])+((c0[5]*vp[11])>>c0[0])+((c0[6]*vp[12])>>c0[0])+((c2[3]*v[5])>>c0[0])+((c1[3]*v[6])>>c0[0])+((c2[4])>>c0[2])+((c2[5])>>c0[2]);
        v[8]=((c0[4]*vp[7])>>c0[0])+((c2[6]*vp[8])>>c0[0])+((c2[7]*vp[10])>>c0[0])+((c0[7]*vp[11])>>c0[0])+((c0[8]*vp[12])>>c0[0])+((c2[8]*v[6])>>c0[0])+((c2[9])>>c0[2])+((c2[10])>>c0[2]);
        v[9]=((c0[9]*v[1])>>c0[0])+((c1[4])>>c0[2]);
        v[10]=((c0[10]*vp[9])>>c0[0])+((c2[11])>>c0[2]);
        v[11]=((c0[5]*vp[7])>>c0[0])+((c0[7]*vp[8])>>c0[0])+((c2[7]*vp[10])>>c0[0])+((c2[12]*vp[11])>>c0[0])+((c0[11]*vp[12])>>c0[0])+((c2[13]*v[4])>>c0[0])+((c1[5]*v[5])>>c0[0])+((c1[6]*v[6])>>c0[0])+((c2[14])>>c0[2])+((c2[10])>>c0[2]);
        v[12]=((c0[6]*vp[7])>>c0[0])+((c0[8]*vp[8])>>c0[0])+((c2[2]*vp[10])>>c0[0])+((c0[11]*vp[11])>>c0[0])+((c2[15]*vp[12])>>c0[0])+((c2[16]*v[4])>>c0[0])+((c2[17]*v[5])>>c0[0])+((c2[18]*v[6])>>c0[0])+((c2[19])>>c0[2])+((c2[5])>>c0[2]);
        v[13]=((c2[20]*v[7])>>c0[0])+((c2[21]*v[8])>>c0[0])+((c1[7]*v[11])>>c0[0])+((c2[22]*v[12])>>c0[0])+((c2[23]*v[2])>>c0[0])+((c1[8]*v[4])>>c0[0])+((c1[9]*v[5])>>c0[0])+((c1[10]*v[6])>>c0[0])+((c1[11])>>c0[2])+((c2[24])>>c0[2]);
        v[14]=((c2[25]*v[7])>>c0[0])+((c2[26]*v[8])>>c0[0])+((c2[27]*v[9])>>c0[0])+((c2[28]*v[11])>>c0[0])+((c2[29]*v[12])>>c0[0])+((c1[12]*v[1])>>c0[0])+((c2[30]*v[4])>>c0[0])+((c2[31]*v[5])>>c0[0])+((c2[32]*v[6])>>c0[0])+((c1[13])>>c0[2])+((c2[33])>>c0[2]);
        v[15]=((c1[14]*v[7])>>c0[0])+((c1[15]*v[8])>>c0[0])+((c1[16]*v[11])>>c0[0])+((c1[17]*v[12])>>c0[0])+((c2[34]*v[2])>>c0[0])+((c1[18]*v[4])>>c0[0])+((c1[19]*v[5])>>c0[0])+((c1[20]*v[6])>>c0[0])+((c1[21])>>c0[2])+((c2[35])>>c0[2]);
        v[16]=c1[22]; for(i=0;i<c0[10];i++) {v[16]=v[16]+v[16]-((((v[16]*v[16])>>c0[0])*v[15])>>c0[1]);}
        v[17]=c1[23]+((v[14]*v[16])>>(c0[0]+c0[10])); for(j=0;j<c0[10];j++) {v[17]=((v[17]*v[17])>>c0[1]);} v[17]=((v[17]*v[3])>>c0[1]); if(v[17]>c2[36]) v[17]=c2[36];
    }
    *temperature_ambient = (int32_t)((c1[0]*v[13])>>c0[1]);
    *humidity_ambient = (int32_t)((c1[1]*v[17])>>c0[1]);
    return ret;
}
#endif
