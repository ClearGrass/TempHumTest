#ifndef SERVERDEVICE_H
#define SERVERDEVICE_H

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

class ServerDevice : public QObject
{
    Q_OBJECT
public:
    static ServerDevice* getInstance();

signals:
    void signal_deviceInit_success(InfoDevice);
    void signal_deviceInit_failed();                                  // 初始化设备失败信号
    void signal_getToken_success(Token);
    void signal_getToken_failed();

protected slots:
    void slot_device_init(QString,QString,QString,QString);
    void slot_get_token(QString,QString);

    void slot_deviceInit_success(QString);
    void slot_deviceInit_failed();
    void slot_getToken_success(QString);
    void slot_getToken_failed();

private:
    explicit ServerDevice(QObject *parent = 0);
    float get_validNum(cJSON *item, QString field);
    QString get_validString(cJSON *item, QString field);

private:
    static ServerDevice* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(ServerDevice::instance)
                delete ServerDevice::instance;
        }
    };
    static FreeInstance free;
};

#endif // SERVERDEVICE_H
