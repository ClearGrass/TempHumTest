#include "wilddogStatus.h"

WilddogStatus *WilddogStatus::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogStatus *WilddogStatus::getInstance()
{
    if(NULL == instance)
    {
        instance = new WilddogStatus();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  WilddogStatus
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogStatus::WilddogStatus(QObject *parent) : QObject(parent)
{
    isDeviceOnline = false;

    timerRetry = new QTimer();
    timerRetry->setSingleShot(true);
    timerRetry->setInterval(100);
    connect(timerRetry, SIGNAL(timeout()), this, SLOT(slot_setValue_retry()));
}

/*******************************************************************************
* Function Name  :  wilddog_init
* Description    :  野狗初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogStatus::wilddog_init(Wilddog_T wilddog)
{
    // 初始化节点
    nodeClient = wilddog_getChild(wilddog, (Wilddog_Str_T*)"client_online");
    nodeDevice = wilddog_getChild(wilddog, (Wilddog_Str_T*)"device_online");
}

/*******************************************************************************
* Function Name  :  slot_device_online
* Description    :  设备在线
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogStatus::slot_device_online(bool device)
{
    isDeviceOnline = device;
    if(isDeviceOnline)
    {
        Wilddog_Node_T * node = NULL;
        node = wilddog_node_createFalse(NULL);

        // 监听当前连接状态
        QString urlConnect = QString(URL_CONNECT);
        QByteArray ba3 = urlConnect.toLatin1();
        const char *cUrl_connect = ba3.data();
        Wilddog_T refConnect = wilddog_initWithUrl((Wilddog_Str_T*)cUrl_connect);
        wilddog_addObserver(refConnect, WD_ET_VALUECHANGE, callback_status, NULL);

        // 当野狗服务器断开时设置值
        wilddog_onDisconnectSetValue(nodeDevice, node, callback_device, NULL);
        wilddog_addObserver(nodeClient, WD_ET_VALUECHANGE, callback_client, NULL);
    }
}

/*******************************************************************************
* Function Name  :  slot_setValue_retry
* Description    :  设置数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogStatus::slot_setValue_retry()
{
    if(isDeviceOnline)
    {
        device_online();
    }
    else
    {
        timerRetry->stop();
    }
}

/*******************************************************************************
* Function Name  :  device_online
* Description    :  设备在线
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogStatus::device_online()
{
    // 将设备节点设置为false
    Wilddog_Node_T * node = NULL;
    node = wilddog_node_createTrue(NULL);
    wilddog_setValue(nodeDevice, node, callback_device, NULL);
    wilddog_node_delete(node);
}

/*******************************************************************************
* Function Name  :  device_offline
* Description    :  设备离线
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogStatus::device_offline()
{
    emit signal_verify_net();
}

/*******************************************************************************
* Function Name  :  callback_device
* Description    :  回调函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogStatus::callback_device(void *arg, Wilddog_Return_T err)
{
    if(err < WILDDOG_HTTP_OK || err >= WILDDOG_HTTP_NOT_MODIFIED)
    {
        qDebug()<<"Online set error!"<<err;

        if(err == -10)
        {
            instance->timerRetry->start();
        }
        return;
    }
    instance->timerRetry->stop();
    qDebug()<<"ONline set success!";
    return;
}

/*******************************************************************************
* Function Name  :  create_nodeClient
* Description    :  创建客户端节点
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogStatus::create_nodeClient()
{
    // 暂时不用
}

/*******************************************************************************
* Function Name  :  callback_client
* Description    :  回调函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogStatus::callback_client(const Wilddog_Node_T *p_snapshot, void *arg, Wilddog_Return_T err)
{
    if(p_snapshot)
    {
        if(p_snapshot->d_wn_type == WILDDOG_NODE_TYPE_TRUE)
        {
            emit instance->signal_client_online(true);
        }
        else if(p_snapshot->d_wn_type == WILDDOG_NODE_TYPE_FALSE)
        {
            emit instance->signal_client_online(false);
        }
    }
    else
    {
        instance->create_nodeClient();
    }
    if(err < WILDDOG_HTTP_OK || err >= WILDDOG_HTTP_NOT_MODIFIED)
    {
        return;
    }
    return;
}

/*******************************************************************************
* Function Name  :  callback_connect
* Description    :  连接的回调函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogStatus::callback_status(const Wilddog_Node_T *p_snapshot, void *arg, Wilddog_Return_T err)
{
    if(err < WILDDOG_HTTP_OK || err >= WILDDOG_HTTP_NOT_MODIFIED)
    {
        wilddog_debug("getValue fail!");
        qDebug()<<"GET CONNECT VALUE FAIL";
        return;
    }
    if(p_snapshot)
    {
        if(p_snapshot->d_wn_type == WILDDOG_NODE_TYPE_TRUE)
        {
            qDebug()<<"Device Online!!!!!!!!";

            // 当前在线
            instance->device_online();
        }
        else if(p_snapshot->d_wn_type == WILDDOG_NODE_TYPE_FALSE)
        {
            qDebug()<<"Device Offline!!!!!!!!";

            // 发送网络异常信号
            instance->device_offline();
        }
    }
    return;
}
