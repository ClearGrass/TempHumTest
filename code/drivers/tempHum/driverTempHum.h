#ifndef DRIVERTEMPHUM_H
#define DRIVERTEMPHUM_H
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QStringList>
#include "dataAirDefine.h"
#include <QFileInfo>
#define DEVICE_TEMPHUM  "/sys/devices/platform/sun5i-i2c.2/i2c-2/2-0044/temp_humidity"

class DriverTempHum : public QObject
{
public:
    static DriverTempHum *getInstance();
    void update_data(float&,float&);
    void update_data(float&,float&,float&,float&);

private:

private:
    explicit DriverTempHum(QObject *parent = 0);
    static DriverTempHum *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DriverTempHum::instance)
                delete DriverTempHum::instance;
        }
    };
    static FreeInstance free;
};

#endif // DriverTEMPHUM_H
