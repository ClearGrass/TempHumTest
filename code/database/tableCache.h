#ifndef TABLECACHE_H
#define TABLECACHE_H

#include <QDebug>
#include <QStringList>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlquery.h>
#include <QDateTime>
#include "dataAirDefine.h"
#include "databaseDefine.h"
#include "globalStruct.h"

class TableCache : public QObject
{
    Q_OBJECT
public:
    explicit TableCache(QSqlDatabase,QObject *parent = 0);

protected slots:
    void slot_update_maxValue(AirDataType, time_t, float);              // 刷新最大值
    void slot_update_minValue(AirDataType, time_t, float);              // 刷新最小值
    void slot_insert_value(AirDataType, time_t, float);                 // 插入一个数据


private:
    void create_table(QSqlDatabase database);                           // 创建数据表
    void table_init();                                                  // 数据表初始化
    bool is_exist(AirDataType,QString);                                              // 是否存在

private:
    QString tableCacheRequest;                                          // 请求列表
    QString tableCache7Days;                                            // ７天数据
    QString tableCache12Hours;                                          // 12小时

};

#endif // TABLECACHE_H
