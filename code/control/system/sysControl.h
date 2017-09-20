#ifndef SYSCONTROL_H
#define SYSCONTROL_H

#include <QEventLoop>
#include "sysButton.h"
#include "battery/sysBattery.h"
#include "device/sysDevice.h"
#include "sysGravity.h"
#include "sysLanguage.h"
#include "sysLight.h"
#include "sysWiFi.h"
#include "sysTime.h"
#include "sysPower.h"
#include "cpu/sysCpu.h"
#include "data/sysData.h"
#include "server/serverControl.h"
#include "file/config/fileConfig.h"

class SysControl : public QThread
{
    Q_OBJECT
public:
    static SysControl* getInstance();
    QString get_value(const QString);
    void set_value(QString,QString);
    WiFi get_currentWiFi();
    InfoBattery get_battery();
    bool get_night();
    bool is_deviceInit();
    bool is_timeInit();
    bool is_languageInit();
    bool is_chargingByAC();
    bool is_ScreenOn();
    void reset();

signals:

    void signal_light_value(int);
    void signal_outLight_value(int);
    void signal_inLight_value(int);

    void signal_change_direction(DIRECTION);
    void signal_update_battery(InfoBattery);
    void signal_button_singleClick();
    void signal_power_off();
    void signal_button_longPress();
    void signal_screen_on();
    void signal_screen_off();

    void signal_net_connected();
    void signal_net_disconnect();
    void signal_net_ip(QString);
    void signal_battery_charging(bool);

    void signal_shell_doubleTap();

    void signal_in_night(bool);

    void signal_set_light(int);
    void signal_autoAdjust_light(bool);
    void signal_offInterval_charging(IntervalType);
    void signal_offInterval_discharging(IntervalType);
    void signal_user_operations();
public slots:
    void slot_screenOff();
    void slot_screenOn();
protected slots:
    void slot_set_lightValue(int);
    void slot_set_lightMode(bool);
    void slot_offInterval_charging(IntervalType);
    void slot_offInterval_discharging(IntervalType);

    void slot_update_direction(DIRECTION);                              // 改变屏幕方向
    void slot_update_light(int);                                        // 改变当前页面显示亮度值
    void slot_update_inLight(int);                                      // 测试，显示设置亮度值
    void slot_update_outLight(int);                                     // 测试，显示外界亮度值
    void slot_update_screen(bool);

    void slot_update_wifiStatus(WiFi);
    void slot_update_battery(InfoBattery);

    void slot_button_doubleClick();
    void slot_button_singleClick();
    void slot_button_longPress();


    void slot_device_powerOff();                                        // 关机
    void slot_device_reboot();
    void slot_delayPowerOff();                                          //延迟关机槽函数

    void slot_in_night(bool);

    void slot_shell_doubleTap();
    void slot_test_net();
    void slot_interface_operations();

private:
    void run();                                                         // 系统开始运行
    void data_init();                                                   // 系统初始化
    void connect_init();
                                                                        // 关机

private:
    SysButton   *sysButton;
    SysBattery  *sysBattery;
    SysDevice   *sysDevice;
    SysGravity  *sysGravity;
    SysLanguage *sysLanguage;
    SysLight    *sysLight;
    SysWiFi     *sysWiFi;
    SysTime     *sysTime;
    SysPower    *sysPower;
    SysData     *sysData;

    ServerDebug *serverDebug;

    FileConfig  *fileConfig;
    QTimer *timerTest;

private:
    bool isConnected;
    bool isScreenOn;
    bool isCharging;
    bool isNight;
    bool isTimeInit;
    bool isDeviceInit;

    InfoBattery battery;
    WiFi   wifi;

    QEventLoop loop;

private:
    SysControl();
    static SysControl *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(SysControl::instance)
                delete SysControl::instance;
        }
    };
    static FreeInstance free;
};

#endif // SYSCONTROL_H
