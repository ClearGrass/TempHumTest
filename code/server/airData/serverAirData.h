#ifndef SERVERAIRDATA_H
#define SERVERAIRDATA_H

#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QUrl>
#include <QTextCodec>
#include <QStringList>
#include "3rdparty/cJSON/cJSON.h"
#include "server/serverDefine.h"
#include "globalStruct.h"
#include "networkManage.h"

class ServerAirData : public QObject
{
    Q_OBJECT
public:
    static ServerAirData* getInstance();

signals:
    void signal_pushData_success();
    void signal_pushData_failed();

protected slots:
    void slot_push_data(QString,QString,QString,QString);
    void slot_pushData_success(QString);                                // 推送数据成功
    void slot_pushData_failed();                                        // 推送数据失败
    void slot_pushData_timeout();                                       // 推送数据失败

private:
    explicit ServerAirData(QObject *parent = 0);

private:
    static ServerAirData* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(ServerAirData::instance)
                delete ServerAirData::instance;
        }
    };
    static FreeInstance free;

};

#endif // SERVERAIRDATA_H
