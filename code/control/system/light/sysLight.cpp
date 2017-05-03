#include "sysLight.h"
#include <QtDebug>
#include "math.h"

SysLight *SysLight::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysLight* SysLight::getInstance()
{
    if(NULL == instance)
    {
        instance = new SysLight();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  SysLight
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysLight::SysLight(QObject *parent):
    QThread(parent)
{
    qRegisterMetaType<IntervalType>("IntervalType");
    data_init();
    connect_init();
    moveToThread(this);
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  初始化基本配置信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::data_init()
{
    int i;
    isCharging = false;

    // 开机默认亮屏
    fileConfig  = FileConfig::getInstance();
    driverLight = DriverLight::getInstance();
    driverLight->light_on();

    timerSampling = new QTimer(this);
    timerSampling->setInterval(LIGHT_SAMPLING_INTERVAL);

    timerDischarging = new QTimer(this);
    timerDischarging->setSingleShot(true);

    timerCharging   = new QTimer(this);
    timerCharging->setSingleShot(true);

    offCharging = fileConfig->get_value(SCREEN_OFF_CHARGING);
    offDischarging = fileConfig->get_value(SCREEN_OFF_DISCHARGING);

    intervalCharging = trans_interval(offCharging);
    intervalDisharging = trans_interval(offDischarging);

    if(intervalCharging != 0)
    {
        isOffScreenCharging = true;
        timerCharging->setInterval(intervalCharging);
        //        timerCharging->setInterval(5000);
    }
    else
    {
        isOffScreenCharging = false;
    }

    if(intervalDisharging != 0)
    {
        isOffScreenDischarging = true;
        timerDischarging->setInterval(intervalDisharging);
    }
    else
    {
        isOffScreenDischarging = false;
    }


    iCurrentLight = fileConfig->get_value(LIGHT_VALUE).toInt();

    isAutoAdjust = false;
    iCurrentLight = qRound(MIN_INLIGHT_VALUE + (MAX_INLIGHT_VALUE - MIN_INLIGHT_VALUE)*(iCurrentLight/100.0));
    driverLight->set_inLight(iCurrentLight);


    iTotalLight = 0;
    iLastLight = -1;

    // 初始化缓存器
    for(i=0; i<LIGHT_CACHE_DEPTH ;++i)
    {
        lightCache.append(0);
    }
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::connect_init()
{
    connect(timerSampling, SIGNAL(timeout()), this, SLOT(slot_autoAdjust_light()));
    connect(timerCharging, SIGNAL(timeout()), this, SLOT(slot_screen_off()));
    connect(timerDischarging, SIGNAL(timeout()), this, SLOT(slot_screen_off()));
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::run()
{
//    if(isCharging)
//    {
//        qDebug()<<"#############################################################CHARGING";
//        if(isOffScreenCharging)
//        {
//            timerCharging->start();
//        }
//    }
//    else
//    {
//        qDebug()<<"#############################################################DISCHARGING!!!";
//        if(isOffScreenDischarging)
//        {
//            timerDischarging->start();
//        }
//    }
    this->exec();
}

/*******************************************************************************
* Function Name  :  slot_set_light
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::slot_set_value(int light)
{

    iCurrentLight = qRound(MIN_INLIGHT_VALUE + (MAX_INLIGHT_VALUE - MIN_INLIGHT_VALUE)*(light/100.0));
    if(iCurrentLight > 250)
    {
        iCurrentLight = 251;
    }

    driverLight->set_inLight(iCurrentLight);
}

/*******************************************************************************
* Function Name  :  slot_device_charging
* Description    :  设备充电
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::slot_battery_charing(bool charging)
{
//    if(charging)
//    {
//        isCharging = true;
//        if(isOffScreenCharging)
//        {
//            timerCharging->start();
//        }
//        if(timerDischarging->isActive())
//        {
//            timerDischarging->stop();
//        }
//        // 点亮屏幕
//        slot_screen_on();
//    }
//    else
//    {
//        isCharging = false;
//        if(isOffScreenDischarging)
//        {
//            timerDischarging->start();
//        }
//        if(timerCharging->isActive())
//        {
//            timerCharging->stop();
//        }
//    }
}

/*******************************************************************************
* Function Name  :  slot_user_operations
* Description    :  用户操作
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::slot_user_operations()
{
//    // 若此时在充电
//    if(isCharging)
//    {
//        // 且允许熄灭屏幕
//        if(isOffScreenCharging)
//        {
//            timerCharging->stop();
//            timerCharging->start();
//        }
//    }
//    else
//    {
//        if(isOffScreenDischarging)
//        {
//            timerDischarging->stop();
//            timerDischarging->start();
//        }
//    }
}

/*******************************************************************************
* Function Name  :  slot_sampling_light
* Description    :  采集亮度并调节
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::slot_autoAdjust_light()
{
    int i;
    int outLight = driverLight->get_outLight();
    emit signal_update_outLight(outLight);

    iLight = light_transform(outLight);
    lightCache.append(iLight);
    lightCache.pop_front();

    iTotalLight = 0;
    for(i =0; i < LIGHT_CACHE_DEPTH; ++i)
    {
        iTotalLight += lightCache.at(i);
    }
    iCurrentLight = qRound(iTotalLight*1.0/LIGHT_CACHE_DEPTH);
    emit signal_update_light(100-(iCurrentLight*100.0)/(MIN_INLIGHT_VALUE -MAX_INLIGHT_VALUE));

    // 数据有变化再刷新
    if (iLastLight != iCurrentLight)
    {
        driverLight->set_inLight(iCurrentLight);
        emit signal_update_inLight(iCurrentLight);
        iLastLight = iCurrentLight;
    }
}

/*******************************************************************************
* Function Name  :  light_transform
* Description    :  根据外界亮度计算此时合理设置亮度
* Input          :  QString outLight    外界亮度
* Output         :  None
* Return         :  应设置的亮度
*******************************************************************************/
int SysLight::light_transform(int outLight)
{
    int light;
    if(outLight>=MAX_OUTLIGHT_VALUE)
    {
        light = MAX_INLIGHT_VALUE;
    }
    else if(outLight<=MIN_OUTLIGHT_VALUE)
    {
        light = MIN_INLIGHT_VALUE;
    }
    else
    {
        // 当光线不为最大值也不为最小值时的自动调节

        // y = ax + b为计算公式
        //        light = -((MIN_INLIGHT_VALUE - MAX_INLIGHT_VALUE)*(outLight*1.0))/(MAX_OUTLIGHT_VALUE - MIN_OUTLIGHT_VALUE) + MIN_INLIGHT_VALUE;

        // y =a*lnx + b为计算公式
        //        double a= (COSY_INLIGHT_VALUE - MIN_INLIGHT_VALUE)/log((COSY_OUTLIGHT_VALUE - MIN_OUTLIGHT_VALUE));
        //        light = a*log(outLight - MIN_OUTLIGHT_VALUE) + MIN_INLIGHT_VALUE;

        // y = a*lnx + b/x +c为计算公式
        double a = (MIN_INLIGHT_VALUE - COSY_INLIGHT_VALUE)/(log(MIN_OUTLIGHT_VALUE) + (0.5*COSY_OUTLIGHT_VALUE/MIN_INLIGHT_VALUE) + 0.5+log(COSY_OUTLIGHT_VALUE));
        double b = a*COSY_OUTLIGHT_VALUE*0.5;
        double c = COSY_INLIGHT_VALUE - a*(log(COSY_OUTLIGHT_VALUE)+0.5);
        light = qRound(a*log(outLight) + b/outLight + c);

    }
    return light;
}

/*******************************************************************************
* Function Name  :  slot_set_offInterval
* Description    :  设置充电时的熄屏间隔
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::slot_offInterval_charging(IntervalType offInterval)
{
    // 若此时正在充电且定时器正在运行
    if(isCharging && timerCharging->isActive())
    {
        timerCharging->stop();
    }
    switch(offInterval)
    {
    case NEVER:
    {
        isOffScreenCharging = false;
        fileConfig->set_value(SCREEN_OFF_CHARGING, "never");
    }
        break;
    case MIN5:
    {
        isOffScreenCharging = true;
        timerCharging->setInterval(5 * 60 * 1000);
        fileConfig->set_value(SCREEN_OFF_CHARGING, "5");
    }
        break;
    case MIN15:
    {
        isOffScreenCharging = true;
        timerCharging->setInterval(15 * 60 * 1000);
        fileConfig->set_value(SCREEN_OFF_CHARGING, "15");
    }
        break;
    case MIN30:
    {
        isOffScreenCharging = true;
        timerCharging->setInterval(30 * 60 * 1000);
        fileConfig->set_value(SCREEN_OFF_CHARGING, "30");
    }
        break;
    default:
    {
        isOffScreenCharging = false;
        fileConfig->set_value(SCREEN_OFF_CHARGING, "never");
    }
        break;
    }

    // 若充电，且可熄屏
    if(isCharging && isOffScreenCharging)
    {
        timerCharging->start();
    }
}

/*******************************************************************************
* Function Name  :  slot_offInterval_discharging
* Description    :  设置未充电时的熄屏间隔
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::slot_offInterval_discharging(IntervalType offInterval)
{
    if(!isCharging && timerDischarging->isActive())
    {
        timerDischarging->stop();
    }

    switch(offInterval)
    {
    case NEVER:
    {
        isOffScreenDischarging = false;
        fileConfig->set_value(SCREEN_OFF_DISCHARGING, "never");
    }
        break;
    case MIN5:
    {
        isOffScreenDischarging = true;
        timerDischarging->setInterval(5 * 60 * 1000);
        fileConfig->set_value(SCREEN_OFF_DISCHARGING, "5");
    }
        break;
    case MIN15:
    {
        isOffScreenDischarging = true;
        timerDischarging->setInterval(15 * 60 * 1000);
        fileConfig->set_value(SCREEN_OFF_DISCHARGING, "15");
    }
        break;
    case MIN30:
    {
        isOffScreenDischarging = true;
        timerDischarging->setInterval(30 * 60 * 1000);
        fileConfig->set_value(SCREEN_OFF_DISCHARGING, "30");
    }
        break;
    default:
    {
        isOffScreenDischarging = false;
        fileConfig->set_value(SCREEN_OFF_DISCHARGING, "never");
    }
        break;
    }

    if(!isCharging && isOffScreenDischarging)
    {
        timerDischarging->start();
    }

}

/*******************************************************************************
* Function Name  :  slot_on_screen
* Description    :  开启屏幕
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::slot_screen_on()
{
    if(!driverLight->is_light_on())
    {
        driverLight->light_on();
        emit signal_update_screen(true);

        // 开启自动更新亮度
        if(isAutoAdjust && !timerSampling->isActive())
        {
            timerSampling->start();
        }
        else
        {
            driverLight->set_inLight(iCurrentLight);
        }
        if(isCharging)
        {
            if(isOffScreenCharging)
            {
                timerCharging->start();
            }
        }
        else
        {
            if(isOffScreenDischarging)
            {
                timerDischarging->start();
            }
        }

    }
}

/*******************************************************************************
* Function Name  :  slot_screen_off
* Description    :  熄灭屏幕
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::slot_screen_off()
{
    if(driverLight->is_light_on())
    {
        driverLight->light_off();
        emit signal_update_screen(false);
        if(timerSampling->isActive())
        {
            timerSampling->stop();
        }
        timerCharging->stop();
        timerDischarging->stop();
    }
}

/*******************************************************************************
* Function Name  :  get_autoAdjust
* Description    :  获取自动调节状态
* Input          :  None
* Output         :  None
* Return         :  自动调节状态
*******************************************************************************/
bool SysLight::get_autoAdjust()
{
    return isAutoAdjust;
}

/*******************************************************************************
* Function Name  :  slot_set_autoMode
* Description    :  设置自动调节状态
* Input          :  bool checked    自动调节状态
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysLight::slot_set_autoMode(bool checked)
{
    isAutoAdjust = checked;
    if(checked)
    {
        timerSampling->start();
        fileConfig->set_value(LIGHT_AUTO,"true");
    }
    else
    {
        timerSampling->stop();
        fileConfig->set_value(LIGHT_AUTO,"false");
    }

}

/*******************************************************************************
* Function Name  :  get_light
* Description    :  获取此时亮度
* Input          :  None
* Output         :  None
* Return         :  此时亮度
*******************************************************************************/
int SysLight::get_light()
{
    return iCurrentLight;
}

/*******************************************************************************
* Function Name  :  trans_interval
* Description    :  字符串转换为间距
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
int SysLight::trans_interval(QString min)
{
    return 0;
    int interval;
    if(min == "never")
    {
        interval = 0;
    }
    else
    {
        interval = 60*1000*(min.toInt());
    }
    return interval;
}
