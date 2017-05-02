#ifndef TABLEAIRDATA_H
#define TABLEAIRDATA_H

#include <QtDebug>
#include <QStringList>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlquery.h>
#include <QFile>
#include <QPair>
#include <QSqlError>
#include <QDateTime>
#include "time.h"
#include "database.h"
#include "dataAirDefine.h"
#include "databaseDefine.h"

class TableAirData : public QObject
{
    Q_OBJECT
public:
    explicit TableAirData(AirDataType,QObject *parent = 0);
    void create_table(QSqlDatabase database);

    AirDataDaily get_dataDaily_today();


    Data12Hours get_data_12Hours();
    Data7Days   get_data_7Days();
    QList<AirDataDaily> *get_cacheData_daily();
    QList<AirDataHourly> *get_cacheData_hourly();

    void save_dataCache(AirData);
    void save_dataHourly(AirDataHourly);
    void save_dataDaily(AirDataDaily);
    void clear_tables();
    void delete_dataDaily(time_t);
    void delete_dataHourly(time_t);

private:
    void table_init();
    bool is_exist(QString);                                             // 是否存在

private:
    Data12Hours data12Hours;
    Data7Days   data7Days;
    QList<AirDataDaily> *listDataDaily;
    QList<AirDataHourly> *listDataHourly;

    AirDataType tableType;
    QString     tableDaily;
    QString     tableHourly;
    QString     tableCache;

};

#endif // TABLEAIRDATA_H
