/*******************************************************************************
* File Name          :  globalDefine.h
* Author             :  LEO
* CPU Type           :  Bran_R8
* IDE                :  Qt 4.8.x
* Version            :  V1.0
* Date               :  08/14/2016
* Description        :  全局定义
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H

/* Includes ------------------------------------------------------------------*/
#include <QString>
//----------------------- System ----------------------------------------------/


#define     SYS_DIRECTION_HORIZONTAL    0                               // 横向
#define     SYS_DIRECTION_VERTICAL      1                               // 纵向
#define     SYS_DIRECTION_HORIZONTAL_N  2                               // 横逆
#define     SYS_DIRECTION_VERTICAL_N    3                               // 纵逆

//-------------------------------- Wi-Fi --------------------------------------/

#define    NET_ABNORMAL    1                                            //网路异常
#define    NET_DISCONNECT  2                                            //网络断开

//--------------------------- Language ----------------------------------------/


//-------------------------View------------------------------------------------/
#define DEFAULT_PAGE        0                                           // 默认页面
#define PM_PAGE             0                                           // PM2.5页面
#define TEMP_PAGE           1                                           // 温度页面
#define HUM_PAGE            2                                           // 湿度页面
#define CO2_PAGE            3                                           // co2页面
#define TVOC_PAGE           4                                           // TVOC页面
#define WEATHRT_PAGE        5                                           //天气页面
#define OPTION_PAGE         6                                           // 设置页面
#define MAIN_PAGE           7                                           // 主页面
#define HIS_PAGE            8                                          // 主页面

#define SIMPLE_INTERVAL     5000+760


#define   HORIZONTAL_MODE 1                                             //横屏模式
#define   VERTICAL_MODE   2                                             //竖屏模式

#define HORIZONTAL_WIDTH   854                                          //横屏宽度
#define HORIZONTAL_HEIGHT  480                                          //横屏高度
#define VERTICAL_WIDTH     480                                          //竖屏宽度
#define VERTICAL_HEIGHT    854                                          //竖屏高度

//-------------------------SysConfig----------------------------------------/
//!
//! 室内外亮度宏定义
//!

#define MAX_INLIGHT_VALUE   0
#define MIN_INLIGHT_VALUE   255
#define MAX_OUTLIGHT_VALUE  3000
#define MIN_OUTLIGHT_VALUE  40

#define COSY_OUTLIGHT_VALUE 400
#define COSY_INLIGHT_VALUE  20


//!
//! 系统配置文件路径
//!

#define SYSTEM_CONFIG_PATH  "bran.config"
#define LANGUAGE_PATH

#define LOGPATH     "/bin/qtapp/debuglog.txt"                           // 日志路径

//-------------------------Timer----------------------------------------/
//!
//! 定时器时间定义
//!
#define POWEROFF_INTERVAL  1500                                         //关机动画时长


#endif // GLOBALDEFINE_H
/******************* (C) COPYRIGHT 2016 LEO *********END OF FILE***************/
