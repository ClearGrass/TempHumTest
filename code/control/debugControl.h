#ifndef DEBUGCONTROL_H
#define DEBUGCONTROL_H
#include <QTimer>
#include <QThread>
#include "function/firmware/funcFirmware.h"
#include "function/city/funcCity.h"
#include "server/sysData/serverDebug.h"
#include "battery/sysBattery.h"
#include "globalStruct.h"
#include "sysControl.h"

class DebugControl : public QThread
{
    Q_OBJECT
public:
    static DebugControl *getInstance();                                        // 获取此时的单例

signals:
    void signal_system_heartbeat(InfoSys);

private slots:
    void slot_net_connected();
    void slot_net_disconnect();
    void slot_update_infoCity(InfoCity);
    void slot_update_infoDevice(InfoDevice);
    void slot_update_infoBattery(InfoBattery);

private:
    void data_init();
    void connect_init();

private:
    InfoSys infoSys;

    ServerDebug     *serverDebug;
    FuncCity        *funcCity;
    FuncFirmware    *funcFirmware;
    SysDevice       *sysDevice;
    SysBattery      *sysBattery;
    SysControl      *sysControl;

    bool isConnected;
    bool isDeviceInit;

private:
    explicit DebugControl(QObject *parent = 0);
    static DebugControl *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DebugControl::instance)
                delete DebugControl::instance;
        }
    };
    static FreeInstance free;
};

#endif // DEBUGCONTROL_H
