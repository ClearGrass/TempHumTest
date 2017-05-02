#ifndef SERVERAPP_H
#define SERVERAPP_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QUrl>
#include <QTextCodec>
#include <QStringList>
#include "server/serverDefine.h"
#include "3rdparty/cJSON/cJSON.h"

class ServerApp : public QObject
{
    Q_OBJECT
public:
    explicit ServerApp(QObject *parent = 0);

signals:
    void signal_feedback_pairCode(int);
    void signal_feedback_cancelPair();

public:
    void get_pairCode();                                                // 获取配对码
    void cancel_pair();                                                 // 取消配对

public slots:
    void slot_reply_get_pairCode();                                     // 配对码返回请求
    void slot_reply_cancel_pair();                                      // 取消配对返回请求

private:
    void analyse_get_pairCode(QString replyData);                       // 解析JSON数据
    void analyse_cancel_pair(QString replyData);                        // 解析JSON数据
    QString get_validValue(cJSON *item, QString field);

private:

    QString deviceID;                                                   // 设备ID

    QNetworkAccessManager *manager;                                     // 网络访问管理器
    QNetworkRequest request;                                            // 请求
    QNetworkReply *replyGetPairCode;                                    // 获取配对码的回复
    QNetworkReply *replyCancelPair;                                     // 取消配对的回复
};

#endif // SERVERAPP_H
