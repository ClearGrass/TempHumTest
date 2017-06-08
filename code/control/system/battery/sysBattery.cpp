#include "sysBattery.h"

SysBattery *SysBattery::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysBattery* SysBattery::getInstance()
{
    if(NULL == instance)
    {
        instance = new SysBattery();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  SysBattery
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysBattery::SysBattery()
    :QThread()
{
    qRegisterMetaType<InfoBattery>("InfoBattery");
    qRegisterMetaType<Power>("Power");
    data_init();
    connect_init();
    moveToThread(this);
}

/*******************************************************************************
* Function Name  :  run
* Description    :  线程运行
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysBattery::run()
{
    slot_refresh_battery();

    //启动定时器
    timerSampling->start();
    this->exec();
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysBattery::data_init()
{
    driverBattery = DriverBattery::getInstance();
    driverBattery->moveToThread(this);

    //创建定时器对象
    timerSampling = new QTimer();
    timerSampling->setInterval(BATTERY_SAMPLING_INTERVAL);

    wilddogPower = WilddogPower::getInstance();
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysBattery::connect_init()
{
    //绑定信号槽  定时刷新电源装填
    connect(timerSampling,SIGNAL(timeout()),this,SLOT(slot_refresh_battery()));
    connect(this, SIGNAL(signal_sync_power(Power)), wilddogPower, SLOT(slot_sync_power(Power)));
}

/*******************************************************************************
* Function Name  :  get_battery
* Description    :  获取电池信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
InfoBattery SysBattery::get_battery()
{
    battery.isExist  = driverBattery->isExist();
    battery.capacity = driverBattery->get_battery_capacity();
    battery.status   = driverBattery->get_battery_status();
    battery.way      = driverBattery->get_charging_way();
    battery.voltage  = driverBattery->get_battery_voltage();
    return battery;
}

/*******************************************************************************
* Function Name  :  is_chargingByAC
* Description    :  判断是否根据底座充电
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysBattery::is_chargingByAC()
{
    // 当此时电源状态为正在充电或充满电时
    if((driverBattery->get_battery_status() == CHARGING) || (driverBattery->get_battery_status() == FULL))
    {
        // 当此时的充电方式为底座充电时
        if(driverBattery->get_charging_way() == AC)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

/*******************************************************************************
* Function Name  :  slot_refresh_battery
* Description    :  刷新电源信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysBattery::slot_refresh_battery()
{
#ifdef Bran_R8
    Power power;
    battery.isExist  = driverBattery->isExist();
    battery.capacity = driverBattery->get_battery_capacity();
    battery.status   = driverBattery->get_battery_status();
    battery.way      = driverBattery->get_charging_way();
    battery.current  = driverBattery->get_battery_current();
    battery.voltage  = driverBattery->get_battery_voltage();

    emit signal_update_batteryInfo(battery);

    power.value = battery.capacity;
    power.timeStamp = QDateTime::currentDateTime().toTime_t();
    emit signal_sync_power(power);

//    // 当电池电量小于我们限定最低电量时，发送关机信号
//    if((battery.capacity <= BATTERY_LOWEST_CAPACITY) && (battery.status != CHARGING))
//    {
//        // 发送关机信号
//        emit signal_device_powerOff();
//    }
#else
    Power power;
    battery.isExist = true;
    battery.capacity = 100;
    battery.status = CHARGING;
    battery.way    = USB;
    emit signal_update_batteryInfo(battery);

    power.value = battery.capacity;
    power.timeStamp = QDateTime::currentDateTime().toTime_t();
    emit signal_sync_power(power);
#endif
}
