#ifndef WilddogControl_H
#define WilddogControl_H

#include <QThread>
#include <QTimer>
#include <QDebug>
#include "3rdparty/cJSON/cJSON.h"
#include "wilddog.h"
#include "wilddog_api.h"
#include "wilddog_config.h"
#include "wilddog_debug.h"
#include "wilddog_port.h"
#include "dataAirDefine.h"
#include "airData/wilddogAirData.h"
#include "sysData/wilddogWeather.h"
#include "sysData/wilddogStatus.h"
#include "sysData/wilddogCity.h"
#include "sysData/wilddogPower.h"
#include "sysData/wilddogApp.h"
#include "sysData/wilddogDevice.h"
#include "device/sysDevice.h"

#define AUTH_RETRY_INTERVAL     5000

class WilddogControl : public QThread
{
    Q_OBJECT
public:
    static WilddogControl *getInstance();

signals:
    void signal_verify_net();                                           // 网络异常
    void signal_token_abnormal();                                       // token异常

    void signal_device_online(bool);                                    // 设备是否在线


protected slots:
    void slot_net_abnormal();
    void slot_token_abnormal();

    void slot_wilddog_sync();

    void slot_net_connected();
    void slot_net_disconnect();

    void slot_update_infoDevice(InfoDevice);                                 // 更新设备ID
    void slot_update_token(QString);                                    // 更新token
    void slot_retry_auth();


private:
    void data_init();
    void connect_init();
    void wilddog_init(QString);
    void token_auth(QString);

    bool wilddog_weather();
    void server_init();

    static void callback_auth(void* arg, Wilddog_Return_T err);


private:
    QTimer *timerSync;
    QTimer *timerAuth;

    Wilddog_T refRoot;
    Wilddog_T refAirData;
    Wilddog_T refWeather;
    Wilddog_T refConnect;

    WilddogAirData  *wilddogAirData;
    WilddogWeather  *wilddogWeather;
    WilddogStatus   *wilddogStatus;
    WilddogDevice   *wilddogDevice;
    WilddogCity     *wilddogCity;
    WilddogApp      *wilddogApp;
    WilddogPower    *wilddogPower;

    bool isAuth;                                                    // 是否认证通过
    bool isConnected;                                               // 网络是否连接
    bool isWilddogInit;
    bool isOnLine;
    bool isTokenValid;
    bool isDeviceOnline;

    QString token;
    QString deviceSN;

    SysDevice *sysDevice;
    FileConfig *fileConfig;

    int num;

private:
    explicit WilddogControl();
    static WilddogControl* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(WilddogControl::instance)
                delete WilddogControl::instance;
        }
    };
    static FreeInstance free;

};


#endif // WilddogControl_H
