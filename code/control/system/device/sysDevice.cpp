#include <QApplication>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QTextCodec>
#include <QStringList>
#include <QtDebug>
#include <QMutableListIterator>
#include <QProcess>
#include "sysDevice.h"

SysDevice *SysDevice::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例个体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysDevice* SysDevice::getInstance()
{
    if(0 == instance)
    {
        instance = new SysDevice();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  SysDevice
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
SysDevice::SysDevice()
    : QThread()
{
    qRegisterMetaType<InfoDevice>("InfoDevice");
    qRegisterMetaType<Token>("Token");
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
void SysDevice::data_init()
{
    serverDevice = ServerDevice::getInstance();
    wilddogDevice = WilddogDevice::getInstance();

    fileConfig   = FileConfig::getInstance();

    timerDevice  = new QTimer();
    timerDevice->setSingleShot(true);
    timerDevice->setInterval(DEVICE_INIT_INTERVAL);

    timerToken   = new QTimer();
    timerDevice->setSingleShot(true);
    timerToken->setInterval(GET_TOKEN_INTERVAL);

    isDeviceInit = true;
    isTimeInit   = true;
    isConnected  = false;
    isTokenValid = true;

    infoDevice.sn            = fileConfig->get_value(DEVICE_SN);
    infoDevice.mac           = fileConfig->get_value(DEVICE_MAC);
    infoDevice.deviceID      = fileConfig->get_value(DEVICE_ID);
    infoDevice.miioDID       = fileConfig->get_value(MIIO_DID);
    infoDevice.miioMAC       = fileConfig->get_value(MIIO_MAC);
    infoDevice.miioKEY       = fileConfig->get_value(MIIO_KEY);
    infoDevice.token.value   = fileConfig->get_value(TOKEN);
    infoDevice.token.expTime = fileConfig->get_value(EXP_TIME).toInt();

    infoDevice.miioInfo      =get_miio_info(infoDevice.miioDID, infoDevice.miioMAC, infoDevice.miioKEY);

    // 判断设备是否初始化
    if(DEFAULT_DEVICE_INIT == fileConfig->get_value(DEVICE_INIT))
    {
        // 代表设备未进行初始化
        isDeviceInit = false;
    }

    // 判断时间是否初始化
    if(DEFAULT_TIME_INIT == fileConfig->get_value(TIME_INIT))
    {
        // 代表时间未进行初始化
        isTimeInit = false;
    }
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysDevice::connect_init()
{
    connect(timerDevice, SIGNAL(timeout()), this, SLOT(slot_retry_deviceInit()));
    connect(timerToken, SIGNAL(timeout()), this, SLOT(slot_retry_getToken()));
    connect(serverDevice, SIGNAL(signal_deviceInit_success(InfoDevice)), this, SLOT(slot_deviceInit_success(InfoDevice)));
    connect(serverDevice, SIGNAL(signal_deviceInit_failed()), this, SLOT(slot_deviceInit_failed()));
    connect(serverDevice, SIGNAL(signal_getToken_success(Token)), this, SLOT(slot_getToken_success(Token)));
    connect(serverDevice, SIGNAL(signal_getToken_failed()), this, SLOT(slot_getToken_failed()));
    connect(this, SIGNAL(signal_device_init(QString,QString,QString,QString)), serverDevice, SLOT(slot_device_init(QString,QString,QString,QString)));
    connect(this, SIGNAL(signal_get_token(QString,QString)), serverDevice, SLOT(slot_get_token(QString,QString)));
    connect(this, SIGNAL(signal_sync_device(QString)), wilddogDevice, SLOT(slot_sync_device(QString)));
}

/*******************************************************************************
* Function Name  :  is_deviceInit
* Description    :  设备是否初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysDevice::is_deviceInit()
{
    return isDeviceInit;
}

/*******************************************************************************
* Function Name  :  is_timeInit
* Description    :  时间是否初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysDevice::is_timeInit()
{
    return isTimeInit;
}

/*******************************************************************************
* Function Name  :  is_tokenValid
* Description    :  token是否有效
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool SysDevice::is_tokenValid()
{
    // 若token超时时间
    if(infoDevice.token.expTime < QDateTime::currentDateTime().toTime_t())
    {
        // 代表此时的token无效
        isTokenValid = false;
    }
    return isTokenValid;
}

/*******************************************************************************
* Function Name  :  get_deviceID
* Description    :  获取设备ID
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
InfoDevice SysDevice::get_infoDevice()
{
    return infoDevice;
}

/*******************************************************************************
* Function Name  :  slot_deviceInit_success
* Description    :  初始化设备信息返回
* Input          :  DeviceInfo deviceInfo   设备信息
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysDevice::slot_deviceInit_success(InfoDevice deviceInfo)
{
    if(timerDevice->isActive())
    {
        timerDevice->stop();
    }

    if(timerToken->isActive())
    {
        timerToken->stop();
    }
    qDebug()<<"<********************************INIT CONFIG ****************************>";
    infoDevice.deviceID = deviceInfo.deviceID;
    infoDevice.token    = deviceInfo.token;

    // 设备ID写入文件中，并广播新的ID
    fileConfig->set_value(DEVICE_ID, deviceInfo.deviceID);
    fileConfig->set_value(TOKEN, deviceInfo.token.value);
    fileConfig->set_value(EXP_TIME, QString::number(deviceInfo.token.expTime));
    fileConfig->set_value(DEVICE_INIT, "true");
    isTokenValid = true;
    isDeviceInit = true;

    // 更新此时的设备信息
    emit signal_sync_device(deviceInfo.deviceID);
    emit signal_update_infoDevice(infoDevice);
    qDebug()<<"<******************************** END INIT ****************************>";
}

/*******************************************************************************
* Function Name  :  slot_pushData_failed
* Description    :  推送数据失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysDevice::slot_token_abnormal()
{
    // 重新获取新的token
    emit signal_get_token(infoDevice.deviceID, infoDevice.sn);
}

/*******************************************************************************
* Function Name  :  slot_net_connected
* Description    :  网络连接函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysDevice::slot_net_connected()
{
    isConnected = true;

    // 若此时设备未初始化
    if(!isDeviceInit)
    {
        if(timerDevice->isActive())
        {
            timerDevice->stop();
        }

        // 且已连接网络
        emit signal_device_init(infoDevice.sn, infoDevice.mac, infoDevice.model, infoDevice.miioInfo);


    }
    // 若此时设备已经初始化
    else
    {
        // 若此时的token无效
        if(!isTokenValid)
        {
            if(timerToken->isActive())
            {
                timerToken->stop();
            }

            // 获取新的token
            emit signal_get_token(infoDevice.deviceID, infoDevice.sn);
        }

    }
}

/*******************************************************************************
* Function Name  :  slot_net_disconnect
* Description    :  网络断开函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysDevice::slot_net_disconnect()
{
    isConnected = false;

    if(timerDevice->isActive())
    {
        timerDevice->stop();
    }
    if(timerToken->isActive())
    {
        timerToken->stop();
    }

}

/*******************************************************************************
* Function Name  :  slot_deviceInit_failed
* Description    :  设备初始化失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysDevice::slot_deviceInit_failed()
{
    if(isConnected)
    {
        timerDevice->start();
    }
}

/*******************************************************************************
* Function Name  :  slot_retry_initDevice
* Description    :  重新初始化设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysDevice::slot_retry_deviceInit()
{
    // 初始化设备ID
    if((!isDeviceInit) && isConnected)
    {
        emit signal_device_init(infoDevice.sn, infoDevice.mac, infoDevice.model, infoDevice.miioInfo);
    }
}

/*******************************************************************************
* Function Name  :  slot_getToken_success
* Description    :  获取token成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysDevice::slot_getToken_success(Token token)
{
    fileConfig->set_value(TOKEN, token.value);
    fileConfig->set_value(EXP_TIME, QString::number(token.expTime));
    emit signal_update_token(token.value);
}

/*******************************************************************************
* Function Name  :  slot_getToken_failed
* Description    :  获取token失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysDevice::slot_getToken_failed()
{
    if(isConnected)
    {
        timerToken->start();
    }
}

/*******************************************************************************
* Function Name  :  slot_retry_getToken
* Description    :  重新获取token
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SysDevice::slot_retry_getToken()
{
    // 初始化设备ID
    if((!isTokenValid) && isConnected)
    {
        emit signal_get_token(infoDevice.deviceID, infoDevice.sn);
    }
}

QString SysDevice::get_miio_info(QString did, QString mac, QString key)
{
    char *cInfo = NULL;
    cJSON *info = NULL;

    info = cJSON_CreateObject();
    cJSON_AddItemToObject(info, "miio_did", cJSON_CreateString((did.toLatin1()).data()));
    cJSON_AddItemToObject(info, "miio_mac", cJSON_CreateString((mac.toLatin1()).data()));
    cJSON_AddItemToObject(info, "miio_key", cJSON_CreateString((key.toLatin1()).data()));

    cInfo = cJSON_Print(info);
    return QString(cInfo);
}
