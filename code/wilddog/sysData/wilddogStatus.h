#ifndef WILDDOGSTATUS_H
#define WILDDOGSTATUS_H

#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QDateTime>
#include "wilddog.h"
#include "wilddog_api.h"
#include "wilddog_config.h"
#include "wilddog_debug.h"
#include "wilddog_port.h"
#include "3rdparty/cJSON/cJSON.h"
#include "wilddog/wilddogDefine.h"
#include "dataAirDefine.h"
#include "globalStruct.h"

class WilddogStatus : public QObject
{
    Q_OBJECT
public:
    static WilddogStatus *getInstance();
    void wilddog_init(Wilddog_T);

signals:
    void signal_client_online(bool);
    void signal_verify_net();

private slots:
    void slot_device_online(bool);
    void slot_setValue_retry();

private:
    void create_nodeClient();
    void device_online();
    void device_offline();
    static void callback_device(void* arg, Wilddog_Return_T err);
    static void callback_client(const Wilddog_Node_T* p_snapshot,void* arg,Wilddog_Return_T err);
    static void callback_status(const Wilddog_Node_T* p_snapshot,void* arg,Wilddog_Return_T err);

private:
    Wilddog_T nodeClient;
    Wilddog_T nodeDevice;

    bool isDeviceOnline;
    QTimer *timerRetry;

private:
    explicit WilddogStatus(QObject *parent = 0);
    static WilddogStatus* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(WilddogStatus::instance)
                delete WilddogStatus::instance;
        }
    };
    static FreeInstance free;
};

#endif // WILDDOGSTATUS_H
