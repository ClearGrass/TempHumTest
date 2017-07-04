#ifndef SYSTEMLIGHT_H
#define SYSTEMLIGHT_H
#include <QThread>
#include <QMutex>
#include <QTimer>
#include "driverLight.h"
#include "globalVariable.h"
#include "file/config/fileConfig.h"

#define MAX_INLIGHT_VALUE   1000
#define MIN_INLIGHT_VALUE   10
#define MAX_OUTLIGHT_VALUE  3000
#define MIN_OUTLIGHT_VALUE  40
#define COSY_OUTLIGHT_VALUE 400
#define COSY_INLIGHT_VALUE  20

#define LIGHT_SAMPLING_NUM      10
#define LIGHT_SAMPLING_INTERVAL 200
#define LIGHT_CACHE_DEPTH       16



class SysLight : public QThread
{
    Q_OBJECT
public:
    static SysLight* getInstance();
    bool get_autoAdjust();
    int get_light();

signals:
    void signal_update_light(int light);
    void signal_update_outLight(int light);
    void signal_update_inLight(int light);
    void signal_update_screen(bool);

protected slots:
    void slot_set_autoMode(bool);
    void slot_set_value(int light);

    void slot_offInterval_charging(IntervalType);
    void slot_offInterval_discharging(IntervalType);
    void slot_autoAdjust_light();

    void slot_screen_on();
    void slot_screen_off();
    void slot_battery_charing(bool);                                     // 设备在充电
    void slot_user_operations();

protected:
    void run();
    explicit SysLight(QObject *parent = 0);
    int autoAdjust_light();
    void data_init();
    void connect_init();
    int light_transform(int outLight);
    int trans_interval(QString);

private:
    QList<int>lightCache;

    QMutex mutex;
    bool isAutoAdjust;
    bool isOffScreenCharging;
    bool isOffScreenDischarging;
    bool isCharging;
    int iCurrentLight;
    int iLastLight;
    int iLight;
    int iTotalLight;
    int num;

    int intervalCharging;
    int intervalDisharging;

    QString offCharging;
    QString offDischarging;

    DriverLight *driverLight;
    FileConfig *fileConfig;
    QTimer *timerSampling;
    QTimer *timerCharging;
    QTimer *timerDischarging;

private:
    static SysLight *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(SysLight::instance)
                delete SysLight::instance;
        }
    };
    static FreeInstance free;
};

#endif // SYSLIGHT_H
