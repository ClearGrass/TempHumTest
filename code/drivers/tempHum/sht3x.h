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

#include "sht_compensation_configuration.h"
#include "sensirion_configuration.h"





/**
 * \file
 *
 * \brief Sensirion SHT driver interface
 *
 * This module provides access to the SHT functionality over a generic I2C
 * interface. It supports measurements without clock stretching only.
 */

#ifndef SHT3X_H
#define SHT3X_H


#ifdef __cplusplus
extern "C" {
#endif

#define STATUS_OK 0
#define STATUS_ERR_BAD_DATA (-1)
#define STATUS_CRC_FAIL (-2)
#define STATUS_UNKNOWN_DEVICE (-3)

/**
 * Detects if a sensor is connected by reading out the ID register.
 * If the sensor does not answer or if the answer is not the expected value,
 * the test fails.
 *
 * @return 0 if a sensor was detected
 */
int8_t sht_probe(void);

/**
 * Starts a measurement and then reads out the results. This function blocks
 * while the measurement is in progress. The duration of the measurement depends
 * on the sensor in use, please consult the datasheet.
 * Temperature is returned in [degree Celsius], multiplied by 1000,
 * and relative humidity in [percent relative humidity], multiplied by 1000.
 *
 * @param temperature   the address for the result of the temperature measurement
 * @param humidity      the address for the result of the relative humidity measurement
 * @return              0 if the command was successful, else an error code.
 */
int8_t sht_measure_blocking_read(int32_t *temperature, int32_t *humidity);

/**
 * Starts a measurement in high precision mode. Use sht_read() to read out the values,
 * once the measurement is done. The duration of the measurement depends on the sensor
 * in use, please consult the datasheet.
 *
 * @return     0 if the command was successful, else an error code.
 */
int8_t sht_measure(void);

/**
 * Reads out the results of a measurement that was previously started by
 * sht_measure(). If the measurement is still in progress, this function
 * returns an error.
 * Temperature is returned in [degree Celsius], multiplied by 1000,
 * and relative humidity in [percent relative humidity], multiplied by 1000.
 *
 * @param temperature   the address for the result of the temperature measurement
 * @param humidity      the address for the result of the relative humidity measurement
 * @return              0 if the command was successful, else an error code.
 */
int8_t sht_read(int32_t *temperature, int32_t *humidity);

#ifndef SHT_GET_ADDR
#define SHT_GET_ADDR
/**
 * Returns the configured SHTxx address.
 *
 * @return SHTxx_ADDRESS
 */
uint8_t sht_get_configured_sht_address(void);
#endif /* SHT_GET_ADDR */

#ifdef __cplusplus
}
#endif

#endif /* SHT3X_H */



#ifndef SHT_COMPENSATION_H
#define SHT_COMPENSATION_H


#ifdef __cplusplus
extern "C" {
#endif
/*
 * Sensirion Temperature+Humidity Compensation
 * Version: cleargrass-iaq-project-name-bran_v5
 */

/**
 * Measures RH/T and performs the Sensirion Temperature+Humidity Compensation.
 * Must be executed exactly every 1 seconds.
 * This function blocks while the measurement is in progress. The duration of
 * the measurement depends on the sensor in use, please consult the datasheet.
 * Temperature is returned in [degree Celsius] (value multiplied by 1000).
 * Relative humidity is returned in [percent relative humidity] (value multiplied by 1000).
 *
 * @param temperature_ambient  pointer to a variable, where compensated temperature
 *                             should be stored (value multiplied by 1000)
 * @param humidity_ambient     pointer to a variable, where compensated relative
 *                             humidity should be stored (value multiplied by 1000)
 * @return                     0 if the command was successful, else an error code.
 */
int8_t sht_measure_blocking_read_compensated_every_1_seconds(float *temperature_ambient, float *humidity_ambient, float *temperature_raw, float *humidity_raw,
                                                             float *status_charging_on, float *status_CPU_load,float *status_WIFI_f, float *status_CPU_load_CPU_f, float *status_LCD_bri);


/**
 * Reads a finished measurement and performs the Sensirion Temperature+Humidity Compensation.
 * Must be executed exactly every 1 seconds.
 * The measurement must have been previously started using sht_measure().
 * If the measurement is still in progress, this function returns an error.
 * Temperature is returned in [degree Celsius] (value multiplied by 1000).
 * Relative humidity is returned in [percent relative humidity] (value multiplied by 1000).
 *
 * @param temperature_ambient  pointer to a variable, where compensated temperature
 *                             should be stored (value multiplied by 1000)
 * @param humidity_ambient     pointer to a variable, where compensated relative
 *                             humidity should be stored (value multiplied by 1000)
 * @return                     0 if the command was successful, else an error code.
 */
int8_t sht_read_compensated_every_1_seconds(float *temperature_ambient, float *humidity_ambient, float *temperature_raw, float *humidity_raw,
                                            float *status_charging_on, float *status_CPU_load,float *status_WIFI_f, float *status_CPU_load_CPU_f, float *status_LCD_bri);



void sht_compensate_every_1_seconds(float temperature_sht, float humidity_sht, float status_WIFI_f, float status_LCD_f, float status_CPU_usage, float status_charging_f, float *temperature_ambient, float *humidity_ambient);
#ifdef __cplusplus
}
#endif

#endif /* SHT_COMPENSATION_H */

#endif

