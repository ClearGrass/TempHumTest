/******************** (C) COPYRIGHT 2016 陆超 **********************************
* File Name          :  DriverTime.h
* Author             :  陆超
* CPU Type           :  Allwinner R8
* IDE                :  Qt Creator
* Version            :  V1.0
* Date               :  10/28/2016
* Description        :  DriverTime header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRIVERTIME_H
#define __DRIVERTIME_H

/* Includes ------------------------------------------------------------------*/
#include <QThread>
#include <QFile>
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <QMutex>
#include "globalVariable.h"

/* Private define ------------------------------------------------------------*/



/* Private typedef -----------------------------------------------------------*/
class DriverTime : public QObject
{

public:
    static DriverTime *getInstance(void);                               // 单例函数
    int set_dateTime(QString);                                              // 设置时间
    int set_timeZone(QString);                                          // 设置时区
    int set_rtc();                                                      // 设置RTC
    int sync_rtc();                                                     // 同步RTC

private:
    explicit DriverTime(void);                                          // 构造函数
    ~DriverTime(void);                                                  // 析构函数

private:
    static DriverTime *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DriverTime::instance)
                delete DriverTime::instance;
        }
    };
    static FreeInstance free;

};

/* Private variables ---------------------------------------------------------*/




#endif /* __DRIVERTIME_H */

/*************************** END OF FILE **************************************/
