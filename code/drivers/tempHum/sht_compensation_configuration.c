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



#define  DUBUG_INTERVAL     10

/*
 * Sensirion Temperature+Humidity Compensation
 * Version: cleargrass-iaq-project-name-bran_v5
 */

/**
 * Gets the status of charging_on get_status_charging_on device and stores it in status_charging_on.
 * status_charging_on must be in between 0...1000, i.e. status_charging_on must have 0 as minimum value and 1000 as maximum value.
 * Hint from technical support: Current status of device. status_charging_on=0 if device runs on battery, status_charging_on=1000 if device is charging.
**/
void get_status_charging_on(int32_t *status_charging_on){
    // IMPLEMENT
    FILE  *stream;
    char  buf[4096];;
    int current_now;
    static int stamp = 0;

    stamp++;

    // 获取cpu主频
    stream = popen("cat /sys/class/power_supply/battery/current_now", "r");
    fread(buf, sizeof(char), sizeof(buf),  stream);
    current_now = atoi(buf) / 1000;

    *status_charging_on = current_now;
    if(*status_charging_on > 160)
    {
        *status_charging_on = 1000;
    }
    else if(*status_charging_on <= 160)
    {
        *status_charging_on = 0;
    }

    if (stamp % DUBUG_INTERVAL == 0)
    {
        printf("------------------------------------------当前电流 current_now = %d \r\n", *status_charging_on);
    }
    pclose(stream);
}

/**
 * Gets the status of CPU_load_CPU_f from device and stores it in status_CPU_load_CPU_f.
 * status_CPU_load_CPU_f must be in between 0...1000, i.e. status_CPU_load_CPU_f must have 0 as minimum value and 1000 as maximum value.
 * Hint from technical support: This flag is for both CPU_load mode and CPU_f mode. And assume that the total self-heating of CPU_load mode is c1, the total self-heating of CPU_f mode is c2.For CPU_load mode, CPU_load =1000 is 100% load; CPU_load=500 is 50% ; CPU_load=0 is 0%;For CPU_f mode, CPU_f =0 is at 120MHz; CPU_f=1000 is at 1.008GHz; then CPU_load_CPU_f = CPU_load*c1/(c1+c2) + CPU_f*c2/(c1+c2).
**/
void get_status_CPU_load_CPU_f(int32_t *status_CPU_load_CPU_f,int32_t *status_CPU_load, int32_t *status_CPU_f){
    // IMPLEMENT
    int cpu_load, cpu_f;
    get_status_CPU_load(&cpu_load);
    get_status_CPU_f(&cpu_f);
    *status_CPU_load = cpu_load;
    *status_CPU_f = cpu_f;
    *status_CPU_load_CPU_f  = cpu_load * 280 *1.0 / 535 + cpu_f * 255 * 1.0 / 535 ;
}

/**
 * Gets the status of LCD_bri from device and stores it in status_LCD_bri.
 * status_LCD_bri must be in between 0...1000, i.e. status_LCD_bri must have 0 as minimum value and 1000 as maximum value.
 * Hint from technical support: LCD_bri=1000 is 100% brightness (max); LCD_bri= 500 is 50% brightness; LCD_bri=0 is 0% brightness;
**/
void get_status_LCD_bri(int32_t *status_LCD_bri){
    // IMPLEMENT
    FILE  *stream;
    char  buf[4096];;
    int light;
    static int stamp = 0;

    stamp++;

    // 获取cpu主频
    stream = popen("cat /usr/bin/qtapp/light", "r");
    fread(buf, sizeof(char), sizeof(buf),  stream);
    light = atoi(buf);

    *status_LCD_bri = light;
    if(*status_LCD_bri > 1000)
    {
        *status_LCD_bri = 1000;
    }
    else if(*status_LCD_bri < 0)
    {
        *status_LCD_bri = 0;
    }

    pclose(stream);
}

/**
 * Gets the status of CPU_load from device and stores it in status_CPU_load.
 * status_CPU_load must be in between 0...1000, i.e. status_CPU_load must have 0 as minimum value and 1000 as maximum value.
 * Hint from technical support: CPU_load =1000 is 100% load; CPU_load=500 is 50% ; CPU_load=0 is 0%
**/
void get_status_CPU_load(int *status_CPU_load){
    // IMPLEMENT
    FILE  *stream;
    char  buf[4096];
    float cpuLoad;
    char charlist[50][50]={""};
    char seg[] = " ";
    int i = 0;
    static int stamp = 0;

    stamp++;
    // 获取cpu占用数据
    stream = popen("top -b -n 1 | grep BranQt4 | grep -v grep", "r");
    fread( buf, sizeof(char), sizeof(buf),  stream);
    pclose(stream);
    char *substr= strtok(buf, seg);
    while (substr != NULL)
    {
        strcpy(charlist[i],substr);
        //printf("%s\n", substr);
        i++;
        substr = strtok(NULL,seg);
    }
    if(i >= 8)
    {
        cpuLoad = atof(charlist[8]);
        *status_CPU_load = (int)(cpuLoad * 10);
    }
    if(*status_CPU_load > 1000)
    {
        *status_CPU_load = 1000;
    }
    else if(*status_CPU_load < 0)
    {
        *status_CPU_load = 0;
    }

    if (stamp % DUBUG_INTERVAL == 0)
    {
        printf("------------------------------------------当前负载 %d\r\n", *status_CPU_load);
    }
}


/**
 * Gets the status of CPU_f from device and stores it in status_CPU_f.
 * status_CPU_f must be in between 0...1000, i.e. status_CPU_f must have 0 as minimum value and 1000 as maximum value.
 * Hint from technical support: CPU_f=0  is 120MHz mode; CPU_f = 50 is 50% mode; CPU_f=100 is 1.008GHz mode
**/

void get_status_CPU_f(int *status_CPU_f)
{

    // IMPLEMENT
    FILE  *stream;
    char  buf[4096];
    double fre;
    static int stamp = 0;

    stamp++;

    // 获取cpu主频
    stream = popen("cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq", "r");
    fread(buf, sizeof(char), sizeof(buf),  stream);
    fre = atof(buf);
    fre = fre/1000000.0;
    pclose(stream);
    if(fre > 0.9)
    {
        if (stamp % DUBUG_INTERVAL == 0)
        {
            printf("------------------------------------------当前主频率 1.008G\r\n");
        }
        *status_CPU_f = 1000;
    }
    else
    {
        if (stamp % DUBUG_INTERVAL == 0)
        {
            printf("------------------------------------------当前主频率 120M\r\n");
        }
        *status_CPU_f = 0;
    }
}


#endif

