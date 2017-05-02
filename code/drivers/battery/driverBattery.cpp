#include "driverBattery.h"
#include "globalVariable.h"

DriverBattery *DriverBattery::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverBattery* DriverBattery::getInstance()
{
    if(NULL == instance)
    {
        instance = new DriverBattery();
    }
    return instance;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-08
* Function Name :   DriverBattery
* Description   :   构造函数
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
DriverBattery::DriverBattery() :
    QObject()
{
}

/*******************************************************************************
* Function Name  :  isExist
* Description    :  是否存在
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool DriverBattery::isExist()
{
    // 电池存在
    QString strFile;
    bool isExist = false;
    QFile file(DEVICE_POWER_EXIST);

    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextStream stream(&file);

        // 电池存在
        strFile = stream.readAll().trimmed();
        if(strFile.toInt())
        {
            isExist = true;
        }
        file.close();
    }
    return isExist;
}

/*******************************************************************************
* Function Name  :  get_battery_capacity
* Description    :  获取电源剩余电量
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
float DriverBattery::get_battery_capacity()
{
    QString strFile;
    float capacity = 0;
    QFile file(DEVICE_POWER_VALUE);

    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextStream stream(&file);

        // 取得 剩余电量
        strFile  = stream.readAll().trimmed();
        capacity = strFile.toFloat();
        file.close();
    }
    return capacity;
}

/*******************************************************************************
* Function Name  :  get_battery_voltage
* Description    :  获取电源电压
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
float DriverBattery::get_battery_voltage()
{
    QString strFile;
    float voltage = 0;
    QFile file(DEVICE_POWER_VOLTAGE);

    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextStream stream(&file);

        // 取得 电池电压
        strFile     = stream.readAll();
        voltage = strFile.toFloat()/1000000;
        voltage = QString::number(voltage, 'f', 2).toFloat();
        file.close();
    }
    return voltage;
}

/*******************************************************************************
* Function Name  :  get_battery_current
* Description    :  获取电源电流
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
float DriverBattery::get_battery_current()
{
    QString strFile;
    float current = 0;
    QFile file(DEVICE_POWER_CURRENT);

    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextStream stream(&file);

        // 取得 电池电流
        strFile  = stream.readAll().trimmed();

        current = (strFile.toFloat()/1000);
        file.close();
    }
    return current;
}

/*******************************************************************************
* Function Name  :  get_battery_status
* Description    :  获取电源状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
BatteryStatusType DriverBattery::get_battery_status()
{
    QString strFile;
    BatteryStatusType status = UNKNOWN;
    QFile file(DEVICE_POWER_STATUS);

    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextStream stream(&file);

        // 取得 电源状态
        strFile = stream.readAll().trimmed();
        if("Charging" == strFile)
        {
            status = CHARGING;
        }
        else if("Discharging" == strFile)
        {
            status = DISCHARGING;
        }
        else if("Full" == strFile)
        {
            status = FULL;
        }
        else if("Not Charging")
        {
            status = NOTCHARGING;
        }
        else{
            status = UNKNOWN;
        }
        file.close();
    }
    return status;
}

/*******************************************************************************
* Function Name  :  get_charging_way
* Description    :  获取充电方式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
ChargingType DriverBattery::get_charging_way()
{
    if(is_charging_ac())
    {
        return AC;
    }
    else if(is_charging_usb())
    {
        return USB;
    }
    else
    {
        return NONE;
    }
}

/*******************************************************************************
* Function Name  :  is_charging_ac
* Description    :  判断是否通过底座充电
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool DriverBattery::is_charging_ac()
{
    QFile file(DEVICE_POWER_AC);
    int ac = 0;
    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextStream stream(&file);

        // 取得 电源状态
        ac = (stream.readAll().trimmed()).toInt();
        file.close();
    }
    if(1 == ac)
    {
        return true;
    }
    return false;
}

/*******************************************************************************
* Function Name  :  is_charing_usb
* Description    :  判断是否通过usb充电
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool DriverBattery::is_charging_usb()
{
    QFile file(DEVICE_POWER_USB);
    int usb = 0;
    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextStream stream(&file);

        // 取得 电源状态
        usb = (stream.readAll().trimmed()).toInt();
        file.close();
    }
    if(1 == usb)
    {
        return true;
    }
    return false;
}
