#include "tableCache.h"

/*******************************************************************************
* Function Name  :  TableCache
* Description    :  构造函数
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
TableCache::TableCache(QSqlDatabase database, QObject *parent)
    :QObject(parent)
{
    table_init();
//    create_table(database);
}

/*******************************************************************************
* Function Name  :  table_init
* Description    :  数据表初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableCache::table_init()
{
//    tableCacheRequest = QString(TABLE_CACHE_REQUEST);
//    tableCache7Days   = QString(TABLE_CACHE_7DAYS);
//    tableCache12Hours = QString(TABLE_CACHE_12HOURS);
}

/*******************************************************************************
* Function Name  :  create_table
* Description    :  创建数据表
* Input          :  QSqlDatabase database 数据库
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableCache::create_table(QSqlDatabase database)
{
}

/*******************************************************************************
* Function Name  :  slot_insert_value
* Description    :  插入值到数据库
* Input          :  AirDataType 数据类型
*                   float value 值
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableCache::slot_insert_value(AirDataType type, time_t timesec, float value)
{

}


/*******************************************************************************
* Function Name  :  slot_update_maxValue
* Description    :  刷新最大值
* Input          :  AirDataType type 数据类型
*                   float maxValue  最大值
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableCache::slot_update_maxValue(AirDataType type, time_t timesec,float maxValue)
{
    QSqlQuery query;
    QDateTime dateTime = QDateTime::fromTime_t(timesec);
    QString strDate = dateTime.toString("yyyy-MM-dd");
    QString updateSql = QString("update %1 set %2 = ? where %3 = ? and %4 = ?").arg(tableCache7Days, FIELD_MAX_VALUE, FIELD_DATA_TYPE,FIELD_DATE);
    QString insertSql = QString("insert into %1 (%2,%3,%4) values(?,?,?)").arg(tableCache7Days, FIELD_MAX_VALUE, FIELD_DATA_TYPE, FIELD_DATE);

    // 若数据库中不存在该天数据
    if(!is_exist(type, strDate))
    {

        qDebug()<<"MAX VALUE INSERT"<<maxValue;
        query.prepare(insertSql);
    }
    else
    {
        qDebug()<<"MAX VALUE UPDATE"<<maxValue;
        query.prepare(updateSql);
    }

    // 绑定数据
    query.addBindValue(maxValue);
    query.addBindValue(type);
    query.addBindValue(strDate);

    if(query.exec())
    {
        return;
    }
    else
    {
        qDebug()<<QString("%1 insert failed!!!").arg(tableCache7Days);
    }

}

/*******************************************************************************
* Function Name  :  slot_update_minValue
* Description    :  刷新最小值
* Input          :  AirDataType type 数据类型
*                   float minValue  最小值
* Output         :  None
* Return         :  None
*******************************************************************************/
void TableCache::slot_update_minValue(AirDataType type,time_t timesec, float minValue)
{
    QSqlQuery query;
    QDateTime dateTime = QDateTime::fromTime_t(timesec);
    QString strDate = dateTime.toString("yyyy-MM-dd");

    QString updateSql = QString("update %1 set %2 = ? where %3 = ? and %4 = ?").arg(tableCache7Days, FIELD_MIN_VALUE, FIELD_DATA_TYPE, FIELD_DATE);
    QString insertSql = QString("insert into %1 (%2,%3,%4) values(?,?,?)").arg(tableCache7Days, FIELD_MIN_VALUE,FIELD_DATA_TYPE, FIELD_DATE);

    // 若数据库中不存在该天数据，则插入数据
    if(!is_exist(type,strDate))
    {
        qDebug()<<"MIN VALUE INSERT"<<minValue;
        query.prepare(insertSql);
    }
    // 若数据库中已存在该天数据，则刷新数据
    else
    {
        qDebug()<<"MIN VALUE UPDATE"<<minValue;
        query.prepare(updateSql);
    }

    query.addBindValue(minValue);
    query.addBindValue(type);
    query.addBindValue(strDate);

    if(query.exec())
    {
        return;
    }
    else
    {
        qDebug()<<QString("%1 insert failed!!!").arg(tableCache7Days);
    }

}


/*******************************************************************************
* Function Name  :  get_maxValue
* Description    :  获取当天最大值
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
bool TableCache::is_exist(AirDataType type, QString strDate)
{
    QSqlQuery query;
    QString selectSql = QString("select %1 from %2 where %3 = ? and %4 = ?").arg(FIELD_MAX_VALUE,tableCache7Days, FIELD_DATA_TYPE, FIELD_DATE);

    query.prepare(selectSql);
    query.addBindValue(type);
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

