#ifndef WILDDOGWEATHER_H
#define WILDDOGWEATHER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QTimer>
#include <QDateTime>
#include "wilddog.h"
#include "wilddog_api.h"
#include "wilddog_config.h"
#include "wilddog_debug.h"
#include "wilddog_port.h"
#include "globalStruct.h"
#include "wilddog/wilddogDefine.h"
#include "file/config/fileConfig.h"

class WilddogWeather : public QObject
{
    Q_OBJECT
public:
    static WilddogWeather *getInstance();
    void wilddog_init(Wilddog_T);

signals:
    void signal_update_weather(InfoWeather);
    void signal_token_abnormal();

private slots:
    void slot_observer_retry();
    void slot_device_online(bool);
    void slot_update_cityID(QString);

private:
    void start_observer(QString cityID);                                // 开始监听
    void analyse_weather(const Wilddog_Node_T *pNode);
    QString get_validString(Wilddog_Node_T *pNode,char *field);
    float get_validValue(Wilddog_Node_T *pNode,char *field);
    static Wilddog_Node_T *node_get(Wilddog_Node_T *p_head,const char* key_name);
    static void callback_weather(const Wilddog_Node_T* p_snapshot, void* arg, Wilddog_Return_T err);

private:
    FileConfig *fileConfig;

    Wilddog_T weather;
    Wilddog_T wilddog;

    bool isDeviceOnline;
    bool isObserving;
    bool isRetryObserve;

    QString cityID;
    QTimer *timerObserver;

private:
    explicit WilddogWeather();
    static WilddogWeather *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(WilddogWeather::instance)
                delete WilddogWeather::instance;
        }
    };
    static FreeInstance free;
};

#endif // SERVERWEATHER_H
