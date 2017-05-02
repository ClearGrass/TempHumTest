#include "serverDevice.h"

ServerDevice *ServerDevice::instance = NULL;

/*******************************************************************************
* Function Name  :  ServerDevice
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerDevice::ServerDevice(QObject *parent)
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
ServerDevice *ServerDevice::getInstance()
{
    if(NULL == instance)
    {
        instance = new ServerDevice();
    }
    return instance;
}


/*******************************************************************************
* Function Name  :  init_deviceInfo
* Description    :  发送初始化设备请求
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDevice::slot_device_init(QString sn, QString mac, QString model,QString info)
{
    QNetworkRequest request;
    NetworkManage *networkManage = new NetworkManage();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    // 初始化post请求地址
    QString strInit = QString(URL_SERVER).append(QString(REQUEST_INIT_DEVICE));

    // 初始化URL
    QUrl urlInitDeviceInfo = QUrl::fromEncoded(strInit.toAscii());

    qDebug()<<"SERVER##############################################DEVICE_INIT";
    qDebug()<<strInit<<sn<<mac<<model<<info;

    // 配置post传递参数
    QByteArray postData;
    postData.append("sn="+sn+"&");
    postData.append("mac="+mac+"&");
    postData.append("model="+model + "&");
    postData.append("info=" + info);
    request.setUrl(urlInitDeviceInfo);

    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_deviceInit_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_deviceInit_failed()));
    connect(networkManage, SIGNAL(signal_request_failed()), this, SLOT(slot_deviceInit_failed()));
    networkManage->post_request(request,postData);
}

/*******************************************************************************
* Function Name  :  slot_deviceInit_success
* Description    :  设备初始化成功
* Input          :  QString replyData 返回数据
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDevice::slot_deviceInit_success(QString replyData)
{
    // 将QString类型数据转换为char类型的数据
    QByteArray ba = replyData.toLatin1();
    char *cReplyData = ba.data();
    InfoDevice deviceInfo;

    // 设置json根
    cJSON *root = cJSON_Parse(cReplyData);

    if(NULL == root)
    {
        return;
    }

    // 若返回code不为0(正常)，返回
    cJSON *code = cJSON_GetObjectItem(root,"code");

    if(0 != code->valueint)
    {
        emit signal_deviceInit_failed();
        return ;
    }

    cJSON *data = cJSON_GetObjectItem(root,"data");

    if(data)
    {
//        cJSON *location = cJSON_GetObjectItem(data,"location");

//        if(location)
//        {
//            // 获取当前时区
////            deviceInfo.cityID       = get_validString(location, "city_id");
////            deviceInfo.cityName     = get_validString(location, "name");
////            deviceInfo.timeZoneName = get_validString(location, "timezone");
////            deviceInfo.timeZoneGMT  = get_validString(location, "timezone_gmt");
//        }

        cJSON *authToken = cJSON_GetObjectItem(data,"auth_token");

        if(authToken)
        {
            deviceInfo.deviceID = get_validString(authToken,"device_id");
            deviceInfo.token.value = get_validString(authToken,"token");
            deviceInfo.token.expTime = get_validNum(authToken,"exp_time");
        }

        // 发送信号
        emit signal_deviceInit_success(deviceInfo);
    }
}

/*******************************************************************************
* Function Name  :  slot_deviceInit_failed
* Description    :  设备初始化失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDevice::slot_deviceInit_failed()
{
    emit signal_deviceInit_failed();
}

/*******************************************************************************
* Function Name  :  slot_get_token
* Description    :  获取新的token
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDevice::slot_get_token(QString deviceID,QString sn)
{
    QNetworkRequest request;
    NetworkManage *networkManage = new NetworkManage();
    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_getToken_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_getToken_failed()));
    connect(networkManage, SIGNAL(signal_request_failed()), this, SLOT(slot_getToken_failed()));

    qDebug()<<"SERVER################################################GET_TOKEN";
    QString strToken = QString(URL_SERVER) + QString(REQUEST_GET_TOKEN) + "?device_id=" + deviceID + "&sn=" + sn;
    QUrl urlGetToken = QUrl::fromEncoded(strToken.toAscii());
    request.setUrl(urlGetToken);
    networkManage->get_request(request);
}

/*******************************************************************************
* Function Name  :  slot_getToken_success
* Description    :  获取token成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDevice::slot_getToken_success(QString replyData)
{
    qDebug()<<replyData;
    Token token;

    // 将QString类型数据转换为char类型的数据
    QByteArray ba = replyData.toLatin1();
    char *cReplyData = ba.data();

    // 设置json根
    cJSON *root = cJSON_Parse(cReplyData);


    if(NULL == root)
    {
        emit signal_getToken_failed();
        return;
    }

    // 若返回code不为0(正常)，返回
    cJSON *code = cJSON_GetObjectItem(root,"code");

    if(0 != code->valueint)
    {
        emit signal_getToken_failed();
        return ;
    }

    cJSON *data = cJSON_GetObjectItem(root,"data");

    if(data)
    {
        token.value = get_validString(data, "token");
        token.expTime = get_validNum(data, "exp_time");
        emit signal_getToken_success(token);
    }
    else
    {
        emit signal_getToken_failed();
    }
}

/*******************************************************************************
* Function Name  :  slot_getToken_failed
* Description    :  获取token 失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerDevice::slot_getToken_failed()
{
    emit signal_getToken_failed();
}

/*******************************************************************************
* Function Name  :  get_validValue
* Description    :  获取有效值，存在指定字段则赋值，不存在赋空值
* Input          :  cJSON *item JSON数据结点
*                :  QString field 字段名称
* Output         :  None
* Return         :  QString 有效值
*******************************************************************************/
QString ServerDevice::get_validString(cJSON *item, QString field)
{
    QString str = QString(cJSON_Print(item));
    if(str.contains(field))
    {
        // 存在此字段
        QByteArray ba = field.toLatin1();
        char *cField = ba.data();
        return QString(cJSON_GetObjectItem(item, cField)->valuestring);
    }
    return "";
}

/*******************************************************************************
* Function Name  :  get_validValue
* Description    :  获取有效值，存在指定字段则赋值，不存在赋空值
* Input          :  cJSON *item JSON数据结点
*                :  QString field 字段名称
* Output         :  None
* Return         :  QString 有效值
*******************************************************************************/
float ServerDevice::get_validNum(cJSON *item, QString field)
{
    QString str = QString(cJSON_Print(item));
    if(str.contains(field))
    {
        // 存在此字段
        QByteArray ba = field.toLatin1();
        char *cField = ba.data();
        return cJSON_GetObjectItem(item, cField)->valuedouble;
    }
    return 0;
}

