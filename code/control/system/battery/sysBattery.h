#ifndef SYSBATTERY_H
#define SYSBATTERY_H

#include <QThread>
#include <QTimer>
#include <QDateTime>
#include "drivers/battery/driverBattery.h"
#include "wilddog/sysData/wilddogPower.h"

#define BATTERY_SAMPLING_INTERVAL       1000                            // 电池检测间隔
#define BATTERY_LOWEST_CAPACITY         2                              // 电池最低电量

class SysBattery : public QThread
{
    Q_OBJECT
public:
    static SysBattery* getInstance();
    bool is_chargingByAC();
    InfoBattery get_battery();

signals:
    void signal_update_batteryInfo(InfoBattery);
    void signal_sync_power(Power);
    void signal_device_powerOff();                                      // 关机信号

protected slots:
    void slot_refresh_battery();

private:
    void run();
    void data_init();
    void connect_init();

private:
    DriverBattery *driverBattery;
    QTimer *timerSampling;
    InfoBattery battery;
    bool isCharging;

    WilddogPower *wilddogPower;

private:
    explicit SysBattery();
    static SysBattery *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(SysBattery::instance)
                delete SysBattery::instance;
        }
    };
    static FreeInstance free;
};

#endif // SYSBATTERY_H
