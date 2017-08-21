#include "sysControl.h"

SysControl *SysControl::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysControl *SysControl::getInstance()
{
    if(NULL == instance)
    {
        instance = new SysControl();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  SysControl
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysControl::SysControl()
    : QThread()
{
    data_init();
    connect_init();

    wifi = sysWiFi->get_currentWiFi();
    if(wifi.status == CONNECTED)
    {
        isConnected = true;
        emit signal_net_connected();
    }
    moveToThread(this);

}

/*******************************************************************************
* Function Name  :  run
* Description    :  系统运行
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::run()
{
//    sysDevice->start();
    sysBattery->start();
    sysTime->start();
    sysGravity->start();
    sysButton->start();
    sysLight->start();
    sysData->start();

#ifndef Bran_R8
    timerTest->start();
#endif
    this->exec();
}

/*******************************************************************************
*                           rcl@2016-11-28
* Function Name  :  data_init
* Description    :  系统数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::data_init()
{
    sysTime     = SysTime::getInstance();
    sysButton   = SysButton::getInstance();
    sysBattery  = SysBattery::getInstance();
    sysGravity  = SysGravity::getInstance();
    sysLanguage = SysLanguage::getInstance();
    sysLight    = SysLight::getInstance();

    sysDevice   = SysDevice::getInstance();
    sysWiFi     = SysWiFi::getInstance();
    sysPower    = SysPower::getInstance();
    sysData     = SysData::getInstance();

    fileConfig  = FileConfig::getInstance();

    timerTest = new QTimer();
    timerTest->setInterval(5000);
    timerTest->setSingleShot(true);

    isCharging  = false;
    isConnected = false;
    isScreenOn  = true;
    isNight     = sysTime->get_night();
    isTimeInit  = sysDevice->is_timeInit();
    isDeviceInit = sysDevice->is_deviceInit();

    battery     = sysBattery->get_battery();
    qDebug()<<"SYSTEM INIT END!!!!!!!!!!!"<<QThread::currentThreadId();
}

/*******************************************************************************
*                           rcl@2016-11-28
* Function Name  :  connect_init
* Description    :  初始化槽函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::connect_init()
{
    // 监听横竖屏的切换
    connect(sysGravity, SIGNAL(signal_change_direction(DIRECTION)), this, SLOT(slot_update_direction(DIRECTION)));
    connect(sysGravity, SIGNAL(signal_shell_doubleTap()), this, SLOT(slot_shell_doubleTap()));

    // 监听屏幕光照的变化
    connect(sysLight, SIGNAL(signal_update_light(int)), this,SLOT(slot_update_light(int)));
    connect(sysLight, SIGNAL(signal_update_inLight(int)), this,SLOT(slot_update_inLight(int)));
    connect(sysLight, SIGNAL(signal_update_outLight(int)), this,SLOT(slot_update_outLight(int)));
    connect(sysLight, SIGNAL(signal_update_screen(bool)), this, SLOT(slot_update_screen(bool)));

    connect(sysTime, SIGNAL(signal_set_night(bool)), this, SLOT(slot_in_night(bool)));

    connect(this, SIGNAL(signal_screen_on()), sysLight, SLOT(slot_screen_on()));
    connect(this, SIGNAL(signal_screen_off()), sysLight, SLOT(slot_screen_off()));
    connect(this, SIGNAL(signal_offInterval_charging(IntervalType)), sysLight, SLOT(slot_offInterval_charging(IntervalType)));
    connect(this, SIGNAL(signal_offInterval_discharging(IntervalType)), sysLight, SLOT(slot_offInterval_discharging(IntervalType)));
    connect(this, SIGNAL(signal_autoAdjust_light(bool)), sysLight, SLOT(slot_set_autoMode(bool)));
    connect(this, SIGNAL(signal_set_light(int)), sysLight, SLOT(slot_set_value(int)));
    connect(this, SIGNAL(signal_user_operations()), sysLight, SLOT(slot_user_operations()));


    connect(sysButton, SIGNAL(signal_button_singleClick()), this, SLOT(slot_button_singleClick()));
//    connect(sysButton, SIGNAL(signal_button_doubleClick()), this, SLOT(slot_button_doubleClick()));
    connect(sysButton, SIGNAL(signal_button_longPress()), this, SLOT(slot_button_longPress()));


    connect(sysWiFi, SIGNAL(signal_update_wifi(WiFi)), this, SLOT(slot_update_wifiStatus(WiFi)));

    connect(this, SIGNAL(signal_net_connected()), sysDevice, SLOT(slot_net_connected()));
    connect(this, SIGNAL(signal_net_disconnect()), sysDevice, SLOT(slot_net_disconnect()));
    connect(this, SIGNAL(signal_net_connected()), sysTime, SLOT(slot_net_connected()));
    connect(this, SIGNAL(signal_net_disconnect()), sysTime, SLOT(slot_net_disconnect()));

    connect(this, SIGNAL(signal_battery_charging(bool)), sysLight, SLOT(slot_battery_charing(bool)));

    connect(sysBattery, SIGNAL(signal_update_batteryInfo(InfoBattery)), this, SLOT(slot_update_battery(InfoBattery)));
    connect(sysBattery, SIGNAL(signal_device_powerOff()), this, SLOT(slot_device_powerOff()));

    connect(sysLanguage, SIGNAL(signal_device_reboot()), this, SLOT(slot_device_reboot()));

    connect(timerTest, SIGNAL(timeout()), this, SLOT(slot_test_net()));
}

/*******************************************************************************
* Function Name  :  get_currentWiFi
* Description    :  获取当前WiFi
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WiFi SysControl::get_currentWiFi()
{
    return wifi;
}

/*******************************************************************************
* Function Name  :  is_deviceInit
* Description    :  设备是否初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysControl::is_deviceInit()
{
    return sysDevice->is_deviceInit();
}

/*******************************************************************************
* Function Name  :  is_timeInit
* Description    :  时间是否初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysControl::is_timeInit()
{
    return sysDevice->is_timeInit();
}

/*******************************************************************************
* Function Name  :  is_languageInit
* Description    :  语言初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysControl::is_languageInit()
{
    return sysLanguage->is_languageInit();
}

/*******************************************************************************
* Function Name  :  is_chargingByAC
* Description    :  判断是否通过底座充电
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysControl::is_chargingByAC()
{
    return sysBattery->is_chargingByAC();
}

bool SysControl::is_ScreenOn()
{
    return isScreenOn;
}

/*******************************************************************************
* Function Name  :  slot_interface_operations
* Description    :  界面操作
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_interface_operations()
{
    if(isScreenOn)
    {
        emit signal_user_operations();
    }
}



/*******************************************************************************
* Function Name  :  slot_update_battery
* Description    :  刷新电池
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_update_battery(InfoBattery battery)
{
    this->battery = battery;

    // 刷新电池信息
    emit signal_update_battery(battery);

    // 刷新电池状态
    if((CHARGING == this->battery.status) || (FULL == this->battery.status))
    {
        if(!isCharging)
        {
            isCharging = true;

            // 发送充电信号，将当前页面模式变为复杂模式
            emit signal_battery_charging(true);
        }
    }
    else
    {
        if(isCharging)
        {
            isCharging = false;

            // 发送断电信号
            emit signal_battery_charging(false);
        }
    }
}

/*******************************************************************************
* Function Name  :  get_battery
* Description    :  获取电池状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
InfoBattery SysControl::get_battery()
{
    return battery;
}

/*******************************************************************************
* Function Name  :  slot_test_net
* Description    :  发送网络连接信号，用于测试，此时的网络状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_test_net()
{
    emit signal_net_connected();
//    emit signal_change_direction(VERTICAL);
//    sysLanguage->language_init(LANG_TW);
}

/*******************************************************************************
* Function Name  :  slot_update_wifiStatus
* Description    :  更新网络状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_update_wifiStatus(WiFi wifi)
{
    this->wifi = wifi;

    // 若此时已联网
    if(CONNECTED == wifi.status)
    {
        if(!isConnected)
        {
            isConnected = true;
            qDebug()<<"@@@@@@@@@@@@@@@ NET_CONNECTED @@@@@@@@@@@@@@@@@@@";

            // 发送联网信号
            emit signal_net_connected();
            emit signal_net_ip(wifi.ip);
            qDebug()<<"isCONNECTED";
        }
    }
    else
    {
        if(isConnected)
        {
            isConnected = false;
            qDebug()<<"@@@@@@@@@@@@@@@ NET_DISCONNECT @@@@@@@@@@@@@@@@@@@";

            // 发送断网信号
            emit signal_net_disconnect();
            emit signal_net_ip(wifi.ip);
        }
    }
}

//!
//! 配置文件功能模块
//!

/*******************************************************************************
*                           rcl@2016-11-29
* Function Name  :  get_value
* Description    :  获取配置值
* Input          :  QString parameter   配置参数
* Output         :  None
* Return         :  None
*******************************************************************************/
QString SysControl::get_value(const QString parameter)
{
    return fileConfig->get_value(parameter);
}

/*******************************************************************************
*                           rcl@2016-11-29
* Function Name  :  set_value
* Description    :  设置配置值
* Input          :  QString parameter   配置参数
* Output         :  QString value       配置值
* Return         :  None
*******************************************************************************/
void SysControl::set_value(QString parameter, QString value)
{
    fileConfig->set_value(parameter, value);
}

void SysControl::reset()
{
    fileConfig->config_reset();
}

//!
//! G-Sensor功能块部分接口
//!

/*******************************************************************************
*                           rcl@2016-11-28
* Function Name  :  slot_update_direction
* Description    :  改变屏幕方向
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_update_direction(DIRECTION direction)
{
    emit signal_change_direction(direction);
}

/*******************************************************************************
* Function Name  :  slot_shell_doubleTap
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_shell_doubleTap()
{
    if(get_value(DOUBLE_CLICK_CHECKING) == "false")
    {
        qDebug() << "ignore double tap" << endl;
        return ;
    }
    // 若屏幕亮着
    if(isScreenOn)
    {
        // 发送敲击信号
        emit signal_shell_doubleTap();
        emit signal_user_operations();
    }
}

//!
//! 时间功能块部分接口
//!

/*******************************************************************************
* Function Name  :  slot_in_night
* Description    :  是否入夜
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_in_night(bool isNight)
{
    this->isNight = isNight;
    emit signal_in_night(isNight);
}

/*******************************************************************************
* Function Name  :  get_night
* Description    :  获取夜晚状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysControl::get_night()
{
    return isNight;
}


//!
//! 亮度功能块部分接口
//!

/*******************************************************************************
*                           rcl@2016-11-28
* Function Name  :  slot_update_light
* Description    :  刷新页面显示亮度值
* Input          :  int light 当前亮度值
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_update_light(int light)
{
    emit signal_light_value(light);
}

/*******************************************************************************
*                           rcl@2016-11-28
* Function Name  :  slot_update_inLight
* Description    :  测试，显示亮度当前设置值
* Input          :  int inLight 当前亮度设置值
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_update_inLight(int light)
{
    emit signal_inLight_value(light);
}

/*******************************************************************************
* Function Name  :  slot_update_outLight
* Description    :  测试，显示外界亮度值
* Input          :  int outLight 外界亮度值
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_update_outLight(int light)
{
    emit signal_outLight_value(light);
}

/*******************************************************************************
* Function Name  :  slot_set_lightValue
* Description    :  设置亮度值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_set_lightValue(int light)
{
    emit signal_set_light(light);
}

/*******************************************************************************
* Function Name  :  slot_set_lightMode
* Description    :  设置亮度模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_set_lightMode(bool mode)
{
    emit signal_autoAdjust_light(mode);
}

/*******************************************************************************
* Function Name  :  slot_update_screen
* Description    :  刷新屏幕状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_update_screen(bool screen)
{
    isScreenOn = screen;
}

/*******************************************************************************
* Function Name  :  slot_offInterval_charging
* Description    :  切换充电时的熄屏间隔
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_offInterval_charging(IntervalType interval)
{
    emit signal_offInterval_charging(interval);
}

/*******************************************************************************
* Function Name  :  slot_offInterval_discharging
* Description    :  切换断电时的熄屏间隔
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_offInterval_discharging(IntervalType interval)
{
    emit signal_offInterval_discharging(interval);
}

//!
//! button 功能模块
//!

/*******************************************************************************
* Function Name  :  slot_button_doubleClick
* Description    :  按钮双击
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_button_doubleClick()
{
    // 若屏幕亮着
    if(isScreenOn)
    {
        // 发送灭屏信号
        emit signal_screen_off();
    }
}

/*******************************************************************************
* Function Name  :  slot_button_longPress
* Description    :  按钮长按
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_button_longPress()
{
    // 长按
    emit signal_button_longPress();

}

/*******************************************************************************
* Author        :   虎正玺@2017-01-25
* Function Name :   slot_screenOff
* Description   :   关闭屏幕
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysControl::slot_screenOff()
{
    // 若屏幕亮着
    if(isScreenOn)
    {
        // 发送灭屏信号
        emit signal_screen_off();
    }
}

/*******************************************************************************
* Function Name  :  slot_device_powerOff
* Description    :  设备关机
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_device_powerOff()
{
    emit signal_power_off();

    QTimer::singleShot(POWEROFF_INTERVAL, this, SLOT(slot_delayPowerOff()));
}

/*******************************************************************************
 *                  虎正玺@2017-01-16
* Function Name  :  slot_delayPowerOff
* Description    :  延迟关机槽
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_delayPowerOff()
{
    sysPower->device_powerOff();
}

/*******************************************************************************
* Function Name  :  slot_device_reboot
* Description    :  设备重启
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_device_reboot()
{
    qDebug()<<"RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR";
    emit signal_power_off();
    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    fileConfig->set_value(SYS_ENTER, "true");
    sysPower->device_reboot();
}

/*******************************************************************************
* Function Name  :  slot_button_singleClick()
* Description    :  单击
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysControl::slot_button_singleClick()
{
    if(isScreenOn)
    {
        // 发送切换屏幕信号
        emit signal_button_singleClick();
        emit signal_user_operations();
    }
    else
    {
        // 发送亮屏信号
        emit signal_screen_on();
    }
}
