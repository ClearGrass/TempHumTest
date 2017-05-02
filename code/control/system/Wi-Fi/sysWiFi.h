/******************** (C) COPYRIGHT 2016 陆超 **********************************
* File Name          :  sysWiFi.h
* Author             :  陆超
* CPU Type           :  Allwinner R8
* IDE                :  Qt Creator
* Version            :  V1.0
* Date               :  10/08/2016
* Description        :  sysWiFi header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSWIFI_H
#define __SYSWIFI_H

/* Includes ------------------------------------------------------------------*/
#include <QThread>
#include <QFile>
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <wifi_intf.h>
#include <pthread.h>
#include <QStringList>
#include <QList>
#include <QMutex>
#include <QMutableListIterator>
#include <QTime>
#include <QProcess>
#include <QTextCodec>
#include <string>
#include "driverWiFi.h"
#include "database.h"
#include "globalVariable.h"
/* Private define ------------------------------------------------------------*/


#define     WIFI_SCAN_INTERVAL          15000
#define     WIFI_UPDATE_INTERVAL        15000

#define     WIFI_CONNECTED              0                               // Wi-Fi已连接
#define     WIFI_DISCONNECT             1                               // Wi-Fi未链接

#define     NET_ONLINE                  0                               // 已联网
#define     NET_OFFLINE                 1                               // 未联网

#define     WIFI_CONNECT_SUCCESS        0xF00B                          // wifi已连接
#define     WIFI_PASSWORD_FAILED        0xF00C                          // 密码错误
#define     WIFI_CONNECT_TIMEOUT        0xF00D                          // 连接超时

#define     WIFI_SIGNAL_STRENGTH_1      -100
#define     WIFI_SIGNAL_STRENGTH_2      -90
#define     WIFI_SIGNAL_STRENGTH_3      -80
#define     WIFI_SIGNAL_STRENGTH_4      -60

#define     WIFI_REMOVE_SUCCESS         0
#define     WIFI_REMOVE_FAILED          1

#define     WIFI_NO_ENCRYPT             1                               //
#define     WIFI_NO_KEY                 2
#define     WIFI_SAVED_KEY              3

#define     DOMAIN_NAME_TEST            "www.baidu.com"
#define     WIFI_COOKIE_LOS E            "wifi cookie lost"

enum WiFiStatus
{
    CONNECTED   = 0,
    DISCONNECT  = 1,
    UNUSEABLE   = 2,
    CONNECTTING = 3
};

typedef struct wifi
{
    QString ssid;                                                       // Wi-Fi名称
    int signalNum;                                                      // 信号格数
    WiFiStatus status;                                                  // 当前状态
    bool lock;                                                          // 是否加密

    QString ip;                                                         // IP地址
    int keymgmt;
    bool key;                                                           // 密码值用于测试
    int strength;                                                       // 信号强度

    void init()                                                         // 初始化函数
    {
        ssid        = "";
        status      = DISCONNECT;
        lock        = true;
        signalNum   = 0;
        ip          = "127.0.0.1";
        keymgmt     = 0;
        key         = false;
        strength    = 0;
    }
}WiFi;

using  std::string;

/* Private typedef -----------------------------------------------------------*/
class SysWiFi : public QThread
{
    Q_OBJECT
public:
    static SysWiFi *getInstance(void);                                  // 单例函数
    int get_keyType(QString ssid);
    WiFi get_currentWiFi();

protected:
    explicit SysWiFi(QObject *parent = 0);                              // 构造函数

signals:
    void signal_update_wifi(WiFi);
    void signal_connect_result(int result);                             // 连接结果
    void signal_update_listWiFi(QList<WiFi> *);                               // 更新WiFi列表
    void signal_remove_result(int result);

protected slots:
    void slot_connect_withKey(QString ssid, QString key);
    void slot_connect_withNetID(QString ssid);
    void slot_connect_publicWiFi(QString ssid);
    void slot_refresh_listWiFi();

    void slot_net_abnormal();
    void slot_scan_wifi();                                              // 开始扫描
    void slot_update_wifi();
    void slot_remove_cookie(QString ssid);
    void slot_remove_allCookies();

private:
    void run();
    void data_init();
    void connect_init();
    void scan_wifi();
    void scan_cookie();
    void refresh_wifi();                                                //
    void verify_net();                                             // 验证网络
    void connect_withKey(QString ssid, QString key);
    void connect_autoMatch();
    int remove_cookie(QString ssid);
    int remove_allCookies();

    int get_wifiStatus();                                               // 获取WiFi状态
    int get_netStatus();                                                // 获取网络状态
    QString get_wifiIP();
    QString get_wifiSSID();

    int get_wifiKeymgmt(QString ssid);                                  //
    int get_wifiIndex(QString ssid);

    int get_wifiSignalNum(QString ssid);

    bool is_cookie(QString ssid);                                       //
    void endless_loop(const char * debugData);                          // 调试死循环
    int trans_signalNum(int strength);                                  // 将信号强度转为信号格数
    int trans_keymgmt(QString strKeymgmt);                              // 将加密方式进行分类
    QString trans_utf8(QString);

    static bool compare_wifi(const WiFi,const WiFi);                    //  Wi-Fi对比方法

private:
    DriverWiFi  *driverWiFi;                                            // 声明Wi-Fi驱动类

    QList<Cookie>   *listCookie;                                        // 已存WiFi列表
    QList<WiFi>     *listWiFi;                                          // 周围WiFi列表

    WiFi currentWiFi;

    QTime timeScan;

    QTimer *timerScan;
    QTimer *timerUpdate;

    int wifiStatus;                                                     // 当前Wi-Fi状态
    int netStatus;                                                      // 目前网络状态
    QProcess *processPing;                                              // ping值获取网络状态
    bool isScaned;

private:
    static SysWiFi *instance;

};

/* Private variables ---------------------------------------------------------*/




#endif /* __SYSWIFI_H */

/*************************** END OF FILE **************************************/
