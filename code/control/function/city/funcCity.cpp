#include "funcCity.h"

FuncCity *FuncCity::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FuncCity* FuncCity::getInstance()
{
    if(NULL == instance)
    {
        instance = new FuncCity();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  FuncCity
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FuncCity::FuncCity()
    :QThread()
{
    qRegisterMetaType<InfoCity>("InfoCity");
    qRegisterMetaType<LanguageType>("LanguageType");
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
void FuncCity::data_init()
{
    fileConfig  = FileConfig::getInstance();
    sysDevice   = SysDevice::getInstance();
    sysTime     = SysTime::getInstance();
    serverCity  = ServerCity::getInstance();
    wilddogCity = WilddogCity::getInstance();
    sysLanguage = SysLanguage::getInstance();

    listCity    = new QList<InfoCity>;
    isConnected = false;
    isCityAuto  = true;
    isManual    = false;

    // 初始化
    timerRetry = new QTimer();
    timerRetry->setInterval(RETRY_LOCATE_INTERVAL);
    timerRetry->setSingleShot(true);

    timerSetCity = new QTimer();
    timerSetCity->setInterval(60000);
    timerSetCity->setSingleShot(true);
    language = sysLanguage->get_languageType();

    // 获取MAC地址
    mac = fileConfig->get_value(DEVICE_MAC);
    isDeviceInit = sysDevice->is_deviceInit();

    // 若设备初始化
    if(isDeviceInit)
    {
        // 获取设备ID
        deviceID = fileConfig->get_value(DEVICE_ID);
    }

    // 若此时为手动设置城市状态
    if("false" == fileConfig->get_value(CITY_AUTO))
    {
        isCityAuto = false;
        cityManual.nameCN = fileConfig->get_value(CITY_NAME_SC);
        cityManual.nameEN = fileConfig->get_value(CITY_NAME_EN);
        cityManual.nameTW = fileConfig->get_value(CITY_NAME_TC);
        // 若此时语言为中文
        switch(language)
        {
        case LANG_CN:
            cityManual.name = cityManual.nameCN;
            break;
        case LANG_EN:
            cityManual.name = cityManual.nameEN;
            break;
        case LANG_TW:
            cityManual.name = cityManual.nameTW;
            break;
        default:
            cityManual.name = cityManual.nameCN;
            break;
        }
        cityManual.cityID = fileConfig->get_value(CITY_ID);
        cityManual.timeZone = fileConfig->get_value(TIME_ZONE);
    }
    else
    {
        isCityAuto = true;

        cityAuto.nameCN = fileConfig->get_value(CITY_NAME_SC);
        cityAuto.nameEN = fileConfig->get_value(CITY_NAME_EN);
        cityAuto.nameTW = fileConfig->get_value(CITY_NAME_TC);

        // 若此时语言为中文
        switch(language)
        {
        case LANG_CN:
            cityAuto.name = cityAuto.nameCN;
            break;
        case LANG_EN:
            cityAuto.name = cityAuto.nameEN;
            break;
        case LANG_TW:
            cityAuto.name = cityAuto.nameTW;
            break;
        default:
            cityAuto.name = cityAuto.nameCN;
            break;
        }
        cityAuto.cityID   = fileConfig->get_value(CITY_ID);
        cityAuto.timeZone = fileConfig->get_value(TIME_ZONE);
    }

    if(isCityAuto)
    {
        cityCurrent = cityAuto;
    }
    else
    {
        cityCurrent= cityManual;
    }

}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::connect_init()
{
    connect(sysDevice, SIGNAL(signal_update_infoDevice(InfoDevice)), this, SLOT(slot_update_infoDevice(InfoDevice)));
    connect(sysLanguage, SIGNAL(signal_change_language(LanguageType)), this, SLOT(slot_update_language(LanguageType)));
    connect(serverCity, SIGNAL(signal_locate_success(InfoCity)), this, SLOT(slot_locate_success(InfoCity)));
    connect(serverCity, SIGNAL(signal_locate_failed()), this, SLOT(slot_locate_failed()));
    connect(serverCity, SIGNAL(signal_searchCity_success(QList<InfoCity>*)), this, SLOT(slot_searchCity_success(QList<InfoCity>*)));
    connect(serverCity, SIGNAL(signal_searchCity_failed()), this, SLOT(slot_searchCity_failed()));
    connect(serverCity, SIGNAL(signal_setCity_success(InfoCity)), this, SLOT(slot_setCity_success(InfoCity)));
    connect(serverCity, SIGNAL(signal_setCity_failed()), this, SLOT(slot_setCity_failed()));
    connect(timerRetry, SIGNAL(timeout()), this, SLOT(slot_retry_locate()));
    connect(timerSetCity, SIGNAL(timeout()), this, SLOT(slot_retry_setCity()));

    connect(this, SIGNAL(signal_locate_device(QString,QString,LanguageType)), serverCity, SLOT(slot_locate_device(QString,QString,LanguageType)));
    connect(this, SIGNAL(signal_search_city(QString,LanguageType)), serverCity, SLOT(slot_search_city(QString,LanguageType)));
    connect(this, SIGNAL(signal_set_city(QString,QString)), serverCity, SLOT(slot_set_city(QString,QString)));

    connect(this, SIGNAL(signal_update_infoCity(InfoCity)), sysTime, SLOT(slot_update_infoCity(InfoCity)));

    // 同步此时的城市ID
    connect(this, SIGNAL(signal_update_cityID(QString)), wilddogCity, SLOT(slot_sync_city(QString)));
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::run()
{
    this->exec();
}

/*******************************************************************************
* Function Name  :  slot_net_connected
* Description    :  网络连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_net_connected()
{
    isConnected = true;

    // 若设备已初始化
    if(isDeviceInit)
    {
        // 发送定位信号
        emit signal_locate_device(deviceID, mac, language);
    }
}

/*******************************************************************************
* Function Name  :  slot_net_disconnect
* Description    :  网络断开
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_net_disconnect()
{
    // 设置标志位断开连接
    isConnected = false;
}

/*******************************************************************************
* Function Name  :  slot_deviceInit_success
* Description    :  设备初始化成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_update_infoDevice(InfoDevice infoDevice)
{
    // 初始化成功则开始定位
    isDeviceInit = true;
    this->deviceID = infoDevice.deviceID;

    if(isConnected)
    {
        emit signal_locate_device(deviceID, mac, language);
    }
}

/*******************************************************************************
* Function Name  :  slot_update_language
* Description    :  刷新语言
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_update_language(LanguageType type)
{
    this->language = type;

    switch(language)
    {
    case LANG_CN:
        cityCurrent.name = cityCurrent.nameCN;
        break;
    case LANG_EN:
        cityCurrent.name = cityCurrent.nameEN;
        break;
    case LANG_TW:
        cityCurrent.name = cityCurrent.nameTW;
        break;
    default:
        cityCurrent.name = cityCurrent.nameEN;
        break;
    }
    emit signal_locate_success(cityCurrent.name);
}

/*******************************************************************************
* Function Name  :  slot_reply_locate
* Description    :  返回定位信息
* Input          :  City city 定位获得的此时城市
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_locate_success(InfoCity city)
{
    qDebug()<<"Locate Success!!!";

    // 获取定位之后的城市名和城市ID并存储
    cityAuto = city;

    // 收到定位反馈信息，更新页面显示信息
    emit signal_locate_success(city.name);
    qDebug()<<"LOCATE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<city.name;

    // 若此时收到了时间信息，发送时间同步信号
    if(city.time !="")
    {
        emit signal_sync_time(city.timeZone,city.time);
    }

    // 若现在为自动定位
    if(isCityAuto && (cityCurrent.cityID != cityAuto.cityID))
    {
        cityCurrent = cityAuto;
        // 更新本地配置文件中存储城市名以及城市ID
        fileConfig->set_value(CITY_NAME_SC, cityCurrent.nameCN);
        fileConfig->set_value(CITY_NAME_EN, cityCurrent.nameEN);
        fileConfig->set_value(CITY_NAME_TC, cityCurrent.nameTW);
        fileConfig->set_value(CITY_ID,      cityCurrent.cityID);

        // 广播城市变化
        emit signal_update_cityID(cityCurrent.cityID);
        emit signal_update_cityName(cityCurrent.name);
        emit signal_update_infoCity(cityCurrent);

        if(isConnected)
        {
            // 设置当前城市，为手动输入
            emit signal_set_city(deviceID, city.cityID);
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_reply_locate
* Description    :  定位失败后的返回信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_locate_failed()
{
    qDebug()<<"Locate Failed!!!";

    if(isManual)
    {
        if(isConnected)
        {
            emit signal_net_abnormal();
        }
        else
        {
            emit signal_net_disconnect();
        }
        isManual = false;
    }

    // 收到定位反馈信息，更新页面显示信息
    if(isConnected)
    {
        timerRetry->start();
    }
}

/*******************************************************************************
* Function Name  :  slot_search_city
* Description    :  搜索城市
* Input          :  QString name 模糊城市名
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_search_city(QString name)
{
    // 联网情况下发送信号，搜索城市
    if(isConnected)
    {
        // 以城市名为参数，模糊检索城市列表
        emit signal_search_city(name,language);
    }
    else
    {
        // 直接返回没网状态
        emit signal_net_disconnect();
    }

}

/*******************************************************************************
* Function Name  :  slot_reply_searchCity
* Description    :  返回定位信息
* Input          :  QList<City> listCity 搜索到的城市列表
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_searchCity_success(QList<InfoCity> *listCity)
{
    // 临时存储此时城市列表
    this->listCity = listCity;

    // 将服务器返回的城市列表发至界面显示
    emit signal_searchCity_success(listCity);
}

/*******************************************************************************
* Function Name  :  slot_reply_searchCity
* Description    :  搜索城市因为网络原因失败后
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_searchCity_failed()
{
    if(isConnected)
    {
        // 发送网络异常
        emit signal_net_abnormal();
    }
    else
    {
        // 发送网络不可连
        emit signal_net_disconnect();
    }
}

/*******************************************************************************
* Function Name  :  set_city
* Description    :  设置城市，改为自动设置则，根据网络定位设置当前城市
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_set_cityManual(QString name)
{
    isCityAuto = false;
    if(name != cityManual.name)
    {
        for(int i = 0; i < listCity->length(); ++i)
        {
            if(listCity->at(i).name == name)
            {
                cityManual = listCity->at(i);
                break;
            }
        }

        if(isConnected && isDeviceInit)
        {
            // 设置当前城市，为手动输入
            emit signal_set_city(deviceID, cityManual.cityID);
        }
    }

    cityCurrent = cityManual;

    // 写入配置文件
    fileConfig->set_value(CITY_AUTO, "false");
    fileConfig->set_value(CITY_NAME_SC, cityCurrent.nameCN);
    fileConfig->set_value(CITY_NAME_EN, cityCurrent.nameEN);
    fileConfig->set_value(CITY_NAME_TC, cityCurrent.nameTW);
    fileConfig->set_value(CITY_ID, cityCurrent.cityID);

    // 广播城市变化
    emit signal_update_cityID(cityCurrent.cityID);
    emit signal_update_cityName(cityCurrent.name);
    emit signal_update_infoCity(cityCurrent);
}

/*******************************************************************************
* Function Name  :  slot_set_cityAuto
* Description    :  设置自动获取城市
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_set_cityAuto()
{
    isCityAuto = true;
    isManual = true;

    cityCurrent = cityAuto;

    // 写入配置文件
    fileConfig->set_value(CITY_AUTO, "true");
    fileConfig->set_value(CITY_NAME_SC, cityCurrent.nameCN);
    fileConfig->set_value(CITY_NAME_EN, cityCurrent.nameEN);
    fileConfig->set_value(CITY_NAME_TC, cityCurrent.nameTW);
    fileConfig->set_value(CITY_ID, cityCurrent.cityID);

    if(isConnected && isDeviceInit)
    {
        // 开始定位
        emit signal_locate_device(deviceID, mac, language);
    }
    else if(!isConnected)
    {
        emit signal_net_disconnect();
    }
    else
    {
        emit signal_net_abnormal();
    }

    // 广播城市变化
    emit signal_update_cityID(cityCurrent.cityID);
    emit signal_update_cityName(cityCurrent.name);
    emit signal_update_infoCity(cityCurrent);
}

/*******************************************************************************
* Function Name  :  slot_reply_setCity
* Description    :  返回定位信息
* Input          :  City city 设置城市返回的城市信息
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_setCity_success(InfoCity city)
{
    qDebug()<<"set City Success!!!!!!!!!!!!!!!!!!!!!!!!!!";
}

/*******************************************************************************
* Function Name  :  slot_reply_setCity
* Description    :  设置城市失败后
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_setCity_failed()
{
    timerSetCity->start();
}

/*******************************************************************************
* Function Name  :  slot_retry_setCity
* Description    :  重新设置城市
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_retry_setCity()
{
    if(isConnected && isDeviceInit)
    {
        // 设置城市
        emit signal_set_city(deviceID, cityCurrent.cityID);
    }
}

/*******************************************************************************
* Function Name  :  slot_retry_locate
* Description    :  定位城市失败后
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncCity::slot_retry_locate()
{
    if(isDeviceInit && isConnected)
    {
        emit signal_locate_device(deviceID, mac, language);
    }
}

/*******************************************************************************
* Function Name  :  get_currentCity
* Description    :  获取城市
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
InfoCity FuncCity::get_currentCity()
{
    return cityCurrent;
}

/*******************************************************************************
* Function Name  :  get_locationManual
* Description    :  获取此时的手动城市名
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FuncCity::get_locationManual()
{
    return cityManual.name;
}

/*******************************************************************************
* Function Name  :  get_locationAuto
* Description    :  获取此时的手动城市名
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FuncCity::get_locationAuto()
{
    return cityAuto.name;
}

/*******************************************************************************
* Function Name  :  get_location
* Description    :  获取此时设置城市名
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString FuncCity::get_location()
{
    return cityCurrent.name;
}

/*******************************************************************************
* Function Name  :  get_locationMode()
* Description    :  获取此时城市设置状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool FuncCity::get_locationMode()
{
    return isCityAuto;
}
