#ifndef GLOBALCONFIGDEFINE_H
#define GLOBALCONFIGDEFINE_H


//!
//! 系统属性名宏定义
//!

#define LIGHT_AUTO                  "LIGHT_AUTO"                        // 自动亮度调节
#define LIGHT_VALUE                 "LIGHT_VALUE"                       // 默认亮度值


#define LANGUAGE                    "LANGUAGE"

#define DEVICE_ID                   "DEVICE_ID"                         // 设备ID
#define TOKEN                       "TOKEN"
#define EXP_TIME                    "EXP_TIME"

#define DEVICE_SN                   "DEVICE_SN"                         // 设备序列号
#define DEVICE_MODEL                "DEVICE_MODEL"                      // 设备型号
#define DEVICE_MAC                  "DEVICE_MAC"
#define MIIO_DID                    "MIIO_DID"
#define MIIO_MAC                    "MIIO_MAC"
#define MIIO_KEY                    "MIIO_KEY"

#define TVOC_UNIT                   "TVOC_UNIT"                         // tVOC单位
#define TEMP_UNIT                   "TEMP_UNIT"                         // 温度界面单位

#define PM25_SAMPLING_CHARGING      "PM25_SAMPLING_CHARGING"
#define PM25_SAMPLING_DISCHARGING   "PM25_SAMPLING_DISCHARGING"

#define SCREEN_OFF_CHARGING         "SCREEN_OFF_CHARGING"
#define SCREEN_OFF_DISCHARGING      "SCREEN_OFF_DISCHARGING"

#define CITY_AUTO                   "CITY_AUTO"
#define CITY_NAME_SC                "CITY_NAME_SC"                      // 简体中文
#define CITY_NAME_EN                "CITY_NAME_EN"                      // 英文
#define CITY_NAME_TC                "CITY_NAME_TC"                      // 繁体中文
#define CITY_ID                     "CITY_ID"                           // 城市ID


#define TIME_MODE                   "TIME_MODE"                         // 时间制(12小时/24小时)
#define NIGHT_MODE                  "NIGHT_MODE"                        //夜间模式

#define TIME_AUTO                   "TIME_AUTO"                         // 自动时间更新
#define NIGHT_AUTO                  "NIGHT_AUTO"                        //自动开启夜间模式

#define TIME_ZONE                   "TIME_ZONE"                         // 时区

#define NIGHT_START_TIME            "NIGHT_START_TIME"
#define NIGHT_END_TIME              "NIGHT_END_TIME"
#define SUNRISE_TIME                "SUNRISE_TIME"
#define SUNSET_TIME                 "SUNSET_TIME"

#define TIME_INIT                   "TIME_INIT"
#define DEVICE_INIT                 "DEVICE_INIT"
#define LANGUAGE_INIT               "LANGUAGE_INIT"

#define SYS_ENTER                   "SYS_ENTER"
#define SYS_INIT                    "SYS_INIT"

#define SUN_INIT                    "SUN_INIT"

#define SOFTWARE_UPGRADE            "SOFTWARE_UPGRADE"                  //软件升级
#define WIFI_MAIN_PAGE              "WIFI_MAIN_PAGE"                    //主页面Wi-Fi是否显示

#define APP_BIND_STATUS             "APP_BIND_STATUS"

#define DOUBLE_CLICK_CHECKING       "DOUBLE_CLICK_CHECKING"             //双击设备顶部触发监测

#define VERTICAL_SCREEN_REFRESH     "VERTICAL_SCREEN_REFRESH"           //竖屏刷新按钮

//!
//! 系统默认属性值宏定义
//!

#define DEFAULT_TEMP_UNIT           "cel"                               // 默认摄氏度
#define DEFAULT_TVOC_UNIT           "mg/m3"

#define DEFAULT_LANGUAGE            ""

#define DEFAULT_LIGHT_AUTO          "true"                              // 默认自动调节
#define DEFAULT_LIGHT_VALUE         "50"                                // 默认50



#define DEFAULT_TIME_AUTO           "true"                              // 默认自动更新
#define DEFAULT_NIGHT_AUTO          "true"

#define DEFAULT_TIME_MODE           "mode_24"                           // 默认24小时制
#define DEFAULT_NIGHT_MODE          "mode_sun"

#define DEFAULT_TIME_ZONE           "Asia/Shanghai"                     // 默认时区

#define DEFAULT_NIGHT_START_TIME    "20:00"
#define DEFAULT_NIGHT_END_TIME      "6:00"
#define DEFAULT_SUNRISE_TIME        "6:00"
#define DEFAULT_SUNSET_TIME         "20:00"
#define DEFAULT_SUN_INIT            "false"

#define DEFAULT_DEVICE_ID           ""                                  // 默认设备ID
#define DEFAULT_TOKEN               ""
#define DEFAULT_EXP_TIME            "1"

#define DEFAULT_DEVICE_SN           ""
#define DEFAULT_DEVICE_MODEL        "Bran"
#define DEFAULT_DEVICE_MAC          ""
#define DEFAULT_MIIO_DID            ""
#define DEFAULT_MIIO_MAC            ""
#define DEFAULT_MIIO_KEY            ""

#define DEFAULT_CITY_AUTO           "true"
#define DEFAULT_CITY_NAME_SC        ""
#define DEFAULT_CITY_NAME_EN        ""
#define DEFAULT_CITY_NAME_TC        ""
#define DEFAULT_CITY_ID             ""

#define DEFAULT_DOUBLE_CLICK_CHECKING       "false"
#define DEFAULT_SCREEN_OFF_CHARGING         "never"                 //接通电源 时屏幕默认熄灭时间
#define DEFAULT_SCREEN_OFF_DISCHARGING      "5"                     //使用电池 时屏幕默认熄灭时间

#define DEFAULT_PM25_SAMPLING_CHARGING      "15"                    //接通电源 时pm2.5默认检测频率
#define DEFAULT_PM25_SAMPLING_DISCHARGING   "30"                    //使用电池 时pm2.5默认检测频率

#define DEFAULT_TIME_INIT           "false"
#define DEFAULT_DEVICE_INIT         "false"
#define DEFAULT_LANGUAGE_INIT       "false"

#define DEFAULT_SYS_ENTER           "false"
#define DEFAULT_SYS_INIT            "false"

#define DEFAULT_SOFTWARE_UPGRADE      "true"                     //使用电池 时屏幕默认熄灭时间
#define DEFAULT_WIFI_MAIN_PAGE        "true"                     //主页面默认显示wifi页面

#define DEFAULT_APP_BIND_STATUS       "0"

#define DEFAULT_VERTICAL_SCREEN_REFRESH "true"
#endif // GLOBALCONFIGDEFINE_H
