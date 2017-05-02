#include "wilddogAirData.h"

WilddogAirData *WilddogAirData::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogAirData *WilddogAirData::getInstance()
{
    if(NULL == instance)
    {
        instance = new WilddogAirData();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  WilddogAirData
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogAirData::WilddogAirData(QObject *parent) : QObject(parent)
{
    isClientOnline = false;
    isDeviceOnline = false;
    dataPM25.type = PM25;
    dataTEMP.type = TEMP;
    dataHUMI.type = HUMI;
    dataTVOC.type = TVOC;
    dataCO2E.type = CO2E;

    timerRetry = new QTimer(this);
    timerRetry->setInterval(RETRY_PUSH_INTERVAL);
//    connect(timerRetry, SIGNAL(timeout()), this, SLOT(slot_retry_pushPM25()));

    timerSync = new QTimer(this);
    timerSync->setInterval(5*60*1000);
    connect(timerSync, SIGNAL(timeout()), this, SLOT(slot_sync_datas()));
}

/*******************************************************************************
* Function Name  :  ref_init
* Description    :  引用初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogAirData::wilddog_init(Wilddog_T wilddog)
{
    monitor = wilddog_getChild(wilddog, (Wilddog_Str_T*)"monitor");
}

/*******************************************************************************
* Function Name  :  slot_client_online
* Description    :  客户端状态更新
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogAirData::slot_client_online(bool client)
{
    isClientOnline = client;
    if(!isClientOnline)
    {
        timerSync->setInterval(SYNC_DATA_INTERVAL);
        timerSync->start();
    }
    else
    {
        timerSync->stop();
    }
}

/*******************************************************************************
* Function Name  :  slot_device_online
* Description    :  设备状态更新
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogAirData::slot_device_online(bool device)
{
    isDeviceOnline = device;
    if(!isDeviceOnline)
    {
        timerSync->stop();
    }
    else
    {
        timerSync->start();
    }
}

/*******************************************************************************
* Function Name  :  slot_push_data
* Description    :  推送数据至野狗端
* Input          :  AirData　airData 推送数据
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogAirData::slot_sync_data(AirData airData)
{
    // 若客户端在线
    if(isClientOnline)
    {
        sync_data(airData);
    }
    else
    {
        switch (airData.type)
        {
            case PM25:
                dataPM25 = airData;
            break;
            case TEMP:
                dataTEMP = airData;
            break;
            case HUMI:
                dataHUMI = airData;
            break;
            case TVOC:
                dataTVOC = airData;
            break;
            case CO2E:
                dataCO2E = airData;
            break;
            default:
            return;
            break;
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_sync_datas
* Description    :  周期性同步数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogAirData::slot_sync_datas()
{
    if(isDeviceOnline)
    {
        sync_data(dataPM25);
        sync_data(dataTEMP);
        sync_data(dataHUMI);
        sync_data(dataTVOC);
        sync_data(dataCO2E);
    }
}

/*******************************************************************************
* Function Name  :  sync_data
* Description    :  同步数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogAirData::sync_data(AirData airData)
{
    Wilddog_Node_T* nodeHead = NULL;
    Wilddog_Node_T* nodeValue = NULL;
    Wilddog_Node_T* nodeTimeStamp = NULL;
    double value;
    switch(airData.type)
    {
        case PM25:
        {
            nodeData = wilddog_getChild(monitor, (Wilddog_Str_T*)"pm25");
        }
        break;
        case TEMP:
        {
            nodeData = wilddog_getChild(monitor, (Wilddog_Str_T*)"temperature");
        }
        break;
        case HUMI:
        {
            nodeData = wilddog_getChild(monitor, (Wilddog_Str_T*)"humidity");
        }
        break;
        case TVOC:
        {
            nodeData = wilddog_getChild(monitor, (Wilddog_Str_T*)"tvoc");
        }
        break;
        case CO2E:
        {
            nodeData = wilddog_getChild(monitor, (Wilddog_Str_T*)"co2");
        }
        break;
        default:
            return;
            break;
    }

    value  = QString::number(airData.value, 'g', 6).toDouble();
    nodeHead      = wilddog_node_createObject(NULL);
    nodeValue     = wilddog_node_createFloat((Wilddog_Str_T*)"value", (double)value);
    nodeTimeStamp = wilddog_node_createFloat((Wilddog_Str_T*)"timestamp", airData.timestamp);
    wilddog_node_addChild(nodeHead,nodeValue);
    wilddog_node_addChild(nodeHead,nodeTimeStamp);
    wilddog_setValue(nodeData, nodeHead, callback_data, NULL);
    wilddog_node_delete(nodeHead);
}

/*******************************************************************************
* Function Name  :  callback
* Description    :  回调函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogAirData::callback_data(void* arg, Wilddog_Return_T err)
{
    if(err < WILDDOG_HTTP_OK || err >= WILDDOG_HTTP_NOT_MODIFIED)
    {
        if(err == 404)
        {
            // 发送
            emit instance->signal_token_abnormal();
        }
        else
        {
            // 发送网络异常信号
            emit instance->signal_net_abnormal();
            if(!instance->timerRetry->isActive())
            {
                instance->timerRetry->start();
            }
        }
    }
    else
    {
        qDebug()<<"PUSH DATA SUCCESSS!!!";
        if(instance->timerRetry->isActive())
        {
            instance->timerRetry->stop();
        }
    }
    return;
}

