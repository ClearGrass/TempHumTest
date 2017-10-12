//
// Created by Yueqian.bao on 17/9/24.
//

#ifndef TEMPHUMFIXENGINE_TEMPHUMFIXENGINE_H
#define TEMPHUMFIXENGINE_TEMPHUMFIXENGINE_H

#include "TempFixPart.h"
#include "EngineCommon.h"
#include "BranSampleNode.h"

class TempHumFixEngine {
public:
    TempHumFixEngine();
    ~TempHumFixEngine();

    float fixTemp(const BranSampleNode& node);
    QString logText;
    void initWarmStart(float targetTemp);

private:
    TempFixPart* m_BasicPart;
    TempFixPart* m_WifiPart;
    TempFixPart* m_ScreenOffPart;
    TempFixPart* m_HighCpuFreqPart;
    TempFixPart* m_BrightnessPart;
    TempFixPart* m_CpuLoadPart;
    TempFixPart* m_BatteryPart;
    TempFixPart* m_ChargingPart;
    TempFixPart* m_TempAlmostFullPart;
    TempFixPart* m_WarmStartPart;
    int m_HighestChargingMA;
};


#endif //TEMPHUMFIXENGINE_TEMPHUMFIXENGINE_H
