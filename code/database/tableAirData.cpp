#include "tableAirData.h"

/*******************************************************************************
* Function Name  :  TableAirData
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TableAirData::TableAirData(AirDataType type, QObject *parent)
    :QObject(parent)
{
    tableType = type;
    listDataDaily  = new QList<AirDataDaily>;
    listDataHourly = new QList<AirDataHourly>;
    table_init();
}

/*******************************************************************************
* Function Name  :  table_init
* Description    :  表名初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableAirData::table_init()
{
    // 判断类型
    switch (tableType)
    {
    case PM25:
        tableDaily   = QString(TABLE_PM25_7DAYS);
        tableHourly  = QString(TABLE_PM25_12HOURS);
        tableCache   = QString(TABLE_PM25_CACHE);
        break;
    case TEMP:
        tableDaily   = QString(TABLE_TEMP_7DAYS);
        tableHourly  = QString(TABLE_TEMP_12HOURS);
        tableCache   = QString(TABLE_TEMP_CACHE);
        break;
    case HUMI:
        tableDaily   = QString(TABLE_HUMI_7DAYS);
        tableHourly  = QString(TABLE_HUMI_12HOURS);
        tableCache   = QString(TABLE_HUMI_CACHE);
        break;
    case TVOC:
        tableDaily   = QString(TABLE_TVOC_7DAYS);
        tableHourly  = QString(TABLE_TVOC_12HOURS);
        tableCache   = QString(TABLE_TVOC_CACHE);
        break;
    case CO2E:
        tableDaily   = QString(TABLE_CO2E_7DAYS);
        tableHourly  = QString(TABLE_CO2E_12HOURS);
        tableCache   = QString(TABLE_CO2E_CACHE);
    default:
        break;
    }
    data7Days.type = tableType;
    data12Hours.type = tableType;
}

/*******************************************************************************
* Function Name  :  create_table
* Description    :  创建表
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableAirData::create_table(QSqlDatabase database)
{
    QSqlQuery queryCreate1;
    QSqlQuery queryCreate2;

    // 判定数据库内有没有包含PM2.5的表
    if(!database.tables().contains(tableDaily))
    {
        // 若不包含，开始创建
        if (!queryCreate1.exec((QString("create table %1 (%2 uint,%3 uint,%4 varchar(30),%5 varchar(30),%6 float,%7 float)").arg(tableDaily,FIELD_TIMESTAMP, FIELD_TIMESTAMP_DAILY, FIELD_DATETIME, FIELD_DATE, FIELD_MAX_VALUE, FIELD_MIN_VALUE))))
        {
            qDebug()<<"############################################";
            qDebug()<<QString("#--------- CREATE %1,FAILED！！！！----#").arg(tableDaily);
            qDebug()<<"############################################";
        }
        else
        {
            qDebug()<<"############################################";
            qDebug()<<QString("#--------- CREATE %1,SUCCESS！！！！----#").arg(tableDaily);
            qDebug()<<"############################################";
        }
    }

    // 判定数据库内有没有包含表
    if(!database.tables().contains(tableHourly))
    {
        // 开始创建
        if (!queryCreate2.exec((QString("create table %1 (%2 uint, %3 uint, %4 varchar(30), %5 varchar(30), %6 float)").arg(tableHourly, FIELD_TIMESTAMP, FIELD_TIMESTAMP_HOURLY, FIELD_DATETIME, FIELD_TIME, FIELD_VALUE))))
        {
            qDebug()<<"############################################";
            qDebug()<<QString("#--------- CREATE %1,FAILED！！！！----#").arg(tableHourly);
            qDebug()<<"############################################";
        }
        else
        {
            qDebug()<<"############################################";
            qDebug()<<QString("#--------- CREATE %1,SUCCESS！！！！----#").arg(tableHourly);
            qDebug()<<"############################################";
        }
    }

    // 判定数据库内有没有包含表
    if(!database.tables().contains(tableCache))
    {
        // 开始创建
        if (!queryCreate2.exec((QString("create table %1 (%2 uint, %3 varchar(30), %4 float)").arg(tableCache, FIELD_TIMESTAMP, FIELD_DATETIME, FIELD_VALUE))))
        {
            qDebug()<<"############################################";
            qDebug()<<QString("#--------- CREATE %1,FAILED！！！！----#").arg(tableCache);
            qDebug()<<"############################################";
        }
        else
        {
            qDebug()<<"############################################";
            qDebug()<<QString("#--------- CREATE %1,SUCCESS！！！！----#").arg(tableCache);
            qDebug()<<"############################################";
        }
    }

}

/*******************************************************************************
* Function Name  :  save_dataCache
* Description    :  存储缓存数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableAirData::save_dataCache(AirData airData)
{
    // 将数据插入PM2.5表中
    QSqlQuery queryInsert;
    QString insertSql = QString("insert into %1 (%2,%3,%4) values(?,?,?)").arg(tableCache, FIELD_TIMESTAMP, FIELD_DATETIME, FIELD_VALUE);
    queryInsert.prepare(insertSql);
    queryInsert.addBindValue(uint(airData.timestamp));
    queryInsert.addBindValue(airData.dateTime);
    queryInsert.addBindValue(airData.value);

    // 准备命令
    if(queryInsert.exec())
    {
        qDebug()<<QString("%1 insert success!!!").arg(tableCache);
    }
    else
    {
        qDebug()<<QString("%1 insert failed!!!").arg(tableCache);
    }
}

/*******************************************************************************
* Function Name  :  save_dataDaily
* Description    :  存取整日数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableAirData::save_dataDaily(AirDataDaily airData)
{
    QSqlQuery query;
    QString updateSql = QString("update %1 set %2 = ?, %3 = ? where %4 = ?").arg(tableDaily, FIELD_MAX_VALUE, FIELD_MIN_VALUE, FIELD_DATE);
    QString insertSql = QString("insert into %1 (%2,%3,%4,%5,%6,%7) values(?,?,?,?,?,?)").arg(tableDaily,FIELD_TIMESTAMP, FIELD_TIMESTAMP_DAILY, FIELD_DATETIME, FIELD_DATE, FIELD_MAX_VALUE, FIELD_MIN_VALUE);

    // 说明数据表中不存在该天记录
    if(!is_exist(airData.date))
    {
        query.prepare(insertSql);
        query.addBindValue(uint(airData.timestamp));
        query.addBindValue(uint(airData.timestampDaily));
        query.addBindValue(airData.dateTime);
        query.addBindValue(airData.date);
        query.addBindValue(airData.max);
        query.addBindValue(airData.min);
    }
    else
    {
        query.prepare(updateSql);
        query.addBindValue(airData.max);
        query.addBindValue(airData.min);
        query.addBindValue(airData.date);
    }
    if(query.exec())
    {
        qDebug()<<QString("%1 insert success!!!").arg(tableDaily);
    }
    else
    {
        qDebug()<<QString("%1 insert failed!!!").arg(tableDaily);
    }
}

/*******************************************************************************
* Function Name  :  save_dataHourly
* Description    :  存取整点数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableAirData::save_dataHourly(AirDataHourly airData)
{
    // 将数据插入PM2.5表中
    QSqlQuery queryInsert;
    QString insertSql = QString("insert into %1 (%2,%3,%4,%5,%6) values(?,?,?,?,?)").arg(tableHourly,FIELD_TIMESTAMP,FIELD_TIMESTAMP_HOURLY,FIELD_DATETIME,FIELD_TIME,FIELD_VALUE);
    queryInsert.prepare(insertSql);
    queryInsert.addBindValue(uint(airData.timestamp));
    queryInsert.addBindValue(uint(airData.timestampHourly));
    queryInsert.addBindValue(airData.dateTime);
    queryInsert.addBindValue(airData.time);
    queryInsert.addBindValue(airData.value);

    // 准备命令
    if(queryInsert.exec())
    {
        qDebug()<<QString("%1 insert success!!!").arg(tableHourly);
    }
    else
    {
        qDebug()<<QString("%1 insert failed!!!").arg(tableHourly);
    }
}

/*******************************************************************************
* Function Name  :  get_maxValue
* Description    :  获取当天最大值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
AirDataDaily TableAirData::get_dataDaily_today()
{
    AirDataDaily dataDaily;
    dataDaily.init(tableType);
    QSqlQuery query;
    QDate date = QDate::currentDate();
    QString strDate = date.toString("yyyy-MM-dd");
    QString selectSql = QString("select %1,%2,%3,%4,%5,%6 from %7 where %8 = ?").arg(FIELD_TIMESTAMP,FIELD_TIMESTAMP_DAILY,FIELD_DATETIME,FIELD_DATE,FIELD_MAX_VALUE,FIELD_MIN_VALUE, tableDaily,FIELD_DATE);
    query.prepare(selectSql);
    query.addBindValue(strDate);

    // 查询所有数据并加入到容器
    if(query.exec())
    {
        if(query.next())
        {
            dataDaily.timestamp = query.value(0).toInt();
            dataDaily.timestampDaily = query.value(1).toInt();
            dataDaily.dateTime = query.value(2).toString();
            dataDaily.date = query.value(3).toString();
            dataDaily.max = query.value(4).toFloat();
            dataDaily.min = query.value(5).toFloat();
        }
    }
    else
    {
        qDebug()<<"GET MAX VALUE FAILED!!!";
    }
    return dataDaily;
}

/*******************************************************************************
* Function Name  :  get_cacheData_daily
* Description    :  获取天数缓存数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QList<AirDataDaily> *TableAirData::get_cacheData_daily()
{
    int i,j;
    QSqlQuery query;
    AirDataDaily airDataDaily;
    QDate date = QDate::currentDate();
    QString strDate;
    QList<QString> *listDate = new QList<QString>;


    for( i = 0; i < 7; ++i)
    {
        strDate = (date.addDays(-i)).toString("yyyy-MM-dd");
        listDate->append(strDate);
    }
    QString selectSql = QString("select %1,%2,%3,%4 from %5").arg(FIELD_DATE,FIELD_TIMESTAMP,FIELD_MAX_VALUE,FIELD_MIN_VALUE,tableDaily);

    // 清空结构体
    listDataDaily->clear();

    // 查询所有数据并加入到容器
    if(query.exec(selectSql))
    {
        while(query.next())
        {
            for( j = 0; j < listDate->length(); ++j)
            {
                if((query.value(0).toString()) == listDate->at(j))
                {
                    airDataDaily.type      = tableType;
                    airDataDaily.timestamp = query.value(1).toInt();
                    airDataDaily.max       = query.value(2).toFloat();
                    airDataDaily.min       = query.value(3).toFloat();
                    listDataDaily->append(airDataDaily);
                }

            }
        }
    }
    else
    {
        qDebug()<<"GET 7 Days Cache Data FAILED!!!";
    }
    return listDataDaily;

}

/*******************************************************************************
* Function Name  :  get_cacheData_hourly
* Description    :  换区时间缓存数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QList<AirDataHourly> *TableAirData::get_cacheData_hourly()
{
    int i,j,m;
    QSqlQuery query;
    AirDataHourly airDataHourly;
    QDateTime datetime = QDateTime::currentDateTime();
    QString strDateTime;
    QList<QString> *listDateTime = new QList<QString>;
    for( i = 1; i <= 12; ++i)
    {
        strDateTime = (datetime.addSecs(-3600*i)).toString("yyyy-MM-dd-hh");
        listDateTime->append(strDateTime);
    }
    QString selectSql = QString("select %1, %2, %3 from %4 order by %5").arg(FIELD_TIME,FIELD_TIMESTAMP, FIELD_VALUE, tableHourly, FIELD_TIMESTAMP);
    listDataHourly->clear();

    // 查询所有数据并加入到容器
    if(query.exec(selectSql))
    {
        while(query.next())
        {
            for( j = 0; j < listDateTime->length(); ++j)
            {
                if((query.value(0).toString()) == listDateTime->at(j))
                {
                    airDataHourly.type      = tableType;
                    airDataHourly.timestamp = query.value(1).toInt();
                    airDataHourly.value     = query.value(2).toFloat();

                    for(m = 0; m < listDataHourly->length(); ++m)
                    {
                        if(airDataHourly.timestamp == listDataHourly->at(m).timestamp)
                        {
                            listDataHourly->removeAt(m);
                        }
                    }
                    listDataHourly->append(airDataHourly);
                }

            }
        }
    }
    else
    {
        qDebug()<<"12 Hours Cache Data Read Failed!!!";
    }
    return listDataHourly;

}

/*******************************************************************************
* Function Name  :  get_data_7Days
* Description    :  获取七天数据
* Input          :  None
* Output         :  None
* Return         :  7天数据
*******************************************************************************/
Data7Days TableAirData::get_data_7Days()
{
    int i,j;
    QSqlQuery query;
    QDate date = QDate::currentDate();
    QString strDate;
    QList<QString> *listDate = new QList<QString>;
    for( i = 0; i < 7; ++i)
    {
        strDate = (date.addDays(-i)).toString("yyyy-MM-dd");
        listDate->append(strDate);
    }
    QString selectSql = QString("select %1, %2,%3 from %4").arg(FIELD_DATE,FIELD_MAX_VALUE,FIELD_MIN_VALUE,tableDaily);

    // 清空结构体
    data7Days.clear();

    // 查询所有数据并加入到容器
    if(query.exec(selectSql))
    {
        while(query.next())
        {
            for( j = 0; j < listDate->length(); ++j)
            {
                if((query.value(0).toString()) == listDate->at(j))
                {
                    qDebug()<<query.value(0).toString()<<j;
                    qDebug()<<query.value(1).toString()<<j;
                    qDebug()<<query.value(2).toString()<<j;
                    data7Days.maxValue[j] = query.value(1).toFloat();
                    data7Days.minValue[j] = query.value(2).toFloat();
                }
            }
        }
    }
    else
    {
        qDebug()<<"GET 7 Days Data FAILED!!!";
    }
    return data7Days;
}

/*******************************************************************************
* Function Name  :  get_data_12Hours
* Description    :  获取12小时数值
* Input          :  None
* Output         :  None
* Return         :  返回12小时数据
*******************************************************************************/
Data12Hours TableAirData::get_data_12Hours()
{
    int i,j;
    QSqlQuery query;
    QDateTime datetime = QDateTime::currentDateTime();
    QString strDateTime;
    QList<QString> *listDateTime = new QList<QString>;
    for( i = 0; i < 12; ++i)
    {
        strDateTime = (datetime.addSecs(-3600*i)).toString("yyyy-MM-dd-hh");
        listDateTime->append(strDateTime);
    }
    QString selectSql = QString("select %1, %2 from %3 order by %4").arg(FIELD_TIME, FIELD_VALUE, tableHourly, FIELD_TIMESTAMP);

    data12Hours.clear();

    // 查询所有数据并加入到容器
    if(query.exec(selectSql))
    {
        while(query.next())
        {
            for( j = 0; j < listDateTime->length(); ++j)
            {
                if((query.value(0).toString()) == listDateTime->at(j))
                {
                    data12Hours.nearestValue[j] = query.value(1).toFloat();
                }
            }
        }
    }
    else
    {
        qDebug()<<"12 Hours Data Read Failed!!!";
    }
    return data12Hours;
}

/*******************************************************************************
* Function Name  :  slot_delete_data7Days
* Description    :  删除过期７天数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableAirData::delete_dataDaily(time_t timeSec)
{
    // 声明变量
    QSqlQuery queryDelete;

    // 字符串变量
    QString deleteSql = QString("delete from %1 where %2 < ?").arg(tableDaily, FIELD_TIMESTAMP);
    queryDelete.prepare(deleteSql);
    queryDelete.addBindValue(uint(timeSec));

    if (!queryDelete.exec())
    {
        qDebug()<<"REMOVE FAILED!!!";
    }
    else
    {
        qDebug()<<"REMOVE SUCCESS";
    }
}

/*******************************************************************************
* Function Name  :  slot_delete_data12Hours
* Description    :  删除12时间数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableAirData::delete_dataHourly(time_t timeSec)
{
    // 声明变量
    QSqlQuery queryDelete;

    // 字符串变量
    QString deleteSql = QString("delete from %1 where %2 < ?").arg(tableHourly,FIELD_TIMESTAMP);
    queryDelete.prepare(deleteSql);
    queryDelete.addBindValue(uint(timeSec));

    if (!queryDelete.exec(deleteSql) )
    {
        qDebug()<<"REMOVE FAILED!!!";
    }
    else
    {
        qDebug()<<"REMOVE SUCCESS";
    }

}

/*******************************************************************************
* Function Name  :  get_maxValue
* Description    :  获取当天最大值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool TableAirData::is_exist(QString strDate)
{
    QSqlQuery query;
    QString selectSql = QString("select %1 from %2 where %3 = ?").arg(FIELD_MAX_VALUE,tableDaily, FIELD_DATE);

    query.prepare(selectSql);
    query.addBindValue(strDate);

    // 查询是否存在该天数据
    if(query.exec())
    {
        if(query.next())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        qDebug()<<"IS EXIST DATE IN THE TABLE ERROR";
    }
    return false;
}

/*******************************************************************************
* Function Name  :  clear_tables
* Description    :  清空数据表
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableAirData::clear_tables()
{

    QSqlQuery queryDelete1,queryDelete2;
    QString deleteSql1 = QString("delete from %1").arg(tableHourly);
    QString deleteSql2 = QString("delete from %1").arg(tableDaily);

    if(queryDelete1.exec(deleteSql1))
    {
        qDebug()<<QString("%1 Clear Success!!!").arg(tableHourly);
    }
    else
    {
        qDebug()<<QString("%1 Clear Failed!!!").arg(tableHourly);
    }

    if(queryDelete2.exec(deleteSql2))
    {
        qDebug()<<QString("%1 Clear Success!!!").arg(tableDaily);
    }
    else
    {
        qDebug()<<QString("%1 Clear Failed!!!").arg(tableDaily);
    }

}

