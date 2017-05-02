#ifndef NETWORKMANAGE_H
#define NETWORKMANAGE_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <QCryptographicHash>
#include <QDir>
#include "globalStruct.h"

class NetworkManage : public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkManage();
    void get_request(QNetworkRequest);
    void post_request(QNetworkRequest,QByteArray);
    void download_file(File);

signals:
    void signal_request_success(QString);
    void signal_request_failed();
    void signal_request_timeout();
    void signal_download_process(qint64,qint64);
    void signal_download_success(QString);
    void signal_download_failed();
    void signal_download_timeout();

protected slots:
    void slot_readyRead();
    void slot_download_process(qint64,qint64);

private:
    QString get_fileMD5(QString);

private:
    QFile *file;
    QTimer *timerDownload;
    QNetworkReply *replyDownload;
    int durationRequest;
    qint64 readySize;

};

#endif // NETWORKMANAGE_H
