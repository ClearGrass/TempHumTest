#ifndef SERVERCITY_H
#define SERVERCITY_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QUrl>
#include <QTextCodec>
#include <QStringList>
#include <QTimer>
#include "3rdparty/cJSON/cJSON.h"
#include "server/serverDefine.h"
#include "globalVariable.h"

class ServerCity : public QObject
{
    Q_OBJECT
public:
    static ServerCity* getInstance();

signals:
    void signal_searchCity_success(QList<InfoCity>*);                       // 返回检索到的城市列表
    void signal_searchCity_failed();

    void signal_locate_success(InfoCity);                                   // 返回定位到的城市信息
    void signal_locate_failed();

    void signal_setCity_success(InfoCity);                                     // 若信号返回，则说明城市设置成功
    void signal_setCity_failed();


private slots:
    void slot_search_city(QString,LanguageType);
    void slot_locate_device(QString,QString,LanguageType);
    void slot_set_city(QString,QString);

    void slot_reply_search_city();                                      // 搜索城市返回信息
    void slot_reply_locate_device();                                    // 定位请求返回信息
    void slot_reply_set_city();                                         // 设置城市请求返回信息
    void slot_timeout_search_city();
    void slot_timeout_set_city();
    void slot_timeout_locate_device();

private:
    explicit ServerCity();
    void analyse_search_city(QString replyData);                        // 解析JSON数据
    void analyse_locate_device(QString replyData);                      // 解析JSON数据
    void analyse_set_city(QString replyData);                           // 解析JSON数据
    QString get_validValue(cJSON *item, QString field);


private:
    QNetworkAccessManager *manager;                                     // 网络接收发送管理器
    QNetworkRequest request;                                            // 请求

    QNetworkReply *replySearchCity;                                     // 搜索城市返回信息
    QNetworkReply *replyLocateDevice;                                   // 定位返回信息
    QNetworkReply *replySetCity;                                        // 设置城市信息

    QTimer *timerLocate;
    QTimer *timerSearchCity;
    QTimer *timerSetCity;

private:
    static ServerCity* instance;                                     // 单例内存释放逻辑
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(ServerCity::instance)
                delete ServerCity::instance;
        }
    };
    static FreeInstance free;

};

#endif // SERVERCITY_H
