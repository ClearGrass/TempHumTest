#ifndef DRIVERLIGHT_H
#define DRIVERLIGHT_H
#include <QFile>
#include <QMutex>
#include <QProcess>
#include <QTextCodec>
#include <QTextStream>
#include <QEventLoop>
#include "driversInit.h"

#define DEVICE_LIGHT    "/sys/devices/virtual/input/input2/light_val"

class DriverLight : public QObject
{
    Q_OBJECT
public:
    static DriverLight* getInstance();                                  // 获取单例
    void light_on(void);                                                // 开关背光灯
    void light_off(void);                                               // 关背光
    void light_change(void);                                            // 背光变换
    bool is_light_on(void);                                             // 获取背光状态

public slots:
    int get_outLight(void);
    void set_inLight(int light);

private:
    DriverLight();                                                       // 构造函数

private:
    QMutex mutex;
    DriversInit *driversInit;
    bool isLightOn;                                                      // 背光灯状态

private:
    static DriverLight *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DriverLight::instance)
                delete DriverLight::instance;
        }
    };
    static FreeInstance free;


};

#endif // DriverLight_H
