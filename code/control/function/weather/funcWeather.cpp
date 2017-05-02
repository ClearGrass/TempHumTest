#include "funcWeather.h"

FuncWeather *FuncWeather::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FuncWeather* FuncWeather::getInstance()
{
    if(NULL == instance)
    {
        instance = new FuncWeather();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  FuncWeather
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FuncWeather::FuncWeather(QObject *parent)
    : QThread(parent)
{
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
void FuncWeather::data_init()
{
    timerKeep        = new QTimer();
    timerKeep->setInterval(WEATHER_KEEP_DURATION);
//    timerKeep->setInterval(10000);
    timerKeep->setSingleShot(true);

    timerObserve    = new QTimer();
    timerObserve->setInterval(10000);
    timerObserve->setSingleShot(true);

    // 声明接口
    sysDevice         = SysDevice::getInstance();
    sysData           = SysData::getInstance();
    sysTime           = SysTime::getInstance();
    fileConfig        = FileConfig::getInstance();
    wilddogWeather    = WilddogWeather::getInstance();

    serverWeather     = ServerWeather::getInstance();
    listWeatherDaily  = new QList<WeatherDaily>();
    listWeatherHourly = new QList<WeatherHourly>();
    currentWeather.set_invalid();

    isConnected  = false;
    isObserving  = false;
    isDeviceInit = sysDevice->is_deviceInit();
    unitTEMP    = sysData->get_unitTEMP();

    if(isDeviceInit)
    {
        deviceID = fileConfig->get_value(DEVICE_ID);
        cityID   = fileConfig->get_value(CITY_ID);
    }
}

/*******************************************************************************
* Function Name  :  slot_request_weather
* Description    :  请求天气数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_request_weather()
{
    qDebug()<<"REQUEST WEATHER!!!!!!!!!!!!!!!!";
    if(isObserving)
    {
        // 若网络已连接
        if(isConnected)
        {
            if(isDeviceInit)
            {
                // 请求天气数据
                emit signal_request_weather(deviceID, cityID);
            }
            else
            {
                // 若此时设备没有初始化
                emit signal_net_abnormal();
            }
        }
        else
        {
            // 若此时的缓存都不为空
            if(!listWeatherDaily->isEmpty() && !listWeatherHourly->isEmpty())
            {

                // 检查上次请求天气数据是否在今天
                if(QDate::currentDate() == lastDate)
                {
                    // 返回上次的请求数据
                    emit signal_update_weatherDaily(listWeatherDaily);
                    if(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh") == lastDateTime.toString("yyyy-MM-dd-hh"))
                    {
                        // 返回上一次数据
                        emit signal_update_weatherHourly(listWeatherHourly);
                    }
                }
            }
            else
            {
                // 返回无网络连接
                emit signal_net_disconnect();
            }
        }
    }
    else
    {
        emit signal_net_disconnect();
    }
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::connect_init()
{
    // 天气改变后发送信号
    connect(this, SIGNAL(signal_request_weather(QString,QString)), serverWeather, SLOT(slot_request_weather(QString,QString)));
    connect(serverWeather, SIGNAL(signal_getWeather7Days_success(QList<WeatherDaily>*)), this, SLOT(slot_update_weatherDaily(QList<WeatherDaily>*)));
    connect(serverWeather, SIGNAL(signal_getWeather12Hours_success(QList<WeatherHourly>*)), this, SLOT(slot_update_weatherHourly(QList<WeatherHourly>*)));
    connect(serverWeather, SIGNAL(signal_getWeather7Days_failed()), this, SLOT(slot_getWeather_failed()));
    connect(serverWeather, SIGNAL(signal_getWeather12Hours_failed()), this, SLOT(slot_getWeather_failed()));

    connect(wilddogWeather, SIGNAL(signal_update_weather(InfoWeather)), this, SLOT(slot_update_weather(InfoWeather)));
    connect(this, SIGNAL(signal_update_cityID(QString)), wilddogWeather, SLOT(slot_update_cityID(QString)));
    connect(sysData, SIGNAL(signal_change_unitTEMP(UnitType)), this, SLOT(slot_update_unitTEMP(UnitType)));
    connect(sysDevice, SIGNAL(signal_update_infoDevice(InfoDevice)), this, SLOT(slot_update_infoDevice(InfoDevice)));
    connect(sysTime, SIGNAL(signal_update_time(TimeType)), this, SLOT(slot_update_time(TimeType)));
    connect(timerKeep, SIGNAL(timeout()), this, SLOT(slot_weather_invalid()));
    connect(timerObserve, SIGNAL(timeout()), this, SLOT(slot_observe_failed()));
}

/*******************************************************************************
* Function Name  :  slot_update_weatherDaily
* Description    :  刷新每天天气
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_update_weatherDaily(QList<WeatherDaily> *listWeatherDaily)
{
    int i;
    WeatherDaily weatherDaily;
    lastDate = QDate::currentDate();
    this->listWeatherDaily->clear();
    for(i = 0; i < listWeatherDaily->length(); ++i)
    {
        weatherDaily = listWeatherDaily->at(i);

        if(unitTEMP == KFC)
        {
            weatherDaily.tempMax = 1.8*listWeatherDaily->at(i).tempMax + 32;
            weatherDaily.tempMin = 1.8*listWeatherDaily->at(i).tempMin + 32;
        }
        else
        {
            weatherDaily.tempMax = listWeatherDaily->at(i).tempMax;
            weatherDaily.tempMin = listWeatherDaily->at(i).tempMin;
        }
        this->listWeatherDaily->append(weatherDaily);
    }

    emit signal_update_weatherDaily(this->listWeatherDaily);
}

/*******************************************************************************
* Function Name  :  slot_update_weatherHourly
* Description    :  刷新每小时天气
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_update_weatherHourly(QList<WeatherHourly> *listWeatherHourly)
{
    int i;
    WeatherHourly weatherHourly;
    lastDateTime = QDateTime::currentDateTime();
    this->listWeatherHourly->clear();
    for(i = 0; i < listWeatherHourly->length(); ++i)
    {
        weatherHourly = listWeatherHourly->at(i);

        if(unitTEMP == KFC)
        {
            weatherHourly.temp = 1.8*listWeatherHourly->at(i).temp + 32;
        }
        else
        {
            weatherHourly.temp = listWeatherHourly->at(i).temp;
        }
        this->listWeatherHourly->append(weatherHourly);
    }
    emit signal_update_weatherHourly(this->listWeatherHourly);
}

/*******************************************************************************
* Function Name  :  slot_getWeather_failed
* Description    :  获取天气失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_getWeather_failed()
{
    // 若此时的缓存都不为空
    if(!listWeatherDaily->isEmpty() && !listWeatherHourly->isEmpty())
    {
        // 检查上次请求天气数据是否在今天
        if(QDate::currentDate() == lastDate)
        {
            // 返回上次的请求数据
            emit signal_update_weatherDaily(listWeatherDaily);
            if(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh") == lastDateTime.toString("yyyy-MM-dd-hh"))
            {
                // 返回上一次数据
                emit signal_update_weatherHourly(listWeatherHourly);
            }
        }
    }
    else
    {
        if(isConnected)
        {
            // 返回无网络连接
            emit signal_net_abnormal();
        }
        else
        {
            emit signal_net_disconnect();
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_update_cityID
* Description    :  刷新城市ID
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_update_cityID(QString cityID)
{
    // 更换城市
    emit signal_update_cityID(cityID);
    this->cityID = cityID;

    // 若城市改变，清空原有天气数据
    this->currentWeather.set_invalid();
    this->listWeatherDaily->clear();
    this->listWeatherHourly->clear();

    if(isConnected)
    {
        emit signal_sampling_weather();
        timerObserve->start();
        emit signal_update_cityID(cityID);
    }
    else
    {
        // 清空此时天气信息
        emit signal_update_weather(currentWeather);
        isObserving = false;
    }
}

/*******************************************************************************
* Function Name  :  slot_update_weather
* Description    :  更新当前天气
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_update_weather(InfoWeather weather)
{
    isObserving = true;
    timerKeep->stop();
    timerObserve->stop();

    currentWeather = weather;

    if((unitTEMP == KFC) && (weather.temp != ERROR_DATA))
    {
        currentWeather.temp = 32 + 1.8*weather.temp;
    }

    // 发送最新天气信息
    emit signal_update_weather(currentWeather);
}

/*******************************************************************************
* Function Name  :  slot_observe_failed
* Description    :  监听天气数据失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_observe_failed()
{
    // 此时天气数据有效
    if(currentWeather.is_valid())
    {
        isObserving = true;
    }
    else
    {
        isObserving = false;
    }
    emit signal_update_weather(currentWeather);
}

/*******************************************************************************
* Function Name  :  slot_net_connected
* Description    :  网络连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_net_connected()
{
    isConnected = true;
    emit signal_sampling_weather();
    timerObserve->start();
}

/*******************************************************************************
* Function Name  :  slot_net_disconnect
* Description    :  网络未连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_net_disconnect()
{
    qDebug()<<"WEATHER DISCONNECT>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    isConnected = false;
    timerKeep->start();
}

/*******************************************************************************
* Function Name  :  slot_weather_invalid
* Description    :  天气失效
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_weather_invalid()
{
    this->currentWeather.set_invalid();
    this->listWeatherDaily->clear();
    this->listWeatherHourly->clear();
    emit signal_update_weather(currentWeather);
    isObserving = false;
}

/*******************************************************************************
* Function Name  :  slot_update_time
* Description    :  刷新时间
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_update_time(TimeType timeType)
{
    if((HOURLY == timeType) || (DAILY == timeType))
    {
        if(timerKeep->isActive())
        {
            timerKeep->stop();

            currentWeather.set_invalid();
            listWeatherDaily->clear();
            listWeatherHourly->clear();
            isObserving = false;
            emit signal_update_weather(currentWeather);
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_update_infoDevice
* Description    :  刷新此时的设备信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_update_infoDevice(InfoDevice infoDevice)
{
    this->deviceID = infoDevice.deviceID;
    isDeviceInit = true;
}

/*******************************************************************************
* Function Name  :  slot_update_unitTEMP
* Description    :  刷新温度单位
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncWeather::slot_update_unitTEMP(UnitType unit)
{
    if(unitTEMP == unit)
    {
        return;
    }
    this->unitTEMP = unit;
    if(currentWeather.is_valid())
    {
        if((unitTEMP == KFC))
        {
            currentWeather.temp = 32 + 1.8*currentWeather.temp;
        }
        else
        {
            currentWeather.temp = (currentWeather.temp - 32)/1.8;
        }

        // 发送最新天气信息
        emit signal_update_weather(currentWeather);
    }

    QMutableListIterator<WeatherDaily> i(*listWeatherDaily);
    QMutableListIterator<WeatherHourly> j(*listWeatherHourly);
    WeatherDaily weatherDaily;
    WeatherHourly weatherHourly;
    if(!listWeatherDaily->isEmpty())
    {
        while(i.hasNext())
        {
            weatherDaily = i.next();
            if(unitTEMP == KFC)
            {
                weatherDaily.tempMax = 1.8*weatherDaily.tempMax + 32;
                weatherDaily.tempMin = 1.8*weatherDaily.tempMin + 32;
            }
            else
            {
                weatherDaily.tempMax = (weatherDaily.tempMax - 32)/1.8;
                weatherDaily.tempMin = (weatherDaily.tempMin - 32)/1.8;
            }
            i.setValue(weatherDaily);
        }
    }

    if(!listWeatherHourly->isEmpty())
    {
        while(j.hasNext())
        {
            weatherHourly = j.next();
            if(unitTEMP == KFC)
            {
                weatherHourly.temp = 1.8*weatherHourly.temp + 32;
            }
            else
            {
                weatherHourly.temp = (weatherHourly.temp - 32)/1.8;
            }
            j.setValue(weatherHourly);
        }
    }
}
