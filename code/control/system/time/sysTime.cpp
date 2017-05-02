#include "sysTime.h"

SysTime *SysTime::instance = 0;

/*******************************************************************************
* Function Name :   getInstance
* Description   :   获取系统时间类 单例实例
* Input         :   None
* Output        :   None
* Return        :   返回单例 对象实例
*******************************************************************************/
SysTime* SysTime::getInstance(void)
{
    // 是否被创建过
    if(instance == 0)
    {
        instance = new SysTime();
    }
    return instance;

}// End of SysTime* SysTime::getInstance()

/*******************************************************************************
* Function Name  :  SysTime
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysTime::SysTime(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<TimeType>("TimeType");
    qRegisterMetaType<InfoNetTime>("InfoNetTime");
    data_init();
    connect_init();
//    slot_update_time();
    moveToThread(this);
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::run()
{
//    if(isTimeInit)
//    {
//        timerUpdate->start();
//    }
    this->exec();
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::data_init()
{
    isTimeInit  = false;
    isSunInit   = false;
    isTimeAuto  = true;
    isNightAuto = true;
    isManual    = false;
    isNight     = false;
    nightType   = MODEL_MANUAL;

    // 获取时区列表
    driverTime   = DriverTime::getInstance();
    sysLanguage  = SysLanguage::getInstance();
    sysDevice    = SysDevice::getInstance();
    serverTime   = ServerTime::getInstance();
    fileConfig   = FileConfig::getInstance();
    fileTimeZone = FileTimeZone::getInstance();

    timerUpdate  = new QTimer();
    timerUpdate->setInterval(1000);

    timerSync    = new QTimer();
    timerSync->setInterval(1000*60*60*6);

    language     = sysLanguage->get_languageType();
    listTimeZone = fileTimeZone->get_listTimeZone(language);
    timeZone     = fileConfig->get_value(TIME_ZONE);

    isDeviceInit = sysDevice->is_deviceInit();

    if(isDeviceInit)
    {
        deviceID = fileConfig->get_value(DEVICE_ID);
        cityID   = fileConfig->get_value(CITY_ID);
    }

    if("true" == fileConfig->get_value(TIME_INIT))
    {
        isTimeInit = true;
    }

    if("true" == fileConfig->get_value(SUN_INIT))
    {
        isSunInit = true;
    }

    if("false" == fileConfig->get_value(NIGHT_AUTO))
    {
        isNightAuto = false;
    }

    if("false" == fileConfig->get_value(TIME_AUTO))
    {
        isTimeAuto = false;
    }

    if("mode_manual" == fileConfig->get_value(NIGHT_MODE))
    {
        nightType = MODEL_MANUAL;
        timeStart = QTime::fromString(fileConfig->get_value(NIGHT_START_TIME), "h:mm");
        timeEnd   = QTime::fromString(fileConfig->get_value(NIGHT_END_TIME), "h:mm");
    }
    else
    {
        nightType = MODEL_AUTO;
        timeStart = QTime::fromString(fileConfig->get_value(SUNSET_TIME), "h:mm");
        timeEnd   = QTime::fromString(fileConfig->get_value(SUNRISE_TIME), "h:mm");
    }

    for(int i = 0; i < listTimeZone->length(); ++i)
    {
        if(listTimeZone->at(i).name == timeZone)
        {
            timeZoneGmt = listTimeZone->at(i).gmt;
        }
    }

    // 同步系统时间
    driverTime->set_timeZone(timeZoneGmt);
    driverTime->sync_rtc();
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::connect_init()
{
    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(slot_update_time()));
    connect(sysLanguage, SIGNAL(signal_change_language(LanguageType)), this, SLOT(slot_update_language(LanguageType)));
    connect(this, SIGNAL(signal_sync_netTime(QString,QString)), serverTime, SLOT(slot_sync_netTime(QString,QString)));
    connect(serverTime, SIGNAL(signal_syncTime_success(InfoNetTime)), this, SLOT(slot_syncTime_success(InfoNetTime)));
    connect(serverTime, SIGNAL(signal_syncTime_failed()), this, SLOT(slot_syncTime_failed()));
    connect(timerSync, SIGNAL(timeout()), this, SLOT(slot_sync_netTime()));
}

/*******************************************************************************
* Function Name  :  slot_update_language
* Description    :  刷新此时的语言
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_update_language(LanguageType type)
{
    language = type;
    listTimeZone = fileTimeZone->get_listTimeZone(language);
}

/*******************************************************************************
* Function Name  :  slot_update_time
* Description    :  刷新时间
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_update_time()
{
    // 获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();
    int hour    = currentTime.toString("H").toInt();
    int minute  = currentTime.toString("m").toInt();
    int second  = currentTime.toString("s").toInt();

    if(isNightAuto)
    {
        // 判断是否入夜
        if(isNight)
        {
            // 判断此时的时间是否在时间断中
            if(!is_night(currentTime))
            {
                // 若无需忽略
                if(!isManual)
                {
                    isNight = false;
                    emit signal_set_night(false);
                }
            }
            else
            {
                if(isManual)
                {
                    isManual = false;
                }
            }
        }
        else
        {
            // 现在属于夜间段
            if(is_night(currentTime))
            {
                if(!isManual)
                {
                    isNight = true;
                    emit signal_set_night(true);
                }
            }
            else
            {
                if(isManual)
                {
                    isManual = false;
                }
            }
        }
    }

    // 若为整点，则报时，储存上一次所取值
    if(second == 0)
    {
        if(minute == 0)
        {
            driverTime->sync_rtc();

            // 判断是否为零点
            if(hour == 0)
            {
                // 发送零点信号
                emit signal_update_time(DAILY);
            }
            else
            {
                // 发送整点信号
                emit signal_update_time(HOURLY);
            }
        }
        else
        {
            emit signal_update_time(MINUTELY);
        }
    }
}

/*******************************************************************************
* Function Name  :  is_night
* Description    :  判断是否为夜晚
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysTime::is_night(QDateTime dateTime)
{
    QTime currentTime = QTime::fromString(dateTime.toString("hh:mm"),"hh:mm");

    if(timeStart == timeEnd)
    {
        return false;
    }
    if(timeStart < timeEnd)
    {
        if(currentTime >= timeStart && currentTime < timeEnd)
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
        if((currentTime >= timeStart) || (currentTime < timeEnd))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_set_night
* Description    :  改变此时是否为夜间模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_set_night(bool isEnter)
{
    if(isEnter)
    {
        // 若现在未入夜
        if(!isNight)
        {
            // 设置入夜
            isNight  = true;
            isManual = true;
            emit signal_set_night(true);
        }
    }
    else
    {
        // 若现在已入夜
        if(isNight)
        {
            // 结束夜间模式
            isNight = false;
            isManual = true;
            emit signal_set_night(false);
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_set_night
* Description    :  设置夜间模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_set_nightPeriod(QString time,bool isStartTime)
{
    if(MODEL_MANUAL == nightType)
    {
        if(isStartTime)
        {
            timeStart = QTime::fromString(time, "h:mm");

        }
        else
        {
            timeEnd   = QTime::fromString(time, "h:mm");
        }
    }
    if(isStartTime)
    {
        fileConfig->set_value(NIGHT_START_TIME, time);

    }
    else
    {
        fileConfig->set_value(NIGHT_END_TIME, time);
    }
}

/*******************************************************************************
* Function Name  :  slot_set_nightAuto
* Description    :  设置夜间模式自动开关
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_set_nightAuto(bool isAuto)
{
    isNightAuto = isAuto;
    if(isAuto)
    {
        fileConfig->set_value(NIGHT_AUTO, "true");
    }
    else
    {
        fileConfig->set_value(NIGHT_AUTO, "false");
    }
}

/*******************************************************************************
* Function Name  :  slot_set_nightMode
* Description    :  设置夜间模式模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_set_nightMode(QString mode)
{
    if("mode_sun" == mode)
    {
        nightType = MODEL_AUTO;
        timeStart = QTime::fromString(fileConfig->get_value(SUNSET_TIME), "h:mm");
        timeEnd   = QTime::fromString(fileConfig->get_value(SUNRISE_TIME), "h:mm");
    }
    else
    {
        nightType = MODEL_MANUAL;
        timeStart = QTime::fromString(fileConfig->get_value(NIGHT_START_TIME), "h:mm");
        timeEnd   = QTime::fromString(fileConfig->get_value(NIGHT_END_TIME), "h:mm");
    }
    fileConfig->set_value(NIGHT_MODE, mode);
}

/*******************************************************************************
* Function Name :   slot_set_time
* Description   :   设置当前时间
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysTime::slot_set_time(QString time)
{
    if(time == "")
    {
        return;
    }
    driverTime->set_dateTime(time);
    driverTime->set_rtc();

    if(!isTimeInit)
    {
        isTimeInit = true;
        timerUpdate->start();
        fileConfig->set_value(TIME_INIT, "true");
        emit signal_timeInit_success();

    }
}

/*******************************************************************************
* Function Name :   slot_set_timeZone
* Description   :   设置当前时区
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysTime::slot_set_timeZone(QString gmt)
{
    driverTime->set_timeZone(gmt);
    driverTime->set_rtc();
}

/*******************************************************************************
* Function Name  :  slot_set_timeAuto
* Description    :  设置时间模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_set_timeAuto(bool isAuto)
{
    this->isTimeAuto = isAuto;
    if(isAuto)
    {
        if(isConnected && isDeviceInit && (cityID != ""))
        {
            emit signal_sync_netTime(deviceID, cityID);
        }
        fileConfig->set_value(TIME_AUTO, "true");
    }
    else
    {
        fileConfig->set_value(TIME_AUTO, "false");
    }
}

/*******************************************************************************
* Function Name  :  slot_sync_netTime
* Description    :  同步时间
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_sync_netTime()
{
    if(isConnected && isDeviceInit && (cityID != ""))
    {
        emit signal_sync_netTime(deviceID, cityID);
    }
}

/*******************************************************************************
* Function Name :   slot_syncTime_success
* Description   :   同步网络时间
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void SysTime::slot_syncTime_success(InfoNetTime infoTime)
{
    if(isTimeAuto)
    {
        QString dateTime = QDateTime::fromTime_t(infoTime.timeStamp).toString("yyyy-MM-dd hh:mm:ss");
        driverTime->set_dateTime(dateTime);
        driverTime->set_rtc();

        if(!isTimeInit)
        {
            isTimeInit = true;
            timerUpdate->start();
            fileConfig->set_value(TIME_INIT, "true");
            emit signal_timeInit_success();
        }
    }

    if(MODEL_AUTO == nightType)
    {
        timeStart = QTime::fromString(infoTime.sunset, "h:mm");
        timeEnd   = QTime::fromString(infoTime.sunrise, "h:mm");
    }
    fileConfig->set_value(SUNRISE_TIME, infoTime.sunrise);
    fileConfig->set_value(SUNSET_TIME, infoTime.sunset);
    fileConfig->set_value(SUN_INIT, "true");
}

/*******************************************************************************
* Function Name  :  slot_syncTime_failed
* Description    :  同步时间失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_syncTime_failed()
{
}

/*******************************************************************************
* Function Name  :  slot_net_connected
* Description    :  网络连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_net_connected()
{
    isConnected = true;
    timerSync->start();

    // 若设备已初始化，且城市不为空
    if(isDeviceInit && (cityID != ""))
    {
        emit signal_sync_netTime(deviceID, cityID);
    }
}

/*******************************************************************************
* Function Name  :  slot_net_disconnect
* Description    :  网络断开连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_net_disconnect()
{
    isConnected = false;
    timerSync->stop();
}

/*******************************************************************************
* Function Name  :  slot_update_infoCity
* Description    :  刷新城市信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_update_infoCity(InfoCity infoCity)
{
    cityID = infoCity.cityID;

    fileConfig->set_value(SUN_INIT, "false");
    timeZoneAuto = infoCity.timeZone;

    // 若此时时间为自动获取的，则设定时区
    if(isTimeAuto)
    {
        timeZone = timeZoneAuto;
        fileConfig->set_value(TIME_ZONE,timeZoneAuto);

        for(int i = 0; i < listTimeZone->length(); ++i)
        {
            if(listTimeZone->at(i).name == timeZone)
            {
                timeZoneGmt = listTimeZone->at(i).gmt;
            }
        }
        driverTime->set_timeZone(timeZoneGmt);
        driverTime->set_rtc();
    }

    // 若此设备已初始化
    if(isDeviceInit && isConnected)
    {
        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$SYNC TIME"<<cityID;
        emit signal_sync_netTime(deviceID, cityID);
    }
}

/*******************************************************************************
* Function Name  :  slot_update_infoDevice
* Description    :  刷新设备信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysTime::slot_update_infoDevice(InfoDevice infoDevice)
{
    deviceID = infoDevice.deviceID;
    isDeviceInit = true;

    if(cityID != "")
    {
        emit signal_sync_netTime(deviceID, cityID);
        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$SYNC TIME"<<deviceID;
    }
}

/*******************************************************************************
* Function Name  :  get_timeZone
* Description    :  获取当前时区
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString SysTime::get_timeZone()
{
    return timeZone;
}

/*******************************************************************************
* Function Name  :  get_listTimeZone
* Description    :  获取时区列表
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QList<TimeZone> *SysTime::get_listTimeZone()
{
    return listTimeZone;
}

/*******************************************************************************
* Function Name  :  get_timeZoneAuto
* Description    :  获取当前时区
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString SysTime::get_timeZoneAuto()
{
    return timeZoneAuto;
}

/*******************************************************************************
* Function Name  :  get_night
* Description    :  获取此时是否为夜间模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysTime::get_night()
{
    return isTimeInit ? isNight : false;
}
