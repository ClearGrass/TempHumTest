#ifndef DRIVERGRAVITY_H
#define DRIVERGRAVITY_H

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QtDebug>
#include <QMutex>

#define DEVICE_GRAVITY  "/sys/devices/platform/sun5i-i2c.2/i2c-2/2-0019/get_xyz"

class DriverGravity : public QObject
{
    Q_OBJECT
public:
    static DriverGravity *getInstance();
    int get_click(void);                                                // 获取按键状态

public slots:
    void sampling_gravity();
    int get_x();
    int get_y();
    int get_z();

private:
    explicit DriverGravity();

private:
    int x;
    int y;
    int z;
    int click;                                                          // 按键状态
    QMutex mutex;

private:
    static DriverGravity *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(DriverGravity::instance)
                delete DriverGravity::instance;
        }
    };
    static FreeInstance free;

};

#endif // DriverGravity_H
