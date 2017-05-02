#ifndef FUNCCITY_H
#define FUNCCITY_H


#include <QThread>
#include <QHostInfo>
#include <QTimer>
#include "globalVariable.h"
#include "server/sysData/serverCity.h"
#include "file/config/fileConfig.h"
#include "device/sysDevice.h"
#include "sysTime.h"
#include "sysLanguage.h"
#include "wilddog/sysData/wilddogCity.h"

#define RETRY_LOCATE_INTERVAL  3600000                                  // 暂定一小时

class FuncCity : public QThread
{
    Q_OBJECT
public:
    static FuncCity *getInstance();                                  // 获取单例
    InfoCity get_currentCity();
    QString get_locationAuto();                                      // 获取当前选中地点
    QString get_locationManual();
    QString get_location();
    bool get_locationMode();                                         // 获取当前模式

signals:
    void signal_locate_device(QString,QString,LanguageType);
    void signal_search_city(QString,LanguageType);
    void signal_set_city(QString,QString);

    void signal_update_infoCity(InfoCity);
    void signal_update_cityID(QString);
    void signal_update_cityName(QString);
    void signal_locate_success(QString);                                 // 刷新当前地点
    void signal_locate_failed();
    void signal_searchCity_success(QList<InfoCity>*);                        // 反馈城市列表
    void signal_net_abnormal();                                         // 网络异常
    void signal_net_disconnect();                                       // 网络断开
    void signal_sync_time(QString,QString);

private slots:
    void slot_search_city(QString name);
    void slot_set_cityAuto();
    void slot_set_cityManual(QString);

    void slot_net_connected();                                          // 网络连接成功
    void slot_net_disconnect();                                         // 网络连接失败

    void slot_locate_success(InfoCity);                                      // 返回此时定位城市信息
    void slot_locate_failed();                                           // 错误情况下，接收函数

    void slot_searchCity_success(QList<InfoCity>*);                          // 返回此时检索到的城市列表
    void slot_searchCity_failed();                                      // 错误情况下，接收函数

    void slot_setCity_success(InfoCity);                                      // 返回此时设置的城市信息
    void slot_setCity_failed();                                          // 错误情况下，接收函数

    void slot_retry_locate();                                           // 重试刷新定位
    void slot_update_infoDevice(InfoDevice);
    void slot_retry_setCity();
    void slot_update_language(LanguageType);

private:
    void run();
    void data_init();
    void connect_init();                                                // 槽连接初始化
    void update_location();
    explicit FuncCity();

private:
    ServerCity *serverCity;                                             // 获取城市信息类
    WilddogCity *wilddogCity;

    SysDevice   *sysDevice;
    SysTime     *sysTime;
    SysLanguage *sysLanguage;
    FileConfig *fileConfig;

    QList<InfoCity> *listCity;

    InfoCity    cityAuto;
    InfoCity    cityManual;
    InfoCity    cityCurrent;

    LanguageType language;

    QTimer *timerRetry;
    QTimer *timerSetCity;

    QString mac;                                                        // 设备MAC地址
    QString deviceID;                                                   // 设备ID
    bool isConnected;
    bool isDeviceInit;
    bool isCityAuto;
    bool isManual;




private:
    static FuncCity *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(FuncCity::instance)
                delete FuncCity::instance;
        }
    };
    static FreeInstance free;
};
#endif // FUNCCITY_H
