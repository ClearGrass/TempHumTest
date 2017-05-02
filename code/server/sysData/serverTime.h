#ifndef SERVERTIME_H
#define SERVERTIME_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QUrl>
#include <QTimer>
#include <QTextCodec>
#include <QStringList>
#include "3rdparty/cJSON/cJSON.h"
#include "server/serverDefine.h"
#include "globalStruct.h"
#include "networkManage.h"

class ServerTime : public QObject
{
    Q_OBJECT
public:
    static ServerTime* getInstance();

    explicit ServerTime(QObject *parent = 0);

signals:
    void signal_syncTime_success(InfoNetTime);
    void signal_syncTime_failed();

protected slots:
    void slot_sync_netTime(QString,QString);
    void slot_syncTime_success(QString);
    void slot_syncTime_failed();

private:
    QString get_validString(cJSON *item, QString field);
    int get_validNum(cJSON *item, QString field);

private:
    static ServerTime* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(ServerTime::instance)
                delete ServerTime::instance;
        }
    };
    static FreeInstance free;

};

#endif // SERVERTIME_H
