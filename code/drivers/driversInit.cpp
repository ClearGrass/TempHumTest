#include "driversInit.h"
#include <QDebug>

DriversInit *DriversInit::instance = 0;

DriversInit *DriversInit::getInstance()
{
    if(0 == instance)
    {
        instance = new DriversInit();
    }
    return instance;
}

DriversInit::DriversInit()
{
    deviceLight = QString(DEVICE_LIGHT_2_5);
    deviceButton = QString(BUTTON_FILE_2_5);
    drivers_init();
}


void DriversInit::drivers_init()
{
    QFile file(DRIVER_FILE);
    QString strFile;
    QStringList listDrivers;
    QStringList listOptions;
    int i,j;
    int value;
    QString path;
    if (file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);

        //读取文件内容
        strFile = stream.readAll();
        listDrivers = strFile.split("\n\n");

        for(; i < listDrivers.length(); ++i)
        {
            if(listDrivers.at(i).contains("bh1721"))
            {
                listOptions = listDrivers.at(i).split("\n");
                for(j = 0; j < listOptions.length(); ++j)
                {
                    if(listOptions.at(j).contains("Sysfs"))
                    {
                        path = (listOptions.at(j).split("=")).value(1);
                        deviceLight = QString( "/sys%1/light_val").arg(path);
                        qDebug()<<deviceLight<<"!!!!!!!!!!!!!!!!!!!!!!!!!!";
                    }
                }
            }
            else if(listDrivers.at(i).contains("axp20-supplyer"))
            {
                listOptions = listDrivers.at(i).split("\n");
                for(j = 0; j < listOptions.length(); ++j)
                {
                    if(listOptions.at(j).contains("Handlers") && listOptions.at(j).contains("event"))
                    {
                        value = (listOptions.at(j).split("event")).value(1).toInt();
                        deviceButton = QString("/dev/input/event%1").arg(value);
                    }
                }
            }
        }
        //关闭文件
        file.close();
    }
    else
    {
        qDebug() << file.errorString();
        return;
    }

}

QString DriversInit::get_device_light()
{
    return deviceLight;
}

QString DriversInit::get_device_button()
{
    return deviceButton;
}
