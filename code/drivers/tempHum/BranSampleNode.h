//
// Created by Yueqian.bao on 17/9/24.
//

#ifndef TEMPHUMFIXENGINE_BRANSAMPLENODE_H
#define TEMPHUMFIXENGINE_BRANSAMPLENODE_H

#include "EngineCommon.h"


#include <ctime>
#include <time.h>
#include "globalVariable.h"



class BranSampleNode {
public:
    void init();

    bool operator<(const BranSampleNode& node) {return m_TimeStamp < node.m_TimeStamp;}
    bool operator>(const BranSampleNode& node) {return m_TimeStamp > node.m_TimeStamp;}
    bool operator==(const BranSampleNode& node) {return m_TimeStamp == node.m_TimeStamp;}

public:
    string m_Flag;
    int m_ChargingOn;
    int m_CpuLoad;
    int m_CpuF;
    int m_CpuLoadF;
    int m_LCDBrightness;
    string m_DateTime;
    bool m_PM25Running;
    int m_PM25;
    float m_Temp;
    float m_TempRaw;
    float m_TempRaw2;
    float m_Humidity;
    float m_HumidityRaw;
    string m_BaseLine;
    int  m_TVOC;
    int m_CO2e;
    int m_CPUFreq;
    float m_CPUUsage;
    int m_Light;
    bool m_Charging;
    float m_Voltage;
    int m_CurrentMA;
    int m_Capacity;
    int m_WiFiStatus;
    bool m_ScreenOn;

    long long m_TimeStamp;
    float m_FixTemp;
};


#endif //TEMPHUMFIXENGINE_BRANSAMPLENODE_H
