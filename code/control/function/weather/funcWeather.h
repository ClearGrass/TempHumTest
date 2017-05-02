#ifndef FUNCWEATHER_H
#define FUNCWEATHER_H

#include <QThread>
#include <QDateTime>
#include "server/sysData/serverWeather.h"
#include "globalVariable.h"
#include "wilddog/sysData/wilddogWeather.h"
#include "data/sysData.h"
#include "file/config/fileConfig.h"
#include "device/sysDevice.h"
#include "sysTime.h"

#define WEATHER_KEEP_DURATION   (30*60*1000)                          // ３０分钟

class FuncWeather : public QThread
{
    Q_OBJECT
public:
    static FuncWeather *getInstance();                                   // 获取单例

signals:
    void signal_update_weatherDaily(QList<WeatherDaily> *);             // 12小时数据
    void signal_update_weatherHourly(QList<WeatherHourly> *);           // 7天数据
    void signal_update_weather(InfoWeather);                                // 刷新当前天气信号

    void signal_sampling_weather();
    void signal_observe_weather(QString cityID);
    void signal_request_weather(QString deviceID, QString cityID);
    void signal_update_cityID(QString);
    void signal_net_abnormal();                                         // 网络异常
    void signal_net_disconnect();                                       // 网络断开


public slots:
    void slot_request_weather();

    void slot_update_unitTEMP(UnitType);
    void slot_update_infoDevice(InfoDevice);
    void slot_update_time(TimeType);

    void slot_update_cityID(QString);
    void slot_update_weatherDaily(QList<WeatherDaily>*);
    void slot_update_weatherHourly(QList<WeatherHourly>*);
    void slot_getWeather_failed();

    void slot_update_weather(InfoWeather);
    void slot_observe_failed();
    void slot_weather_invalid();                                        // 当前天气失效

    void slot_net_connected();
    void slot_net_disconnect();

private:
    void data_init();
    void connect_init();                                                // 槽连接初始化
    explicit FuncWeather(QObject *parent = 0);

private:
    WilddogWeather *wilddogWeather;
    ServerWeather *serverWeather;                                       // 与服务器接口
    SysData   *sysData;

    FileConfig *fileConfig;
    SysDevice *sysDevice;
    SysTime     *sysTime;

    QString cityID;                                                     // 城市ID
    QString deviceID;                                                   // 设备ID

    QDate lastDate;
    QDateTime lastDateTime;
    QList<WeatherDaily> *listWeatherDaily;
    QList<WeatherHourly> *listWeatherHourly;
    InfoWeather currentWeather;

    bool isConnected;
    bool isDeviceInit;
    bool isObserving;

    QTimer *timerKeep;
    QTimer *timerObserve;

    UnitType unitTEMP;

private:
    static FuncWeather *instance;                                        // 单例模式声明及释放过程
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(FuncWeather::instance)
                delete FuncWeather::instance;
        }
    };
    static FreeInstance free;
};

#endif // FUNCWEATHER_H
