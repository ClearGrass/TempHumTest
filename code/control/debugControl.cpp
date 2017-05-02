#include "debugControl.h"

DebugControl *DebugControl::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DebugControl *DebugControl::getInstance()
{
    if(NULL == instance)
    {
        instance = new DebugControl();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  DebugControl
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DebugControl::DebugControl(QObject *parent)
    : QThread(parent)
{
    qRegisterMetaType<InfoSys>("InfoSys");
    data_init();
    connect_init();
    moveToThread(this);
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DebugControl::data_init()
{
    isConnected = false;
    isDeviceInit = true;

    sysControl   = SysControl::getInstance();
    sysBattery   = SysBattery::getInstance();
    sysDevice    = SysDevice::getInstance();
    funcCity     = FuncCity::getInstance();
    funcFirmware = FuncFirmware::getInstance();
    serverDebug  = ServerDebug::getInstance();

    infoSys.device  = sysDevice->get_infoDevice();
    infoSys.battery = sysBattery->get_battery();
    infoSys.city    = funcCity->get_currentCity();
    infoSys.version = funcFirmware->get_version();

    isDeviceInit = sysDevice->is_deviceInit();
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DebugControl::connect_init()
{
    connect(sysControl, SIGNAL(signal_net_connected()), this, SLOT(slot_net_connected()));
    connect(sysControl, SIGNAL(signal_net_disconnect()), this, SLOT(slot_net_disconnect()));
    connect(sysDevice, SIGNAL(signal_update_infoDevice(InfoDevice)), this, SLOT(slot_update_infoDevice(InfoDevice)));
    connect(sysBattery, SIGNAL(signal_update_batteryInfo(InfoBattery)), this, SLOT(slot_update_infoBattery(InfoBattery)));
    connect(funcCity, SIGNAL(signal_update_infoCity(InfoCity)), this, SLOT(slot_update_infoCity(InfoCity)));
    connect(this, SIGNAL(signal_system_heartbeat(InfoSys)), serverDebug, SLOT(slot_system_heartbeat(InfoSys)));
}

/*******************************************************************************
* Function Name  :  slot_net_connected
* Description    :  网络连接以后
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DebugControl::slot_net_connected()
{
    isConnected = true;

    // 若此时的设备已经初始化
    if(isDeviceInit)
    {
        // 第一次同步数据
        emit signal_system_heartbeat(infoSys);
    }
}

/*******************************************************************************
* Function Name  :  slot_net_disconnect
* Description    :  网络断开连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DebugControl::slot_net_disconnect()
{
    isConnected = false;
}

/*******************************************************************************
* Function Name  :  slot_update_infoBattery
* Description    :  刷新电池的信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DebugControl::slot_update_infoBattery(InfoBattery infoBattery)
{
    if((infoSys.battery.capacity != infoBattery.capacity) || (infoSys.battery.status != infoBattery.status))
    {
        infoSys.battery = infoBattery;

        if(isConnected)
        {
            emit signal_system_heartbeat(infoSys);
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_update_infoCity
* Description    :  刷新城市信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DebugControl::slot_update_infoCity(InfoCity infoCity)
{
    infoSys.city = infoCity;
    if(isConnected && isDeviceInit)
    {
        emit signal_system_heartbeat(infoSys);
    }
}

/*******************************************************************************
* Function Name  :  slot_update_infoDevice
* Description    :  刷新设备信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DebugControl::slot_update_infoDevice(InfoDevice infoDevice)
{
    isDeviceInit = true;
    infoSys.device = infoDevice;
    if(isConnected)
    {
        emit signal_system_heartbeat(infoSys);
    }
}




