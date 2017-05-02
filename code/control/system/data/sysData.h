#ifndef SYSDATA_H
#define SYSDATA_H

#include <QThread>
#include "file/config/fileConfig.h"


class SysData : public QThread
{
    Q_OBJECT
public:
    static SysData *getInstance();
    UnitType get_unitTVOC();                                                // 获取tvoc单位
    UnitType get_unitTEMP();                                                // 获取温度单位
    IntervalType get_intervalCharging();
    IntervalType get_intervalDischarging();

signals:
    void signal_change_unitTEMP(UnitType);
    void signal_change_unitTVOC(UnitType);
    void signal_intervalSampling_charging(IntervalType);
    void signal_intervalSampling_discharging(IntervalType);

protected slots:
    void slot_change_unitTEMP(UnitType);                                            // 设置温度单位
    void slot_change_unitTVOC(UnitType);                                            // 设置tVoc单位
    void slot_intervalSampling_charging(IntervalType);
    void slot_intervalSampling_discharging(IntervalType);

private:
    explicit SysData(QObject *parent = 0);
    void data_init();
    void connect_init();

private:
    FileConfig *fileConfig;
    UnitType unitTVOC;
    UnitType unitTEMP;
    IntervalType intervalCharging;
    IntervalType intervalDischarging;

private:
    static SysData *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(SysData::instance)
                delete SysData::instance;
        }
    };
    static FreeInstance free;
};

#endif // SYSDATA_H
