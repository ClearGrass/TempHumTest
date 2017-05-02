#include "serverTime.h"

ServerTime *ServerTime::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerTime* ServerTime::getInstance()
{
    if(NULL == instance)
    {
        instance = new ServerTime();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  ServerTime
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerTime::ServerTime(QObject *parent) : QObject(parent)
{

}

/*******************************************************************************
* Function Name  :  slot_get_sun
* Description    :  获取日升日落时间
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerTime::slot_sync_netTime(QString deviceID, QString cityID)
{
    QNetworkRequest request;
    NetworkManage *networkManage = new NetworkManage();
    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_syncTime_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_syncTime_failed()));
    connect(networkManage, SIGNAL(signal_request_failed()), this, SLOT(slot_syncTime_failed()));
    QString strSun = QString(URL_SERVER) + QString(REQUEST_SUN_TIME) + "?device_id=" + deviceID + "&city_id=" + cityID;
    qDebug()<<strSun;
    QUrl urlGetSun = QUrl::fromEncoded(strSun.toAscii());
    request.setUrl(urlGetSun);
    networkManage->get_request(request);
}

/*******************************************************************************
* Function Name  :  slot_getSun_success
* Description    :  获取日升日落时间成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerTime::slot_syncTime_success(QString replyData)
{
    qDebug()<<replyData;
    InfoNetTime netTime;

    // 将QString类型数据转换为char类型的数据
    QByteArray ba = replyData.toLatin1();
    char *cReplyData = ba.data();

    // 设置json根
    cJSON *root = cJSON_Parse(cReplyData);


    if(NULL == root)
    {
        emit signal_syncTime_failed();
        return;
    }

    // 若返回code不为0(正常)，返回
    cJSON *code = cJSON_GetObjectItem(root,"code");

    if(0 != code->valueint)
    {
        emit signal_syncTime_failed();
        return ;
    }

    cJSON *data = cJSON_GetObjectItem(root,"data");

    if(data)
    {
        netTime.dateTime = get_validString(data, "time");
        netTime.timeStamp = get_validNum(data, "timestamp");
        netTime.sunrise = get_validString(data, "sunrise");
        netTime.sunset = get_validString(data, "sunset");
        emit signal_syncTime_success(netTime);
    }
    else
    {
        emit signal_syncTime_failed();
    }
}

/*******************************************************************************
* Function Name  :  slot_getTime_failed
* Description    :  获取日升日落失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerTime::slot_syncTime_failed()
{
    // 发送获取失败信号
    emit signal_syncTime_failed();
}

/*******************************************************************************
* Function Name  :  get_validString
* Description    :  获取有效值，存在指定字段则赋值，不存在赋空值
* Input          :  cJSON *item JSON数据结点
*                :  QString field 字段名称
* Output         :  None
* Return         :  QString 有效值
*******************************************************************************/
QString ServerTime::get_validString(cJSON *item, QString field)
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
* Function Name  :  get_validNum
* Description    :  获取有效值，存在指定字段则赋值，不存在赋空值
* Input          :  cJSON *item JSON数据结点
*                :  QString field 字段名称
* Output         :  None
* Return         :  QString 有效值
*******************************************************************************/
int ServerTime::get_validNum(cJSON *item, QString field)
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

