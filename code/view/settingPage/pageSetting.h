#ifndef PAGESETTING_H
#define PAGESETTING_H

#include <QWidget>
#include <QDeclarativeView>

#include "dataobject.h"
#include "wifiDataobject.h"
#include "math.h"
#include "sysControl.h"
#include "sysWiFi.h"
#include "sysTime.h"
#include "sysLanguage.h"
#include "cityDataobject.h"
#include "data/sysData.h"
#include "control/function/firmware/funcFirmware.h"
#include "control/function/city/funcCity.h"
#include "app/funcApp.h"
#define FILE_SYSVERSION "./SysSVN"

class PageSetting : public QWidget
{
    Q_OBJECT
public:
    explicit PageSetting(QWidget *parent = 0);
    ~PageSetting();
    void set_wifiEntery(int entry);
    void set_wifiWizardLanguage(QString lan);
    void set_otaError(int errorCode);
    void show_scrollBarAniation();

signals:
    void signal_jumpMainPage();
    void signal_jumpWiFiPage();
    void signal_jumpSettingPage();

    void signal_timeModeChange();                                       //二小时制修改信号

    void singal_changeScrollValue();                                    //修改滚动条位置
    void signal_initScrllValue();                                       //初始化滚动条位置

    void signal_autoChangeValue();                                      //自动调节亮度
    void signal_isAutoChange();
    void signal_nightInit();
    void signal_outLight();                                             //外界亮度
    void signal_inLight();                                              //

    void signal_initLang();                                             //初始化语言

    void signal_initTvocUnit();                                         //初始化tvoc单位
    void signal_intNightTime();

    void signal_initTempUnit();                                         //初始化温度单位
    void signal_initTimeDate();                                         //初始化时间日期

    void signal_initDateTimeSpinner();
    void signal_connect_withKey(QString ssid, QString key);
    void signal_showWifiList();
    void signal_connRes();
    void signal_connect_withNetID(QString);
    void signal_connectPublicWifi(QString);                             //连接公有Wi-Fi
    void signal_remove_cookie(QString ssid);
    void signal_remove_allCookie();                                     //
    void signal_wifi_connStatChange();                                  // WiFi连接状态改变
    void signal_jumpLangeWizard();

    void signal_initTimeZoneListPos();
    void signal_set_timeZone(QString);
    void signal_set_timeAuto(bool);
    void signal_set_dateTime(QString);

    void signal_set_lightValue(int);
    void signal_set_lightMode(bool);
    void signal_changeEntry();                                          //改变入口

    void signal_updateProgress();
    void signal_update_firmware();
    void signal_no_firmware();
    void signal_update_download_fail();
    void signal_update_download_success();
    void signal_initFirwarePage();
    void signal_errorInfo();


    void signal_showCityList();
    void signal_localnet_abnormal();
    void signal_loaclnet_disconnect();
    void signal_autoLocalRes();
    void signal_initLocalpage();
    void signal_localNetIpChange();

    void signal_resetDb();
    void signal_wizardFinish();

    void signal_start_download();
    void signal_check_update();
    void signal_check_updateDebug();

    void signal_device_reboot();
    void signal_interface_operations();

    void signal_set_night(bool);
    void signal_set_nightAuto(bool);
    void signal_set_nightMode(QString);
    void signal_set_nightPeriod(QString,bool);

    void signal_offInterval_charging(IntervalType);
    void signal_offInterval_discharging(IntervalType);

    void signal_intervalPM25_charging(IntervalType);
    void signal_intervalPM25_discharging(IntervalType);

    void signal_change_unitTEMP(UnitType);
    void signal_change_unitTVOC(UnitType);

    void signal_change_language(LanguageType);

    void signal_back();

    void signal_set_cityAuto();
    void signal_set_cityManual(QString);
    void signal_search_city(QString);

    void signal_refresh_listWiFi();
    void signal_showScrollBarAnimatio();
    void signal_refreshAppQrcode();
    void signal_setAppQrcode();
    void signal_setQrcodeTimeout();
    void signal_updateAppBindStatus(int);
    void signal_removeQrcode();                                         //删除二维码
    void signal_net_disconnect();                                       // 网络断开
    void signal_net_abnormal();                                         // 网络异常
    void signal_timeInit_success();                                //时间初始化成功
public slots:
    void on_page_defalut_back();
    void slot_setNightMode(int);                                        //设置夜间模式
    void slot_timeModeChange();                                         //24小时制修改
    void slot_setScrollValue(int value);                                //设置滚动条位置
    int set_getScrollValue();                                           //获取滚动条位置
    void  slot_setTempUnit(bool flag);                                   //设置温度单位
    bool slot_curTempUnitIsFahrenheit();                                //当前温度单位为华氏度
    void auto_changeValue(int value);                                   //自动调节亮度
    void slot_setAutoChangeLight(bool  checked);                        //设置是否自动调节亮度
    void slot_setLightValue(int value);                                 //设置亮度值
    int slot_getLightValue();                                           //获取当前亮度值
    void slot_setWifiStatus(int index,int status);
    QString slot_getSerialNumber();                                     //获取设备序列号
    QString slot_getDeviceMAC();                                        //后去mac
    bool slot_isAuto();                                                 //获取是否自动调节亮度
    int slot_getNightMode();
    QString slot_getAotoNight();
    QString slot_getSunRise();
    QString slot_getSunRiseInit();
    void slot_setLanguage(int);                                          //设置语言
    int  slot_getLanguage();
    int  slot_getTvocUnit();                                             //
    QString slot_getTvocUnitName();
    void slot_setTvocUnit(int index);
    void slot_setAotoNight(bool check);
    void slot_setSunRise(QString check);
    QString slot_getNightInterver();
    QString slot_getNightInterTime(int flag);
    QString slot_getAmp(int flag);
    void slot_setNightIntrTime(QString timeInterver, bool isStartTime);
    void slot_set24Hours(bool flag);
    bool slot_get24Hours();
    void slot_setAutoGetTime(bool flag);
    bool slot_getAutoGetTime();
    QString slot_getCurDateTime();
    QString slot_getDatetime(QString strDatetime);
    void slot_setZoneTime(QString zoneTime);
    int slot_getCurZoneIndex();
    QString slot_getTimeZone();
    bool slot_wifiListIsEmpty();
    void slot_wifiConnect(QString ssid, QString pwd);
    void slot_connect_result(int result);
    int slot_getWifiConnRes();
    int slot_get_keyType(QString ssid);
    void slot_connect_withNetID(QString ssid);
    void slot_connect_publicWiFi(QString ssid);

    void slot_wifiListAllowRefresh(bool flag);
    void slot_forgetWifi(QString ssid);
    void slot_forgetAllWifi();
    int slot_getWifiConnStatu();
    QString slot_getCurConnSSid();

    void slot_wifi_status(WiFi wifi);                                    //连接不可用

    void slot_setDateTime(QString datime);
    void slot_showWifiPage();                                             //显示wifi页面
    void slot_showMiAppPage();                                            //显示米家手机app
    int slot_getEntry();                                                  //获取wifi入口

    QString slot_getCurVersion(bool );                                    //获取当前版本
    QString slot_getCurSystem(bool );                                    //获取当前版本
    QString slot_getVersionDes();
    int slot_start_download();                                            //开始下载固件
    void slot_start_checkUpdate(int type);                                        //检查更新

    int slot_getProgress();                                               //获取进度条百分比

    void slot_updateProgress(qint64,qint64);
    void slot_update_firmware(QString,QString );
    void slot_update_firmware();

    void slot_update_download_success();
    void slot_ota_net_abnormal();
    void slot_ota_net_disconnect();
    int slot_getErrorCode();
    void slot_setAutoLocal();
    QString slot_get_locationAuto();
    QString slot_get_locationMannual();
    QString slot_getLocaltionCity();
    bool slot_getLocalMode();
    void set_cityManual(QString strCity);
    void slot_searchCity(QString str);
    void slot_searchRecCityList(QList<InfoCity> *cityList);
    int  slot_getSearchCityNum();
    void slot_local_net_abnormal();
    void slot_local_net_disconnect();
    void slot_netIp(QString ip);
    int slot_getLocalErrorCode();
    void slot_locate_success(QString);
    void slot_locate_failed();
    QString slot_getLocalIp();
    QString slot_getAutoLocalRes();
    void slot_outLight_value(int value);
    void slot_intLight_value(int value);
    int slot_getOutOrInLightValue(int type);
    void slot_reset();
    void slot_restart();
    void slot_jumpLanWizard();
    void slot_wizardFinish();
    bool slot_isUsLang();                                               //
    QString slot_getWifiWizardLanguage();
    QString slot_getPmDetectingFrequency(int type);
    QString slot_getScreenAutoInterval(int type);
    void slot_setPmDetetingFrequency(int type,QString value);
    void slot_setScreenAutoInterval(int type,QString value);
    void slot_setScreenAutoIndex(int index);
    int slot_getScreenAutoIndex();
    void slot_setPmDetetingSettingType(int type);
    int slot_getPmDetetingSettingType();
    int slot_getScreenSettingScrollValue();
    void slot_setSceenSettingScrollvalue(int value);
    void slot_in_night(bool);
    int  slot_getCurPageIndex();
    QString slot_getAutoTimezone();
    void slot_setMainWifiPageIsHide();
    QString slot_getAppQrcode();                                    //获取米家App二维码
    void slot_refresh_appQrcode();                                  //刷新二维码
    void slot_removeQrcode();                                       //删除二维码
    void slot_setQrcode(QString qrcode);
    QString slot_getQrcode();
    void slot_qrcodeTimeout();
    bool slot_getAppBindStat();                                     //获取米加app绑定状态
    void slot_updatebindstat(int);
    bool slot_getDoubleTapChecking();
    void slot_setDoubleTapChecking(bool checked);
    void slot_net_abnormal();
    void slot_net_disconnect();
    void slot_timeInit_success();                                     //时间初始化成功
    bool slot_timeIsInit();                                            //时间是否被初始化

public slots:
    void slot_setQMl(int i);

    void refresh_page(QList<WiFi>* wifiList);
    void slot_back();

private:
    void page_init();
    void connect_init();
    void control_init();
    QString get_sysVersion();
    void set_zoneInfo();                                                //设置时区信息
    void init_light();                                                  //初始化亮度
    void init_nightMode();                                              //

private:
    QDeclarativeView *view;
    QList<QObject*> dataZoneList;                                      //时区信息列表
    QList<QObject*> wifiDataList;                                       //wifi列表
    QList<QObject*> cityDataList;                                      //城市列表
    QDeclarativeContext *ctxt;
    SysControl *sysControl;
    SysWiFi     *sysWiFi;
    SysData     *sysData;
    SysTime     *sysTime;
    SysLanguage *sysLanguage;

    FuncFirmware *funcFirmware;                                            //固件版本
    FuncCity *funcCity;
    FuncApp  *funcApp;                                                  //

    bool isNight;

    QPoint posPressed;                                                  // 按下坐标
    QPoint posRelease;                                                  // 抬起坐标

    int iScrollValue;                                                   //滚动条位置

    int  iCurPage;                                                      //当前页面index
    QString sCurSsid;
    int curLightValue;                                                  //当前亮度值
    int iWifiConnectResult;                                             //wifi链接结果
    bool wifiListAllowRefresh;

    int wifiConnStat;                                                   //wifi 连接状态
    int wifiEntry;                                                      //wifi入口（从主界面进入还是设置界面）
    QString newVersion;
    QString newVersionDes;
    int downLoadProcess;                                                //下载进度
    int otaErrorCode;                                                      //错误代码
    int localErrorCode;                                                 //定位错误代码
    QString autoCity;
    int curTimeZoneIndex;

    int outLightValue;                                                 //外界亮度
    int inLightValue;                                                  //室内亮度
    QString strIp;
    QString wifiPageWizardLanguage;
    int initScreenIndex;
    int pmDetectingFrequencySettingType;
    int iScreenSettingScrollValue;                                      //屏幕设置页面滚动条位置
    QString qrCode;
    bool  appBindStatus;

};

#endif // PAGESETTING_H
