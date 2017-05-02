#include "wilddogControl.h"

WilddogControl *WilddogControl::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogControl *WilddogControl::getInstance()
{
    if(NULL == instance)
    {
        instance = new WilddogControl();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  WilddogControl
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogControl::WilddogControl()
    : QThread()
{   
    qRegisterMetaType<InfoWeather>("InfoWeather");
    qRegisterMetaType<AirData>("AirData");
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
void WilddogControl::data_init()
{
    fileConfig     = FileConfig::getInstance();
    sysDevice      = SysDevice::getInstance();

    wilddogWeather = WilddogWeather::getInstance();
    wilddogAirData = WilddogAirData::getInstance();
    wilddogStatus = WilddogStatus::getInstance();
    wilddogCity = WilddogCity::getInstance();
    wilddogDevice = WilddogDevice::getInstance();
    wilddogPower = WilddogPower::getInstance();
    wilddogApp  = WilddogApp::getInstance();

    wilddogWeather->moveToThread(this);
    wilddogAirData->moveToThread(this);
    wilddogApp->moveToThread(this);
    wilddogStatus->moveToThread(this);
    wilddogCity->moveToThread(this);
    wilddogDevice->moveToThread(this);
    wilddogPower->moveToThread(this);

    timerSync      = new QTimer(this);
    timerSync->setInterval(100);

    timerAuth     = new QTimer(this);
    timerAuth->setInterval(AUTH_RETRY_INTERVAL);
    timerAuth->setSingleShot(true);

    num = 0;

    isDeviceOnline = false;
    isAuth        = false;
    isConnected   = false;
    isWilddogInit = false;
    isTokenValid  = false;

    deviceSN = fileConfig->get_value(DEVICE_SN);

    isTokenValid  = sysDevice->is_tokenValid();
    if(isTokenValid)
    {
        token     = fileConfig->get_value(TOKEN);
        wilddog_init(deviceSN);
    }
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogControl::connect_init()
{
    connect(this, SIGNAL(signal_token_abnormal()), sysDevice, SLOT(slot_token_abnormal()));
    connect(sysDevice, SIGNAL(signal_update_infoDevice(InfoDevice)), this, SLOT(slot_update_infoDevice(InfoDevice)));
    connect(sysDevice, SIGNAL(signal_update_token(QString)), this, SLOT(slot_update_token(QString)));
    connect(timerSync, SIGNAL(timeout()), this, SLOT(slot_wilddog_sync()));
    connect(wilddogWeather, SIGNAL(signal_token_abnormal()), this, SLOT(slot_token_abnormal()));
    connect(wilddogAirData, SIGNAL(signal_token_abnormal()), this, SLOT(slot_token_abnormal()));
    connect(wilddogAirData, SIGNAL(signal_net_abnormal()), this, SLOT(slot_net_abnormal()));
    connect(timerAuth, SIGNAL(timeout()), this, SLOT(slot_retry_auth()));
    connect(wilddogStatus, SIGNAL(signal_client_online(bool)), wilddogAirData, SLOT(slot_client_online(bool)));

    connect(this, SIGNAL(signal_device_online(bool)), wilddogAirData, SLOT(slot_device_online(bool)));
    connect(this, SIGNAL(signal_device_online(bool)), wilddogApp, SLOT(slot_device_online(bool)));
    connect(this, SIGNAL(signal_device_online(bool)), wilddogCity, SLOT(slot_device_online(bool)));
    connect(this, SIGNAL(signal_device_online(bool)), wilddogDevice, SLOT(slot_device_online(bool)));
    connect(this, SIGNAL(signal_device_online(bool)), wilddogPower, SLOT(slot_device_online(bool)));
    connect(this, SIGNAL(signal_device_online(bool)), wilddogStatus, SLOT(slot_device_online(bool)));
    connect(this, SIGNAL(signal_device_online(bool)), wilddogWeather, SLOT(slot_device_online(bool)));
}

/*******************************************************************************
* Function Name  :  wilddog_init
* Description    :  创建野狗引用
* Input          :  QString deviceID    设备ID
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogControl::wilddog_init(QString deviceSN)
{

    qDebug()<<"WILDOG INIT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    QString urlDevice = URL_DEVICE(deviceSN);
    QByteArray ba1 = urlDevice.toLatin1();
    const char* cUrl_device = ba1.data();
    Wilddog_T ref = wilddog_initWithUrl((Wilddog_Str_T *)cUrl_device);

    wilddogAirData->wilddog_init(ref);
    wilddogStatus->wilddog_init(ref);
    wilddogCity->wilddog_init(ref);
    wilddogDevice->wilddog_init(ref);
    wilddogPower->wilddog_init(ref);
    wilddogApp->wilddog_init(ref);

    // 创建天气数据引用
    QString urlWeather = QString(URL_WEATHER);
    QByteArray ba2 = urlWeather.toLatin1();
    const char* cUrl_weather = ba2.data();
    Wilddog_T refWeather = wilddog_initWithUrl((Wilddog_Str_T*)cUrl_weather);

    // 创建节点监听
    wilddogWeather->wilddog_init(refWeather);
    isWilddogInit = true;
}

/*******************************************************************************
* Function Name  :  slot_net_connected
* Description    :  网络连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogControl::slot_net_connected()
{
    isConnected = true;

    // 不断同步
    timerSync->start();

    if(isWilddogInit)
    {
        // 数据在线
        wilddog_goOnline();
        token   = fileConfig->get_value(TOKEN);

        qDebug()<<"Token"<<token<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        token_auth(token);
    }
}

/*******************************************************************************
* Function Name  :  slot_net_disconnect
* Description    :  网络断开
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogControl::slot_net_disconnect()
{
    isConnected = false;

    timerSync->stop();

    // 野狗数据已初始化
    if(isWilddogInit)
    {
        // 数据离线
        wilddog_goOffline();
        isDeviceOnline = false;
        emit signal_device_online(false);
    }
}

/*******************************************************************************
* Function Name  :  token_auth
* Description    :  系统认证
* Input          :  QString token
* Output         :  None
* Return         :  bool 认证结果
*******************************************************************************/
void WilddogControl::token_auth(QString token)
{
    QByteArray ba = token.toLatin1();
    char *cToken = ba.data();
    int i = wilddog_auth((Wilddog_Str_T *)"bran-test.wilddogio.com", (u8*)cToken, strlen(cToken), callback_auth, NULL);
    if( i != 0)
    {
        qDebug()<<"AUTH ERROR!!!!!!!!!!!!!";
        isAuth =false;
        timerAuth->start();
    }
}

/*******************************************************************************
* Function Name  :  callback_auth
* Description    :  回调函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogControl::callback_auth(void* arg, Wilddog_Return_T err)
{
    if(err < WILDDOG_ERR_NOERR || err >= WILDDOG_HTTP_BAD_REQUEST)
    {
        instance->isAuth = false;
        instance->timerAuth->start();
        qDebug()<<"Auth Fail!!!";
    }
    else
    {
        instance->isAuth = true;
        instance->num = 0;
        instance->isDeviceOnline = true;
        qDebug()<<"Auth Success!!!";
        emit instance->signal_device_online(true);
    }
}

/*******************************************************************************
* Function Name  :  slot_wilddog_sync
* Description    :  数据同步
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogControl::slot_wilddog_sync()
{
    // 野狗初始化
    if(isWilddogInit)
    {
        wilddog_increaseTime(100);
        wilddog_trySync();
    }
}

/*******************************************************************************
* Function Name  :  slot_pushData_failed
* Description    :  推送数据失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogControl::slot_net_abnormal()
{
    // 发送数据推送失败信号,通过这个网络异常的信号，来进行判定此时的WiFi是否正常连接
//    emit signal_verify_net();
}

/*******************************************************************************
* Function Name  :  slot_token_abnormal
* Description    :  token异常
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogControl::slot_token_abnormal()
{
    // 设为未认证
    isAuth = false;
    isDeviceOnline = false;
    emit signal_device_online(false);
    emit signal_token_abnormal();
}

/*******************************************************************************
* Function Name  :  slot_update_deviceID
* Description    :  刷新此时的设备ID
* Input          :  QString deviceID 设备ID
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogControl::slot_update_infoDevice(InfoDevice infoDevice)
{
    wilddog_init(deviceSN);

    if(isConnected)
    {
        token  = infoDevice.token.value;
        token_auth(token);
    }
}

/*******************************************************************************
* Function Name  :  slot_update_token
* Description    :  刷新此时的token
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogControl::slot_update_token(QString token)
{
    this->token = token;
    isTokenValid = true;
    if(isConnected)
    {
        if(isWilddogInit)
        {
            token_auth(token);
        }
        else
        {
            wilddog_init(deviceSN);
            token_auth(token);
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_retry_auth
* Description    :  重试认证
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogControl::slot_retry_auth()
{
    if(isTokenValid)
    {
        if(num >= 3)
        {
            emit signal_token_abnormal();
        }
        else
        {
            token = fileConfig->get_value(TOKEN);
            token_auth(token);
        }
    }
    else
    {
        emit signal_token_abnormal();
    }
}
