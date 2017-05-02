#include "wilddogWeather.h"
#include "stdlib.h"

WilddogWeather * WilddogWeather::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例个体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogWeather* WilddogWeather::getInstance()
{
    if(NULL == instance)
    {
        instance = new WilddogWeather();
    }
    return instance;
}

/*******************************************************************************
* Function Name :   WilddogWeather
* Description   :   构造函数
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
WilddogWeather::WilddogWeather()
{
    isDeviceOnline = false;
    isObserving = false;
    isRetryObserve = false;

    timerObserver = new QTimer();
    timerObserver->setSingleShot(true);
    timerObserver->setInterval(1000);
    connect(timerObserver, SIGNAL(timeout()), this, SLOT(slot_observer_retry()));

    fileConfig = FileConfig::getInstance();
    cityID = fileConfig->get_value(CITY_ID);
}

/*******************************************************************************
* Function Name  :  wilddog_init
* Description    :  引用初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogWeather::wilddog_init(Wilddog_T wilddog)
{
    this->wilddog = wilddog;
}

/*******************************************************************************
* Function Name  :  slot_device_online
* Description    :  设备状态更新
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogWeather::slot_device_online(bool device)
{
    isDeviceOnline = device;

    // 此时连线
    if(isDeviceOnline)
    {
        // 若此时正在监听
        if(isObserving)
        {
            // 需要重新监听
            if(isRetryObserve)
            {
                start_observer(cityID);
            }
        }
        else
        {
            start_observer(cityID);
        }
    }
    else
    {
        timerObserver->stop();
    }
}

/*******************************************************************************
* Function Name  :  slot_update_city
* Description    :  刷新城市
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogWeather::slot_update_cityID(QString cityID)
{
    this->cityID = cityID;

    // 若此时设备在线
    if(isDeviceOnline)
    {
        start_observer(cityID);
    }
    else
    {
        isRetryObserve = true;
    }
}

/*******************************************************************************
* Function Name  :  start_observer
* Description    :  开始监听
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogWeather::start_observer(QString cityID)
{
    isRetryObserve = false;
    if(isObserving)
    {
        // 关闭之前监听
        wilddog_removeObserver(weather, WD_ET_VALUECHANGE);
        isObserving = false;
    }

    QByteArray ba = cityID.toLatin1();
    char *cCityID = ba.data();
    Wilddog_T city = wilddog_getChild(wilddog, (Wilddog_Str_T*)cCityID);
    weather = wilddog_getChild(city,(Wilddog_Str_T*)"weather");
    wilddog_addObserver(weather, WD_ET_VALUECHANGE, callback_weather, NULL);
}

/*******************************************************************************
* Function Name  :  slot_observer_retry
* Description    :  重试监听
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogWeather::slot_observer_retry()
{
    if(isDeviceOnline)
    {
        // 开始监听该城市
        start_observer(cityID);
    }
}

/*******************************************************************************
* Function Name :   callback_observer
* Description   :   监听数据节点的回调函数
* Input         :   const Wilddog_Node_T* p_snapshot 取回的数据镜像
*                   void* arg 用户传递的值
*                   Wilddog_Return_T err 状态码
* Output        :   None
* Return        :   None
*******************************************************************************/
void WilddogWeather::callback_weather(const Wilddog_Node_T* p_snapshot, void* arg, Wilddog_Return_T err)
{
    if(p_snapshot)
    {
        // 若此时有数据改变，进行解析
        instance->analyse_weather(p_snapshot);
    }
    if((err != WILDDOG_HTTP_OK) && (err != WILDDOG_ERR_RECONNECT))
    {
        qDebug()<<"Observe Weather Failed--------------"<<err;
        if(err == 404)
        {
            emit instance->signal_token_abnormal();
        }
        else
        {
            // 重新开始监听
            instance->timerObserver->start();
        }
        return;
    }
    instance->isObserving = true;
    qDebug()<<"observe data!!!";
    return;
}

/*******************************************************************************
* Function Name :   node_get
* Description   :   获取节点的子节点
* Input         :   Wilddog_Node_T *p_head 节点
*                   const char* key_name 子节点key
* Output        :   None
* Return        :   Wilddog_Node_T * 子节点指针
*******************************************************************************/
Wilddog_Node_T *WilddogWeather::node_get(Wilddog_Node_T *p_head,const char* key_name)
{
    Wilddog_Node_T *node = p_head;
    u8 len = 0;
    while(node )
    {
        if( node->p_wn_key )
        {
            len = strlen((const char*)key_name)>strlen((const char*)node->p_wn_key)\
                    ?strlen((const char*)node->p_wn_key):strlen((const char*)key_name);
            if( !memcmp(key_name,node->p_wn_key,len))
                return node;
        }
        node=node->p_wn_next;
        if(node == p_head)
            return NULL;
    }
    return NULL;
}

/*******************************************************************************
* Function Name :   analyse_weather
* Description   :   分析监听的节点
* Input         :   Wilddog_Node_T *pNode 监听的节点
* Output        :   None
* Return        :   None
*******************************************************************************/
void WilddogWeather::analyse_weather(const Wilddog_Node_T *pNode)
{
    InfoWeather weatherInfo;

    // 获取子节点
    Wilddog_Node_T *weather = pNode->p_wn_child;
    if(weather)
    {
        // 实时天气数据读取过程

        weatherInfo.skycon = get_validString(weather,"skycon");
        weatherInfo.pm25   = qRound(get_validValue(weather,"pm25"));
        weatherInfo.temp   = qRound(get_validValue(weather,"temperature"));
        weatherInfo.humi   = qRound(get_validValue(weather,"humidity"));

        qDebug()<<"Weather:"<<weatherInfo.skycon;
        qDebug()<<"PM25:"<<weatherInfo.pm25;
        qDebug()<<"TEMP:"<<weatherInfo.temp;
        qDebug()<<"HUMI:"<<weatherInfo.humi;

        // 发送此时天气信息
        emit signal_update_weather(weatherInfo);
    }
}

/*******************************************************************************
* Function Name :   get_validString
* Description   :   获取有效值
* Input         :   Wilddog_Node_T *pNode 节点名称
*                   char *field *节点key名称
* Output        :   None
* Return        :   None
*******************************************************************************/
QString WilddogWeather::get_validString(Wilddog_Node_T *pNode,char *field)
{
    // 获取此时子结点
    Wilddog_Node_T *node = node_get(pNode,field);
    if(node)
    {
        // 返回结点值
        return QString((char *)node->p_wn_value);
    }
    return "";
}

/*******************************************************************************
* Function Name :   get_validValue
* Description   :   获取有效值
* Input         :   Wilddog_Node_T *pNode 节点名称
*                   char *field *节点key名称
* Output        :   None
* Return        :   None
*******************************************************************************/
float WilddogWeather::get_validValue(Wilddog_Node_T *pNode,char *field)
{
    float value = 9999;
    int len = 8;
    // 获取此时子结点
    Wilddog_Node_T *node = node_get(pNode,field);
    if(node)
    {
        if((node->d_wn_type == WILDDOG_NODE_TYPE_NUM))
        {
            // 返回结点值
            value = *(s32*)(node->p_wn_value);
        }
        else if(node->d_wn_type == WILDDOG_NODE_TYPE_FLOAT)
        {
            // 返回结点值
            value = *(wFloat*)(node->p_wn_value);
        }
    }
    return value;
}
