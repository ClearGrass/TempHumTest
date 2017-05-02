#ifndef SERVERFIRMWARE_H
#define SERVERFIRMWARE_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QUrl>
#include <QTextCodec>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QByteArray>
#include <QTimer>
#include <QEventLoop>
#include "3rdparty/cJSON/cJSON.h"
#include "server/serverDefine.h"
#include "globalVariable.h"
#include "networkManage.h"

class ServerFirmware : public QObject
{
    Q_OBJECT
public:
    static ServerFirmware *getInstance();

signals:
    void signal_checkFirmware_success(InfoFirmware);                        // 返回固件信息
    void signal_checkFirmware_failed();                                 // 检查更新失败
    void signal_download_process(qint64,qint64);
    void signal_download_success();
    void signal_download_failed();

public slots:
    void slot_check_update(QString,QString,QString versionType = "release");
    void slot_download_file(InfoFirmware);

    void slot_checkUpdate_success(QString);
    void slot_checkUpdate_failed();


                                 // 接收服务器返回请求
    void slot_download_process(qint64,qint64);
    void slot_download_success(QString);                                      // 完成下载
    void slot_download_failed();

private:
    explicit ServerFirmware(QObject *parent = 0);
    void start_download(File);
    void analyse_reply_download(QString fileName);
    QString get_validString(cJSON *item, QString field);
    int get_validNum(cJSON *item, QString field);


private:
    InfoFirmware newFirmware;
    int index;
    int totalSize;
    int readySize;

private:
    static ServerFirmware* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(ServerFirmware::instance)
                delete ServerFirmware::instance;
        }
    };
    static FreeInstance free;

};

#endif // SERVERFIRMWARE_H
