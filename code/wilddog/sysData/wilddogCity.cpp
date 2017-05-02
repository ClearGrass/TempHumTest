#include "wilddogCity.h"

WilddogCity *WilddogCity::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogCity *WilddogCity::getInstance()
{
    if(NULL == instance)
    {
        instance = new WilddogCity();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  WilddogCity
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogCity::WilddogCity(QObject *parent) : QObject(parent)
{
    isDeviceOnline = false;

    fileConfig = FileConfig::getInstance();
    cityID = fileConfig->get_value(CITY_ID);

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
void WilddogCity::wilddog_init(Wilddog_T wilddog)
{
    city = wilddog_getChild(wilddog, (Wilddog_Str_T*)"city_id");
}

/*******************************************************************************
* Function Name  :  slot_sync_city
* Description    :  同步城市信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogCity::slot_sync_city(QString cityID)
{
    if(cityID != "")
    {
        this->cityID = cityID;
        if(isDeviceOnline)
        {
            sync_city(cityID);
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
void WilddogCity::slot_setValue_retry()
{
    sync_city(cityID);
}

/*******************************************************************************
* Function Name  :  slot_device_online
* Description    :  设备上线
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogCity::slot_device_online(bool device)
{
    isDeviceOnline = device;

    // 若此时的设备已在线
    if(isDeviceOnline && cityID != "")
    {
        sync_city(cityID);
    }
    else
    {
        timerRetry->stop();
    }
}

/*******************************************************************************
* Function Name  :  sync_city
* Description    :  设置当前城市
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogCity::sync_city(QString cityID)
{
    Wilddog_Node_T *pNode = NULL;
    QByteArray ba = cityID.toLatin1();
    const char *cCity = ba.data();
    pNode = wilddog_node_createUString(NULL, (Wilddog_Str_T*)cCity);
    wilddog_setValue(city, pNode, callback_city, NULL);
    wilddog_node_delete(pNode);
}

/*******************************************************************************
* Function Name  :  callback_city
* Description    :  回调函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogCity::callback_city(void *arg, Wilddog_Return_T err)
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
