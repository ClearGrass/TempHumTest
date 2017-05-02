#ifndef MIIOCONTROL_H
#define MIIOCONTROL_H

#include <QThread>
#include <QTcpSocket>
#include <QTimer>
#include <QHostAddress>
#include "3rdparty/cJSON/cJSON.h"

#define MIIO_SERVER_IP          "127.0.0.1"
#define MIIO_SERVER_PORT        54322

#define METHOD_GET_BIND_KEY     1

typedef struct bind_key
{
    QString value;
    int expire;
}BindKey;

class MiioControl : public QThread
{
    Q_OBJECT
public:
    static MiioControl* getInstance();

signals:
    void signal_update_bindKey(BindKey);
    void signal_update_bindStatus(int);
    void signal_updateBindKey_failed();

private slots:
    void slot_miio_dataRecived();
    void slot_miio_connected();
    void slot_miio_disconnect();

    void slot_request_bindkey();

private:
    void run();
    void data_init();
    void connect_init();


    void send_msg(QString);
    void analyse_method(QString);
    void analyse_result(QString);
    void miio_restore();
    void reply_result_OK(int);

    QString get_validString(cJSON *item, QString field);
    int get_validNum(cJSON *item, QString field);

private:
    QTcpSocket *miioSocket;
    QTimer *timer;
    bool isConnected;

private:
    static MiioControl *instance;
    explicit MiioControl(QObject *parent = 0);

};

#endif // MIIOCONTROL_H
