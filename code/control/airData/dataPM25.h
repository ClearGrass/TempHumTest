#ifndef DATAPM25_H
#define DATAPM25_H
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QMutex>
#include <QStringList>
#include <QEventLoop>
#include "data/sysData.h"
#include "globalVariable.h"
#include "driverPM25.h"
#include "dataAirDefine.h"


/*******************************************************************************
* Description   :   PM2.5数据类，包含”传感器取值“/“数据库交互”/”服务器交互“/”前台页面交互“，
*                   继承QThread 独立线程
*
******************************************************************************/

class DataPM25 : public QThread
{
    Q_OBJECT
public:
    static DataPM25 *getInstance();                                     // 获取PM2.5单例

signals:
    void signal_update_data(AirData);
    void signal_update_dataDaily(AirDataDaily);
    void signal_sampling_start(time_t);
    void signal_sampling_off();

protected slots:
    void slot_update_dataPM25(float);                                       // 刷新数值
    void slot_update_dataPM5(float);                                       // 刷新数值
    void slot_update_dataPM10(float);                                       // 刷新数值
    void slot_start_sampling();                                         // 开始采集数据
    void slot_stop_sampling();                                          // 停止采集数据
    void slot_ready_timeout();                                          // 准备超时
    void slot_dataDaily_today(AirDataDaily);
    void slot_battery_charging(bool);
    void slot_interval_charging(IntervalType);
    void slot_interval_discharging(IntervalType);

private:
    explicit DataPM25(QObject *parent = 0);
    void run();                                                         // 线程循环事件
    void data_init();                                                   // 数据初始化
    void connect_init();                                                // 信号槽初始化

private:
    DriverPM25 *driverPM25;                                             // PM2.5传感器类
    SysData *sysData;

    QTimer *timerCharging;
    QTimer *timerDischarging;

    QTimer *timerReady;                                                 // 取值准备定时器
    QTimer *timerInterval;                                              // 取值间隔定时器
    QTimer *timerSampling;                                              // 采样持续时间

    AirData dataPM25;
    AirDataDaily dataDailyPM25;

    QStringList listDataPM25;
    int index;

    float maxValue;
    float minValue;

    bool isSampling;
    bool isCharging;

    IntervalType intervalCharging;
    IntervalType intervalDischarging;

private:
    static DataPM25* instance;                                          // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DataPM25::instance)
                delete DataPM25::instance;
        }
    };
    static FreeInstance free;

};

#endif // DATAPM25_H
