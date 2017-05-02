#ifndef FUNCAPP_H
#define FUNCAPP_H

#include <QThread>
#include <QPainter>
#include <QDebug>
#include <QFile>
#include <QPixmap>
#include <QDir>
#include <QTimer>
#include "globalStruct.h"
#include "dataAirDefine.h"
#include "miio/miioControl.h"
#include "file/config/fileConfig.h"
#include "3rdparty/qrcode/qrencode.h"

#define QRCODE_PATH         "qrcode.png"

class FuncApp : public QThread
{
    Q_OBJECT
public:
    static FuncApp * getInstance();
    bool get_bindkey_qrcode(QString&);
    bool get_bindState();

signals:
    void signal_request_bindkey();

    void signal_create_qrcode(QString);
    void signal_update_qrcode(QString);

    void signal_qrcode_timeout();
    void signal_update_bindStatus(int);

    void signal_net_abnormal();                                         // 网络异常
    void signal_net_disconnect();                                       // 网络断开

private slots:
    void slot_refresh_qrcode();
    void slot_remove_qrcode();

    void slot_update_bindKey(BindKey);
    void slot_updateBindKey_failed();

    void slot_update_bindStatus(int);
    void slot_createQRCode_result(int);

    void slot_net_connected();
    void slot_net_disconnect();

private:
    explicit FuncApp(QObject *parent = 0);
    void data_init();
    void connect_init();
    void run();

private:
    FileConfig *fileConfig;
    MiioControl *miioControl;
    BindKey bindKey;
    QTimer *timerValid;
    bool isConnected;
    bool isBind;

private:
    static FuncApp *instance;
    class FreeInstance
    {
    public:
        ~FreeInstance()
        {
            if(FuncApp::instance)
                delete FuncApp::instance;
        }
    };
    static FreeInstance free;
};

#endif // FUNCAPP_H
