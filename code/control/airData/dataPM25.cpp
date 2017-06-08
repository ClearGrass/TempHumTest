#include "dataPM25.h"

DataPM25 *DataPM25::instance = NULL;

/*******************************************************************************
* Function Name  :  get_Instance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DataPM25 *DataPM25::getInstance()
{
    if(NULL == instance)
    {
        instance = new DataPM25();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  DataPM25
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DataPM25::DataPM25(QObject *parent)
    :QThread(parent)
{
//    data_init();
//    connect_init();
    moveToThread(this);
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::run()
{
//    // 开机检测
//    slot_start_sampling();

//    if(isCharging)
//    {
//        timerCharging->start();
//    }
//    else
//    {
//        timerDischarging->start();
//    }
//    this->exec();
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::data_init()
{
    driverPM25 = DriverPM25::getInstance();
    sysData    = SysData::getInstance();

    dataPM25.type = PM25;
    dataPM25.value = ERROR_DATA;

    dataDailyPM25.init(PM25);

    isSampling  = false;
    isCharging  = false;

    intervalCharging = sysData->get_intervalCharging();
    intervalDischarging = sysData->get_intervalDischarging();

    // 初始化默认数值列表
    listDataPM25 = QString(DATALIST_PM25).split(",");
    index = 0;

    // 新建自动更新定时器
    timerCharging = new QTimer();
    timerCharging->setInterval(intervalCharging*60*1000);

    timerDischarging = new QTimer();
    timerDischarging->setInterval(intervalDischarging*60*1000);

    // 新建准备数据定时器
    timerReady = new QTimer();
    timerReady->setInterval(PM25_SAMPLING_READY_TIMEOUT);
    timerReady->setSingleShot(true);

    // 新建采样持续定时器
    timerSampling = new QTimer();
    timerSampling->setInterval(PM25_SAMPLING_DURATION);
    timerSampling->setSingleShot(true);

}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽连接
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::connect_init()
{
//    connect(timerCharging, SIGNAL(timeout()), this, SLOT(slot_start_sampling()));
//    connect(timerDischarging, SIGNAL(timeout()), this, SLOT(slot_start_sampling()));
//    connect(timerSampling, SIGNAL(timeout()), this, SLOT(slot_stop_sampling()));
//    connect(timerReady, SIGNAL(timeout()), this, SLOT(slot_ready_timeout()));
//    connect(driverPM25, SIGNAL(signal_update_dataPM25(float)), this, SLOT(slot_update_dataPM25(float)));
//    connect(driverPM25, SIGNAL(signal_update_dataPM5(float)), this, SLOT(slot_update_dataPM5(float)));
//    connect(driverPM25, SIGNAL(signal_update_dataPM10(float)), this, SLOT(slot_update_dataPM10(float)));
//    connect(sysData, SIGNAL(signal_intervalSampling_charging(IntervalType)), this, SLOT(slot_interval_charging(IntervalType)));
//    connect(sysData, SIGNAL(signal_intervalSampling_discharging(IntervalType)), this, SLOT(slot_interval_discharging(IntervalType)));

}

/*******************************************************************************
* Function Name  :  slot_start_sampling
* Description    :  开始检测
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::slot_start_sampling()
{
    return ;

    qDebug()<<">>>>>>>>>>>>>>>>>>>>>>>>>>>START SAMPLING<<<<<<<<<<<<<<<<<<<<<<<<";
    if(!isSampling)
    {
        time_t currentTime = QDateTime::currentDateTime().toTime_t();
        emit signal_sampling_start(currentTime);

        // 唤醒驱动开始检测,并开始准备定时器计时
        driverPM25->start_sampling();
        timerReady->start();
        isSampling = true;
    }
}

/*******************************************************************************
* Function Name  :  slot_stop_sampling
* Description    :  停止检测
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::slot_stop_sampling()
{
    return ;

    // 停止采样，关闭传感器停止检测，并设置当前状态,存储当前PM2.5数值
    isSampling = false;
    driverPM25->stop_sampling();
    emit signal_sampling_off();
    qDebug()<<">>>>>>>>>>>>>>> PM2.5 stop sampling <<<<<<<<<<<<<<<<<<<<<<<<<<<<";
}

/*******************************************************************************
* Function Name  :  slot_ready_timeout
* Description    :  准备超时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::slot_ready_timeout()
{
    return ;

    qDebug()<<">>>>>>>>>>>>>>>>>>>>Sampling Ready Timeout<<<<<<<<<<<<<<<<<<<<<<";
    dataPM25.value = ERROR_DATA;
    dataPM25.timestamp = QDateTime::currentDateTime().toTime_t();
    emit signal_update_data(dataPM25);
    timerSampling->start();
}

/*******************************************************************************
* Function Name  :  slot_update_data
* Description    :  刷新数据
* Input          :  float pmData    新的PM2.5数据
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::slot_update_dataPM25(float pmData)
{
    return ;
    // 进入正在检测状态
    if(timerReady->isActive())
    {
        timerReady->stop();
    }
    if(!timerSampling->isActive())
    {
        timerSampling->start();
    }

    dataPM25.timestamp = QDateTime::currentDateTime().toTime_t();
    dataPM25.dateTime  = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    dataPM25.value = pmData;

    // 发送更新PM2.5的数据
    emit signal_update_data(dataPM25);

    // 若不为错误数据
    if(dataPM25.value != ERROR_DATA)
    {
        if(dataDailyPM25.isValid())
        {
            if(dataPM25.value > dataDailyPM25.max)
            {
                // 刷新此时最大值
                dataDailyPM25.max = dataPM25.value;
                emit signal_update_dataDaily(dataDailyPM25);
            }
            if(dataPM25.value < dataDailyPM25.min)
            {
                dataDailyPM25.min = dataPM25.value;
                emit signal_update_dataDaily(dataDailyPM25);
            }
        }
        else
        {
            dataDailyPM25.init(dataPM25);
            emit signal_update_dataDaily(dataDailyPM25);
        }
    }
}

/*******************************************************************************
* Function Name  :  slot_update_dataPM5
* Description    :  刷新PM5
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::slot_update_dataPM5(float value)
{
    return ;

    AirData dataPM5;
    dataPM5.type = PM5;
    dataPM5.value = value;
    emit signal_update_data(dataPM5);
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::slot_update_dataPM10(float value)
{
    return ;

    AirData dataPM10;
    dataPM10.type = PM10;
    dataPM10.value = value;
    emit signal_update_data(dataPM10);
}

/*******************************************************************************
* Function Name  :  slot_set_dataDaily
* Description    :  设置当天数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::slot_dataDaily_today(AirDataDaily airDataDaily)
{
    return ;

    if(airDataDaily.type = PM25)
    {
        dataDailyPM25 = airDataDaily;
    }
}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::slot_interval_charging(IntervalType interval)
{
    return ;

    intervalCharging = interval;
    timerCharging->setInterval(interval*60*1000);
    if(isCharging)
    {
        if(!isSampling)
        {
            slot_start_sampling();
        }
        timerCharging->stop();
        timerCharging->start();
    }
}

/*******************************************************************************
* Function Name  :  slot_interval_discharging
* Description    :  改变断电时频率
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::slot_interval_discharging(IntervalType interval)
{
    return ;

    intervalDischarging = interval;
    timerDischarging->setInterval(interval*60*1000);
    if(!isCharging)
    {
        if(!isSampling)
        {
            slot_start_sampling();
        }
        timerDischarging->stop();
        timerDischarging->start();
    }
}

/*******************************************************************************
* Function Name  :  slot_battery_charging
* Description    :  电池状态改变
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataPM25::slot_battery_charging(bool charging)
{
    return ;

    if(isCharging != charging)
    {
        isCharging = charging;
        if(!isSampling)
        {
            // 开始检测
            slot_start_sampling();
        }
        // 此时充电
        if(isCharging)
        {
            timerDischarging->stop();
            timerCharging->start();
        }
        else
        {
            timerCharging->stop();;
            timerDischarging->start();
        }
    }
}

