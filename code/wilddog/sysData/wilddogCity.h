#ifndef WILDDOGCITY_H
#define WILDDOGCITY_H

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

class WilddogCity : public QObject
{
    Q_OBJECT
public:
    static WilddogCity *getInstance();
    void wilddog_init(Wilddog_T);

private slots:
    void slot_sync_city(QString);
    void slot_device_online(bool);
    void slot_setValue_retry();

private:
    static void callback_city(void* arg, Wilddog_Return_T err);

private:
    void sync_city(QString);

private:
    Wilddog_T city;
    bool isDeviceOnline;

    FileConfig *fileConfig;

    QTimer *timerRetry;

    QString cityID;

private:
    explicit WilddogCity(QObject *parent = 0);
    static WilddogCity* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(WilddogCity::instance)
                delete WilddogCity::instance;
        }
    };
    static FreeInstance free;

};

#endif // WILDDOGCITY_H
