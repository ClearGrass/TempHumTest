#include "serverCity.h"


ServerCity *ServerCity::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerCity *ServerCity::getInstance()
{
    if(NULL == instance)
    {
        instance = new ServerCity();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  ServerCity
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerCity::ServerCity()
    : QObject()
{
    // 网络接收类管理
    manager = new QNetworkAccessManager(this);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    timerSetCity = new QTimer();
    timerSearchCity = new QTimer();
    timerLocate = new QTimer();
    timerSetCity->setInterval(10000);
    timerLocate->setInterval(10000);
    timerSearchCity->setInterval(10000);
    timerSetCity->setSingleShot(true);
    timerLocate->setSingleShot(true);
    timerSearchCity->setSingleShot(true);
    connect(timerLocate, SIGNAL(timeout()), this, SLOT(slot_timeout_locate_device()));
    connect(timerSearchCity, SIGNAL(timeout()), this, SLOT(slot_timeout_search_city()));
    connect(timerSetCity, SIGNAL(timeout()), this, SLOT(slot_timeout_set_city()));
}


/*******************************************************************************
* Function Name  :  slot_search_city
* Description    :  发送搜索城市请求
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::slot_search_city(QString name, LanguageType language)
{
    QString lang = "zh_CN";
    if(LANG_CN == language)
    {
        lang = "zh_CN";
    }
    else if(LANG_TW == language)
    {
        lang = "zh_TW";
    }
    else
    {
        lang = "en";
    }
    QString strSearch = QString(URL_SERVER) + QString(REQUEST_SEARCH_CITY) + "?" + "name=" + name + "&lang=" + lang;
    qDebug()<<strSearch;

    // 初始化URL
    QUrl urlSearchCity = QUrl::fromEncoded(strSearch.toAscii());
    request.setUrl(urlSearchCity);
    replySearchCity = manager->get(request);
    timerSearchCity->start();
    connect(replySearchCity,SIGNAL(finished()),this,SLOT(slot_reply_search_city()));
}

/*******************************************************************************
* Function Name  :  slot_locate_device
* Description    :  发送定位请求
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::slot_locate_device(QString deviceID, QString mac, LanguageType language)
{
    QString lang = "zh_CN";
    if(LANG_CN == language)
    {
        lang = "zh_CN";
    }
    else if(LANG_TW == language)
    {
        lang = "zh_TW";
    }
    else
    {
        lang = "en";
    }
    QString strLocate = QString(URL_SERVER) + QString(REQUEST_LOCATE_DEVICE) + "?device_id=" + deviceID + "&mac=" + mac + "&lang=" + lang;
    qDebug()<<strLocate<<QThread::currentThreadId();

    // 初始化URL
    QUrl urlLocateDevice = QUrl::fromEncoded(strLocate.toAscii());
    request.setUrl(urlLocateDevice);
    replyLocateDevice = manager->get(request);
    timerLocate->start();
    connect(replyLocateDevice,SIGNAL(finished()),this,SLOT(slot_reply_locate_device()));
}

/*******************************************************************************
* Function Name  :  slot_set_city
* Description    :  发送设置城市请求
* Input          :  QString cityID 城市ID
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::slot_set_city(QString deviceID,QString cityID)
{
    // 初始化设备信息
    QByteArray postData;
    QString strSetCity = QString(URL_SERVER).append(QString(REQUEST_SET_CITY));
    qDebug()<<"Request"<<strSetCity<<deviceID<<cityID;

    // 初始化设备命令
    QUrl urlsetCity = QUrl::fromEncoded(strSetCity.toAscii());

    // 配置post传递参数
    postData.append("device_id=" + deviceID + "&");
    postData.append("city_id=" + cityID);
    request.setUrl(urlsetCity);
    replySetCity =  manager->post(request,postData);
    timerSetCity->start();
    connect(replySetCity,SIGNAL(finished()),this,SLOT(slot_reply_set_city()));
}

/*******************************************************************************
* Function Name  :  slot_reply_search_city
* Description    :  接收到服务器返回的城市列表
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::slot_reply_search_city()
{
    timerSearchCity->stop();
    if(replySearchCity && replySearchCity->error() == QNetworkReply::NoError)
    {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QByteArray data = replySearchCity->readAll();
        QString replyData;
        replyData.prepend(data);
        qDebug()<<"searchCity"<<replyData;
        analyse_search_city(replyData);
    }
    else
    {
        qDebug()<<replySearchCity->errorString();
        emit signal_searchCity_failed();
    }
}

/*******************************************************************************
* Function Name  :  slot_timeout_search_city
* Description    :  检索城市超时处理
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::slot_timeout_search_city()
{
    emit signal_searchCity_failed();
}

/*******************************************************************************
* Function Name  :  slot_reply_locate_device
* Description    :  接收到服务器返回的定位信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::slot_reply_locate_device()
{
    timerLocate->stop();
    if(replyLocateDevice && replyLocateDevice->error() == QNetworkReply::NoError)
    {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QByteArray data = replyLocateDevice->readAll();
        QString replyData;
        replyData.prepend(data);
        qDebug()<<"locate"<<replyData;
        analyse_locate_device(replyData);

    }
    else
    {
        qDebug()<<replyLocateDevice->errorString();
        emit signal_locate_failed();
    }
}

/*******************************************************************************
* Function Name  :  slot_timeout_locate_device
* Description    :  定位设备超时处理
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::slot_timeout_locate_device()
{
    emit signal_locate_failed();
}

/*******************************************************************************
* Function Name  :  slot_reply_set_city
* Description    :  接收到服务器返回的设置城市信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::slot_reply_set_city()
{
    timerSetCity->stop();
    if(replySetCity && replySetCity->error() == QNetworkReply::NoError)
    {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QByteArray data = replySetCity->readAll();
        QString replyData;
        replyData.prepend(data);

        qDebug()<<replyData;

        // 解析返回json数据
        analyse_set_city(replyData);
    }
    else
    {
        emit signal_setCity_failed();
        qDebug()<<replySetCity->errorString();
    }
}

/*******************************************************************************
* Function Name  :  slot_timeout_set_city
* Description    :  设置城市超时处理
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::slot_timeout_set_city()
{
    emit signal_setCity_failed();
}

/*******************************************************************************
* Function Name  :  analyse_search_city
* Description    :  解析对应JSON数据，并做出相关处理
* Input          :  QString replyData 服务器返回的JSON数据串
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::analyse_search_city(QString replyData)
{
    InfoCity city;

    // 将QString类型数据转换为char类型的数据
    QByteArray ba = replyData.toLatin1();
    char *cReplyData = ba.data();
    QList<InfoCity> *listCity = new QList<InfoCity>;

    // 设置json根
    cJSON *root = cJSON_Parse(cReplyData);


    if(NULL == root)
    {
        return;
    }

    // 若返回code不为0(正常)，返回
    cJSON *code = cJSON_GetObjectItem(root,"code");

    if(10101 == code->valueint)
    {
        emit signal_searchCity_success(listCity);
        return;
    }
    else if(0 != code->valueint)
    {
        qDebug()<<replyData;
        emit signal_searchCity_failed();
        return ;
    }

    cJSON *data = cJSON_GetObjectItem(root,"data");

    if(data)
    {
        cJSON *device = cJSON_GetObjectItem(data,"device");
        if(device)
        {
            // 获取城市个数
            int size = cJSON_GetArraySize(device);

            for(int i = 0; i < size; ++i)
            {
                // 循环解析
                cJSON *deviceItem = cJSON_GetArrayItem(device,i);
                if(deviceItem)
                {
                    char *everyCity= cJSON_Print(deviceItem);
                    qDebug()<<QString(everyCity);

                    cJSON *cityItem = cJSON_Parse(everyCity);
                    city.cityID      = get_validValue(cityItem,"city_id");
                    city.name        = get_validValue(cityItem,"name");
                    city.nameCN      = get_validValue(cityItem,"name_cn");
                    city.nameEN      = get_validValue(cityItem,"name_en");
                    city.nameTW      = get_validValue(cityItem,"name_cn_tw");
                    city.country     = get_validValue(cityItem,"country");
                    city.countryCode = get_validValue(cityItem,"country_code");
                    city.area        = get_validValue(cityItem,"area_first");
                    city.timeZone    = get_validValue(cityItem,"timezone");
                    city.cityGrade   = get_validValue(cityItem,"city_grade");
                    city.timeZoneGMT = get_validValue(cityItem,"timezone_gmt");
                    listCity->append(city);
                }
            }
            // 将结果反馈出去
            emit signal_searchCity_success(listCity);
        }
    }
}


/*******************************************************************************
* Function Name  :  analyse_locate_device
* Description    :  解析对应JSON数据，并做出相关处理
* Input          :  QString replyData 服务器返回的JSON数据串
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::analyse_locate_device(QString replyData)
{
    // 将QString类型数据转换为char类型的数据
    QByteArray ba = replyData.toLatin1();
    char *cReplyData = ba.data();
    InfoCity city;
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
        emit signal_locate_failed();
        return ;
    }

    cJSON *data = cJSON_GetObjectItem(root,"data");

    if(data)
    {
        city.cityID      = get_validValue(data,"city_id");
        city.name        = get_validValue(data,"name");
        city.nameCN      = get_validValue(data,"name_cn");
        city.nameEN      = get_validValue(data,"name_en");
        city.nameTW      = get_validValue(data,"name_cn_tw");
        city.country     = get_validValue(data,"country");
        city.countryCode = get_validValue(data,"country_code");
        city.area        = get_validValue(data,"area_first");
        city.timeZone    = get_validValue(data,"timezone");
        city.cityGrade   = get_validValue(data,"city_grade");
        city.timeZoneGMT = get_validValue(data,"timezone_gmt");
        city.time        = get_validValue(data,"time");

        // 将结果反馈出去
        emit signal_locate_success(city);
    }
}

/*******************************************************************************
* Function Name  :  analyse_set_city
* Description    :  解析对应JSON数据，并做出相关处理
* Input          :  QString replyData 服务器返回的JSON数据串
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerCity::analyse_set_city(QString replyData)
{
    InfoCity city;

    // 将QString类型数据转换为char类型的数据
    QByteArray ba = replyData.toLatin1();
    char *cReplyData = ba.data();

    // 设置json根
    cJSON *root = cJSON_Parse(cReplyData);


    if(NULL == root)
    {
        emit signal_setCity_failed();
        return;
    }

    // 若返回code不为0(正常)，返回
    cJSON *code = cJSON_GetObjectItem(root,"code");

    if(0 != code->valueint)
    {
        emit signal_setCity_failed();
        return ;
    }

    cJSON *data = cJSON_GetObjectItem(root,"data");

    if(data)
    {

        cJSON *device = cJSON_GetObjectItem(data,"devices");

        int size = cJSON_GetArraySize(device);
        int i = 0;
        cJSON *deviceItem;

        for(i=0; i< size; i++)
        {
            deviceItem = cJSON_GetArrayItem(device,0);

            if(deviceItem)
            {
                char *everyCity= cJSON_Print(deviceItem);
//                qDebug()<<QString(everyCity);

//                cJSON *cityItem = cJSON_Parse(everyCity);

//                city.cityID      = get_validValue(cityItem,"city_id");
//                city.name        = get_validValue(cityItem,"name");
//                city.country     = get_validValue(cityItem,"country");
//                city.countryCode = get_validValue(cityItem,"country_code");
//                city.area        = get_validValue(cityItem,"area_cn_first");
//                city.timeZone    = get_validValue(cityItem,"timezone");
//                city.cityGrade   = get_validValue(cityItem,"city_grade");
//                city.timeZoneGMT = get_validValue(cityItem,"timezone_gmt");

                // 将结果反馈出去
                emit signal_setCity_success(city);
            }

        }
    }
    if (root)
        cJSON_Delete(root);
}

/*******************************************************************************
* Function Name  :  get_validValue
* Description    :  获取有效值，存在指定字段则赋值，不存在赋空值
* Input          :  cJSON *item JSON数据结点
*                :  QString field 字段名称
* Output         :  None
* Return         :  QString 有效值
*******************************************************************************/
QString ServerCity::get_validValue(cJSON *item, QString field)
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
