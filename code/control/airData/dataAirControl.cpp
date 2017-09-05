#include "dataAirControl.h"

DataAirControl *DataAirControl::instance = NULL;

/*******************************************************************************
* Function Name  :  get_Instance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DataAirControl *DataAirControl::getInstance()
{
    if(NULL == instance)
    {
        instance = new DataAirControl();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  DataAirControl
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
DataAirControl::DataAirControl()
    :QThread()
{
    qRegisterMetaType<time_t>("time_t");
    qRegisterMetaType<AirData>("AirData");
    qRegisterMetaType<AirDataType>("AirDataType");
    qRegisterMetaType<AirDataDaily>("AirDataDaily");
    qRegisterMetaType<AirDataHourly>("AirDataHourly");
    qRegisterMetaType<Data7Days>("Data7Days");
    qRegisterMetaType<Data12Hours>("Data12Hours");
    data_init();
    connect_init();
    moveToThread(this);
}

/*******************************************************************************
* Function Name  :  run
* Description    :  线程入口函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::run()
{
    dataPm25->start();
    dataTempHumi->start();
    dataTvocCo2e->start();

    // 发送初始化当天最大值和最小值信号
    emit signal_dataDaily_today(dataDailyPM25);
    emit signal_dataDaily_today(dataDailyTEMP);
    emit signal_dataDaily_today(dataDailyHUMI);
    emit signal_dataDaily_today(dataDailyTVOC);
    emit signal_dataDaily_today(dataDailyCO2E);
    this->exec();
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::data_init()
{
    database        = Database::getInstance();
    dataPm25        = DataPM25::getInstance();
    dataTempHumi    = DataTempHumi::getInstance();
    dataTvocCo2e    = DataTvocCo2e::getInstance();

    fileConfig      = FileConfig::getInstance();
    sysControl      = SysControl::getInstance();
    serverControl   = ServerControl::getInstance();
    wilddogAirData  = WilddogAirData::getInstance();
    sysTime         = SysTime::getInstance();
    sysData         = SysData::getInstance();

    unitTEMP        = sysData->get_unitTEMP();
    unitTVOC        = sysData->get_unitTVOC();

    dataDailyPM25.init(PM25);
    dataDailyTEMP.init(TEMP);
    dataDailyHUMI.init(HUMI);
    dataDailyTVOC.init(TVOC);
    dataDailyCO2E.init(CO2E);

    dataDailyPM25 = database->get_dataDaily_today(PM25);
    dataDailyTEMP = database->get_dataDaily_today(TEMP);
    dataDailyHUMI = database->get_dataDaily_today(HUMI);
    dataDailyTVOC = database->get_dataDaily_today(TVOC);
    dataDailyCO2E = database->get_dataDaily_today(CO2E);
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::connect_init()
{
    connect(dataPm25, SIGNAL(signal_update_data(AirData)), this, SLOT(slot_update_data(AirData)));
    connect(dataTempHumi, SIGNAL(signal_update_data(AirData)), this, SLOT(slot_update_data(AirData)));
    connect(dataTvocCo2e, SIGNAL(signal_update_data(AirData)), this, SLOT(slot_update_data(AirData)));

    connect(dataPm25, SIGNAL(signal_update_dataDaily(AirDataDaily)), this, SLOT(slot_update_dataDaily(AirDataDaily)));
    connect(dataTempHumi, SIGNAL(signal_update_dataDaily(AirDataDaily)), this, SLOT(slot_update_dataDaily(AirDataDaily)));
    connect(dataTempHumi, SIGNAL(signal_update_rawData(float, float, float)), this, SLOT(slot_updateRaw(float,float, float)));
    connect(dataTempHumi, SIGNAL(signal_update_tempFlag(int,int,int,int,int)), this, SIGNAL(signal_update_tempFlag(int,int,int, int, int)));

    connect(dataTvocCo2e, SIGNAL(signal_update_dataDaily(AirDataDaily)), this, SLOT(slot_update_dataDaily(AirDataDaily)));

    connect(dataPm25, SIGNAL(signal_sampling_off()), this, SLOT(slot_sampling_off()));
    connect(dataPm25, SIGNAL(signal_sampling_start(time_t)), this, SLOT(slot_sampling_start(time_t)));

    connect(this, SIGNAL(signal_sampling_pm25()), dataPm25, SLOT(slot_start_sampling()));
    connect(this, SIGNAL(signal_battery_charging(bool)), dataPm25, SLOT(slot_battery_charging(bool)));
    connect(this, SIGNAL(signal_intervalPM25_charging(IntervalType)), dataPm25, SLOT(slot_interval_charging(IntervalType)));
    connect(this, SIGNAL(signal_intervalPM25_discharging(IntervalType)), dataPm25, SLOT(slot_interval_discharging(IntervalType)));

    connect(this, SIGNAL(signal_dataDaily_today(AirDataDaily)), dataPm25, SLOT(slot_dataDaily_today(AirDataDaily)));
    connect(this, SIGNAL(signal_dataDaily_today(AirDataDaily)), dataTempHumi, SLOT(slot_dataDaily_today(AirDataDaily)));
    connect(this, SIGNAL(signal_dataDaily_today(AirDataDaily)), dataTvocCo2e, SLOT(slot_dataDaily_today(AirDataDaily)));

    connect(sysTime, SIGNAL(signal_update_time(TimeType)), this, SLOT(slot_update_time(TimeType)));
    connect(sysTime, SIGNAL(signal_timeInit_success()), this, SLOT(slot_timeInit_success()));

//    connect(this, SIGNAL(signal_save_dataCache(AirData)), database, SLOT(slot_save_dataCache(AirData)));
//    connect(this, SIGNAL(signal_save_dataDaily(AirDataDaily)), database, SLOT(slot_save_dataDaily(AirDataDaily)));
//    connect(this, SIGNAL(signal_save_dataHourly(AirDataHourly)), database, SLOT(slot_save_dataHourly(AirDataHourly)));
//    connect(this, SIGNAL(signal_get_historyDaily(AirDataType)), database, SLOT(slot_get_historyDaily(AirDataType)));
//    connect(this, SIGNAL(signal_get_historyHourly(AirDataType)), database, SLOT(slot_get_historyHourly(AirDataType)));

//    connect(database, SIGNAL(signal_update_historyDaily(Data7Days)), this, SLOT(slot_update_historyDaily(Data7Days)));
//    connect(database, SIGNAL(signal_update_historyHourly(Data12Hours)), this, SLOT(slot_update_historyHourly(Data12Hours)));

//    connect(this, SIGNAL(signal_sync_data(AirData)), wilddogAirData, SLOT(slot_sync_data(AirData)));
    connect(sysData, SIGNAL(signal_change_unitTEMP(UnitType)), this, SLOT(slot_change_unitTEMP(UnitType)));
    connect(sysData, SIGNAL(signal_change_unitTVOC(UnitType)), this, SLOT(slot_change_unitTVOC(UnitType)));
}

/*******************************************************************************
* Function Name  :  slot_update_data
* Description    :  更新最新值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_update_data(AirData airData)
{
    // 更新到野狗数据，使其同步
    emit signal_sync_data(airData);

    // 根据数据类型，进行数据转换
    switch(airData.type)
    {
    case PM25:
        dataPM25 = airData;
        break;
    case TEMP:
        dataTEMP = airData;

        break;
    case HUMI:
        dataHUMI = airData;
        break;
    case TVOC:
        dataTVOC = airData;

        break;
    case CO2E:
        dataCO2E = airData;
        break;
    default:
        break;
    }

    //　更新页面的数据
    emit signal_update_data(airData);
}

/*******************************************************************************
* Function Name  :  slot_update_dataDaily
* Description    :  刷新当天值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_update_dataDaily(AirDataDaily airDataDaily)
{
    switch(airDataDaily.type)
    {
    case PM25:
        dataDailyPM25 = airDataDaily;
        break;
    case TEMP:
        dataDailyTEMP = airDataDaily;
        break;
    case HUMI:
        dataDailyHUMI = airDataDaily;
        break;
    case TVOC:
        dataDailyTVOC = airDataDaily;
        break;
    case CO2E:
        dataDailyCO2E = airDataDaily;
        break;
    default:
        break;
    }
    emit signal_save_dataDaily(airDataDaily);
}

/*******************************************************************************
* Function Name  :  slot_change_unitTEMP
* Description    :  改变温度的单位
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_change_unitTEMP(UnitType unit)
{
    unitTEMP = unit;
}

/*******************************************************************************
* Function Name  :  slot_change_unitTVOC
* Description    :  改变TVOC的单位
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_change_unitTVOC(UnitType unit)
{
    unitTVOC = unit;
}

/*******************************************************************************
* Function Name  :  slot_sampling_start
* Description    :  采集开始
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_sampling_start(time_t time)
{
    emit signal_sampling_start(time);
}

/*******************************************************************************
* Function Name  :  slot_sampling_off
* Description    :  采集结束
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_sampling_off()
{
    emit signal_sampling_off();
}

/*******************************************************************************
* Function Name  :  slot_sampling_pm25
* Description    :  开始检测PM25
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_sampling_pm25()
{
    emit signal_sampling_pm25();
}

/*******************************************************************************
* Function Name  :  slot_get_historyDaily
* Description    :  获取当天历史数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_get_historyDaily(AirDataType type)
{
    emit signal_get_historyDaily(type);
}

/*******************************************************************************
* Function Name  :  slot_get_historyHourly
* Description    :  获取小时历史数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_get_historyHourly(AirDataType type)
{
    emit signal_get_historyHourly(type);
}

/*******************************************************************************
* Function Name  :  slot_update_historyDaily
* Description    :  刷新整天历史数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_update_historyDaily(Data7Days data)
{

    if(TEMP == data.type)
    {
        // 若温度单位为华氏度
        if(KFC == unitTEMP)
        {
            for(int i =0; i < 7; ++i)
            {
                if(data.maxValue[i] != ERROR_DATA)
                {
                    data.maxValue[i] = 1.8*(data.maxValue[i]) + 32;
                    data.maxValue[i] = QString::number(data.maxValue[i], 'f', 2).toFloat();
                }

                if(data.minValue[i] != ERROR_DATA)
                {
                    data.minValue[i] = 1.8*(data.minValue[i]) + 32;
                    data.minValue[i] = QString::number(data.minValue[i], 'f', 2).toFloat();
                }
            }

        }
    }

    if(TVOC == data.type)
    {
        if(PPM == unitTVOC)
        {
            for(int i =0; i < 7; ++i)
            {
                if(data.maxValue[i] != ERROR_DATA)
                {
                    data.maxValue[i] = data.maxValue[i]/1000.0;
                    data.maxValue[i] = QString::number(data.maxValue[i], 'f', 2).toFloat();
                }

                if(data.minValue[i] != ERROR_DATA)
                {
                    data.minValue[i] = data.minValue[i]/1000.0;
                    data.minValue[i] = QString::number(data.minValue[i], 'f', 2).toFloat();
                }
            }
        }
        else if(PPB == unitTVOC)
        {
        }
        else
        {
            for(int i =0; i < 7; ++i)
            {
                if(data.maxValue[i] != ERROR_DATA)
                {
                    data.maxValue[i] = (data.maxValue[i]/1000.0)*1.223;
                    data.maxValue[i] = QString::number(data.maxValue[i], 'f', 2).toFloat();
                }

                if(data.minValue[i] != ERROR_DATA)
                {
                    data.minValue[i] = (data.minValue[i]/1000.0)*1.223;
                    data.minValue[i] = QString::number(data.minValue[i], 'f', 2).toFloat();
                }
            }
        }
    }

    emit signal_update_historyDaily(data);
}

/*******************************************************************************
* Function Name  :  slot_update_historyHourly
* Description    :  刷新整点历史数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_update_historyHourly(Data12Hours data)
{

    if(TEMP == data.type)
    {
        // 单位若为华氏度
        if(KFC == unitTEMP)
        {
            for(int i =0; i < 12; ++i)
            {
                if(data.nearestValue[i] != ERROR_DATA)
                {
                    qDebug()<<data.nearestValue[i];
                    data.nearestValue[i] = 1.8*(data.nearestValue[i]) + 32;
                    data.nearestValue[i] = QString::number(data.nearestValue[i], 'f', 2).toFloat();
                }
            }
        }
    }

    if(TVOC == data.type)
    {
        if(PPM == unitTVOC)
        {
            for(int i =0; i < 12; ++i)
            {
                if(data.nearestValue[i] != ERROR_DATA)
                {
                    data.nearestValue[i] = (data.nearestValue[i])/1000.0;
                    data.nearestValue[i] = QString::number(data.nearestValue[i], 'f', 2).toFloat();
                }
            }
        }
        else if(PPB == unitTVOC)
        {
        }
        else
        {
            for(int i =0; i < 12; ++i)
            {
                if(data.nearestValue[i] != ERROR_DATA)
                {
                    data.nearestValue[i] = ((data.nearestValue[i])/1000.0)*1.223;
                    data.nearestValue[i] = QString::number(data.nearestValue[i], 'f', 2).toFloat();
                }
            }
        }

    }

    emit signal_update_historyHourly(data);
}

/*******************************************************************************
* Function Name  :  slot_update_time
* Description    :  刷新时间
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_update_time(TimeType type)
{
    AirDataHourly dataHourly;
    if(DAILY == type)
    {
        dataDailyPM25.init(dataPM25);
        dataDailyTEMP.init(dataTEMP);
        dataDailyHUMI.init(dataHUMI);
        dataDailyTVOC.init(dataTVOC);
        dataDailyCO2E.init(dataCO2E);

        // 新的一天,将数据转化为每天数据，并发送信号，让数据库存储
        emit signal_save_dataDaily(dataDailyPM25);
        emit signal_save_dataDaily(dataDailyTEMP);
        emit signal_save_dataDaily(dataDailyHUMI);
        emit signal_save_dataDaily(dataDailyTVOC);
        emit signal_save_dataDaily(dataDailyCO2E);

        emit signal_dataDaily_today(dataDailyPM25);
        emit signal_dataDaily_today(dataDailyTEMP);
        emit signal_dataDaily_today(dataDailyHUMI);
        emit signal_dataDaily_today(dataDailyTVOC);
        emit signal_dataDaily_today(dataDailyCO2E);

        dataHourly.init(dataPM25);
        emit signal_save_dataHourly(dataHourly);
        dataHourly.init(dataTEMP);
        emit signal_save_dataHourly(dataHourly);
        dataHourly.init(dataHUMI);
        emit signal_save_dataHourly(dataHourly);
        dataHourly.init(dataTVOC);
        emit signal_save_dataHourly(dataHourly);
        dataHourly.init(dataCO2E);
        emit signal_save_dataHourly(dataHourly);
    }
    else if(HOURLY == type)
    {
        qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@HOURLY";
        dataHourly.init(dataPM25);
        emit signal_save_dataHourly(dataHourly);
        dataHourly.init(dataTEMP);
        emit signal_save_dataHourly(dataHourly);
        dataHourly.init(dataHUMI);
        emit signal_save_dataHourly(dataHourly);
        dataHourly.init(dataTVOC);
        emit signal_save_dataHourly(dataHourly);
        dataHourly.init(dataCO2E);
        emit signal_save_dataHourly(dataHourly);
    }
    else
    {
        qDebug()<<"*************************************************************MINUTE";

        // 每分钟存储一次数据，方便测试
        emit signal_save_dataCache(dataPM25);
        emit signal_save_dataCache(dataTEMP);
        emit signal_save_dataCache(dataHUMI);
        emit signal_save_dataCache(dataTVOC);
        emit signal_save_dataCache(dataCO2E);
    }
}

/*******************************************************************************
* Function Name  :  slot_timeInit_success
* Description    :  时间初始化成功
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_timeInit_success()
{

}

/*******************************************************************************
* Function Name  :  run
* Description    :  循环主体
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_intervalPM25_charging(IntervalType interval)
{
    emit signal_intervalPM25_charging(interval);
}

/*******************************************************************************
* Function Name  :  slot_intervalPM25_discharging
* Description    :  断电频率改变
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_intervalPM25_discharging(IntervalType interval)
{
    emit signal_intervalPM25_discharging(interval);
}

void DataAirControl::slot_updateRaw(float temp, float hum, float secondTemp)
{
    emit signal_update_rawData(temp, hum, secondTemp);
}

/*******************************************************************************
* Function Name  :  slot_battery_charging
* Description    :  电池状态切换
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void DataAirControl::slot_battery_charging(bool isCharging)
{
    emit signal_battery_charging(isCharging);
}
