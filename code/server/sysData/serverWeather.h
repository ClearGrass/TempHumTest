#ifndef SERVERWEATHER_H
#define SERVERWEATHER_H
#include <QDebug>
#include <QNetworkRequest>
#include <QUrl>
#include <QTextCodec>
#include <QStringList>
#include "3rdparty/cJSON/cJSON.h"
#include "server/serverDefine.h"
#include "globalVariable.h"
#include "networkManage.h"

#define     CLEAR_DAY               "CLEAR_DAY",                                  // 晴天
#define     CLEAR_NIGHT             "CLEAR_NIGHT",                                // 晴夜
#define     PARTLY_CLOUDY_DAY       "PARTLY_CLOUDY_DAY",                          // 多云天
#define     PARTLY_CLOUDY_NIGHT     "PARTLY_CLOUDY_NIGHT",                        // 多云夜
#define     CLOUDY                  "CLOUDY",                                     // 阴
#define     RAIN                    "RAIN",                                       // 雨
#define     SNOW                    "SNOW",                                       // 雪
#define     WIND                    "WIND",                                       // 风
#define     FOG                     "FOG",                                        // 雾
#define     HAZE                    "HAZE",                                       // 霾
#define     SLEET                   "SLEET"                                       // 冻雨

typedef struct weatherDaily
{
    QString skycon;                                                     // 天气类型
    int tempMax;                                                      // 最高温度
    int tempMin;                                                      // 最低温度
}WeatherDaily;


typedef struct weatherHourly
{
    QString skycon;
    int temp;
}WeatherHourly;

class ServerWeather : public QObject
{
    Q_OBJECT
public:
    static ServerWeather* getInstance();

signals:
    void signal_getWeather7Days_success(QList<WeatherDaily>*);
    void signal_getWeather12Hours_success(QList<WeatherHourly>*);
    void signal_getWeather7Days_failed();
    void signal_getWeather12Hours_failed();

protected slots:
    void slot_request_weather(QString deviceID, QString cityID);
    void slot_getWeather_success(QString);
    void slot_getWeather_failed();
    void slot_getWeather_timeout();

private:
    explicit ServerWeather(QObject *parent = 0);
    void analyse_weather_7Days(cJSON *);                      // 解析JSON数据
    void analyse_weather_12Hours(cJSON *);                    // 解析JSON 数据
    QString get_validString(cJSON *item, QString field);
    float get_validValue(cJSON *item, QString field);

private:
    static ServerWeather* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(ServerWeather::instance)
                delete ServerWeather::instance;
        }
    };
    static FreeInstance free;
};

#endif // SERVERWEATHER_H
