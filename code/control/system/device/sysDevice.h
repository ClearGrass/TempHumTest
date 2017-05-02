#ifndef SYSDEVICE_H
#define SYSDEVICE_H

#include <QList>
#include <QThread>
#include <QTimer>
#include <QDateTime>
#include "globalVariable.h"
#include "server/sysData/serverDevice.h"
#include "file/config/fileConfig.h"
#include "wilddog/sysData/wilddogDevice.h"
#include "3rdparty/cJSON/cJSON.h"

#define     DEVICE_INIT_INTERVAL    5000
#define     GET_TOKEN_INTERVAL      5000

class SysDevice : public QThread
{
    Q_OBJECT
public:
    static SysDevice* getInstance();
    bool is_deviceInit();
    bool is_timeInit();
    bool is_tokenValid();

    InfoDevice get_infoDevice();

signals:
    void signal_update_infoDevice(InfoDevice);                          // 更新设备成功
    void signal_update_token(QString);                                  // 更新token成功
    void signal_sync_device(QString);
    void signal_device_init(QString,QString,QString,QString);
    void signal_get_token(QString,QString);

private slots:
    void slot_token_abnormal();

    void slot_net_connected();
    void slot_net_disconnect();

    void slot_deviceInit_success(InfoDevice);
    void slot_deviceInit_failed();
    void slot_retry_deviceInit();

    void slot_getToken_success(Token);
    void slot_getToken_failed();
    void slot_retry_getToken();

private:
    void data_init();
    void connect_init();
    QString get_miio_info(QString,QString,QString);

private:
    ServerDevice *serverDevice;
    WilddogDevice *wilddogDevice;
    FileConfig *fileConfig;

    bool isTimeInit;
    bool isConnected;
    bool isDeviceInit;
    bool isTokenValid;

    QTimer *timerDevice;
    QTimer *timerToken;

    InfoDevice infoDevice;


private:
    SysDevice();
    static SysDevice *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(SysDevice::instance)
                delete SysDevice::instance;
        }
    };
    static FreeInstance free;

};

#endif // SYSDEVICE_H
