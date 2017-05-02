#ifndef DRIVERSINIT_H
#define DRIVERSINIT_H
#include <QFile>
#include <QTextStream>
#include <QStringList>

#define DRIVER_FILE "/proc/bus/input/devices"

#define BUTTON_FILE_2_5     "/dev/input/event3"                         // 按键文件
#define BUTTON_FILE_2_0     "/dev/input/event4"                         // 按键文件

#define DEVICE_LIGHT_2_5    "/sys/devices/virtual/input/input2/light_val"
#define DEVICE_LIGHT_2_0    "/sys/devices/virtual/input/input3/light_val"
class DriversInit
{
public:
    static DriversInit* getInstance();
    QString get_device_light();
    QString get_device_button();

private:
    DriversInit();
    void drivers_init();

private:
    QString deviceLight;
    QString deviceButton;
    static DriversInit *instance;
};

#endif // DRIVERSINIT_H
