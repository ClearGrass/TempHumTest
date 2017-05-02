#include "wilddogPower.h"

WilddogPower *WilddogPower::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogPower *WilddogPower::getInstance()
{
    if(NULL == instance)
    {
        instance = new WilddogPower();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  WilddogPower
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogPower::WilddogPower(QObject *parent)
    : QObject(parent)
{
    isDeviceOnline = false;
    timerRetry = new QTimer();
    timerRetry->setSingleShot(true);
    timerRetry->setInterval(100);
    connect(timerRetry, SIGNAL(timeout()), this, SLOT(slot_setValue_retry()));
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogPower::wilddog_init(Wilddog_T wilddog)
{
    Wilddog_T monitor = wilddog_getChild(wilddog, (Wilddog_Str_T*)"monitor");
    nodePower = wilddog_getChild(monitor, (Wilddog_Str_T*)"power");
}

/*******************************************************************************
* Function Name  :  slot_sync_power
* Description    :  同步此时的电量
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogPower::slot_sync_power(Power power)
{
    if(this->power.value == power.value)
    {
        return;
    }
    this->power = power;
    if(isDeviceOnline)
    {
        sync_power(power);
    }
}

/*******************************************************************************
* Function Name  :  slot_setValue_retry
* Description    :  重复设置值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogPower::slot_setValue_retry()
{
    if(isDeviceOnline)
    {
        sync_power(power);
    }
}

/*******************************************************************************
* Function Name  :  slot_device_online
* Description    :  若设备在线状态改变
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogPower::slot_device_online(bool device)
{
    isDeviceOnline = device;

    // 此时设备上线，推送目前电量等信息
    if(isDeviceOnline)
    {
        sync_power(power);
    }

}

/*******************************************************************************
* Function Name  :  sync_power
* Description    :  同步目前电量
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogPower::sync_power(Power power)
{
    Wilddog_Node_T* nodeHead = NULL;
    Wilddog_Node_T* nodeValue = NULL;
    Wilddog_Node_T* nodeTimeStamp = NULL;

//    qDebug()<<"POWER:"<<power.value<<power.timeStamp;

    nodeHead      = wilddog_node_createObject(NULL);
    nodeValue     = wilddog_node_createNum((Wilddog_Str_T*)"value", power.value);
    nodeTimeStamp = wilddog_node_createFloat((Wilddog_Str_T*)"timestamp", power.timeStamp);
    wilddog_node_addChild(nodeHead,nodeValue);
    wilddog_node_addChild(nodeHead,nodeTimeStamp);
    wilddog_setValue(nodePower, nodeHead, callback_power, NULL);
    wilddog_node_delete(nodeHead);
}

/*******************************************************************************
* Function Name  :  callback_power
* Description    :  回调函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogPower::callback_power(void *arg, Wilddog_Return_T err)
{
    if(err < WILDDOG_HTTP_OK || err >= WILDDOG_HTTP_NOT_MODIFIED)
    {
//        qDebug()<<"PUSH POWER FAILED!!!"<<err;

        if(err == -10)
        {
            instance->timerRetry->start();
        }
    }
    else
    {
        instance->timerRetry->stop();
        qDebug()<<"PUSH POWER DATA SUCCESSS!!!";
    }
    return;
}




