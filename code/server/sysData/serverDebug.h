#ifndef SERVERDEBUG_H
#define SERVERDEBUG_H
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

class ServerDebug : public QObject
{
    Q_OBJECT
public:
    static ServerDebug* getInstance();

signals:
    void signal_upload_success();
    void signal_upload_failed();
    void signal_heartbeat_success();
    void signal_heartbeat_failed();

private slots:
    void slot_upload_log(QString,QString,QString,QString);
    void slot_system_heartbeat(InfoSys);

    void slot_upload_success(QString);
    void slot_upload_failed();

    void slot_heartbeat_success(QString);
    void slot_heartbeat_failed();


private:


private:
    explicit ServerDebug(QObject *parent = 0);
    static ServerDebug* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(ServerDebug::instance)
                delete ServerDebug::instance;
        }
    };
    static FreeInstance free;
};

#endif // SERVERDEBUG_H
