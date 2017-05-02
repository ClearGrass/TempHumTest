#ifndef WILDDOGAPP_H
#define WILDDOGAPP_H

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

class WilddogApp : public QObject
{
    Q_OBJECT
public:
    static WilddogApp *getInstance();
    void wilddog_init(Wilddog_T);

private slots:
    void slot_device_online(bool);

private:
    static void callback_app(const Wilddog_Node_T* p_snapshot, void* arg, Wilddog_Return_T err);

private:
    Wilddog_T app;
    bool isDeviceOnline;

private:
    explicit WilddogApp(QObject *parent = 0);
    static WilddogApp* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(WilddogApp::instance)
                delete WilddogApp::instance;
        }
    };
    static FreeInstance free;

};

#endif // WILDDOGAPP_H
