/*
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

#ifndef SENSIRION_CONFIGURATION_H
#define SENSIRION_CONFIGURATION_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The I2C address of the SHT sensor. Only uncomment if you have a special
 * version of a sensor, such as the SHTW2A.
 */
/* #define SHT_ADDRESS 0x70 */

/**
 * The clock period of the i2c bus in microseconds. Increase this, if your GPIO
 * ports cannot support a 200 kHz output rate.
 */
#define I2C_CLOCK_PERIOD_USEC 10

/**
 * Typedef section for types commonly defined in <stdint.h>
 * You are welcome to use #include <stdint.h> instead and discard
 * the list by uncommenting the following #define. In this case, please make sure
 * int64_t is also defined!
 */
/* #define USE_SENSIRION_STDINT_TYPES 0 */

/**
 * Uncomment the following line to enable clock stretching.
 * Note that clock stretching will keep the bus busy until the sensor releases it
 * and will impact the communication with other i2c devices on the same bus.
 */
/* #define USE_SENSIRION_CLOCK_STRETCHING 1 */



#ifndef USE_SENSIRION_STDINT_TYPES
#define USE_SENSIRION_STDINT_TYPES 1
#endif /* USE_SENSIRION_STDINT_TYPES */
#if USE_SENSIRION_STDINT_TYPES
typedef long long int int64_t;
//typedef long int32_t;
typedef unsigned long uint32_t;
typedef short int16_t;
typedef unsigned short uint16_t;
//typedef char int8_t;
typedef unsigned char uint8_t;
#endif /* USE_SENSIRION_STDINT_TYPES */

#ifndef USE_SENSIRION_CLOCK_STRETCHING
#define USE_SENSIRION_CLOCK_STRETCHING 0
#endif /* USE_SENSIRION_CLOCK_STRETCHING */


#ifdef __cplusplus
}
#endif

#endif /* SENSIRION_CONFIGURATION_H */



#ifndef I2C_H
#define I2C_H


#ifdef __cplusplus
extern "C" {
#endif

#define     I2C_DEVICE_PATH         "/dev/i2c-2"
#define     I2C_READ_FAILED         -1
#define     I2C_WRITE_FAILED        -2

void sensirion_i2c_init(void);

short sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count);

short sensirion_i2c_write(uint8_t address, const uint8_t* data, uint16_t count);

void sensirion_sleep_usec(uint32_t useconds);

#ifdef __cplusplus
}
#endif

#endif /* I2C_H */

#endif
