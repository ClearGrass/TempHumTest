#ifndef SERVERCONTROL_H
#define SERVERCONTROL_H

#include <QThread>
#include "3rdparty/cJSON/cJSON.h"
#include "server/airData/serverAirData.h"
#include "server/sysData/serverCity.h"
#include "server/sysData/serverDevice.h"
#include "server/sysData/serverFirmware.h"
#include "server/sysData/serverWeather.h"
#include "device/sysDevice.h"
#include "server/sysData/serverDebug.h"
#include "database.h"

class ServerControl : public QThread
{
    Q_OBJECT
public:
    static ServerControl *getInstance();

signals:
    void signal_push_data(QString deviceID, QString json, QString, QString);

protected slots:
    void slot_push_data();                                              // 推送数据

    void slot_net_connected();
    void slot_net_disconnect();
    void slot_update_deviceID(QString);

protected:
    explicit ServerControl(QObject *parent = 0);

private:
    void data_init();
    void connect_init();

    QString trans_json(cacheData);

private:
    ServerAirData   *serverAirData;
    ServerCity      *serverCity;
    ServerDevice    *serverDevice;
    ServerFirmware  *serverFirmware;
    ServerWeather   *serverWeather;
    ServerDebug     *serverDebug;

    Database *database;
    SysDevice *sysDevice;

    QList<AirDataDaily> *listDataDaily;
    QList<AirDataHourly> *listDataHourly;

    QString deviceID;                                                   // 设备ID
    QTimer *timerPush;

    bool isConnected;
    bool isDeviceInit;

private:
    static ServerControl* instance;                                 // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(ServerControl::instance)
                delete ServerControl::instance;
        }
    };
    static FreeInstance free;

};

#endif // SERVERCONTROL_H
