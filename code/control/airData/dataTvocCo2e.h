#ifndef DATATVOCCO2E_H
#define DATATVOCCO2E_H
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QStringList>
#include <QDateTime>
#include "tvocCo2e/driverTvocCo2e.h"
#include "dataAirDefine.h"
#include "globalVariable.h"

/*******************************************************************************
* Description   :   tVOC&CO2e数据类，包含”传感器取值“/“数据库交互”/”服务器交互“/”前台页面交互“，
*                   继承QThread 独立线程
*
*******************************************************************************/
class DataTvocCo2e : public QThread
{
    Q_OBJECT
public:
    static DataTvocCo2e *getInstance();                                 // 获取温湿度单例

signals:
    void signal_update_data(AirData);
    void signal_update_dataDaily(AirDataDaily);

protected slots:
    void slot_update_data();                                            // 刷新数值
    void slot_dataDaily_today(AirDataDaily);

private:
    explicit DataTvocCo2e(QObject *parent = 0);
    void run();                                                         // 线程循环事件
    void data_init();                                                   // 数据初始化
    void connect_init();                                                // 信号槽初始化

    void sgp_set_iaq_baseline(QString);
    QString sgp_get_iaq_baseline();

private:
    AirData dataTVOC;
    AirData dataCO2E;

    AirDataDaily dataDailyTVOC;
    AirDataDaily dataDailyCO2E;

    QTimer *timerUpdate;                                                // 更新数据定时器

    int index;
    QStringList listDataTvoc;
    QStringList listDataCo2e;

    float maxTvoc;
    float minTvoc;
    float maxCo2e;
    float minCo2e;

    DriverTvocCo2e *driverTvocCo2e;

private:
    static DataTvocCo2e* instance;                                      // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DataTvocCo2e::instance)
                delete DataTvocCo2e::instance;
        }
    };
    static FreeInstance free;
};

#endif // DATATVOCCO2E_H
