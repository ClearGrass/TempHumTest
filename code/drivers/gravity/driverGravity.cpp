#include "driverGravity.h"


DriverGravity *DriverGravity::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverGravity *DriverGravity::getInstance()
{
    if(NULL == instance)
    {
        instance = new DriverGravity();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  DriverGravity
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverGravity::DriverGravity()
    : QObject()
{
    x     = 0;
    y     = 0;
    click = 0;
}

/*******************************************************************************
* Function Name  :  sampling_gravity
* Description    :  采集传感器数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverGravity::sampling_gravity()
{
    mutex.lock();
#ifdef Bran_R8
    static int i;
    QStringList listStr1;

    // 打开文件
    QFile file(DEVICE_GRAVITY);
    if (file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);
        QString strReadLine;

        //读取文件内容
        strReadLine = stream.readLine();
        listStr1    = strReadLine.split(";");
        click       = 0;

        if(3 == listStr1.length())
        {
            x = (listStr1.at(0)).split("=").at(1).trimmed().toInt();
            y = (listStr1.at(1)).split("=").at(1).trimmed().toInt();
            z = (listStr1.at(2)).split("=").at(1).trimmed().toInt();
        }
        else if(5 == listStr1.length())
        {
            x     = (listStr1.at(0)).split("=").at(1).trimmed().toInt();
            y     = (listStr1.at(1)).split("=").at(1).trimmed().toInt();
            z     = (listStr1.at(2)).split("=").at(1).trimmed().toInt();
            click = (listStr1.at(3)).split("=").at(1).trimmed().toInt();
        }

        if (i++ >= 100)
        {
            i = 0;
            qDebug("x=%d y=%d z=%d click=0x%02X length=%d", x, y, z, click, listStr1.length());
        }
        //关闭文件
        file.close();
    }
    else
    {
//        qDebug() << file.errorString();
        return;
    }
#endif
    mutex.unlock();

}

/*******************************************************************************
* Function Name  :  get_x
* Description    :  获取X值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
int DriverGravity::get_x()
{
    return x;
}

/*******************************************************************************
* Function Name  :  get_y
* Description    :  获取Y值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
int DriverGravity::get_y()
{
    return y;
}

int DriverGravity::get_z()
{
    return z;
}

/*******************************************************************************
*                           陆超@2016-12-22
* Function Name :   get_click
* Description   :   获取按键状态
* Input         :   None
* Output        :   None
* Return        :   1 有双击 0 无双击
*******************************************************************************/
int DriverGravity::get_click(void)
{
    // 出现双击
    if (click & 0x60)
    {
        qDebug("-------------------------------检测到双击\n");
        return 1;
    }
    else
    {
        return 0;
    }

}// End of int DriverGravity::get_click(void)
