/******************** (C) COPYRIGHT 2016 陆超 **********************************
* File Name          :  driverWiFi.h
* Author             :  陆超
* CPU Type           :  Allwinner R8
* IDE                :  Qt Creator
* Version            :  V1.0
* Date               :  09/30/2016
* Description        :  DriverWiFi header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRIVERWIFI_H
#define __DRIVERWIFI_H

/* Includes ------------------------------------------------------------------*/
#include <QThread>
#include <QFile>
#include <QTimer>
#include "wifi_intf.h"

/* Private define ------------------------------------------------------------*/
#define     WIFI_STATUS_OFFSET          0xf000                          // 调试用status地址偏移
#define     WIFI_STATE_OFFSET           0x1000                          // 调试用state地址偏移

#define     WIFI_MAC_ADDR_SIGN          "HWaddr "                       // 提取MAC地址标志位
#define     WIFI_IP_ADDR_SIGN           "inet addr:"                    // 提取IP地址标志为


#define     WIFI_KEY_NONE               0                               // 不加密
#define     WIFI_KEY_WPA_PSK            1                               // WPA加密
#define     WIFI_KEY_WPA2_PSK           2                               // WPA2加密
#define     WIFI_KEY_WEP                3                               // WEP加密

#define     WIFI_SLEEP_TIME             (200 * 1000)                    // 200ms 延时
#define     WIFI_CONNECT_AP_TIMEOUT     ((40 * 1000 * 1000) / WIFI_SLEEP_TIME)  // 超时时间
#define     WIFI_WAIT_LABEL_TIMEOUT     ((20 * 1000 * 1000) / WIFI_SLEEP_TIME)  // 等label同步超时时间
#define     WIFI_BUSY_SLEEP_TIME        (2000 * 1000)                    // 2000ms Wi-Fi busy延时
#define     WIFI_BUSY_TIMEOUT           ((40 * 1000 * 1000) / WIFI_BUSY_SLEEP_TIME)  // 超时时间
#define     WIFI_CMD_RETRY              3                               // 重试次数

/* Private typedef -----------------------------------------------------------*/
class DriverWiFi
{


private:

//    int event_label;                                                    // 事件标示


    const aw_wifi_interface_t *p_wifi_interface;                        // wifi接口指针

    tKEY_MGMT Get_Key_Type(int key_mgmt);                               // 转换加密类型
    DriverWiFi();                                                       // 构造函数
    ~DriverWiFi(void);                                                  // 析构函数

public:
    static DriverWiFi *getInstance(void);                               // 单例函数



    int WiFi_on(void);                                                  // 打开Wi-Fi
    int WiFi_off(void);                                                 // 关闭Wi-Fi
    int WiFi_scan(void);                                                // 扫描Wi-Fi
    int WiFi_disconnectAP(void);                                        // 断开AP连接
    int get_wifi_state(void);                                           // 获取Wi-Fi状态

    int WiFi_Get_MAC_IP(char *MAC, char *IP);                           // 获取MAC和IP
    int WiFi_listNetworks(char *p_networks, int iSize);                 // 列出保存的网络
    int WiFi_remove_all_networks(void);                                 // 删除所有网络失败
    int WiFi_remove_network(char *ssid, int key_mgmt);                  // 删除某个AP
    int WiFi_connectAP_withNetID(const char *net_id);                   // 使用netID连接Wi-Fi

    int WiFi_addNetwork(char *SSID, char *key, int keymgmt);            // 连接匿名ap

    int WiFi_gitNetID(char *SSID, int keymgmt, char *netID);            // 获取netID
    int WiFi_scan_results(char *scan_results, int *len);                // 获取扫描结果
    int WiFi_waitBusy(void);                                            // 等忙
    int WiFi_waitLabel(int event_label);                                // 等label同步

private:
    static DriverWiFi *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DriverWiFi::instance)
                delete DriverWiFi::instance;
        }
    };
    static FreeInstance free;
};

/* Private variables ---------------------------------------------------------*/




#endif /* __DRIVERWIFI_H */

/*************************** END OF FILE **************************************/
