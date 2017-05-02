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
    driverTempHumi = DriverTempHum::getInstance();
    dataTEMP.type = TEMP;
    dataHUMI.type = HUMI;

    dataDailyTEMP.init(TEMP);
    dataDailyHUMI.init(HUMI);

    dataTEMP.value = ERROR_DATA;
    dataHUMI.value = ERROR_DATA;

    // 初始化默认数值列表
    listDataTemp = QString(DATALIST_TEMP).split(",");
    listDataHumi = QString(DATALIST_HUMI).split(",");
    index = 0;

    // 新建自动更新定时器
    timerUpdate = new QTimer();
    timerUpdate->setInterval(TEMP_HUMI_REFRESH_INTERVAL);
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
    float tempValue;
    float humiValue;
    static int i;
#ifdef Bran_R8
    // R8获取传感器数据
    //    driverTempHumi->update_data(tempValue,humiValue);

    int temperature, humidity;
    short err = sht_measure_blocking_read(&temperature, &humidity);

    if (err == STATUS_OK)
    {
        tempValue = (float)temperature / 1000.0;
        humiValue = (float)humidity / 1000.0;
        if (i++ >= 10)
        {
            i = 0;
            qDebug("--------------------------------------------temperature: %f degC, humidity: %f  \n", tempValue, humiValue);
        }
    }
    else
    {
        tempValue = 999.99;
        humiValue = 999.99;
        qDebug("error reading measurement\n");
    }
#else
    // PC测试获取模拟数据
    tempValue = listDataTemp.at(index).toFloat();
    humiValue = listDataHumi.at(index).toFloat();
    index ++;
    index %= MIN(listDataTemp.length(), listDataHumi.length());
#endif

    // 发送数据存储信号,获取当前时间
    time_t currentTime = QDateTime::currentDateTime().toTime_t();
    dataTEMP.timestamp = currentTime;
    dataHUMI.timestamp = currentTime;
    dataTEMP.dateTime  = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    dataHUMI.dateTime  = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    dataTEMP.value = tempValue;
    dataHUMI.value = humiValue;

    // 发送更新数据信号
    emit signal_update_data(dataTEMP);
    emit signal_update_data(dataHUMI);

    // 若不为错误数据
    if(dataTEMP.value != ERROR_DATA)
    {
        if(dataDailyTEMP.isValid())
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
        if(dataDailyHUMI.isValid())
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
