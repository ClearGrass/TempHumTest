#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include <QEventLoop>
#include <QProcess>
#include <QThread>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTextCodec>
#include <QFontDatabase>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QEvent>
#include <QDebug>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include "globalVariable.h"
#include "linux/sysinfo.h"
#include "database.h"
#include "mainViewQml.h"
#include "pageSetting.h"
#include "setupwizard.h"
#include "wizardwait.h"
#include "powerOn/pagePowerOn.h"
#include "power/pagePowerOff.h"
#include "wizardwait.h"
#include "dataAirControl.h"
#include "control/function/app/funcApp.h"
#include "control/function/firmware/funcFirmware.h"
#include "control/function/city/funcCity.h"
#include "control/function/weather/funcWeather.h"
#include "sysControl.h"
#include "wilddog/wilddogControl.h"
#include "serverControl.h"
#include "debugControl.h"
#include "3rdparty/qrcode/qrencode.h"
#include "dropDown.h"

#define SCREEN_OFF_INTERVAL         30000
#define POWER_OFF_INTERVAL          (30*60*1000)
#define QML_POWEROFF_SOURCE_URL     "qrc:/resource/qml/poweroff/PowerOffAnimation.qml"


class MainControl : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MainControl(QObject *parent = 0);

signals:
    void signal_interface_operations();
    void signal_change_page();
    void signal_back_page();
    void signal_mouseMove();
    void signal_marginTouch(bool);
    void signal_createQRCode_result(int);

protected:
    bool eventFilter(QObject *, QEvent *);

private slots:
    void slot_enter_system();
    void slot_init_system();
    void slot_button_singleClick();
    void slot_button_longPress();
    void slot_jump_pageSetting();
    void slot_jump_pageMain();
    void slot_device_powerOff();
    void slot_screen_on();
    void slot_screen_off();
    void slot_setMatrix(QMatrix matrix, DIRECTION direction);
    void slot_create_qrcode(QString);
    void slot_dropUp();
private:
    void data_init();
    void page_init(void);
    void widget_init(void);
    void system_init(void);
    void connect_init(void);
    void style_init();
private:
    WizardWait *pageInit;
    SetupWizard *pageWizard;
    PagePowerOn *pagePowerOn;
    PagePowerOff *pagePowerOff;
    MainViewQML *mainViewQml;
    PageSetting *pageSetting;
    DropDown    *dropDownQml;

    FileConfig      *fileConfig;
    SysWiFi         *sysWiFi;
    Database        *database;
    SysControl      *sysControl;

    FuncApp         *funcApp;
    FuncCity        *funcCity;
    FuncWeather     *funcWeather;
    FuncFirmware    *funcFirmware;

    DataAirControl  *dataAirControl;
    WilddogControl  *wilddogControl;
    ServerControl   *serverControl;
    DebugControl    *debugControl;

    QGraphicsProxyWidget *pageWizardProxy;
    QGraphicsProxyWidget *pageMainProxy;
    QGraphicsProxyWidget *pageSettingProxy;
    QGraphicsProxyWidget *pagePowerOnProxy;
    QGraphicsProxyWidget *pagePowerOffProxy;
    QGraphicsProxyWidget *pageInitProxy;
    QGraphicsProxyWidget *dropDownProxy;

    QGraphicsScene *mainScene;                                             //场景
    int currentDirection;
    MouseType mouseType;
    QPropertyAnimation *dropDownAnimation;
    QPropertyAnimation *dropUpAnimation;
private:
    QTimer *timerPowerOff;
    QTimer *timerScreenOff;
    bool isScreenOn;
    bool bAllowDrop;                                                      //允许下拉
};

#endif // MAINCONTROL_H
