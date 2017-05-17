#ifndef DATAAIRDEFINE_H
#define DATAAIRDEFINE_H

#include <QDateTime>
#include <QString>
#include "globalStruct.h"

/*******************************************************************************
* Description   :   存储所有和空气数据有关“采集”/"存储"/"上传"/"刷新"的宏定义；
*
******************************************************************************/

#define     MAX(a,b)                    ((a) > (b) ? (a) : (b))         // 取较大值
#define     MIN(a,b)                    ((a) < (b) ? (a) : (b))         // 取较小值

//!
//! 基本空气数据宏定义
//!

#define     TIME_24HOURS                (24 * 60 *60)                   // 24小时时长(s)
#define     TIME_7DAYS                  (7 * TIME_24HOURS)              // 7天时长 (s)

//!
//! PM2.5 数据采集宏定义
//!
#define     PM25_REFRESH_INTERVAL       (5 * 60 * 1000)                // PM2.5刷新时间（ms）
#define     PM25_SAMPLING_NUM           20                              // 总取值时间内取值的数目
#define     PM25_SAMPLING_DURATION      30000                           // 采样持续时间
#define     PM25_SAMPLING_TIMEOUT       20000                           // PM2.5采样超时
#define     PM25_SAMPLING_READY_TIMEOUT 20000                           // 采样准备超时
#define     PM25_SAVE_INTERVAL          (1 * 60 * 1000)                 // PM2.5存储间隔

//!
//! 温度 & 湿度 数据采集宏定义
//!
#define     TEMP_HUMI_REFRESH_INTERVAL  1000                            // 温湿度刷新时间间隔（ms）
#define     TEMP_HUMI_SAMPLING_TIMEOUT  5000                            // 温湿度采样超时
#define     TEMP_HUMI_SAVE_INTERVAL     (1000*60*2)                     // 存储数据库间隔

//!
//! tVOC & CO2e 数据类宏定义
//!
#define     TVOC_CO2E_REFRESH_INTERVAL  1000                            // tvoc&co2e刷新间隔
#define     TVOC_CO2E_SAMPLING_TIMEOUT  5000                            // tvoc&co2采样超时
#define     TVOC_CO2E_SAVE_INTERVAL     (1000*60*2)                     // 存储数据库间隔



//!
//! PC临时系统模拟数据
//!

#define DATALIST_PM25   "56,58,49,57,32,33,38,39,40,46,56,69,87,93,100,103,109,122,133,138,143,145,156,177,178,180,192,192,195,198,203,200,208,201,194,192,200,203,192,198,187,189,172,173,176,152,142,130,109,93,87,86,86,80,73,68,64,60,55,45,40,43,38,28,25,22,22,20,19,19,15,17,17,17,19,15,10,9,8,7,6,6,9,5,3,2,0,0,0,0,1,1,1,0,0,0,0,0,1,0,1,0,,1,0,1,,0,0,0,,1,0,1,2,0,0,1,2,10,15,18,21,22,32,34,43,40,43,46,47,49,42,47,50,43,39,42,48,46"
#define DATALIST_TEMP   "28,28,27,27,27,27,26,26,26,26,27,27,26,26,26,26,26,25,25,25,25,25,25,25,25,24,24,25,25,25,25,25,25,26,26,26,26,26,26,26,27,27,27,27,27,27,28,28,28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,28,28,28,28,28,28,27,27,26,26,26,26,26,26,26,26,26,25,25,25,25,25,25,25,25,25,25,25,25,24,24,24,24,24,24,24,24,24,23,23,23,23,23,23,23,23,23,23,23,23,22,22,22,22,22,22,22,22,21,21,21,21,21,21,21,21,21,21,21,21,21,22,22,23,23,23,23,23,23,24,24,24,24,24,25,25,25,25,26,26,26,26,26,26,26,27,26,26,27,27,27,27,27,27,27,27"
#define DATALIST_HUMI    "32,32,32,32,32,32,33,34,35,35,35,35,35,35,37,37,37,37,37,37,37,38,38,38,38,38,39,39,39,40,40,40,41,42,43,43,45,47,49,52,55,55,58,60,60,60,60,62,62,62,62,64,66,66,66,68,68,68,69,70,70,70,70,70,70,70,69,69,68,68,66,65,66,66,65,65,,63,63,63,63,63,63,63,60,61,60,60,60,63,63,63,63,60,58,58,58,58,57,56,56,56,56,56,55,54,54,54,54,54,54,54,54,54,52,52,52,52,52,52,50,50,50,50,50,50,50,48,48,48,48,48,47,47,47,47,47,47,47,45,45,45,43,43,43,42,41,41,41,41,41,41,41,40,40,41,40,40,39,39,39"
#define DATALIST_TVOC   "3.01,0.08,0.07,0.09,0.11,0.12,0.13,0.15,0.20,0.21,0.23,0.28,0.27,0.30,0.31,0.32,0.33,0.38,0.41,0.41,0.42,0.40,0.35,0.29,0.28,0.28,0.18,0.17,0.15,0.14,0.13,0.12,0.12,0.11,0.10,0.09,0.09,0.07,0.05,0.03,0.03,0.03,0.02,0.02,0.03,0.02,0.03,0.03,0.03,0.05"
#define DATALIST_CO2E   "480,510,630,800,740,720,830,850,890,980,1204,1474,1567,1890,1940,2032,2135,2303,2568,2734,2746,2849,3054,3062,3103,3248,3362,3443,3456,3561,3543,3633,3655,3734,3890,3976,4080,4090,4077,4123,4135,4088,4087,3935,3934,3988,3543,3134,2687,2341,1498,1087,765,767,743,712,734,702,650,687,620,573,550,586,537,620,650,635,643,634,689,720,734,789,720,734,638,642,630,520,499,478,448,430,466,487,453,439"

enum AirDataType
{
    PM25 = 0,                                                           // PM2.5数据
    TEMP = 1,                                                           // 温度数据
    HUMI = 2,                                                           // 湿度数据
    CO2E = 3,                                                            // CO2e数据
    TVOC = 4,                                                           // TVOC数据
    PM5  = 5,
    PM10 = 6
};

typedef struct airData
{
    AirDataType type;
    float       value;
    time_t      timestamp;
    QString     dateTime;
    QString     keyName;

    void init(AirDataType type)
    {
        switch(type)
        {
        case PM25:
            keyName = "pm25";
        break;
        case TEMP:
            keyName = "temperature";
        break;
        case HUMI:
            keyName = "humidity";
        break;
        case TVOC:
            keyName = "tvoc";
        break;
        case CO2E:
            keyName = "co2e";
        break;
        case PM5:
            keyName = "pm5";
        break;
        case PM10:
            keyName = "pm10";
        break;
        default:
            keyName = "unknown";
        break;
        }
        this->type = type;
        this->value = ERROR_DATA;
    }

}AirData;

typedef struct airDataDaily
{
    AirDataType type;
    float       max;
    float       min;
    time_t      timestamp;
    time_t      timestampDaily;
    QString     dateTime;
    QString     date;

    void init(AirDataType type)
    {
        this->type = type;
        this->max = ERROR_DATA;
        this-> min = ERROR_DATA;
        QDateTime dateTime = QDateTime::currentDateTime();
        this->dateTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        this->date = dateTime.toString("yyyy-MM-dd");
        this->timestamp = dateTime.toTime_t();
        this->timestampDaily = QDateTime::fromString(this->date, "yyyy-MM-dd").toTime_t();
    }

    void init(AirData airData)
    {
        this->max = airData.value;
        this->min = airData.value;
        this->type = airData.type;
        QDateTime dateTime = QDateTime::currentDateTime();
        this->dateTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        this->date = dateTime.toString("yyyy-MM-dd");
        this->timestamp = dateTime.toTime_t();
        this->timestampDaily = QDateTime::fromString(this->date, "yyyy-MM-dd").toTime_t();
    }

    bool isValid()
    {
        if((this->max == ERROR_DATA) || (this->min == ERROR_DATA))
        {
            return false;
        }
        return true;
    }

    bool operator != (const airDataDaily & other) const
    {
        if((this->type == other.type) && (this->max == other.max) && (this->min == other.min) && (this->timestamp == other.timestamp) && (this->timestampDaily == other.timestampDaily) && (this->dateTime == other.dateTime) && (this->date == other.date))
        {
            return false;
        }
        return true;
    }

}AirDataDaily;

typedef struct airDataHourly
{
    AirDataType type;
    float       value;
    time_t      timestamp;
    time_t      timestampHourly;
    QString     dateTime;
    QString     time;
    void init(AirDataType type)
    {
        this->type = type;
        this->value = ERROR_DATA;
        this->timestamp = QDateTime::currentDateTime().toTime_t();
        QDateTime dateTime = QDateTime::currentDateTime();
        this->dateTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        this->time     = dateTime.toString("yyyy-MM-dd-hh");
        this->timestamp = dateTime.toTime_t();
        this->timestampHourly = QDateTime::fromString(this->time, "yyyy-MM-dd-hh").toTime_t();
    }

    void init(AirData airData)
    {
        this->value = airData.value;
        this->type = airData.type;
        this->timestamp = airData.timestamp;
        QDateTime dateTime = QDateTime::currentDateTime();
        this->dateTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        this->time     = dateTime.toString("yyyy-MM-dd-hh");
        this->timestamp = dateTime.toTime_t();
        this->timestampHourly = QDateTime::fromString(this->time, "yyyy-MM-dd-hh").toTime_t();
    }

}AirDataHourly;


typedef struct data7Days                                                // 七天数据
{
    float       maxValue[7];                                            // 最大数据
    float       minValue[7];                                            // 最小数据
    AirDataType type;

    void clear()
    {
        int i;
        for(i = 0; i < 7; ++i)
        {
            maxValue[i] = ERROR_DATA;
            minValue[i] = ERROR_DATA;
        }
    }
}Data7Days;

typedef struct data12Hours                                              // 24小时数据
{
    float       nearestValue[12];                                     	// 大于且最近接近该整点的数值
    AirDataType type;
    void clear()
    {
        int i;
        for(i = 0; i < 12; ++i)
        {
            nearestValue[i] = ERROR_DATA;
        }
    }

}Data12Hours;


#endif // DATAAIRDEFINE_H
