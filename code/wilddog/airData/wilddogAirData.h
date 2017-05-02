#ifndef WILDDOGAIRDATA_H
#define WILDDOGAIRDATA_H

#include <QDebug>
#include <QTimer>
#include <QThread>
#include "wilddog.h"
#include "wilddog_api.h"
#include "wilddog_config.h"
#include "wilddog_debug.h"
#include "wilddog_port.h"
#include "3rdparty/cJSON/cJSON.h"
#include "wilddog/wilddogDefine.h"
#include "dataAirDefine.h"
#include "globalStruct.h"

#define RETRY_PUSH_INTERVAL     1000
#define SYNC_DATA_INTERVAL      60000


class WilddogAirData : public QObject
{
    Q_OBJECT
public:
    static WilddogAirData *getInstance();
    void wilddog_init(Wilddog_T);

signals:
    void signal_net_abnormal();
    void signal_token_abnormal();

public slots:
    void slot_sync_data(AirData);
    void slot_sync_datas();

    void slot_client_online(bool);
    void slot_device_online(bool);

protected:
    void sync_data(AirData);
    explicit WilddogAirData(QObject *parent = 0);
    static void callback_data(void* arg, Wilddog_Return_T err);

private:
    Wilddog_T monitor;
    Wilddog_T nodeData;

    QTimer *timerRetry;

    QTimer *timerSync;

    AirData dataPM25;
    AirData dataTEMP;
    AirData dataHUMI;
    AirData dataTVOC;
    AirData dataCO2E;

    bool isClientOnline;
    bool isDeviceOnline;

private:
    static WilddogAirData* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(WilddogAirData::instance)
                delete WilddogAirData::instance;
        }
    };
    static FreeInstance free;

};

#endif // WILDDOGAIRDATA_H
