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

//#include <linux/module.h>
//#include <linux/init.h>
//#include <linux/slab.h>
//#include <linux/i2c.h>
//#include <linux/hwmon.h>
//#include <linux/hwmon-sysfs.h>
//#include <linux/err.h>
//#include <linux/delay.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "sensirion_configuration.h"
#include "sht3x.h"



uint8_t sht_address;
int i2c_device;

/*
 * INSTRUCTIONS
 * ============
 *
 * Implement all functions where they are marked as IMPLEMENT.
 * Follow the function specification in the comments.
 */

/**
 * Initialize all hard- and software components that are needed for the I2C
 * communication.
 */
void sensirion_i2c_init()
{

    i2c_device = open(I2C_DEVICE_PATH, O_RDWR);
    if (i2c_device < 0)
    {
        return;
    }
    /* get sht address of used sensor */
    sht_address = sht_get_configured_sht_address();
    /* open specific device */
    ioctl(i2c_device, I2C_SLAVE, sht_address);
}

int sensirion_i2c_catch()
{
    int i = 0;
    i2c_device = 0;
    do
    {
        i++;
        i2c_device = open(I2C_DEVICE_PATH, O_RDWR);
        if (i >= 10)
        {
            printf("-------------------------------------------------SHT30 I2c  打开失败\r\n");
            return -1;
        }
        usleep(10000);
    }while(i2c_device < 0);

    /* get sht address of used sensor */
    sht_address = sht_get_configured_sht_address();
    /* open specific device */
    ioctl(i2c_device, I2C_SLAVE, sht_address);

    return 0;
}

void sensirion_i2c_free()
{

    close(i2c_device);

}

/**
 * Execute one read transaction on the I2C bus, reading a given number of bytes.
 * If the device does not acknowledge the read command, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to read from
 * @param data    pointer to the buffer where the data is to be stored
 * @param count   number of bytes to read from I2C and store in the buffer
 * @returns 0 on success, error code otherwise
 */
short sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count)
{
//    if (sensirion_i2c_catch() != 0)
//    {
//        return I2C_READ_FAILED;
//    }
    if (sht_address != address)
    {
        return I2C_READ_FAILED;
    }
    if (read(i2c_device, data, count) != (int)count)
    {
        return I2C_READ_FAILED;
    }

//    sensirion_i2c_free();
	return 0;
 
}

/**
 * Execute one write transaction on the I2C bus, sending a given number of bytes.
 * The bytes in the supplied buffer must be sent to the given address. If the
 * slave device does not acknowledge any of the bytes, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to write to
 * @param data    pointer to the buffer containing the data to write
 * @param count   number of bytes to read from the buffer and send over I2C
 * @returns 0 on success, error code otherwise
 */
short sensirion_i2c_write(uint8_t address, const uint8_t* data, uint16_t count)
{
//    if (sensirion_i2c_catch() != 0)
//    {
//        return I2C_WRITE_FAILED;
//    }
    if (sht_address != address)
    {
        return I2C_WRITE_FAILED;
    }
    if (write(i2c_device, data, count) != (int)count)
    {
        return I2C_WRITE_FAILED;
    }

//    sensirion_i2c_free();

    return 0;
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(int useconds) {
    // IMPLEMENT
	usleep(useconds);
}

#endif
