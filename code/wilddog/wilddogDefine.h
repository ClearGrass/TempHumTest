#ifndef WILDDOGDEFINE_H
#define WILDDOGDEFINE_H

//!
//! 野狗功能块宏定义
//!

#define URL_WILDDOG             "https://bran-test.wilddogio.com"
#define URL_AIR_DATA(deviceID)  (QString("https://bran-test.wilddogio.com/bran_info/%1/monitor").arg(deviceID))
#define URL_WEATHER             "https://bran-test.wilddogio.com/weather_info"

#define URL_CONNECT             "https://bran-test.wilddogio.com/.info/connected"
#define URL_DEVICE(deviceID)    (QString("https://bran-test.wilddogio.com/bran_info/%1").arg(deviceID))

#define     KEY_PM25                "pm25"
#define     KEY_TEMP                "temperature"
#define     KEY_HUMI                "humidity"
#define     KEY_TVOC                "tvoc"
#define     KEY_CO2E                "co2e"
#define     KEY_DEVICE_ID           "device_id"
#define     KEY_TIME                "timestamp"
#define     KEY_TIME_PM25           "timestamp_pm25"










#endif // WILDDOGDEFINE_H
