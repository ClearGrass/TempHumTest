﻿/******************** (C) COPYRIGHT 2016 陆超 **********************************
* File Name          :  globalStruct.h
* Author             :  陆超
* CPU Type           :  ALlwinner R8
* IDE                :  Qt Creator 4.10
* Version            :  V1.0
* Date               :  11/21/2016
* Description        :  全局结构体
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_STRUCT_H
#define __GLOBAL_STRUCT_H

/* Includes ------------------------------------------------------------------*/
#include "globalDefine.h"
#include <QList>
#include <QString>
/* Private define ------------------------------------------------------------*/

#define     ERROR_DATA                  9999                            // PM2.5错误数据

typedef struct _LogInfo
{
    int status_chargingOn;
    int status_cpuLoad;
    int status_cpuF;
    int status_cpuLoadF;
    int status_LCDBrightness;
    QString sDatetime;
    bool pm25Running;

    float tVOC;             //tVOC值
    float CO2e;             //二氧化碳值
    QString baseline;       //baseline
    float   cleargrass_temp;           //青萍热启动校准后的温度值
    float   cleargrass_hum;            //青萍热启动校准后的湿度值
    float   sensirion_temp;           //sensirion校准后的温度值
    float   sensirion_hum;            //sensirion校准后的湿度值

    float  cg_temp_add;               //青萍校准最终值
    float   temp_raw;       //温度原始值
    float   second_temp_raw;    //第二个温度传感器值
    float   hum_raw;        //湿度原始值
    int     pm25;           //pm2.5值
    float   cpu_load;       //cpu占有率
    float   cpu_fre;        //cpu频率 单位G
    float    capacity;       //电量
    bool    charging;       //是否充电
    int     light;          //亮度值
    QString cityID;        //城市id
    QString deviceID;      //城市id
    int     wifi_status;   //Wi-Fi状态

    bool screenOn;
    int currentMA;
    int voltage;

    _LogInfo()
    {
        this->tVOC = ERROR_DATA;
        this->CO2e = ERROR_DATA;
        this->baseline = "000000";
        this->cleargrass_temp =ERROR_DATA;
        this->cleargrass_hum = ERROR_DATA;
        this->sensirion_hum = ERROR_DATA;
        this->sensirion_temp = ERROR_DATA;
        this->temp_raw = ERROR_DATA;
        this->hum_raw = ERROR_DATA;
        this->pm25 = ERROR_DATA;
        this->cpu_load = -1;
        this->cpu_fre = -1;
        this->capacity = -1;
        this->light = -1;
        this->deviceID = "";
        this->cityID = "";
        this->wifi_status = 0;
        this->screenOn = 1;
        this->currentMA = 0;
        this->voltage = 0;

        this->status_chargingOn = 0;
        this->status_cpuLoad = 0;
        this->status_cpuF = 0;
        this->status_cpuLoadF = 0;
        this->status_LCDBrightness = 0;
        this->sDatetime = "";
        this->pm25Running = false;

    }

}LogInfo;


enum PageType
{
    PAGE_MAIN       = 0,
    PAGE_SETTING    = 1,
    PAGE_POWER_ON   = 2,
    PAGE_POWER_OFF  = 3,
    PAGE_WIZARD     = 4,
    PAGE_HIS        = 5
};

enum TimeType
{
    DAILY    = 0,
    HOURLY   = 1,
    MINUTELY = 2
};

enum UnitType
{
    CEL     = 0,
    KFC     = 1,
    MG_M3   = 2,
    PPM     = 3,
    PPB     = 4
};

enum LanguageType
{
    LANG_EN = 0,                                                        // 英文
    LANG_CN = 1,                                                        // 中文简体
    LANG_TW = 2                                                         // 中文繁体
};

enum IntervalType
{
    NEVER = 999,
    MIN5  = 5,
    MIN15 = 15,
    MIN30 = 30
};

enum ChargingType
{
    AC   = 0,
    USB  = 1,
    NONE = 2
};

enum BatteryStatusType
{
    UNKNOWN     = 0,                                                    // 未知
    CHARGING    = 1,                                                    // 正在充电
    DISCHARGING = 2,                                                    // 放电
    NOTCHARGING = 3,                                                    // 未充电
    FULL        = 4                                                     // 满电
};

enum MouseType{
    Release = 1,
    Press
};

//!
//! 历史数据结构体
//!
typedef struct HistoryData
{
    long timeSec;
    QString time;
    float value;
} HistoryData;

//!
//! 设备信息结构体
//!
typedef struct token
{
    QString value;
    int expTime;
}Token;

typedef struct infoDevice
{
    QString deviceID;
    QString mac;
    QString sn;
    QString model;
    QString miioDID;
    QString miioMAC;
    QString miioKEY;
    Token token;
    QString miioInfo;

}InfoDevice;

//!
//! 时间信息结构体
//!
typedef struct infoNetTime
{
    QString dateTime;
    int timeStamp;
    QString sunrise;
    QString sunset;

}InfoNetTime;

typedef struct time
{
    int month;
    int day;
    int hour;
    int minute;
    int second;

}Datetime;

typedef struct timeZone
{
    QString name;                                                       //
    QString gmt;
    QString description;

}TimeZone;

//!
//! 电池信息结构体
//!
typedef struct infoBattery
{
    float   capacity;                                                   //剩余电量：单位 %
    bool    isExist;                                                    //电池存在：0 不存在 1 存在
    BatteryStatusType status;
    float voltage;                                                      //电压
    ChargingType   way;
    float  current;

}InfoBattery;

//!
//! 固件版本信息结构体
//!
typedef struct file
{
    QString md5;
    QString url;
    QString name;
    int size;

}File;

typedef struct infoFirmware
{
    QList<File> listFile;
    QString version;
    QString desc;
    QString createdTime;
    QString type;
    qint64 size;

    void init()
    {
        listFile.clear();
    }
    bool isEmpty()
    {
        return listFile.isEmpty();
    }

}InfoFirmware;

typedef struct infoVersion
{
    QString system;
    QString firmware;

}InfoVersion;

//!
//! 城市信息结构体
//!
typedef struct infoCity
{
    QString cityID;                                                     // 城市ID
    QString name;                                                     // 中文城市名称

    QString nameEN;
    QString nameCN;
    QString nameTW;

    QString country;                                                    // 国家名称
    QString countryCode;                                                // 国家代码
    QString area;                                                     // 地区英文名
    QString timeZone;                                                   // 时区
    QString cityGrade;                                                  // 城市级别
    QString timeZoneGMT;
    QString time;

}InfoCity;

//!
//! 天气信息结构体
//!
typedef struct infoWeather
{
    QString skycon;                                                     // 天气概况
    int temp;                                                       // 温度
    int pm25;                                                       // pm25
    float humi;                                                       // 湿度

    infoWeather()
    {
        set_invalid();
    }
    void set_invalid()
    {
        skycon = "";
        temp   = ERROR_DATA;
        pm25   = ERROR_DATA;
        humi   = ERROR_DATA;
    }
    bool is_valid()
    {
        if("" == skycon)
        {
            return false;
        }
        return true;
    }

}InfoWeather;


typedef struct infoSys
{
    InfoDevice device;
    InfoBattery battery;
    InfoCity    city;
    InfoVersion version;

}InfoSys;

typedef struct power
{
    int value;
    time_t timeStamp;
}Power;

typedef struct cookie
{
    QString ssid;                                                       // Wi-Fi名称
    QString netID;                                                      // NetID
    QString flags;                                                      // 标志位
}Cookie;

/******************************** current page index **************************/



/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

#endif /* __GLOBAL_STRUCT_H */

/******************* (C) COPYRIGHT 2016 陆超 ***** END OF FILE******************/
