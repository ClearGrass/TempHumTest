#ifndef SERVERDEFINE_H
#define SERVERDEFINE_H
#include <QString>

///
///! 向服务器发送的请求宏定义
///
#define     URL_SERVER                  "http://branapi.cleargrass.com"     // 请求地址

#define     REQUEST_INIT_DEVICE         "/device_init"                  // 初始化设备
#define     REQUEST_GET_DEVICE_INFO     "/device_info"                  // 获取设备信息
#define     REQUEST_SEARCH_CITY         "/city"                         // 搜索城市
#define     REQUEST_LOCATE_DEVICE       "/locate_device"                // 定位
#define     REQUEST_SET_CITY            "/set_city"                     // 设置城市
#define     REQUEST_GET_PAIR_CODE       "/pair_code"                    // 获取配对码
#define     REQUEST_CANCEL_PAIR         "/cancel_pair_from_device"      // 取消配对
#define     REQUEST_GET_TOKEN           "/get_token"                    // 获取token，用于验证
#define     REQUEST_CHECK_UPDATE        "/check_update"                 // 检查更新
#define     REQUEST_WEATHER_FORECAST    "/weather/forecast"             // 天气预报
#define     REQUEST_DATA_REPORT         "/data/report"                  // 报告传感器数据
#define     REQUEST_SUN_TIME            "/weather/sunrise_sunset"       // 获取日出日落时间

#define     REQUEST_UPLOAD_LOG          "/upload_device_log"            // 上传设备日志文件
#define     REQUEST_HEARTBEAT           "/heartbeat"                    // 心跳

#endif // SERVERDEFINE_H
