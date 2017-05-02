#include "funcApp.h"

FuncApp *FuncApp::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FuncApp *FuncApp::getInstance()
{
    // 是否被创建过
    if(0 == instance)
    {
        // 没有的话新创建
        instance = new FuncApp();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  FuncApp
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
FuncApp::FuncApp(QObject *parent)
    : QThread(parent)
{
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
void FuncApp::data_init()
{
    isConnected = false;
    isBind = false;

    timerValid = new QTimer();
    timerValid->setInterval(5*60*1000);
    timerValid->setSingleShot(true);
    miioControl = MiioControl::getInstance();
    fileConfig = FileConfig::getInstance();

    if("1" == fileConfig->get_value(APP_BIND_STATUS))
    {
        isBind = true;
    }
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncApp::connect_init()
{
    connect(this, SIGNAL(signal_request_bindkey()), miioControl, SLOT(slot_request_bindkey()));
    connect(miioControl, SIGNAL(signal_update_bindKey(BindKey)), this, SLOT(slot_update_bindKey(BindKey)));
    connect(miioControl, SIGNAL(signal_updateBindKey_failed()), this, SLOT(slot_updateBindKey_failed()));
    connect(miioControl, SIGNAL(signal_update_bindStatus(int)), this, SLOT(slot_update_bindStatus(int)));
    connect(timerValid, SIGNAL(timeout()), this, SIGNAL(signal_qrcode_timeout()));
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncApp::run()
{
    miioControl->start();
    this->exec();
}

/*******************************************************************************
* Function Name  :  slot_net_connected
* Description    :  网络连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncApp::slot_net_connected()
{
    isConnected = true;
}

/*******************************************************************************
* Function Name  :  slot_net_disconnect
* Description    :  网络断开连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncApp::slot_net_disconnect()
{
    isConnected = false;
}

/*******************************************************************************
* Function Name  :  get_bindkey_qrcode
* Description    :  获取配对码二维码
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool FuncApp::get_bindkey_qrcode(QString &codePath)
{
    return false;
}

/*******************************************************************************
* Function Name  :  get_bindState
* Description    :  获取绑定状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool FuncApp::get_bindState()
{
    return isBind;
}

/*******************************************************************************
* Function Name  :  slot_remove_qrcode
* Description    :  删除二维码
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncApp::slot_remove_qrcode()
{
    if(timerValid->isActive())
    {
        timerValid->stop();
    }
}

/*******************************************************************************
* Function Name  :  slot_refresh_bindkey
* Description    :  刷新此时的bindkey
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncApp::slot_refresh_qrcode()
{
    if(timerValid->isActive())
    {
        timerValid->stop();
    }
    if(!isBind)
    {
        if(isConnected)
        {
            // 向miio发送请求获取bindkey
            emit signal_request_bindkey();
        }
        else
        {
            emit signal_net_disconnect();
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_update_bindKey
* Description    :  刷新绑定码
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncApp::slot_update_bindKey(BindKey bindKey)
{
    this->bindKey = bindKey;
    timerValid->start();

    // 生成新的二维码
    emit signal_create_qrcode(bindKey.value);
}

/*******************************************************************************
* Function Name  :  slot_updateBindKey_failed
* Description    :  刷新绑定码超时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncApp::slot_updateBindKey_failed()
{
    if(isConnected)
    {
        emit signal_net_abnormal();
    }
    else
    {
        emit signal_net_disconnect();
    }
}

/*******************************************************************************
* Function Name  :  slot_update_bindState
* Description    :  刷新此时的绑定状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncApp::slot_update_bindStatus(int state)
{
    emit signal_update_bindStatus(state);
    if(state == 1)
    {
        // 已被绑定
        isBind = true;
        qDebug()<<"#################################";
        fileConfig->set_value(APP_BIND_STATUS, "1");
    }
    else
    {
        isBind = false;
        fileConfig->set_value(APP_BIND_STATUS, "0");
    }
}

/*******************************************************************************
* Function Name  :  slot_createQRCode_result
* Description    :  创建二维码成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void FuncApp::slot_createQRCode_result(int result)
{
    if(0 == result)
    {
        QString path = QString("%1/%2").arg(QDir::currentPath()).arg(QRCODE_PATH);
        emit signal_update_qrcode(path);
    }
}


