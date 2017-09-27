#include "dataTvocCo2e.h"

DataTvocCo2e *DataTvocCo2e::instance = NULL;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DataTvocCo2e *DataTvocCo2e::getInstance()
{
    if(NULL == instance)
    {
        instance = new DataTvocCo2e();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  DataTvocCo2e
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DataTvocCo2e::DataTvocCo2e(QObject *parent)
    :QThread(parent)
{
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
void DataTvocCo2e::run()
{
    // 驱动开始运行
    driverTvocCo2e->start();
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
void DataTvocCo2e::data_init()
{
    dataTVOC.type = TVOC;
    dataCO2E.type = CO2E;
    dataTVOC.value = ERROR_DATA;
    dataCO2E.value = ERROR_DATA;

    dataDailyTVOC.type = TVOC;
    dataDailyCO2E.type = CO2E;
    dataDailyTVOC.max = ERROR_DATA;
    dataDailyTVOC.min = ERROR_DATA;
    dataDailyCO2E.max = ERROR_DATA;
    dataDailyCO2E.min = ERROR_DATA;

    // 初始化默认数值列表
    listDataTvoc = QString(DATALIST_TVOC).split(",");
    listDataCo2e = QString(DATALIST_CO2E).split(",");
    index = 0;

    // 新建自动更新定时器
    timerUpdate = new QTimer();
    timerUpdate->setInterval(TVOC_CO2E_REFRESH_INTERVAL);


    stopSensorTimer = new QTimer(this);
    stopSensorTimer->setInterval(1000 * 60 * 60 * 2);
    stopSensorTimer->setSingleShot(true);
    // 初始化驱动层tvoc&co2e
    driverTvocCo2e = DriverTvocCo2e::getInstance();
    driverTvocCo2e->moveToThread(this);
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataTvocCo2e::connect_init()
{
    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(slot_update_data()));
    connect(stopSensorTimer, SIGNAL(timeout()), driverTvocCo2e, SLOT(slot_stop_SGP30()));
    stopSensorTimer->start();
}

/*******************************************************************************
* Function Name  :  slot_update_data
* Description    :  刷新数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataTvocCo2e::slot_update_data()
{

    float tvocValue;
    float co2eValue;
    static int i;

#ifdef Bran_R8
    // 刷新此时的数据
    driverTvocCo2e->update_data(tvocValue,co2eValue);
    if (i++ >= 10)
    {
        i = 0;
        qDebug("\r\n\r\n-----------------------------------------------------------------------------tVOC = %f CO2 = %f\r\n", tvocValue, co2eValue);
    }
#else
    // PC测试获取模拟数据
    tvocValue = listDataTvoc.at(index).toFloat();
    co2eValue = listDataCo2e.at(index).toFloat();

    index ++;
    index %= MIN(listDataTvoc.length(), listDataCo2e.length());

#endif

    time_t currentTime = QDateTime::currentDateTime().toTime_t();
    dataTVOC.timestamp = currentTime;
    dataCO2E.timestamp = currentTime;

    dataTVOC.dateTime  = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    dataCO2E.dateTime  = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    dataTVOC.value = tvocValue;
    dataCO2E.value = co2eValue;

    // 发送更新数据信号
    emit signal_update_data(dataTVOC);
    emit signal_update_data(dataCO2E);
}

/*******************************************************************************
* Function Name  :  slot_set_dataDaily
* Description    :  设置当天数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataTvocCo2e::slot_dataDaily_today(AirDataDaily airDataDaily)
{
    if(airDataDaily.type == TVOC)
    {
        dataDailyTVOC = airDataDaily;
    }
    else if(airDataDaily.type == CO2E)
    {
        dataDailyCO2E = airDataDaily;
    }
}

