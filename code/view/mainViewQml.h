#ifndef MAINVIEWQML_H
#define MAINVIEWQML_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QTimer>
#include <QTimeLine>
#include <QObject>
#include <QtDeclarative>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QEvent>
#include <QtGlobal>

#include "sysControl.h"
#include "sysGravity.h"
#include "cpu/sysCpu.h"

#include "dataAirControl.h"
#include "historyPage.h"
#include "globalVariable.h"
#include "sysDefine.h"
#include "data/sysData.h"
#include "control/function/city/funcCity.h"
#include "sysOCC.h"
#define DEVICE_POWER_VALUE          "/sys/class/power_supply/battery/capacity"                      //电池容量
#define DEVICE_POWER_STATUS         "/sys/class/power_supply/battery/status"                        //充电状态
#define QML_SOURCE_URL              "qrc:/resource/qml/testQml/Main.qml"

#define POWERON_CONFIRM_INTERVER     (60 * 1000 * 30)
#define TAP_INTERVAL                 (5 * 1000)
#include "driverWiFi.h"

enum PAGE
{
    PM = 0,
    TEMPERATURE,
    HUMIDITY
};
class MainViewQML : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float pmValue READ get_pmValue WRITE set_pmValue NOTIFY signal_pmChanged)
    Q_PROPERTY(float tempValue READ get_tempValue WRITE set_tempValue NOTIFY signal_tempChanged)
    Q_PROPERTY(float humValue READ get_humValue WRITE set_humValue NOTIFY signal_humChanged)

public:
    Q_INVOKABLE float get_pmValue();
    Q_INVOKABLE float get_tempValue();
    Q_INVOKABLE int get_tempUnit();
    Q_INVOKABLE float get_humValue();
    explicit MainViewQML(QWidget *parent = 0);


public slots:
    void set_pmValue(const float newValue);
    void set_tempValue(float newValue);
    void set_humValue(float newValue);
    void set_pm5Value(float newValue);
    void set_pm10Value(float newValue);
    void slot_setCO2eValue(const float newValue);
    void slot_settVOCValue(const float newValue);
    float pmValue();
    float tempValue();
    float humValue();
    float  slot_getPm5Value();
    float  slot_getPm10Value();
    float  slot_getTvocValue();
    float slot_getCO2eValue();
    float slot_getBatteryCapacity();                                    //获取当前电池容量
    bool slot_getBatteryStatusIsCharging();                             //判断当前电池是否充电
    int  slot_getCurrentIndex();                                        //获取当前主页index
    int  slot_getRomationAngle();                                       //获取横竖屏旋转角度
    void change_direction(DIRECTION);
    void slot_poweroff();                                               //关机
    void slot_noPoweroff();                                             //取消关机
    void slot_noPoweron();                                              //取消开机
    void slot_sysButtonLongPress();
    void slot_poweroffAnimation();
    int slot_getWifiIntensity();                                        //获取wifi强度
    QString slot_getWifissid();                                       //获取Wi-Fi ssid
    void slot_showWifiPage();                                           //显示wifi配置界面
    void slot_showMiAppPage();
    void slot_hideWifiPage();                                           //隐藏wifi配置界面
    QString slot_getNightMode();                                        //获取夜间模式参数
    void slot_setNightMode(bool);                                  //设置夜间模式
    void slot_doubleTap();
    void slot_timeModeChange();                                         //时间模式
    bool slot_getTimeMode();                                            //获取24小时制
    void slot_update_battery(InfoBattery);

    QString slot_getTempValue();                                        //获取温度值 精确到小数点后两位
    QString slot_getHumValue();                                         //获取湿度值，精确到小数点后两位
    void slot_updatePmData_lastTime(time_t time);                       //pm2.5更新时间
    void slot_pmLastUpdateTime();                                       //获取pm2.5更新时间 描述字符串
    QString slot_getpmLastUpdateTime();
    QString slot_getLocaltion();                                        //获取定位信息
    QString slot_getOutDoorTemp();                                      //获取室外温度
    QString slot_getOutDoorPm25();                                      //获取室外pm2.5
    QString slot_getOutDoorWeatherIcon();                               //获取天气图标
    void slot_finishPmRefresh();                                            //显示刷新按钮
    bool slot_isUsLang();                                               //
    void slot_setCurrentIndex(int index);
    void slot_updateUnit(UnitType unit);
    void slot_device_horizontalPrompt();
    void slot_device_pickUp();
    void slot_update_firmware();                                        //检测到新版本
    QString slot_getTvocUnit();
    bool slot_mainWifiIsShow();                                          //获取是否显示wifi
    void slot_screenOff();
    bool slot_isVerticalScreenMode();                                         //竖屏模式
    void slot_recoverPageIndex();
    void slot_mouseMove();
    void slot_mouseMarginTouch(bool);
    bool slot_getVerticalScreenRefresh();                            //获取竖屏刷新按钮状态
    bool slot_getAppBindStatus();
    bool slot_timeIsInit();                                            //时间是否被初始化
    void slot_timeInit_success();                                     //时间初始化成功

    ////////////////////////////////test///////////////////////////////////////////////////

    void slot_setLightValue(int value);                                 //设置亮度值
    int slot_getCurLightValue();
    QString slot_getWifiStatus();
    QString slot_getBatteryCurrentelectricity();
    QString slot_getBatteryTime();
    QString slot_getScreenTime();
    void slot_setCpuUseage100();
    void slot_setCpuUsage0();
    void slot_set120M();
    void slot_set1G();
    void slot_update_usage(QString);
    void slot_update_fre(float f);

    QString slot_getUsage();
    QString slot_getFre();
    void slot_pmOn();
    void slot_pmOff();
    QString  slot_getBaseLine();
    void slot_modeSwitch();
    void slot_syncRTC();
    //////////////////////////////////end test/////////////////////////////////////////////////////

signals:
    void signal_pmChanged();
    void signal_tempChanged();
    void signal_tempUnitChanged();
    void signal_tvocUnitChange();
    void signal_humChanged();
    void signal_pm5Changed();
    void signal_pm10Changed();
    void signal_tVOCValueChanged();
    void signal_CO2evaluechanged();

    void signal_nextPage();
    void signal_settingBack();

    void signal_jumpOptionPage();

    void signal_showPmValue();
    void signal_getPmData();

    void signal_horizonScreen();                                           //横屏
    void signal_vertialScreen();                                           //竖屏
    void signal_screenMode(int );                                          //屏幕模式

    void signal_buttonLongPress();
    void signal_wifiIntensity();                                           //wifi强度变化信号

    void signal_jumpWiFiPage();                                             //
    void signal_jumpAppPage();                                              //打开米家手机app
    void signal_changeNightMode();                                          //改变夜间模式

    void signal_timeModeChange();                                           //24小时制修改

    void singal_reading();                                                  //发送正在检测信息

    void signal_updatePmElapsed();                                          //

    void signal_updateLocal();                                              //更新定位信息
    void signal_update_weather();
    void signal_sampling_weather();
    void signal_startPmAnimation();                                         //开始pm动画
    void signal_pmDetected();                                              //显示刷新按钮
    void signal_changeCurrentIndex();                                       //主页面改变信号

    void signal_device_horizontalPrompt();
    void signal_device_pickUp();
    void signal_device_powerOff();
    void signal_interface_operations();
    void signal_update_firmware();
    void signal_screenoff();
    void siganl_startTimer();
    void signal_setMatrix(QMatrix matrix, DIRECTION direction);
    void signal_mouseMove();
    void signal_marginTouch();
    void signal_no_marginTouch();
    void signal_updateAppBindstat();
    void signal_timeInit_success();                                //时间初始化成功
    void signal_updateUsage();
    ////////////////////////////////test///////////////////////////////////////////////////

    void signal_set_lightValue(int);                                         //设置亮度
    void signal_screenOff();

    void signal_change_fre(bool);

    void signal_pmOn();
    void signal_pmOff();
    void signal_updateFre();
    void signal_set_timeAuto(bool);

    void signal_testFinished();
    void signal_cpuLoad();
    void signal_cpuNoLoad();
    void signal_autoLightChanged();
    //////////////////////////////////end test/////////////////////////////////////////////////////

protected slots:
    void show_historyPage(int curIndex);
    void hide_historyPage();
    void show_optionPage();
    void refresh_pmData();
    void slot_sysbuttonClick();
    void slot_update_wifi(WiFi);                                    //Wi-Fi状态
    void slot_update_location(QString);                                   //更新定位
    void slot_update_weather(InfoWeather);                                    //更新天气结构体
    void slot_update_airData(AirData);
    void slot_sampling_weather();                                          //开始获取天气数据
    void slot_updateAppBindstat(int);
    void slot_save_data();


private:
    void init(void);
    void scene_init(void);
    void data_init(void);
    void animation_init(void);
    void connect_init(void);

    void set_humTips(float fValue);                                     //设置湿度评价信息
    void set_temTips(float fValue);                                     //设置温度评价信息

    QString get_englishDate(QDate date);                                //获取英文时间
    QString get_wifiStatus();
    void read_baseline();
private:
    HistoryPage *historyPage;                                           //历史页面实例

    QDeclarativeView *view;                                             //用来加载qml脚本的视图实例

    float fPmValue;
    float fTempValue;
    float fHumValue;
    float fPm5Value;
    float fPm10Value;
    float fCO2eValue;
    float ftVOCValue;
    bool is_simple;
    QString strPmColor;
    int iCurrentPage;

    QMatrix matrix;
    int currentDirection;
    int romationAngle;                                                  //横竖屏旋转角度
    int curScreenMode;                                                  //当前屏幕类型
    int iWifiIntensity;                                                 //wifi强度

    time_t pmPrevUpdateTime;                                           //pm2.5更新时间
    QString sElapseTime;                                                //
    QTimer *pmUpdateTimer;                                               //pm2.5更新时间 定时器

    QString sLocation;                                                  //定位


    InfoWeather stWeather;                                                  //天气
    SysControl  *sysControl;
    SysData     *sysData;
    SysWiFi     *sysWiFi;
    SysGravity  *sysGravity;
    SysTime     *sysTime;

    DataAirControl *dataAirControl;
    FuncCity    *funcCity;
    FuncWeather *funcWeather;
    int  tap_index;                                                     //敲击壳体时记录当前页面的下标
    int  mainPageIndex;                                                  //当前页面下标
    bool mouseIsRelease;                                                //鼠标被释放

    InfoBattery battery;
    WiFi    wifi;
    bool    isNight;
    int iAppBindStatus;                                                 //App绑定状态

    ////////////////////////////////test///////////////////////////////////////////////////

    QString wifiStatus;
    DriverBattery *driverbattery;
    int timeclock;
    bool ischarging;
    bool isLightOn;
    int  screenOnTime;
    SysOCC *sysOCC;
    SysCPU *sysCPU;
    QString usage;
    float fre;
    QString filePath;
    bool pm25IsOn;
    int lightValue;
    DriverPM25 *driverPM25;                                             // PM2.5传感器类
    QString baseline;
    QStringList falgModelist;
    int modeIndex;
    QTimer *modeSwitchTimer;
    int num;
    //////////////////////////////////end test/////////////////////////////////////////////////////


};

#endif // MAINVIEWQML_H
