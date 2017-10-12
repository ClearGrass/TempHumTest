//
// Created by Yueqian.bao on 17/9/24.
//

#ifndef TEMPHUMFIXENGINE_TEMPFIXPART_H
#define TEMPHUMFIXENGINE_TEMPFIXPART_H

#include "EngineCommon.h"

#define MAX_ANCHOR_SIZE (10)

class TempFixPart {
public:
    TempFixPart(const string& name, int * pointArray, int targetValue, double targetTemp);
    ~TempFixPart();

    float adjust(int value, int times = 1);
    void setCurrentTemp(double temp) { m_CurrentTemp = temp; }
    double getCurrentTemp() { return m_CurrentTemp; }


private:
    int m_AnchorPoint[MAX_ANCHOR_SIZE];

    int m_TargetValue;
    double m_TargetTemp;
    double m_CurrentTemp;

    string m_EngineName;
};


#endif //TEMPHUMFIXENGINE_TEMPFIXPART_H
