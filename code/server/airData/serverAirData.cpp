#include "serverAirData.h"

ServerAirData *ServerAirData::instance = NULL;

ServerAirData::ServerAirData(QObject *parent) : QObject(parent)
{
}

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerAirData *ServerAirData::getInstance()
{
    if(NULL == instance)
    {
        instance = new ServerAirData();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  push_historyData
* Description    :  推送历史数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerAirData::slot_push_data(QString deviceID, QString datas, QString daily, QString cityID)
{
    QNetworkRequest request;
    NetworkManage *networkManage = new NetworkManage();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QString strReport = QString(URL_SERVER).append(QString(REQUEST_DATA_REPORT));
    // 初始化URL
    QUrl urlReport = QUrl::fromEncoded(strReport.toAscii());

    qDebug()<<strReport;

    // 配置post传递参数
    QByteArray postData;
    postData.append("device_id="+deviceID);
    if(datas != "")
    {
        postData.append("&datas=" + datas);
    }
    if(daily != "")
    {
        postData.append("&daily=" + daily);
    }
    if(cityID != "")
    {
        postData.append("&city_id=" + cityID);
    }

    request.setUrl(urlReport);

    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_pushData_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_pushData_timeout()));
    connect(networkManage, SIGNAL(signal_request_failed()), this, SLOT(slot_pushData_failed()));

    // 自定义网络管理器发送数据
    networkManage->post_request(request, postData);
}

/*******************************************************************************
* Function Name  :  slot_pushData_success
* Description    :  推送历史数据成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerAirData::slot_pushData_success(QString replyData)
{
    // 成功后返回成功信号
    emit signal_pushData_success();
}

/*******************************************************************************
* Function Name  :  slot_pushData_failed
* Description    :  推送历史数据失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerAirData::slot_pushData_failed()
{
    // 失败后重复写入
    emit signal_pushData_failed();
}

/*******************************************************************************
* Function Name  :  slot_pushData_timeout
* Description    :  推送历史数据超时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerAirData::slot_pushData_timeout()
{
    // 推送数据超时
    emit signal_pushData_failed();
}
