#ifndef DATABASEDEFINE_H
#define DATABASEDEFINE_H
#include "globalStruct.h"

//!
//! 数据库基本宏定义
//!

#define DATABASE_NAME   "BranQt4.db"

//!
//! 数据库表名宏定义
//!

#define TABLE_PM25_12HOURS      "table_pm25_hourly"                    // PM2.5 12小时数据表
#define TABLE_TEMP_12HOURS      "table_temp_hourly"                    // 温度 12小时数据表
#define TABLE_HUMI_12HOURS      "table_humi_hourly"                    // 湿度 12小时数据表
#define TABLE_TVOC_12HOURS      "table_tvoc_hourly"                    // TVOC 12小时数据表
#define TABLE_CO2E_12HOURS      "table_co2e_hourly"                    // CO2E 12小时数据表

#define TABLE_PM25_7DAYS        "table_pm25_daily"                      // PM2.5 7天数据表
#define TABLE_TEMP_7DAYS        "table_temp_daily"                      // 温度 7天数据表
#define TABLE_HUMI_7DAYS        "table_humi_daily"                      // 湿度 7天数据表
#define TABLE_TVOC_7DAYS        "table_tvoc_daily"                      // TVOC ７天数据表
#define TABLE_CO2E_7DAYS        "table_co2e_daily"                      // CO2E 7天数据表

#define TABLE_PM25_CACHE        "table_pm25_cache"                      // PM2.5 7天数据表
#define TABLE_TEMP_CACHE        "table_temp_cache"                      // 温度 7天数据表
#define TABLE_HUMI_CACHE        "table_humi_cache"                      // 湿度 7天数据表
#define TABLE_TVOC_CACHE        "table_tvoc_cache"                      // TVOC ７天数据表
#define TABLE_CO2E_CACHE        "table_co2e_cache"                      // CO2E 7天数据表

#define TABLE_CACHE_REQUEST     "table_cache_request"                   // 请求缓存表

//!
//! 数据库字段名宏定义
//!

#define FIELD_DATETIME          "dateTime"

#define FIELD_TIMESTAMP         "timeStamp"
#define FIELD_TIMESTAMP_DAILY   "timeStamp_daily"
#define FIELD_TIMESTAMP_HOURLY  "timeStamp_hourly"

#define FIELD_DATE              "date"
#define FIELD_TIME              "time"
#define FIELD_MAX_VALUE         "maxValue"
#define FIELD_MIN_VALUE         "minValue"
#define FIELD_VALUE             "value"

#define FIELD_REQUEST_TYPE      "requestType"
#define FIELD_DATA_TYPE         "dataType"

//!
//! 数据类型定义
//!



#endif // DATABASEDEFINE_H
