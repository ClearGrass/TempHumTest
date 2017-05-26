#include "driverLight.h"
#include <QtDebug>

/* Private variables ---------------------------------------------------------*/
DriverLight *DriverLight::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverLight *DriverLight::getInstance()
{
    if(NULL == instance)
    {
        instance = new DriverLight();
    }
    return instance;
}

/* Private function prototypes -----------------------------------------------*/
DriverLight::DriverLight()
    : QObject()
{
    driversInit = DriversInit::getInstance();
}

int DriverLight::get_outLight()
{
#ifdef Bran_R8
    int light = 200;

    QString strFile = driversInit->get_device_light();
    QFile file(strFile);
    //     以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QString line;
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
        QTextStream stream(&file);
        line = stream.readLine();
        light = line.trimmed().toInt();
        file.close();
    }
    else
    {
        qDebug("光度文件读取失败");
    }
    return light;
#endif
}

void DriverLight::set_inLight(int light)
{

#ifdef Bran_R8
    system(qPrintable(QString("/usr/bin/qtapp/baklight %1").arg(light)));
    system(qPrintable(QString("echo %1 > /usr/bin/qtapp/light").arg(light)));

#endif


}

/*******************************************************************************
*                           陆超@2016-12-23
* Function Name  :  light_on
* Description    :  开背光
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverLight::light_on(void)
{
    mutex.lock();
#ifdef Bran_R8
    QFile file("/sys/class/lcd_ctrl/lcd_state");
    if(file.exists())
    {
        system("echo 1 > /sys/class/lcd_ctrl/lcd_state");
    }
    else
    {
        system("echo on > /sys/power/state");
    }
    isLightOn = true;
#endif
    mutex.unlock();

}// End of void DriverLight::light_on(void)

/*******************************************************************************
*                           陆超@2016-12-23
* Function Name  :  light_off
* Description    :  关背光
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverLight::light_off(void)
{
    mutex.lock();
#ifdef Bran_R8
    QFile file("/sys/class/lcd_ctrl/lcd_state");
    if(file.exists())
    {
        system("echo 0 > /sys/class/lcd_ctrl/lcd_state");
    }
    else
    {
        system("echo mem > /sys/power/state");
    }
    isLightOn = false;
#endif
    mutex.unlock();

}// End of void DriverLight::light_off(void)

/*******************************************************************************
*                           陆超@2016-12-23
* Function Name  :  light_change
* Description    :  亮灭转换
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverLight::light_change(void)
{
#ifdef Bran_R8
    if (isLightOn == true)
    {
        light_off();
    }
    else
    {
        light_on();
    }
#endif

}// End of void DriverLight::light_off(void)

/*******************************************************************************
*                           陆超@2016-12-23
* Function Name  :  is_light_on
* Description    :  获取背光状态
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool DriverLight::is_light_on(void)
{

#ifdef Bran_R8

   return isLightOn;

#endif

}// End of bool DriverLight::is_light_on(void)
