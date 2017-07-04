#include <QDebug>
#include "driverTempHum.h"
#ifdef Bran_R8
#include "sht3x.h"
#endif

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
#ifdef Bran_R8
    if(sht_probe() != STATUS_OK)
    {
        LogControl::error(OBJ_TEMP_HUMI,"SHT sensor probing failed");
    }
    LogControl::debug(OBJ_TEMP_HUMI,"SHT sensor probing success");

    QFileInfo dir("/usr/bin/qtapp/debugFile");
    if(!dir.exists())
    {
        system(qPrintable("mkdir /usr/bin/qtapp/debugFile"));
    }

#endif
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

/*******************************************************************************
* Function Name  :  update_data
* Description    :  刷新数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DriverTempHum::update_data(float &temp, float &humi, float &tempRaw, float &humiRaw)
{
#ifdef Bran_R8
    // R8获取传感器数据
    static int i = 0;
    int tempValue, humiValue, rawTemp, rawHumi;
    int status_charging_on, status_CPU_load, status_LCD_bri;
    short err = sht_measure_blocking_read_compensated_every_1_seconds(&tempValue, &humiValue, &rawTemp, &rawHumi, &status_charging_on, &status_CPU_load, &status_LCD_bri);
    if(err == STATUS_OK)
    {
        temp    = (float)tempValue / 1000.0;
        humi    = (float)humiValue / 1000.0;
        tempRaw = (float)rawTemp / 1000.0;
        humiRaw = (float)rawHumi / 1000.0;

        if((temp > 50)||(temp < -20))
        {
            temp = ERROR_DATA;
        }
        if((humi > 100)||(humi < 0))
        {
            humi = ERROR_DATA;
        }
    }
    else
    {
        temp = ERROR_DATA;
        humi = ERROR_DATA;
    }
    if (i++ >= 60)
    {
        i = 0;
        LogControl::debug(OBJ_TEMP_HUMI,QString("温度:%1, 湿度:%2, 原有温度:%3, 原有湿度:%4, 电流:%5, CPU负载:%6, 亮度值:%7")
                          .arg(temp).arg(humi).arg(tempRaw).arg(humiRaw).arg(status_charging_on).arg(status_CPU_load).arg(status_LCD_bri));
    }
#endif
}

