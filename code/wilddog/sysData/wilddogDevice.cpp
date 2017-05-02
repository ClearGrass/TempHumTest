#include "wilddogDevice.h"

WilddogDevice *WilddogDevice::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogDevice *WilddogDevice::getInstance()
{
    if(NULL == instance)
    {
        instance = new WilddogDevice();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  WilddogDevice
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogDevice::WilddogDevice(QObject *parent) : QObject(parent)
{
    isDeviceOnline = false;

    fileConfig = FileConfig::getInstance();
    deviceID = fileConfig->get_value(DEVICE_ID);

    timerRetry = new QTimer();
    timerRetry->setSingleShot(true);
    timerRetry->setInterval(100);
    connect(timerRetry, SIGNAL(timeout()), this, SLOT(slot_setValue_retry()));
}

/*******************************************************************************
* Function Name  :  wilddog_init
* Description    :  野狗数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogDevice::wilddog_init(Wilddog_T wilddog)
{
    device = wilddog_getChild(wilddog, (Wilddog_Str_T*)"device_id");
}

/*******************************************************************************
* Function Name  :  slot_sync_device
* Description    :  同步城市信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogDevice::slot_sync_device(QString deviceID)
{
    if(deviceID != "")
    {
        this->deviceID = deviceID;
        if(isDeviceOnline)
        {
            sync_device(deviceID);
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_setValue_retry
* Description    :  重复设置数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogDevice::slot_setValue_retry()
{
    sync_device(deviceID);
}

/*******************************************************************************
* Function Name  :  slot_device_online
* Description    :  设备上线
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogDevice::slot_device_online(bool device)
{
    isDeviceOnline = device;

    // 若此时的设备已在线
    if(isDeviceOnline && deviceID != "")
    {
        sync_device(deviceID);
    }
    else
    {
        timerRetry->stop();
    }
}

/*******************************************************************************
* Function Name  :  sync_device
* Description    :  设置当前设备
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogDevice::sync_device(QString deviceID)
{
    Wilddog_Node_T *pNode = NULL;
    QByteArray ba = deviceID.toLatin1();
    const char *cDevice = ba.data();
    pNode = wilddog_node_createUString(NULL, (Wilddog_Str_T*)cDevice);
    wilddog_setValue(device, pNode, callback_device, NULL);
    wilddog_node_delete(pNode);
}

/*******************************************************************************
* Function Name  :  callback_city
* Description    :  回调函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogDevice::callback_device(void *arg, Wilddog_Return_T err)
{
    if(err < WILDDOG_HTTP_OK || err >= WILDDOG_HTTP_NOT_MODIFIED)
    {
        if(err == -10)
        {
            instance->timerRetry->start();
        }
        return;
    }
    instance->timerRetry->stop();
    return;
}
