#include "serverApp.h"

/*******************************************************************************
* Function Name  :  ServerApp
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerApp::ServerApp(QObject *parent) : QObject(parent)
{
    // 网络接收类管理
    manager = new QNetworkAccessManager();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    deviceID = "1_9B366";

}

/*******************************************************************************
* Function Name  :  get_pairCode
* Description    :  获取配对码
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerApp::get_pairCode()
{
    // 初始化GET请求地址
    QString strGet = QString(URL_SERVER) + QString(REQUEST_GET_PAIR_CODE) + "?device_id = " + deviceID;

    // 初始化URL
    QUrl urlGetPairCode = QUrl::fromEncoded(strGet.toAscii());
    request.setUrl(urlGetPairCode);
    replyGetPairCode =  manager->get(request);
    connect(replyGetPairCode,SIGNAL(finished()),this,SLOT(slot_reply_get_pairCode()));

}

/*******************************************************************************
* Function Name  :  slot_reply_get_pairCode
* Description    :  获取配对码的返回信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerApp::slot_reply_get_pairCode()
{
    if(replyGetPairCode && replyGetPairCode->error() == QNetworkReply::NoError)
    {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QByteArray data = replyGetPairCode->readAll();
        QString replyData;
        replyData.prepend(data);

        // 解析JSON数据
        analyse_get_pairCode(replyData);
    }
    else
    {
        qDebug()<<replyGetPairCode->errorString();
    }
}

/*******************************************************************************
* Function Name  :  cancel_pair
* Description    :  取消配对
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerApp::cancel_pair()
{
    // 初始化POST请求地址
    QString strCancel = QString(URL_SERVER).append(QString(REQUEST_CANCEL_PAIR));

    // 初始化URL
    QUrl urlCancelPair = QUrl::fromEncoded(strCancel.toAscii());

    // 配置post传递参数
    QByteArray postData;
    postData.append("device_id ="+ deviceID);
    request.setUrl(urlCancelPair);
    replyCancelPair =  manager->post(request,postData);
    connect(replyCancelPair,SIGNAL(finished()),this,SLOT(slot_reply_cancel_pair()));
}

/*******************************************************************************
* Function Name  :  slot_reply_cancel_pair
* Description    :  取消配对的返回信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerApp::slot_reply_cancel_pair()
{
    if(replyCancelPair && replyCancelPair->error() == QNetworkReply::NoError)
    {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QByteArray data = replyCancelPair->readAll();
        QString replyData;
        replyData.prepend(data);

        // 解析JSON数据
        analyse_cancel_pair(replyData);
        qDebug()<<replyData;
    }
    else
    {
        qDebug()<<replyCancelPair->errorString();
    }
}

/*******************************************************************************
* Function Name  :  analyse_get_piarCode
* Description    :  解析对应JSON数据，并做出相关处理
* Input          :  QString replyData 服务器返回的JSON数据串
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerApp::analyse_get_pairCode(QString replyData)
{
    // 将QString类型数据转换为char类型的数据
    QByteArray ba = replyData.toLatin1();
    char *cReplyData = ba.data();

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
        return ;
    }

    cJSON *data = cJSON_GetObjectItem(root,"data");

    if(data)
    {
        int pairCode = cJSON_GetObjectItem(data,"pair_code")->valueint;

//        cJSON *pairType = cJSON_GetObjectItem(data,"pair_type");
//        cJSON *expTime = cJSON_GetObjectItem(data,"exp_time");
        emit signal_feedback_pairCode(pairCode);
    }
}

/*******************************************************************************
* Function Name  :  analyse_cancel_pair
* Description    :  解析对应JSON数据，并做出相关处理
* Input          :  QString replyData 服务器返回的JSON数据串
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerApp::analyse_cancel_pair(QString replyData)
{
    // 将QString类型数据转换为char类型的数据
    QByteArray ba = replyData.toLatin1();
    char *cReplyData = ba.data();

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
        return ;
    }

    cJSON *data = cJSON_GetObjectItem(root,"data");

    if(data)
    {
    }
}

/*******************************************************************************
* Function Name  :  get_validValue
* Description    :  获取有效值，存在指定字段则赋值，不存在赋空值
* Input          :  cJSON *item JSON数据结点
*                :  QString field 字段名称
* Output         :  None
* Return         :  QString 有效值
*******************************************************************************/
QString ServerApp::get_validValue(cJSON *item, QString field)
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
