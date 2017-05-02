#ifndef DRIVERPM25_H
#define DRIVERPM25_H
#include <QThread>
#include <QFile>
#include <QTimer>
#include <QtDebug>
#include <QTextCodec>
#include <QTextStream>
#include <QEventLoop>
#include <QMutex>
#include "dataAirDefine.h"
#include "qextserialport.h"
#include "qextserialenumerator.h"

#define DEVICE_PM25     "/dev/Boost_En"
#define UART_PM25       "ttyS1"

class DriverPM25 : public QThread
{
    Q_OBJECT
public:
    static DriverPM25 *getInstance();


    bool power_off(void);                                               // 关闭PM2.5电源

signals:
    void signal_update_dataPM25(float);
    void signal_update_dataPM5(float);
    void signal_update_dataPM10(float);

public slots:
    void slot_recive_pmData(void);
    void start_sampling();
    void stop_sampling();

private:
    explicit DriverPM25();
    void run();                                                         // 线程循环事件

    bool open_uart(const QString &uartName);                             // 打开串口
    void close_uart(void);                                               // 关闭串口
    bool power_on(void);                                                 // 打开PM2.5电源
    void data_transform(QByteArray *DataPM2_5, float *fPM2_5, float *fPM5, float *fPM10);

private:
    QextSerialPort *uartPort;                                           // 串口指针
    bool powerStatus;
    QMutex mutex;

private:
    static DriverPM25 *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DriverPM25::instance)
                delete DriverPM25::instance;
        }
    };
    static FreeInstance free;

};

#endif // DriverPM25_H
