#ifndef DRIVERTVOCCO2E_H
#define DRIVERTVOCCO2E_H

#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QStringList>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include "globalStruct.h"

#define DEVICE_TVOCCO2E     "/sys/devices/platform/sun5i-i2c.2/i2c-2/2-0058/iaq_measure"
#define FILE_IAQ_BASELINE   "/sys/devices/platform/sun5i-i2c.2/i2c-2/2-0058/iaq_baseline"
#define FILE_MEASURE_TEST   "/sys/devices/platform/sun5i-i2c.2/i2c-2/2-0058/measure_test"
#define FILE_BASELINE       "baseline.conf"

#define BASELINE_VALID_S            (7 * 24 * 3600)     // 1 week
#define CLEANING_PHASE_S            (15 * 60)             // 15 minutes
#define EARLY_OPERATION_PHASE_S     (12 * 3600)  // 12 hours
#define PERSISTANCE_INTERVAL_S      (3600)        // 1 hour



class DriverTvocCo2e : public QObject
{
    Q_OBJECT
public:
    static DriverTvocCo2e *getInstance();
    void start();
    void update_data(float&,float&);
signals:

private slots:
    void slot_store_baseline();
    void slot_iaq_init();
    void slot_stop_SGP30();


private:
    void data_init();
    void connect_init();
    void iaq_init();
    void set_baseline(QString);
    QString get_baseline();
    void store_baseline_to_disk(QString,long);
    bool load_baseline_from_disk(QString&,long&);

private:
    QTimer *timerClean;
    QTimer *timerStore;
    QTimer *timerOperation;

    long timeSave;


private:
    explicit DriverTvocCo2e(QObject *parent = 0);
    static DriverTvocCo2e *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DriverTvocCo2e::instance)
                delete DriverTvocCo2e::instance;
        }
    };
    static FreeInstance free;
};

#endif // DRIVERTVOCCO2E_H
