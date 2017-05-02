#ifndef WILDDOGPOWER_H
#define WILDDOGPOWER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include "wilddog.h"
#include "wilddog_api.h"
#include "wilddog_config.h"
#include "wilddog_debug.h"
#include "wilddog_port.h"
#include "globalStruct.h"
#include "wilddog/wilddogDefine.h"

class WilddogPower : public QObject
{
    Q_OBJECT
public:
    static WilddogPower *getInstance();
    void wilddog_init(Wilddog_T);

private slots:
    void slot_sync_power(Power);
    void slot_device_online(bool);
    void slot_setValue_retry();

private:
    void sync_power(Power);
    static void callback_power(void* arg, Wilddog_Return_T err);

private:
    Wilddog_T nodePower;
    Power power;

    QTimer *timerRetry;
    bool isDeviceOnline;

private:
    explicit WilddogPower(QObject *parent = 0);
    static WilddogPower* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(WilddogPower::instance)
                delete WilddogPower::instance;
        }
    };
    static FreeInstance free;
};

#endif // WILDDOGPOWER_H
