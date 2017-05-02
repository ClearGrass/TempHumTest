#include <QThread>
#include "serverWeather.h"

ServerWeather *ServerWeather::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerWeather *ServerWeather::getInstance()
{
    if(NULL == instance)
    {
        instance = new ServerWeather();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  ServerWeather
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ServerWeather::ServerWeather(QObject *parent) : QObject(parent)
{
}

/*******************************************************************************
* Function Name  :  slot_request_weather
* Description    :  获取７天天气信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerWeather::slot_request_weather(QString deviceID, QString cityID)
{
    QNetworkRequest request;
    NetworkManage *networkManage = new NetworkManage();
    connect(networkManage, SIGNAL(signal_request_success(QString)), this, SLOT(slot_getWeather_success(QString)));
    connect(networkManage, SIGNAL(signal_request_timeout()), this, SLOT(slot_getWeather_timeout()));
    connect(networkManage, SIGNAL(signal_request_failed()), this, SLOT(slot_getWeather_failed()));

    QString strWeather = QString(URL_SERVER) + QString(REQUEST_WEATHER_FORECAST) + "?device_id=" + deviceID + "&city_id=" + cityID;

    // 初始化URL
    QUrl urlWeather = QUrl::fromEncoded(strWeather.toAscii());
    request.setUrl(urlWeather);

    // 自定义网络管理器发送数据
    networkManage->get_request(request);

}


/*******************************************************************************
* Function Name  :  slot_getWeather7Days_success
* Description    :  获取天气数据成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerWeather::slot_getWeather_success(QString replyData)
{
    qDebug()<<replyData;

    QByteArray ba;
    char *cReplyData = NULL;
    cJSON *root      = NULL;
    cJSON *code      = NULL;
    cJSON *data      = NULL;
    cJSON *daily     = NULL;
    cJSON *hourly    = NULL;

    // 将QString类型数据转换为char类型的数据
    ba = replyData.toLatin1();
    cReplyData = ba.data();

    // 设置json根
    root = cJSON_Parse(cReplyData);

    if(NULL == root)
    {
        return;
    }

    // 若返回code不为0(正常)，返回
    code = cJSON_GetObjectItem(root,"code");

    if(0 != code->valueint)
    {
        // 获取天气预报失败
        emit signal_getWeather7Days_failed();
        emit signal_getWeather12Hours_failed();
        return ;
    }

    data = cJSON_GetObjectItem(root,"data");

    if(data)
    {
        // 7天天气预报
        daily = cJSON_GetObjectItem(data,"daily");

        // 若有数据
        if(daily)
        {

            analyse_weather_7Days(daily);
        }
        else
        {
            emit signal_getWeather7Days_failed();
        }

        // 12小时天气预报
        hourly = cJSON_GetObjectItem(data,"hourly");

        // 若有数据
        if(hourly)
        {
            analyse_weather_12Hours(hourly);
        }
        else
        {
            emit signal_getWeather12Hours_failed();
        }

        // 发送信号
    }
}

/*******************************************************************************
* Function Name  :  slot_getWeather_failed
* Description    :  获取天气数据失败
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerWeather::slot_getWeather_failed()
{
    // 发送获取天气数据失败
    emit signal_getWeather7Days_failed();
    emit signal_getWeather12Hours_failed();
}

/*******************************************************************************
* Function Name  :  slot_getWeather_timeout
* Description    :  获取７天天气数据超时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerWeather::slot_getWeather_timeout()
{
    // 发送获取天气数据失败
    emit signal_getWeather7Days_failed();
    emit signal_getWeather12Hours_failed();
}

/*******************************************************************************
* Function Name  :  analyse_weather_forecast
* Description    :  分析７天数据天气数据
* Input          :  QString replyData 请求返回数据
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerWeather::analyse_weather_7Days(cJSON *daily)
{
    int i,size;
    char *everyDay     = NULL;
    cJSON *dayItem     = NULL;
    cJSON *wfDay       = NULL;
    cJSON *skycon      = NULL;
    cJSON *temperature = NULL;
    QString date;
    QList<WeatherDaily> *weather7Days = new QList<WeatherDaily>;
    WeatherDaily weather;

    // 获取天气个数
    size = cJSON_GetArraySize(daily);

    for(i = 0; i < size; ++i)
    {
        // 循环解析
        dayItem = cJSON_GetArrayItem(daily,i);
        if(dayItem)
        {
            everyDay = cJSON_Print(dayItem);
            wfDay    = cJSON_Parse(everyDay);
            date = get_validString(wfDay,"date");
            skycon = cJSON_GetObjectItem(wfDay, "skycon");
            weather.skycon = get_validString(skycon, "value");
            temperature = cJSON_GetObjectItem(wfDay, "temperature");
            weather.tempMax = get_validValue(temperature, "max");
            weather.tempMin = get_validValue(temperature, "min");
            weather7Days->append(weather);
        }
    }
    emit signal_getWeather7Days_success(weather7Days);
}

/*******************************************************************************
* Function Name  :  analyse_weather_24Hours
* Description    :  分析24小时数据
* Input          :  QString replyData 请求返回数据
* Output         :  None
* Return         :  None
*******************************************************************************/
void ServerWeather::analyse_weather_12Hours(cJSON *hourly)
{
    int i,size;
    cJSON *hourItem    = NULL;
    char *everyHour    = NULL;
    cJSON *wfHour      = NULL;
    cJSON *skycon      = NULL;
    cJSON *temperature = NULL;
    size = cJSON_GetArraySize(hourly);
    QList<WeatherHourly> *weather12Hours = new QList<WeatherHourly>;
    WeatherHourly weather;

    for(i = 0; i < size; ++i)
    {
        hourItem = cJSON_GetArrayItem(hourly, i);
        if(hourItem)
        {
            everyHour = cJSON_Print(hourItem);
            wfHour    = cJSON_Parse(everyHour);
            skycon = cJSON_GetObjectItem(wfHour, "skycon");
            weather.skycon = get_validString(skycon, "value");
            temperature = cJSON_GetObjectItem(wfHour, "temperature");
            weather.temp = get_validValue(temperature, "value");
            weather12Hours->append(weather);
        }
    }
    emit signal_getWeather12Hours_success(weather12Hours);

}

/*******************************************************************************
* Function Name  :  get_validValue
* Description    :  获取有效值，存在指定字段则赋值，不存在赋空值
* Input          :  cJSON *item JSON数据结点
*                :  QString field 字段名称
* Output         :  None
* Return         :  QString 有效值
*******************************************************************************/
QString ServerWeather::get_validString(cJSON *item, QString field)
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
float ServerWeather::get_validValue(cJSON *item, QString field)
{
    QString str = QString(cJSON_Print(item));
    if(str.contains(field))
    {
        // 存在此字段
        QByteArray ba = field.toLatin1();
        char *cField = ba.data();
        return cJSON_GetObjectItem(item, cField)->valuedouble;
    }
    return 9999;
}




