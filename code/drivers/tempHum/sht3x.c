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
 * Version: cleargrass-iaq-project-name-bran_v5
 */

/* Definition of global constants and variables */
static const int32_t c2[35] = {73727, 523813, 1879048192, 221308, -264241152, 523748, 204580, -51814, 523789, 1610612736,
                               425651, -226492416, 523696, -64574, -164156, -58118, 73728, -62075, -58661, -62143,
                               22937600, -41969, -23592960, -1610612736, 265917, 251294, 266207, 37255, 93563, 179788,
                               86886, -786432, 11726618, 18927601, 65536};
static const int16_t c1[26] = {1000, 25000, 500, -256, 10139, -18344, 9525, -24647, 12797, 30177,
                               -8696, -21841, -20282, -4288, 18432, 18372, -15091, -14261, -15108, -2114,
                               -5310, -10203, -4931, -1042, 6554, 16384};
static const float c0[107] = {1.0000000000, 0.9894115925, -0.0050286897, 0.0035573428, -0.0025373292, -0.0176862087, 0.0666943341, 0.0039762715, 1.0015621185, 0.0051123165,
                              0.0029432077, -0.0077405563, 0.0301103573, 0.0095754825, 0.0040270253, 0.9699417353, -0.0042977994, 0.0504533462, 0.0276855268, -0.0050600190,
                              -0.0024013105, 0.0074735954, 0.9989154935, 0.0005009650, 0.0315656438, 0.9986017346, -0.0010051042, 0.0000788704, -0.0001593348, 0.0017001175,
                              -0.0009503227, 0.0000117839, 0.9998781085, 0.0005055637, 0.0004446321, 0.0009435145, -0.0005258211, 0.0013020372, 0.0005841971, 0.9974586964,
                              -0.0013308909, 0.0010007482, 0.0021077823, -0.0007078543, -0.0005487924, 0.0005360773, 0.9997127056, 0.0011542848, 0.0000020970, 0.9956094623,
                              -0.0017681960, 0.0295550581, 0.0082308045, 1.0032920837, 0.0121799996, -0.1430058628, -0.1127503887, -0.1328477114, -0.1286462694, 0.0005608799,
                              -0.0000264320, -0.0011427251, -0.0016119533, -0.0923265517, -0.0380489007, -0.0034561816, -0.0000020776, -0.0000029368, -0.0031953431, -0.0207886845,
                              0.5549640656, 0.0024739224, 0.0023754274, 3.1968047619, 8.4792108536, 16.5520019531, 7.9284338951, 7.9480309486, 0.5159306526, 0.6685907841,
                              0.3333463967, 0.0619914420, -0.1749899834, 0.8352339268, 0.2925345600, 1.8776977062, 0.2246724516, -0.0592944771, 0.3155021071, 2.1917438507,
                              -0.0131601393, 0.0676164255, 0.0438348800, 0.3333333433, 0.0563751422, -0.2896538973, -0.1877789795, 4.2837800980, -1.4279266596, -0.0031994930,
                              0.0164389051, 0.0106571354, 0.2681199908, 0.0810400024, 59.1073341370, 256.0000000000, 100.0000000000};
static float v[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0};
static int32_t t1[5] = {0, 0, 0, 0, 0};
static uint8_t t0[8] = {54, 130, 0, 0, 0, 0, 0, 0};

int8_t sht_measure_blocking_read_compensated_every_1_seconds(float *temperature_ambient, float *humidity_ambient, float *temperature_raw, float *humidity_raw,
                                                             float *status_charging_on, float *status_CPU_load,float *status_WIFI_f, float *status_CPU_load_CPU_f, float *status_LCD_bri) {
    int8_t ret = sht_measure();
    if (ret == STATUS_OK) {
        sensirion_sleep_usec(MEASUREMENT_DURATION_USEC);
        ret = sht_read_compensated_every_1_seconds(temperature_ambient, humidity_ambient,temperature_raw,humidity_raw,status_charging_on,status_CPU_load,status_WIFI_f,status_CPU_load_CPU_f,status_LCD_bri);
    }
    return ret;
}

int8_t sht_read_compensated_every_1_seconds(float *temperature_ambient, float *humidity_ambient, float *temperature_raw, float *humidity_raw,
                                            float *status_charging_on, float *status_CPU_load,float *status_WIFI_f, float *status_CPU_load_CPU_f, float *status_LCD_bri) {
    int8_t ret; uint8_t i, j, k;
    ret = sht_common_read_ticks(SHT3X_ADDRESS, &t1[0], &t1[1]);
    ret |= sensirion_i2c_write(SHT3X_ADDRESS, t0, 2);
    sensirion_sleep_usec(c1[2]);
    ret |= sensirion_i2c_read(SHT3X_ADDRESS, t0+2, 6);
    *temperature_raw = (((21875 * t1[0]) >> 13) - 45000)/1000.0;
    *humidity_raw = ((12500 * t1[1]) >> 13) / 1000.0;
    get_status_charging_on(status_charging_on);
    get_status_LCD_bri(status_LCD_bri);
    get_status_CPU_load(status_CPU_load);
    *status_CPU_load_CPU_f = -1;

    sht_compensate_every_1_seconds(*temperature_raw, *humidity_raw, *status_WIFI_f, *status_LCD_bri, *status_CPU_load, *status_charging_on, temperature_ambient, humidity_ambient);

    return ret;
}


void sht_compensate_every_1_seconds(float temperature_sht, float humidity_sht, float status_WIFI_f, float status_LCD_f, float status_CPU_usage, float status_charging_f, float *temperature_ambient, float *humidity_ambient) {
    uint8_t j, k;


    v[1]=temperature_sht; v[2]=humidity_sht; v[3]=status_WIFI_f; v[4]=status_LCD_f; v[5]=status_CPU_usage; v[6]=status_charging_f;
    if(v[0]==0) {
        v[0]=c0[0];
        v[17]=c0[0]*v[1]+c0[65]*v[3]+c0[66]*v[4]+c0[67]*v[5]+c0[68]*v[6]+c0[69];
        v[19]=c0[75]*v[17];
        v[20]=c0[76]*v[17];
        v[21]=c0[77]*v[17];
        v[22]=c0[90]*v[19]+c0[91]*v[20]+c0[92]*v[21]+c0[93]*v[17];
        v[23]=c0[94]*v[19]+c0[95]*v[20]+c0[96]*v[21]+c0[97]*v[1]+c0[98]*v[17];
        v[24]=c0[99]*v[19]+c0[100]*v[20]+c0[101]*v[21]+c0[102]*v[1]+c0[103]*v[17]+c0[104];
        v[25]=c0[0]+(v[23]/v[24])/c0[105]; for(j=0;j<8;j++) {v[25]=v[25]*v[25];} v[25]=v[25]*v[2]; if(v[25]>c0[106]) v[25]=c0[106];
    } else {
        float vp[26]; int k; for(k=0;k<26;k++) {vp[k]=v[k];}
        v[7]=c0[1]*vp[7]+c0[2]*vp[8]+c0[3]*vp[9]+c0[4]*vp[10]+c0[5]*v[3]+c0[6];
        v[8]=c0[7]*vp[7]+c0[8]*vp[8]+c0[9]*vp[9]+c0[10]*vp[10]+c0[11]*v[3]+c0[12];
        v[9]=c0[13]*vp[7]+c0[14]*vp[8]+c0[15]*vp[9]+c0[16]*vp[10]+c0[17]*v[3]+c0[18];
        v[10]=c0[19]*vp[7]+c0[20]*vp[8]+c0[21]*vp[9]+c0[22]*vp[10]+c0[23]*v[3]+c0[24];
        v[11]=c0[25]*vp[11]+c0[26]*vp[12]+c0[27]*vp[13]+c0[28]*vp[14]+c0[29]*v[4]+c0[30]*v[5];
        v[12]=c0[31]*vp[11]+c0[32]*vp[12]+c0[33]*vp[13]+c0[34]*vp[14]+c0[35]*v[4]+c0[36]*v[5];
        v[13]=c0[37]*vp[11]+c0[38]*vp[12]+c0[39]*vp[13]+c0[40]*vp[14]+c0[41]*v[4]+c0[42]*v[5];
        v[14]=c0[43]*vp[11]+c0[44]*vp[12]+c0[45]*vp[13]+c0[46]*vp[14]+c0[47]*v[4]+c0[48]*v[5];
        v[15]=c0[49]*vp[15]+c0[50]*vp[16]+c0[51]*v[6];
        v[16]=c0[52]*vp[15]+c0[53]*vp[16]+c0[54]*v[6];
        v[17]=c0[55]*v[7]+c0[56]*v[8]+c0[57]*v[9]+c0[58]*v[10]+c0[59]*v[11]+c0[60]*v[12]+c0[61]*v[13]+c0[62]*v[14]+c0[63]*v[15]+c0[64]*v[16]+c0[0]*v[1]+c0[65]*v[3]+c0[66]*v[4]+c0[67]*v[5]+c0[68]*v[6]+c0[69];
        v[18]=c0[55]*v[7]+c0[56]*v[8]+c0[57]*v[9]+c0[58]*v[10]+c0[59]*v[11]+c0[60]*v[12]+c0[61]*v[13]+c0[62]*v[14]+c0[63]*v[15]+c0[64]*v[16]+c0[70]*v[3]+c0[71]*v[4]+c0[72]*v[5]+c0[73]*v[6]+c0[74]; if(v[18]<0) {v[18]=0;}
        v[19]=c0[78]*vp[19]+c0[79]*vp[20]+c0[80]*vp[21]+c0[81]*v[17];
        v[20]=c0[82]*vp[19]+c0[83]*vp[20]+c0[84]*vp[21]+c0[85]*v[17];
        v[21]=c0[86]*vp[19]+c0[87]*vp[20]+c0[88]*vp[21]+c0[89]*v[17];
        v[22]=c0[90]*v[19]+c0[91]*v[20]+c0[92]*v[21]+c0[93]*v[17];
        v[23]=c0[94]*v[19]+c0[95]*v[20]+c0[96]*v[21]+c0[97]*v[1]+c0[98]*v[17];
        v[24]=c0[99]*v[19]+c0[100]*v[20]+c0[101]*v[21]+c0[102]*v[1]+c0[103]*v[17]+c0[104];
        v[25]=c0[0]+(v[23]/v[24])/c0[105]; for(j=0;j<8;j++) {v[25]=v[25]*v[25];} v[25]=v[25]*v[2]; if(v[25]>c0[106]) v[25]=c0[106];
    }
    *temperature_ambient = v[22];
    *humidity_ambient = v[25];

//    printf("temperature_sht= %f\r\n", temperature_sht);
//    printf("humidity_sht= %f\r\n", humidity_sht);
//    printf("status_WIFI_f= %f\r\n", status_WIFI_f);
//    printf("status_LCD_f= %f\r\n", status_LCD_f);
//    printf("status_CPU_usage= %f\r\n", status_CPU_usage);
//    printf("status_charging_f= %f\r\n", status_charging_f);
//    printf("temperature_ambient= %f\r\n", *temperature_ambient);
//    printf("humidity_ambient= %f\r\n", *humidity_ambient);


}


#endif

