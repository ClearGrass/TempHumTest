#ifndef WILDDOGDEVICE_H
#define WILDDOGDEVICE_H

#include <QDebug>
#include <QTimer>
#include "wilddog.h"
#include "wilddog_api.h"
#include "wilddog_config.h"
#include "wilddog_debug.h"
#include "wilddog_port.h"
#include "3rdparty/cJSON/cJSON.h"
#include "wilddog/wilddogDefine.h"
#include "dataAirDefine.h"
#include "globalStruct.h"
#include "file/config/fileConfig.h"

class WilddogDevice : public QObject
{
    Q_OBJECT
public:
    static WilddogDevice *getInstance();
    void wilddog_init(Wilddog_T);

private slots:
    void slot_sync_device(QString);
    void slot_device_online(bool);
    void slot_setValue_retry();

private:
    static void callback_device(void* arg, Wilddog_Return_T err);

private:
    void sync_device(QString);

private:
    Wilddog_T device;
    bool isDeviceOnline;

    FileConfig *fileConfig;

    QTimer *timerRetry;

    QString deviceID;

private:
    explicit WilddogDevice(QObject *parent = 0);
    static WilddogDevice* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(WilddogDevice::instance)
                delete WilddogDevice::instance;
        }
    };
    static FreeInstance free;
};

#endif // WILDDOGDEVICE_H
