#include "wilddogApp.h"

WilddogApp *WilddogApp::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogApp *WilddogApp::getInstance()
{
    if(NULL == instance)
    {
        instance = new WilddogApp();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  WilddogApp
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
WilddogApp::WilddogApp(QObject *parent) : QObject(parent)
{

}

/*******************************************************************************
* Function Name  :  wilddog_init
* Description    :  野狗初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogApp::wilddog_init(Wilddog_T wilddog)
{
    app = wilddog_getChild(wilddog, (Wilddog_Str_T*)"command");
}

/*******************************************************************************
* Function Name  :  slot_device_online
* Description    :  设备状态信息更新
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void WilddogApp::slot_device_online(bool device)
{
    isDeviceOnline = device;

    // 若此时设备上线，则开始监听
    if(isDeviceOnline)
    {
        wilddog_addObserver(app, WD_ET_VALUECHANGE, callback_app, NULL);
    }
}

/*******************************************************************************
* Function Name :   callback_app
* Description   :   监听数据节点的回调函数
* Input         :   const Wilddog_Node_T* p_snapshot 取回的数据镜像
*                   void* arg 用户传递的值
*                   Wilddog_Return_T err 状态码
* Output        :   None
* Return        :   None
*******************************************************************************/
void WilddogApp::callback_app(const Wilddog_Node_T* p_snapshot, void* arg, Wilddog_Return_T err)
{
    if(p_snapshot)
    {
        // 若此时有数据改变，进行解析
//        instance->analyse_weather(p_snapshot);
    }

    if((err != WILDDOG_HTTP_OK) && (err != WILDDOG_ERR_RECONNECT))
    {
        return;
    }
    qDebug()<<"observe data!!!";
    return;
}

