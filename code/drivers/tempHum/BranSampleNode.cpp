//
// Created by Yueqian.bao on 17/9/24.
//

#include "BranSampleNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

time_t strTime2unix(const char timeStamp[])
{
    struct tm tm;
    memset(&tm, 0, sizeof(tm));

    sscanf(timeStamp, "%d-%d-%d-%d-%d-%d",
           &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
           &tm.tm_hour, &tm.tm_min, &tm.tm_sec);

    tm.tm_year -= 1900;
    tm.tm_mon--;

    return mktime(&tm);
}


void BranSampleNode::init()
{
    this->m_ChargingOn = g_loginfo.status_chargingOn;
    this->m_CpuLoad = g_loginfo.status_cpuLoad;
    this->m_CpuF = g_loginfo.status_cpuF;
    this->m_CpuLoadF = g_loginfo.status_cpuLoadF;
    this->m_LCDBrightness = g_loginfo.status_LCDBrightness;
    this->m_DateTime = g_loginfo.sDatetime.toStdString();
    this->m_PM25Running = g_loginfo.pm25Running;
    this->m_PM25 = g_loginfo.pm25;
    this->m_Temp = g_loginfo.cleargrass_temp;
    this->m_TempRaw = g_loginfo.temp_raw;
    this->m_TempRaw2 = g_loginfo.second_temp_raw;
    this->m_Humidity = g_loginfo.cleargrass_hum;
    this->m_HumidityRaw = g_loginfo.hum_raw;
    this->m_BaseLine = g_loginfo.baseline.toStdString();
    this->m_TVOC = g_loginfo.tVOC;
    this->m_CO2e = g_loginfo.CO2e;
    this->m_CPUFreq = qRound(g_loginfo.cpu_fre);
    this->m_CPUUsage = g_loginfo.cpu_load;
    this->m_Light = g_loginfo.light;
    this->m_Charging = g_loginfo.charging;
    this->m_Voltage = g_loginfo.voltage;
    this->m_CurrentMA = g_loginfo.currentMA;
    this->m_Capacity = g_loginfo.capacity;
    this->m_WiFiStatus = g_loginfo.wifi_status;
    this->m_ScreenOn = g_loginfo.screenOn;

    m_TimeStamp = strTime2unix(m_DateTime.c_str());

}
