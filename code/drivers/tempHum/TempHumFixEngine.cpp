//
// Created by Yueqian.bao on 17/9/24.
//

#include "TempHumFixEngine.h"
#include "LogControl.h"

#define CONST_TEMP_DIFF (0.3)
#define CONST_CHARGING_MA (428)

TempHumFixEngine::TempHumFixEngine()
{
    logText = "";
    m_HighestChargingMA = CONST_CHARGING_MA;

    int basicAnchor[] = {65, 82, 119, 163, 224, 306, 371, 584, 1016, 3053};
    m_BasicPart = new TempFixPart("basic mode", basicAnchor, 1, 6.45);

    int wifiAnchor[] = {37, 50, 87, 124, 178,  253, 396, 590, 725, 1218};
    m_WifiPart = new TempFixPart("wifi", wifiAnchor, 1, 0.96);

    int screenOffAnchor[] = {154, 208, 310, 221, 270, 293, 422, 562, 1118, 2376};
    m_ScreenOffPart = new TempFixPart("screen off", screenOffAnchor, 1, -0.40);

    int highCpuAnchor[] = {100, 124, 149, 175, 205, 264, 331, 540, 794, 2298};
    m_HighCpuFreqPart = new TempFixPart("high cpu freq", highCpuAnchor, 1, 2.1);

    int brightAnchor[] = {138, 127, 131, 149, 198, 241, 373, 425, 858, 2054};
    m_BrightnessPart = new TempFixPart("brightness", brightAnchor, 1000, 2.35);

    int cpuLoadAnchor[] = {84, 112, 124, 148, 241, 307, 369, 441, 624, 1636};
    m_CpuLoadPart = new TempFixPart("cpu load", cpuLoadAnchor, 1000, 2.85);

    int batteryAnchor[] = {88, 134, 874, 1612, 1770, 1957, 2481, 2870, 2746, 4860};
    m_BatteryPart = new TempFixPart("battery", batteryAnchor, 1, -0.80);

    int chargingAnchor[] = {50, 102, 124, 198, 241, 307, 373, 495, 724, 2636};
    m_ChargingPart = new TempFixPart("charging", chargingAnchor, CONST_CHARGING_MA * CONST_CHARGING_MA, 3.05);

    int almostFullAnchor[] = {125, 156, 176, 194, 232, 246, 134, 301, 234, 249};
    m_TempAlmostFullPart = new TempFixPart("almost full", almostFullAnchor, 1, -0.80);


    m_WarmStartPart = NULL;
}

TempHumFixEngine::~TempHumFixEngine()
{
    delete m_BasicPart;
    delete m_WifiPart;
    delete m_ScreenOffPart;
    delete m_HighCpuFreqPart;
    delete m_BrightnessPart;
    delete m_CpuLoadPart;
    delete m_BatteryPart;
    delete m_ChargingPart;
    delete m_TempAlmostFullPart;
}

float TempHumFixEngine::fixTemp(const BranSampleNode& node)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss:zzz");

    float resultTemp = node.m_TempRaw;
    resultTemp -= m_BasicPart->adjust(1);
    if(g_tempSaveLog)
        logText += QString("[%1]ClearGrass温补信息：m_BasicPart,resultTemp = %2\n").arg(time).arg(resultTemp);
    resultTemp -= m_WifiPart->adjust((node.m_WiFiStatus == 1 ||node.m_WiFiStatus== 2) ? 0 : 1);
    if(g_tempSaveLog)
        logText += QString("[%1]ClearGrass温补信息：m_WifiPart,resultTemp = %2\n").arg(time).arg(resultTemp);
    resultTemp -= m_ScreenOffPart->adjust(node.m_ScreenOn == 0 ? 1 : 0);
    if(g_tempSaveLog)
        logText += QString("[%1]ClearGrass温补信息：m_ScreenOffPart,resultTemp = %2\n").arg(time).arg(resultTemp);
    resultTemp -= m_HighCpuFreqPart->adjust(node.m_CPUFreq == 1 ? 1 : 0);
    if(g_tempSaveLog)
        logText += QString("[%1]ClearGrass温补信息：m_HighCpuFreqPart,resultTemp = %2\n").arg(time).arg(resultTemp);
    resultTemp -= m_BrightnessPart->adjust(node.m_LCDBrightness);
    if(g_tempSaveLog)
        logText += QString("[%1]ClearGrass温补信息：m_BrightnessPart,resultTemp = %2\n").arg(time).arg(resultTemp);
    if(node.m_CPUFreq == 1)
    {
        resultTemp -= m_CpuLoadPart->adjust(node.m_CpuLoad);
        if(g_tempSaveLog)
            logText += QString("[%1]ClearGrass温补信息：m_CpuLoadPart,resultTemp = %2\n").arg(time).arg(resultTemp);
    }
    else
    {
        resultTemp -= m_CpuLoadPart->adjust(0);

    }
    resultTemp -= m_BatteryPart->adjust(node.m_Charging ? 0 : 1);
    if(g_tempSaveLog)
        logText +=QString("[%1]ClearGrass温补信息：m_BatteryPart,resultTemp = %2\n").arg(time).arg(resultTemp);
    if(node.m_CurrentMA > m_HighestChargingMA)
        m_HighestChargingMA = node.m_CurrentMA;
    int ma = node.m_CurrentMA * CONST_CHARGING_MA / m_HighestChargingMA;
    if(node.m_CurrentMA > 50)
        resultTemp -= m_ChargingPart->adjust(ma * ma);
    else
        resultTemp -= m_ChargingPart->adjust(0);
    if(g_tempSaveLog)
        logText +=QString("[%1]ClearGrass温补信息：m_ChargingPart,m_CurrentMA = %2， resultTemp = %3\n").arg(time).arg(node.m_CurrentMA).arg(resultTemp);

    if(m_WarmStartPart != NULL)
    {
        resultTemp -= m_WarmStartPart->adjust(0);
        if(g_tempSaveLog)
            logText +=QString("[%1]ClearGrass温补信息：m_WarmStartPart,resultTemp = %2\n").arg(time).arg(resultTemp);
    }
    //    resultTemp -= m_TempAlmostFullPart->adjust(node.m_Charging.find("Discharging") == string::npos && node.m_Capacity >= 97 && node.m_Capacity != 100 ? 1 : 0);
    resultTemp += CONST_TEMP_DIFF;
    if(g_tempSaveLog)
        logText +=QString("[%1]ClearGrass温补信息：resultTemp = %2\n\n\n").arg(time).arg(resultTemp);
    return resultTemp;


}

void TempHumFixEngine::initWarmStart(float targetTemp)
{
    int basicAnchor[] = {65, 82, 119, 163, 224, 306, 371, 584, 1016, 3053};
    m_WarmStartPart = new TempFixPart("basic mode", basicAnchor, 1, targetTemp);
    m_WarmStartPart->setCurrentTemp(targetTemp);
}
