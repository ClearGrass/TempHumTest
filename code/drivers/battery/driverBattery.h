#ifndef RDRIVERBATTERY_H
#define RDRIVERBATTERY_H
#include <QThread>
#include <QFile>
#include <QTimer>
#include <QTextStream>
#include <QTextCodec>
#include <QtDebug>
#include "globalStruct.h"

#define DEVICE_POWER_VALUE      "/sys/class/power_supply/battery/capacity"   //剩余电量：单位 %
#define DEVICE_POWER_STATUS     "/sys/class/power_supply/battery/status"     //检测电池状态
#define DEVICE_POWER_EXIST      "/sys/class/power_supply/battery/present"    //检测电池存在：0 不存在 1 存在
#define DEVICE_POWER_VOLTAGE    "/sys/class/power_supply/battery/voltage_now" //电池电压：单位 uV
#define DEVICE_POWER_CURRENT    "/sys/class/power_supply/battery/current_now" //电池电流：单位 uA 正数充电，负数放电
#define DEVICE_POWER_AC         "/sys/class/power_supply/ac/present"          //检测是否底座充电 1 底座充电 0 无底座充电
#define DEVICE_POWER_USB        "/sys/class/power_supply/usb/present"         //检测是否USB充电  1 USB充电 0 无USB

class DriverBattery : public QObject
{
    Q_OBJECT
public:
    static DriverBattery *getInstance();
    bool isExist();                                                     // 判断是否存在
    float get_battery_capacity();                                       // 获取剩余电量
    ChargingType   get_charging_way();                                  // 获取充电方式
    BatteryStatusType get_battery_status();                                // 获取当前状态
    bool is_charging_ac();                                              // 是否为底座充电
    bool is_charging_usb();                                             // 是否为usb充电
    float get_battery_voltage();                                        // 获取电池电压
    float get_battery_current();                                        // 获取电池电流

private:
    DriverBattery();


private:
    static DriverBattery *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DriverBattery::instance)
                delete DriverBattery::instance;
        }
    };
    static FreeInstance free;

};

#endif // RDRIVERBATTERY_H
