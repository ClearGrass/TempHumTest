//
// Created by Yueqian.bao on 17/9/24.
//

#include <math.h>
#include "TempFixPart.h"
#include <QString>
TempFixPart::TempFixPart(const string &name, int * pointArray, int targetValue, double targetTemp)
{
    m_EngineName = name;
    for(int i = 0; i < MAX_ANCHOR_SIZE; ++i)
    {
        m_AnchorPoint[i] = pointArray[i];
    }
    m_TargetValue = targetValue;
    m_TargetTemp = targetTemp;
    m_CurrentTemp = 0;
}

TempFixPart::~TempFixPart()
{
}

float TempFixPart::adjust(int value, int times)
{
    if(value > m_TargetValue)
        value = m_TargetValue;
    if(value < 0)
        value = 0;
    double currentTargetTemp = m_TargetTemp * value / m_TargetValue;
    if(fabs(m_CurrentTemp) > fabs(currentTargetTemp))
    {
        if(m_TargetTemp == currentTargetTemp)
            return m_CurrentTemp;
        double percent = (m_TargetTemp - m_CurrentTemp) / (m_TargetTemp - currentTargetTemp);
        if(percent >= 1.0f)
            return m_CurrentTemp;
        if(percent < 0.0f)
            percent = 0.0f;
        int index = percent * MAX_ANCHOR_SIZE;
        m_CurrentTemp = m_CurrentTemp - (m_TargetTemp - currentTargetTemp) * times / MAX_ANCHOR_SIZE / m_AnchorPoint[index];

    }
    else
    {
        if(currentTargetTemp == 0)
        {
            return m_CurrentTemp;
        }
        double percent = m_CurrentTemp / currentTargetTemp;
        if(percent >= 1.0f)
            return m_CurrentTemp;
        if(percent < 0.0f)
            percent = 0.0f;
        int index = percent * MAX_ANCHOR_SIZE;
        double temp = currentTargetTemp * times / MAX_ANCHOR_SIZE / m_AnchorPoint[index];
        m_CurrentTemp = m_CurrentTemp + temp;
    }

    return m_CurrentTemp;

}
