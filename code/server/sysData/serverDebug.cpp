#include "serverDebug.h"

ServerDebug *ServerDebug::instance = NULL;

/*******************************************************************************
* Function Name  :  ServerDebug
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerDebug::ServerDebug(QObject *parent)
    : QObject(parent)
{
}

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerDebug *ServerDebug::getInstance()
{
    if(NULL == instance)
    {
        instance = new ServerDebug();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  slot_device_heartbeat
* Description    :  设备心跳
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDebug::slot_system_heartbeat(InfoSys sysInfo)
{
    QNetworkRequest request;
    QString status;
    NetworkManage *networkManage = new NetworkManage();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    // 初始化post请求地址
    QString strHeartbeat = QString(URL_SERVER).append(QString(REQUEST_HEARTBEAT));
    qDebug()<<strHeartbeat;

    // 初始化URL
    QUrl urlHeartbeat = QUrl::fromEncoded(strHeartbeat.toAscii());
    if(sysInfo.battery.status == FULL)
    {
        status = "FULL";
    }
    else if(sysInfo.battery.status == CHARGING)
    {
        status = "CHARGE";
    }
    else
    {
        status = "NORMAL";
    }

    // 配置post传递参数
    QByteArray postData;
    postData.append("device_id=" + sysInfo.device.deviceID);
    postData.append("&sn=" + sysInfo.device.sn);
    postData.append("&mac=" + sysInfo.device.mac);
    postData.append("&power=" + QString::number(sysInfo.battery.capacity));
    postData.append("&city_id=" + sysInfo.city.cityID);
    postData.append("&firmware_version=" + sysInfo.version.firmware);
    postData.append("&system_version=" + sysInfo.version.system);
    postData.append("&status=" + status);
    request.setUrl(urlHeartbeat);

    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_heartbeat_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_heartbeat_failed()));
    connect(networkManage, SIGNAL(signal_request_failed()), this, SLOT(slot_heartbeat_failed()));
    networkManage->post_request(request,postData);
}

/*******************************************************************************
* Function Name  :  slot_upload_log
* Description    :  推送日志文件
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDebug::slot_upload_log(QString deviceID, QString mac, QString sn, QString fileName)
{
    QNetworkRequest request;
    NetworkManage *networkManage = new NetworkManage();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    // 初始化post请求地址
    QString strUpload = QString(URL_SERVER).append(QString(REQUEST_UPLOAD_LOG));

    // 初始化URL
    QUrl urlUpload = QUrl::fromEncoded(strUpload.toAscii());
    qDebug()<<strUpload<<sn<<mac<<fileName;

    // 配置post传递参数
    QByteArray postData;
    postData.append("device_id=" + deviceID);
    postData.append("&sn=" + sn);
    postData.append("&mac=" + mac);
    postData.append("&fileName=" + fileName);
    request.setUrl(urlUpload);

    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_upload_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_upload_failed()));
    connect(networkManage, SIGNAL(signal_request_failed()), this, SLOT(slot_upload_failed()));
    networkManage->post_request(request,postData);

}

/*******************************************************************************
* Function Name  :  slot_heartbeat_success
* Description    :  心跳成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDebug::slot_heartbeat_success(QString reply)
{
    emit signal_heartbeat_success();
    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<reply;
}

/*******************************************************************************
* Function Name  :  slot_heartbeat_failed
* Description    :  心跳失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDebug::slot_heartbeat_failed()
{
    emit signal_heartbeat_failed();
    qDebug()<<"##########################################";
}

/*******************************************************************************
* Function Name  :  slot_upload_success
* Description    :  推送设备日志成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDebug::slot_upload_success(QString)
{
    emit signal_upload_success();
}

/*******************************************************************************
* Function Name  :  slot_upload_failed
* Description    :  上传设备日志失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDebug::slot_upload_failed()
{
    emit signal_upload_failed();
}
