#ifndef DATABASE_H
#define DATABASE_H
#include <QThread>
#include <QtDebug>
#include <QStringList>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlquery.h>
#include <QFile>
#include <QPair>
#include <QSqlError>
#include <QTimer>
#include "dataAirDefine.h"
#include "time.h"
#include "databaseDefine.h"
#include "tableAirData.h"

enum CURRENT_WIFI_STATUS
{
    ON = true,
    OFF = false
};

typedef struct cacheData
{
    QList<AirDataDaily> *listDataDaily;
    QList<AirDataHourly> *listDataHourly;

    bool isEmpty()
    {
        if(listDataDaily->isEmpty() && listDataHourly->isEmpty())
        {
            return true;
        }
        return false;
    }

}CacheData;

class TableAirData;

class Database : public QThread
{
    Q_OBJECT
public:
    static Database* getInstance();
    AirDataDaily get_dataDaily_today(AirDataType);

    QList<AirDataDaily> *get_cacheDat_daily();
    QList<AirDataHourly> *get_cacheData_hourly();

signals:
    void signal_update_historyDaily(Data7Days);
    void signal_update_historyHourly(Data12Hours);

protected slots:
    void slot_get_historyDaily(AirDataType);
    void slot_get_historyHourly(AirDataType);

    void slot_save_dataCache(AirData);
    void slot_save_dataDaily(AirDataDaily);
    void slot_save_dataHourly(AirDataHourly);

    void slot_clear_tables();
    void slot_delete_datas7Days();
    void slot_delete_datas12Hours();

private:
    explicit Database(QObject *parent = 0);
    ~Database();
    bool open_database(void);
    void close_database(void);
    void data_init();
    void connect_init();

private:
    TableAirData *tablePM25;
    TableAirData *tableTemp;
    TableAirData *tableHumi;
    TableAirData *tableTvoc;
    TableAirData *tableCo2e;

    QList<AirDataDaily> *listDataDaily;
    QList<AirDataHourly> *listDataHourly;

    bool isOpen;
    QSqlDatabase database;

private:
    static Database *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(Database::instance)
                delete Database::instance;
        }
    };
    static FreeInstance free;
};

#endif // DATABASE_H
