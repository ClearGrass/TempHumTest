#ifndef SYSPOWER_H
#define SYSPOWER_H
#include <QEventLoop>
#include "driverPM25.h"
#include "driverTime.h"
#include "unistd.h"

class SysPower : public QObject
{
    Q_OBJECT
public:
    static SysPower *getInstance(void);                                  // 单例函数
    void device_powerOff();
    void device_reboot();

signals:

public slots:

private:
    explicit SysPower(QObject *parent = 0);
    void data_init();
    void ready_powerOff();

private:
    DriverPM25 *driverPM25;
    DriverTime *driverTime;
    QEventLoop loop;

private:
    static SysPower *instance;                                           //系统时间单例 实例
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(SysPower::instance)
                delete SysPower::instance;
        }
    };
    static FreeInstance free;

};

#endif // SYSPOWER_H
