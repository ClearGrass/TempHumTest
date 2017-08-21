#ifndef DATATEMPHUMI_H
#define DATATEMPHUMI_H
#include <QThread>
#include <QTimer>
#include <QDateTime>
#include "driverTempHum.h"
#include "dataAirDefine.h"
#include "globalVariable.h"


/*******************************************************************************
* Description   :   温湿度数据类，包含”传感器取值“/“数据库交互”/”服务器交互“/”前台页面交互“，
*                   继承QThread 独立线程
*
*******************************************************************************/
class DataTempHumi : public QThread
{
    Q_OBJECT
public:
    static DataTempHumi *getInstance();                                 // 获取温湿度单例

signals:
    void signal_update_data(AirData);
    void signal_update_dataDaily(AirDataDaily);
    void signal_update_rawData(float rawTemp, float rawhum);           //原始值
    void signal_update_tempFlag(int status_charging_on, int status_CPU_load,int status_CPU_f,int status_CPU_load_CPU_f, int status_LCD_bri);
protected slots:
    void slot_update_data();                                            // 刷新数值
    void slot_dataDaily_today(AirDataDaily);

private:
    explicit DataTempHumi(QObject *parent = 0);
    void run();                                                         // 线程循环事件
    void data_init();                                                   // 数据初始化
    void connect_init();                                                // 信号槽初始化

private:
    DriverTempHum *driverTempHumi;                                      // 温湿度传感器类

    QTimer *timerUpdate;

    AirData dataHUMI;
    AirData dataTEMP;

    AirDataDaily dataDailyHUMI;
    AirDataDaily dataDailyTEMP;

    float maxTemp;
    float minTemp;
    float maxHumi;
    float minHumi;

    int index;

    QStringList listDataTemp;
    QStringList listDataHumi;

private:
    static DataTempHumi* instance;                                      // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DataTempHumi::instance)
                delete DataTempHumi::instance;
        }
    };
    static FreeInstance free;

};

#endif // DATATEMPHUMI_H
