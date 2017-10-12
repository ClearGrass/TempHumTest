#include <QDebug>
#include "dataTempHumi.h"

#ifdef Bran_R8
#include "sht3x.h"
#endif

DataTempHumi *DataTempHumi::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  DataTempHumi单例
*******************************************************************************/
DataTempHumi *DataTempHumi::getInstance()
{
    if(NULL == instance)
    {
        instance = new DataTempHumi();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  DataTempHumi
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DataTempHumi::DataTempHumi(QObject *parent)
    :QThread(parent)
{
#ifdef Bran_R8
    if(sht_probe() != STATUS_OK)
    {
        qDebug("------------------------------------SHT sensor probing failed\n");
    }
    qDebug("------------------------------------SHT sensor probing success\n");
#endif
    data_init();
    connect_init();
    moveToThread(this);
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataTempHumi::run()
{
    timerUpdate->start();
    this->exec();
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataTempHumi::data_init()
{
    dataTEMP.init(TEMP);
    dataHUMI.init(HUMI);

    dataDailyTEMP.init(TEMP);
    dataDailyHUMI.init(HUMI);

    // 初始化默认数值列表
    listDataTemp = QString(DATALIST_TEMP).split(",");
    listDataHumi = QString(DATALIST_HUMI).split(",");
    index = 0;

    // 新建自动更新定时器
    timerUpdate = new QTimer();
    timerUpdate->setInterval(TEMP_HUMI_REFRESH_INTERVAL);

    existsSecondSensor = QFile("/sys/devices/platform/sun5i-i2c.2/i2c-2/2-004a/sts30_temp_input").exists();
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataTempHumi::connect_init()
{
    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(slot_update_data()));
}

/*******************************************************************************
* Function Name  :  slot_update_data
* Description    :  刷新数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataTempHumi::slot_update_data()
{
    float tempValue, rawTempValue;
    float humiValue, rawHumiValue;
    float tempRowSecond,clearGrass_temp;
#ifdef Bran_R8
    // R8获取传感器数据

    //获取第二个传感器值
    get_secondSensorTemp(tempRowSecond);

    int temperature, humidity, rawTemp, rawHumi;
    int status_charging_on, status_CPU_load,status_CPU_f,status_CPU_load_CPU_f, status_LCD_bri;
    short err = sht_measure_blocking_read_compensated_every_1_seconds(&temperature, &humidity, &rawTemp, &rawHumi,
                                                                      &status_charging_on, &status_CPU_load, &status_CPU_f, &status_CPU_load_CPU_f, &status_LCD_bri);


    if (err == STATUS_OK)
    {
        tempValue    = (float)temperature / 1000.0;
        humiValue    = (float)humidity / 1000.0;
        rawTempValue = (float)rawTemp / 1000.0;
        rawHumiValue = (float)rawHumi / 1000.0;

        bran_node.init();
        clearGrass_temp = tempHumEngine.fixTemp(bran_node);


//        if(tempValue != rawTempValue)
//            humiValue = (humiValue - rawHumiValue) * (clearGrass_temp - rawTempValue) / (tempValue - rawTempValue) + rawHumiValue;

    }
    else
    {
        tempValue = ERROR_DATA;
        humiValue = ERROR_DATA;
        qDebug("error reading measurement\n");
    }


    emit signal_update_tempFlag(status_charging_on, status_CPU_load,status_CPU_f,status_CPU_load_CPU_f, status_LCD_bri);

#else
    // PC测试获取模拟数据
    tempValue = listDataTemp.at(index).toFloat();
    humiValue = listDataHumi.at(index).toFloat();
    index ++;
    index %= MIN(listDataTemp.length(), listDataHumi.length());
#endif


    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.toString("yyyy-MM-dd");
    time_t timestampDaily = QDateTime::fromString(date, "yyyy-MM-dd").toTime_t();

    // 发送数据存储信号,获取当前时间
    dataTEMP.timestamp = dateTime.toTime_t();
    dataHUMI.timestamp = dateTime.toTime_t();
    dataTEMP.dateTime  = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    dataHUMI.dateTime  = dateTime.toString("yyyy-MM-dd hh:mm:ss");

    dataTEMP.value = tempValue;
    dataHUMI.value = humiValue;

    //发送原始值
    emit signal_update_rawData(rawTempValue, rawHumiValue, tempRowSecond, clearGrass_temp);
    // 发送更新数据信号
    emit signal_update_data(dataTEMP);
    emit signal_update_data(dataHUMI);

    // 若不为错误数据
    if(dataTEMP.value != ERROR_DATA)
    {
        // 若此时整日数据时间和当前采集数据时间一致
        if((dataDailyTEMP.isValid()) && (timestampDaily == dataDailyTEMP.timestampDaily))
        {
            // 若数据大于当天最大值
            if(dataTEMP.value > dataDailyTEMP.max)
            {
                // 刷新此时最大值
                dataDailyTEMP.max = dataTEMP.value;
                emit signal_update_dataDaily(dataDailyTEMP);
            }
            if(dataTEMP.value < dataDailyTEMP.min)
            {
                dataDailyTEMP.min = dataTEMP.value;
                emit signal_update_dataDaily(dataDailyTEMP);
            }
        }
        else
        {
            dataDailyTEMP.init(dataTEMP);
            emit signal_update_dataDaily(dataDailyTEMP);
        }
    }

    // 若不为错误数据
    if(dataHUMI.value != ERROR_DATA)
    {
        // 若此时整日数据时间和当前采集数据时间一致
        if((dataDailyHUMI.isValid()) && (timestampDaily == dataDailyHUMI.timestampDaily))
        {
            // 若数据大于当天最大值
            if(dataHUMI.value > dataDailyHUMI.max)
            {
                // 刷新此时最大值
                dataDailyHUMI.max = dataHUMI.value;
                emit signal_update_dataDaily(dataDailyHUMI);
            }
            if(dataHUMI.value < dataDailyHUMI.min)
            {
                dataDailyHUMI.min = dataHUMI.value;
                emit signal_update_dataDaily(dataDailyHUMI);
            }
        }
        else
        {
            dataDailyHUMI.init(dataHUMI);
            emit signal_update_dataDaily(dataDailyHUMI);
        }
    }
}


/*******************************************************************************
* Author        :   虎正玺@2017-08
* Function Name :   get_secondSensorTemp
* Description   :   获取第二个传感器的温度值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void DataTempHumi::get_secondSensorTemp(float &temp)
{
    if(!existsSecondSensor)
    {
        temp = ERROR_DATA;
        return ;
    }

    // IMPLEMENT
    FILE  *stream;
    char  buf[126];

    // 温度值
    stream = popen("cat /sys/devices/platform/sun5i-i2c.2/i2c-2/2-004a/sts30_temp_input", "r");
    fread(buf, sizeof(char), sizeof(buf),  stream);
    temp = atoi(buf) / 1000.0;

    pclose(stream);
}



/*******************************************************************************
* Function Name  :  slot_set_dataDaily
* Description    :  设置当天数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataTempHumi::slot_dataDaily_today(AirDataDaily airDataDaily)
{
    if(airDataDaily.type == TEMP)
    {
        dataDailyTEMP = airDataDaily;
    }
    else if(airDataDaily.type == HUMI)
    {
        dataDailyHUMI = airDataDaily;
    }
}
