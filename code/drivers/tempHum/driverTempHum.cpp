#include <QDebug>
#include "driverTempHum.h"

DriverTempHum *DriverTempHum::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverTempHum* DriverTempHum::getInstance()
{
    if(NULL == instance)
    {
        instance = new DriverTempHum();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  DriverTempHum
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DriverTempHum::DriverTempHum(QObject *parent)
    : QObject()
{

}

/*******************************************************************************
* Function Name  :  update_data
* Description    :  刷新数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverTempHum::update_data(float &temp, float &humi)
{
    QFile file(DEVICE_TEMPHUM);
    QStringList listFile;

    // 文件温度 湿度 内容
    QString strFile,strTemp,strHum;

    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
        QTextStream stream(&file);

        // 取得 温度值
        strFile     = stream.readAll();
        //qDebug() << "[TempHum]strFile is" << strFile;

        listFile = strFile.split(",");
        if(listFile.length() < 2)
        {
            temp = ERROR_DATA;
            humi  = ERROR_DATA;
            return;
        }

        strTemp = (((listFile.at(0)).split("=")).at(1)).trimmed();
        strHum  = (((listFile.at(1)).split("=")).at(1)).trimmed();

        temp = strTemp.toInt()/1000.0;
        humi = strHum.toInt()/1000.0;

        // 对数据进行四舍五入，保留两位小数
        temp = QString::number(temp, 'f', 2).toFloat();
        humi  = QString::number(humi, 'f', 2).toFloat();

        if((temp >= 100) || (temp < -40))
        {
            temp = ERROR_DATA;
        }
        if((humi < 0 )||(humi > 100))
        {
            humi = ERROR_DATA;
        }

        file.close();
    }
    else
    {
        temp = ERROR_DATA;
        humi  = ERROR_DATA;
    }
}

