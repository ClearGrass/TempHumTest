#ifndef SYSTEMTIME_H
#define SYSTEMTIME_H
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <QDateTime>
#include <QThread>
#include <QTimer>
#include <unistd.h>
#include "time.h"

#include "file/config/fileConfig.h"
#include "file/timeZone/fileTimeZone.h"
#include "driverTime.h"
#include "sysLanguage.h"
#include "device/sysDevice.h"
#include "server/sysData/serverTime.h"
#include "globalVariable.h"

enum NightType
{
    MODEL_MANUAL = 0,
    MODEL_AUTO   = 1
};

class SysTime : public QThread
{
    Q_OBJECT
public:
    static SysTime *getInstance(void);                                  // 单例函数
    QList<TimeZone> *get_listTimeZone();
    QString get_timeZone();
    QString get_timeZoneAuto();
    bool get_night();

signals:
    void signal_sync_netTime(QString,QString);
    void signal_set_night(bool);
    void signal_update_time(TimeType);
    void signal_timeInit_success();

protected slots:
    void slot_sync_netTime();
    void slot_update_language(LanguageType);
    void slot_update_time();

    void slot_syncTime_success(InfoNetTime);
    void slot_syncTime_failed();

    void slot_set_time(QString);
    void slot_set_timeAuto(bool);
    void slot_set_timeZone(QString gmt);

    void slot_set_night(bool);
    void slot_set_nightAuto(bool);
    void slot_set_nightMode(QString);
    void slot_set_nightPeriod(QString,bool);

    void slot_net_connected();
    void slot_net_disconnect();
    void slot_update_infoDevice(InfoDevice);
    void slot_update_infoCity(InfoCity);

private:
    void run();
    void data_init();
    void connect_init();
    bool is_night(QDateTime);
    explicit SysTime(QObject *parent = 0);                              //构造函数

private:
    DriverTime   *driverTime;                                             // 时间驱动
    FileConfig   *fileConfig;
    FileTimeZone *fileTimeZone;
    SysLanguage  *sysLanguage;
    SysDevice    *sysDevice;
    ServerTime   *serverTime;

    LanguageType language;
    NightType nightType;

    bool isTimeAuto;
    bool isNightAuto;

    bool isNight;
    bool isManual;

    bool isTimeInit;
    bool isSunInit;
    bool isConnected;
    bool isDeviceInit;

    QString timeZone;
    QString timeZoneGmt;
    QString timeZoneAuto;
    QString deviceID;
    QString cityID;

    QTime timeStart;
    QTime timeEnd;

    QList<TimeZone> *listTimeZone;

    QTimer *timerUpdate;
    QTimer *timerSync;

private:
    static SysTime *instance;                                           //系统时间单例 实例
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(SysTime::instance)
                delete SysTime::instance;
        }
    };
    static FreeInstance free;

};

#endif // SYSTEMTIME_H
