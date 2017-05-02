#include "database.h"

Database *Database::instance = 0;

/*******************************************************************************
* Function Name  :  getInstance
* Description    :  获取单例
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
Database* Database::getInstance()
{
    if(0 == instance)
    {
        instance = new Database();
    }
    return instance;
}

/*******************************************************************************
* Function Name  :  Database
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
Database::Database(QObject *parent)
    : QThread(parent)
{
    data_init();
    connect_init();
    moveToThread(this);
}

/*******************************************************************************
* Function Name  :  ~Database
* Description    :  析构函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
Database::~Database()
{
    if(database.open())
    {
        database.close();
    }
}

/*******************************************************************************
* Function Name  :  data_init
* Description    :  数据库表初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Database::data_init()
{
    listDataDaily  = new QList<AirDataDaily>;
    listDataHourly = new QList<AirDataHourly>;
    tablePM25      = new TableAirData(PM25);
    tableTemp      = new TableAirData(TEMP);
    tableHumi      = new TableAirData(HUMI);
    tableTvoc      = new TableAirData(TVOC);
    tableCo2e      = new TableAirData(CO2E);

    // 开启数据库
    isOpen = open_database();
    if(isOpen)
    {
        tablePM25->create_table(database);
        tableTemp->create_table(database);
        tableHumi->create_table(database);
        tableTvoc->create_table(database);
        tableCo2e->create_table(database);
    }
}

/*******************************************************************************
* Function Name  :  connect_init
* Description    :  信号槽初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Database::connect_init()
{
}

/*******************************************************************************
* Function Name  :  open_database
* Description    :  打开数据库
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool Database::open_database(void)
{
    if (QFile::exists(DATABASE_NAME))
    {
        qDebug() << "DB is exist!";
    }
    else
    {
        qDebug() << "DB is not exist,need to create!";
    }

    // 添加sqlite数据库
    if(!QSqlDatabase::contains("qt_sql_default_connection")){
        database = QSqlDatabase::addDatabase("QSQLITE");
    }
    // 设置数据库名称
    database.setDatabaseName(DATABASE_NAME);
    if (!database.open())
    {
        qDebug() <<"Database open failed! \n";
        return false;
    } else {
        qDebug() <<"Database open !!!!!";
    }
    return true;
}

/*******************************************************************************
* Function Name  :  slot_insert_value
* Description    :  插入数值
* Input          :  float data 数据
*                :  time_t timeSec
*                :  AirDataType type
* Output         :  None
* Return         :  None
*******************************************************************************/
void Database::slot_save_dataCache(AirData airData)
{
    if(!isOpen)
    {
        return;
    }
    switch (airData.type)
    {
    case PM25:
        tablePM25->save_dataCache(airData);
        break;
    case TEMP:
        tableTemp->save_dataCache(airData);
        break;
    case HUMI:
        tableHumi->save_dataCache(airData);
        break;
    case TVOC:
        tableTvoc->save_dataCache(airData);
        break;
    case CO2E:
        tableCo2e->save_dataCache(airData);
    default:
        break;
    }
}

/*******************************************************************************
* Function Name  :  slot_save_dataDaily
* Description    :  存储整日数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Database::slot_save_dataDaily(AirDataDaily airData)
{
    if(!isOpen)
    {
        return;
    }
    switch (airData.type)
    {
    case PM25:
        tablePM25->save_dataDaily(airData);
        break;
    case TEMP:
        tableTemp->save_dataDaily(airData);
        break;
    case HUMI:
        tableHumi->save_dataDaily(airData);
        break;
    case TVOC:
        tableTvoc->save_dataDaily(airData);
        break;
    case CO2E:
        tableCo2e->save_dataDaily(airData);
    default:
        break;
    }
    slot_get_historyDaily(airData.type);
}

/*******************************************************************************
* Function Name  :  slot_save_dataHourly
* Description    :  存储整点数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Database::slot_save_dataHourly(AirDataHourly airData)
{
    if(!isOpen)
    {
        return;
    }
    switch (airData.type)
    {
    case PM25:
        tablePM25->save_dataHourly(airData);
        break;
    case TEMP:
        tableTemp->save_dataHourly(airData);
        break;
    case HUMI:
        tableHumi->save_dataHourly(airData);
        break;
    case TVOC:
        tableTvoc->save_dataHourly(airData);
        break;
    case CO2E:
        tableCo2e->save_dataHourly(airData);
    default:
        break;
    }
    slot_get_historyHourly(airData.type);
}


/*******************************************************************************
* Function Name  :  get_maxValue
* Description    :  获取之前最大值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
AirDataDaily Database::get_dataDaily_today(AirDataType type)
{
    AirDataDaily dataDaily;
    dataDaily.init(type);
    // 若数据库未被打开
    if(!isOpen)
    {
        return dataDaily;
    }
    switch (type)
    {
    case PM25:
        dataDaily = tablePM25->get_dataDaily_today();
        break;
    case TEMP:
        dataDaily = tableTemp->get_dataDaily_today();
        break;
    case HUMI:
        dataDaily = tableHumi->get_dataDaily_today();
        break;
    case TVOC:
        dataDaily = tableTvoc->get_dataDaily_today();
        break;
    case CO2E:
        dataDaily = tableCo2e->get_dataDaily_today();
        break;
    default:
        break;
    }
    return dataDaily;
}

/*******************************************************************************
* Function Name  :  slot_delete_datas
* Description    :  删除７天前数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Database::slot_delete_datas7Days()
{
    // 获取七天之前的时间戳
    QDateTime dateTime = QDateTime::currentDateTime().addDays(-6);
    QString strDate    = dateTime.toString("yyyy-MM-dd");
    time_t  timeStamp  = QDateTime::fromString(strDate, "yyyy-MM-dd").toTime_t();
    tablePM25->delete_dataDaily(timeStamp);
    tableTemp->delete_dataDaily(timeStamp);
    tableHumi->delete_dataDaily(timeStamp);
    tableTvoc->delete_dataDaily(timeStamp);
    tableCo2e->delete_dataDaily(timeStamp);
}

/*******************************************************************************
* Function Name  :  slot_delete_datas12Hours
* Description    :  删除12小时前数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Database::slot_delete_datas12Hours()
{
    // 获取七天之前的时间戳
    QDateTime dateTime = QDateTime::currentDateTime().addSecs(-11*60*60);
    QString timeStr = dateTime.toString("yyyy-MM-dd-hh");
    time_t timeStamp = QDateTime::fromString(timeStr, "yyyy-MM-dd-hh").toTime_t();
    tablePM25->delete_dataHourly(timeStamp);
    tableTemp->delete_dataHourly(timeStamp);
    tableHumi->delete_dataHourly(timeStamp);
    tableTvoc->delete_dataHourly(timeStamp);
    tableCo2e->delete_dataHourly(timeStamp);
}

/*******************************************************************************
* Function Name  :  slot_get_historyHourly
* Description    :  获取12小时历史数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Database::slot_get_historyHourly(AirDataType type)
{
    Data12Hours dataHourly;
    dataHourly.clear();
    switch (type)
    {
    case PM25:
        dataHourly = tablePM25->get_data_12Hours();
        break;
    case TEMP:
        dataHourly = tableTemp->get_data_12Hours();
        break;
    case HUMI:
        dataHourly = tableHumi->get_data_12Hours();
        break;
    case TVOC:
        dataHourly = tableTvoc->get_data_12Hours();
        break;
    case CO2E:
        dataHourly = tableCo2e->get_data_12Hours();
        break;
    default:
        break;
    }
    emit signal_update_historyHourly(dataHourly);
}

/*******************************************************************************
* Function Name  :  slot_get_historyDaily
* Description    :  获取7天历史数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Database::slot_get_historyDaily(AirDataType type)
{
    Data7Days dataDaily;
    dataDaily.clear();
    switch (type)
    {
    case PM25:
        dataDaily = tablePM25->get_data_7Days();
        break;
    case TEMP:
        dataDaily = tableTemp->get_data_7Days();
        break;
    case HUMI:
        dataDaily = tableHumi->get_data_7Days();
        break;
    case TVOC:
        dataDaily = tableTvoc->get_data_7Days();
        break;
    case CO2E:
        dataDaily = tableCo2e->get_data_7Days();
        break;
    default:
        break;
    }
    emit signal_update_historyDaily(dataDaily);
}

/*******************************************************************************
* Function Name  :  get_cacheData_daily
* Description    :  获取缓存数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QList<AirDataDaily> *Database::get_cacheDat_daily()
{
    int i;
    QList<AirDataDaily> *listDailyPM25 = new QList<AirDataDaily>;
    QList<AirDataDaily> *listDailyTEMP = new QList<AirDataDaily>;
    QList<AirDataDaily> *listDailyHUMI = new QList<AirDataDaily>;
    QList<AirDataDaily> *listDailyTVOC = new QList<AirDataDaily>;
    QList<AirDataDaily> *listDailyCO2E = new QList<AirDataDaily>;

    listDataDaily->clear();
    // 若数据库未被打开
    if(!isOpen)
    {
        return listDataDaily;
    }
    listDailyPM25 = tablePM25->get_cacheData_daily();
    listDailyTEMP = tableTemp->get_cacheData_daily();
    listDailyHUMI = tableHumi->get_cacheData_daily();
    listDailyTVOC = tableTvoc->get_cacheData_daily();
    listDailyCO2E = tableCo2e->get_cacheData_daily();

    for(i = 0; i < listDailyPM25->length(); ++i)
    {
        listDataDaily->append(listDailyPM25->at(i));
    }
    for(i = 0; i < listDailyTEMP->length(); ++i)
    {
        listDataDaily->append(listDailyTEMP->at(i));
    }

    for(i = 0; i < listDailyHUMI->length(); ++i)
    {
        listDataDaily->append(listDailyHUMI->at(i));
    }

    for(i = 0; i < listDailyTVOC->length(); ++i)
    {
        listDataDaily->append(listDailyTVOC->at(i));
    }

    for(i = 0; i < listDailyCO2E->length(); ++i)
    {
        listDataDaily->append(listDailyCO2E->at(i));
    }
    return listDataDaily;

}

/*******************************************************************************
* Function Name  :  get_cacheData_hourly
* Description    :  获取缓存数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QList<AirDataHourly> *Database::get_cacheData_hourly()
{
    int i;
    QList<AirDataHourly> *listHourlyPM25 = new QList<AirDataHourly>;
    QList<AirDataHourly> *listHourlyTEMP = new QList<AirDataHourly>;
    QList<AirDataHourly> *listHourlyHUMI = new QList<AirDataHourly>;
    QList<AirDataHourly> *listHourlyTVOC = new QList<AirDataHourly>;
    QList<AirDataHourly> *listHourlyCO2E = new QList<AirDataHourly>;
    listDataHourly->clear();

    // 数据库未被打开
    if(!isOpen)
    {
        return listDataHourly;
    }
    listHourlyPM25 = tablePM25->get_cacheData_hourly();
    listHourlyTEMP = tableTemp->get_cacheData_hourly();
    listHourlyHUMI = tableHumi->get_cacheData_hourly();
    listHourlyTVOC = tableTvoc->get_cacheData_hourly();
    listHourlyCO2E = tableCo2e->get_cacheData_hourly();

    for(i = 0; i < listHourlyPM25->length(); ++i)
    {
        listDataHourly->append(listHourlyPM25->at(i));
    }
    for(i = 0; i < listHourlyTEMP->length(); ++i)
    {
        listDataHourly->append(listHourlyTEMP->at(i));
    }

    for(i = 0; i < listHourlyHUMI->length(); ++i)
    {
        listDataHourly->append(listHourlyHUMI->at(i));
    }

    for(i = 0; i < listHourlyTVOC->length(); ++i)
    {
        listDataHourly->append(listHourlyTVOC->at(i));
    }

    for(i = 0; i < listHourlyCO2E->length(); ++i)
    {
        listDataHourly->append(listHourlyCO2E->at(i));
    }
    return listDataHourly;
}

/*******************************************************************************
* Function Name  :  slot_clear_tables
* Description    :  清空数据表
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Database::slot_clear_tables()
{
    tablePM25->clear_tables();
    tableTemp->clear_tables();
    tableHumi->clear_tables();
    tableTvoc->clear_tables();
    tableCo2e->clear_tables();
}

/*******************************************************************************
* Function Name  :  close_database
* Description    :  关闭数据库
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Database::close_database(void)
{
    if(isOpen)
    {
        database.close();
    }
}
