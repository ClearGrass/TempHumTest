#ifndef DATAAIRCONTROL_H
#define DATAAIRCONTROL_H

#include <QThread>
#include "dataPM25.h"
#include "dataTempHumi.h"
#include "dataTvocCo2e.h"
#include "sysControl.h"
#include "wilddog/airData/wilddogAirData.h"
#include "serverControl.h"
#include "sysTime.h"
#include "data/sysData.h"

class DataAirControl : public QThread
{
    Q_OBJECT
public:
    static DataAirControl *getInstance();

signals:
    void signal_update_data(AirData);
    void signal_update_historyDaily(Data7Days);
    void signal_update_historyHourly(Data12Hours);

    void signal_sync_data(AirData);

    void signal_save_dataDaily(AirDataDaily);
    void signal_save_dataHourly(AirDataHourly);
    void signal_save_dataCache(AirData);

    void signal_get_historyDaily(AirDataType);
    void signal_get_historyHourly(AirDataType);

    void signal_sampling_start(time_t);
    void signal_dataDaily_today(AirDataDaily);
    void signal_sampling_off();
    void signal_sampling_pm25();

    void signal_battery_charging(bool);
    void signal_intervalPM25_charging(IntervalType);
    void signal_intervalPM25_discharging(IntervalType);
    void signal_update_rawData(float temp, float hum);

private slots:
    void slot_update_data(AirData);                                     // 更新数据
    void slot_change_unitTEMP(UnitType);
    void slot_change_unitTVOC(UnitType);
    void slot_update_dataDaily(AirDataDaily);

    void slot_get_historyDaily(AirDataType);
    void slot_get_historyHourly(AirDataType);
    void slot_update_historyDaily(Data7Days);
    void slot_update_historyHourly(Data12Hours);

    void slot_sampling_start(time_t);
    void slot_sampling_off();
    void slot_sampling_pm25();

    void slot_update_time(TimeType);
    void slot_timeInit_success();
    void slot_battery_charging(bool);
    void slot_intervalPM25_charging(IntervalType);
    void slot_intervalPM25_discharging(IntervalType);
    void slot_updateRaw(float temp, float hum);

private:
    explicit DataAirControl();
    void run();
    void data_init();
    void connect_init();

private:
    UnitType unitTEMP;
    UnitType unitTVOC;

    AirData dataPM25;
    AirData dataTEMP;
    AirData dataHUMI;
    AirData dataTVOC;
    AirData dataCO2E;

    AirDataDaily dataDailyPM25;
    AirDataDaily dataDailyTEMP;
    AirDataDaily dataDailyHUMI;
    AirDataDaily dataDailyTVOC;
    AirDataDaily dataDailyCO2E;

private:
    DataPM25 *dataPm25;
    DataTempHumi *dataTempHumi;
    DataTvocCo2e *dataTvocCo2e;
    Database *database;
    FileConfig *fileConfig;

    WilddogAirData *wilddogAirData;
    ServerControl *serverControl;
    SysControl  *sysControl;
    SysTime     *sysTime;
    SysData     *sysData;

private:
    static DataAirControl* instance;                                          // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DataAirControl::instance)
                delete DataAirControl::instance;
        }
    };
    static FreeInstance free;
};

#endif // DATAAIRCONTROL_H
