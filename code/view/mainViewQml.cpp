#include "mainViewQml.h"
#include "globalVariable.h"
#include "sysWiFi.h"
#include <iomanip>
#include <QTimer>


MainViewQML::MainViewQML(QWidget *parent)
    :QWidget(parent)
{
    //    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    this->setStyleSheet("background: transparent;border:0px");
    //    resize(HORIZONTAL_WIDTH,HORIZONTAL_HEIGHT);
    filePath = "./debugFile/123.cxv";
    wifiStatus = "Disconnect";
    baseline = "0x000000";
    //初始化界面信息
    init();

    //数据类线程初始化
    data_init();


    //信号槽连接定义
    connect_init();

    //初始化场景
    scene_init();

    //删除历史文件

}

/*******************************************************************************
* Author        :   虎正玺@2016-10-08
* Function Name :   init
* Description   :   初始化界面信息
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::init()
{
    usage = "0";
    lightValue = 0;
    //当前屏幕方向
    fre = 1.008;
    num = 0;
    currentDirection = 0;
    timeclock = 0;
    ischarging = false;
    isLightOn = true;
    screenOnTime = 0;


    //初始化变量
    fPmValue = -1;
    fTempValue = 0;
    fCO2eValue = 0;
    ftVOCValue = 0;
    fHumValue = 0;
    romationAngle = 0;
    iWifiIntensity = -1;
    mainPageIndex = 0;
    tap_index = -1;
    mouseIsRelease = true;

    pmPrevUpdateTime = QDateTime::currentDateTime().toTime_t();

    curScreenMode = HORIZONTAL_MODE;

    //pm2.5更新时间描述定时器
    pmUpdateTimer = new QTimer();

    pmUpdateTimer->setInterval(6000);
    falgModelist.append("Basic_mode_and_wifi_off");
    falgModelist.append("Basic mode");
    falgModelist.append("Basic_mode_and_screen_off");
    falgModelist.append("Basic mode");
    falgModelist.append("High CPU Freq mode");
    falgModelist.append("LCD max brightness mode");
    falgModelist.append("CPU max load");
    modeIndex = 0;
    modeSwitchTimer  = new QTimer();
    freSwitchTimer = new QTimer();
    pm_off_timer = new QTimer(this);
    pm_on_timer = new QTimer(this);

    saveDataTimer = new QTimer(this);



    //    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   data_init
* Description   :   数据类线程初始化
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::data_init()
{
    sysCPU = new SysCPU();
    sysocc1 = new SysOCC1();
    stsocc2 = new SysOCC2();

    sysOCC = new SysOCC();

    driverPM25 = DriverPM25::getInstance();
    driverPM25->start();
    sysControl      = SysControl::getInstance();
    sysWiFi         = SysWiFi::getInstance();
    sysGravity      = SysGravity::getInstance();
    sysData         = SysData::getInstance();
    sysTime         = SysTime::getInstance();
    //    funcWeather     = FuncWeather::getInstance();
    //        funcCity        = FuncCity::getInstance();
    dataAirControl  = DataAirControl::getInstance();

    isNight = sysControl->get_night();
    battery = sysControl->get_battery();
    wifi    = sysWiFi->get_currentWiFi();
    //        sLocation =  funcCity->get_location();
    iAppBindStatus = sysControl->get_value(APP_BIND_STATUS).toInt();
    if(isNight)
    {
        qDebug()<<"NIGHT___MODE";
    }
    else
    {
        qDebug()<<"DAY____MODE";
    }
    sysCPU->start();
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   connect_init
* Description   :   信号槽链接初始化
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::connect_init()
{
    connect(dataAirControl, SIGNAL(signal_sampling_start(time_t)), this, SLOT(slot_updatePmData_lastTime(time_t)));
    connect(dataAirControl, SIGNAL(signal_sampling_off()), this, SLOT(slot_finishPmRefresh()));
    connect(dataAirControl, SIGNAL(signal_update_data(AirData)), this, SLOT(slot_update_airData(AirData)));
    connect(dataAirControl, SIGNAL(signal_update_rawData(float,float, float , float)), this, SLOT(slot_updataRawData(float, float, float, float)));
    connect(dataAirControl, SIGNAL(signal_update_tempFlag(int,int,int, int, int)), this, SLOT(slot_updateTempFlag(int,int,int, int, int)));
    connect(this, SIGNAL(signal_getPmData()), dataAirControl, SLOT(slot_sampling_pm25()));
    connect(pmUpdateTimer, SIGNAL(timeout()), this, SLOT(slot_pmLastUpdateTime()));
    //    pmUpdateTimer->start();
    connect(sysData, SIGNAL(signal_change_unitTEMP(UnitType)), this, SLOT(slot_updateUnit(UnitType)));
    connect(sysData, SIGNAL(signal_change_unitTVOC(UnitType)), this, SLOT(slot_updateUnit(UnitType)));
    connect(sysGravity, SIGNAL(signal_device_horizontalPrompt()), this, SLOT(slot_device_horizontalPrompt()));
    connect(sysGravity, SIGNAL(signal_device_pickUp()), this, SLOT(slot_device_pickUp()));

    //    connect(funcWeather, SIGNAL(signal_update_weather(InfoWeather)), this, SLOT(slot_update_weather(InfoWeather)));
    //    connect(funcWeather, SIGNAL(signal_sampling_weather()), this, SLOT(slot_sampling_weather()));
    //        connect(funcCity, SIGNAL(signal_update_cityName(QString)), this, SLOT(slot_update_location(QString)));
    //    connect(sysControl, SIGNAL(signal_change_direction(DIRECTION)), this, SLOT(change_direction(DIRECTION)));
    connect(sysControl, SIGNAL(signal_update_battery(InfoBattery)), this, SLOT(slot_update_battery(InfoBattery)));
    connect(sysControl, SIGNAL(signal_shell_doubleTap()), this, SLOT(slot_doubleTap()));
    connect(sysControl, SIGNAL(signal_in_night(bool)), this, SLOT(slot_setNightMode(bool)));
    connect(sysWiFi, SIGNAL(signal_update_wifi(WiFi)), this, SLOT(slot_update_wifi(WiFi)));
    connect(sysTime, SIGNAL(signal_timeInit_success()), this, SLOT(slot_timeInit_success()));
    connect(this, SIGNAL(signal_set_timeAuto(bool)), sysTime, SLOT(slot_set_timeAuto(bool)));

    connect(this, SIGNAL(signal_screenoff()), sysControl, SLOT(slot_screenOff()));
    connect(this, SIGNAL(signal_set_lightValue(int)), sysControl, SLOT(slot_set_lightValue(int)));

    connect(sysCPU, SIGNAL(signal_update_cpuOcc(QString)), this, SLOT(slot_update_usage(QString)));
    connect(sysCPU, SIGNAL(signal_update_cpuFre(float)), this, SLOT(slot_update_fre(float)));

    connect(this, SIGNAL(signal_change_fre(bool)), sysCPU, SLOT(slot_change_fre(bool)));
    connect(this, SIGNAL(signal_pmOn()), driverPM25, SLOT(start_sampling()));
    connect(this, SIGNAL(signal_pmOff()), driverPM25, SLOT(stop_sampling()));
    connect(driverPM25, SIGNAL(signal_update_dataPM25(float)), this, SLOT(set_pmValue(float)));

    connect(modeSwitchTimer, SIGNAL(timeout()), this, SLOT(slot_modeSwitch()));
    connect(freSwitchTimer, SIGNAL(timeout()), this, SLOT(slot_switchFre()));
    connect(saveDataTimer, SIGNAL(timeout()), this, SLOT(slot_save_data()));
    connect(pm_off_timer, SIGNAL(timeout()), this, SLOT(slot_pm_off_timeout()));
    connect(pm_on_timer, SIGNAL(timeout()), this,SLOT(slot_pm_on_timeout()));
    //两个小时
    //    modeSwitchTimer->start(1000 *60 *60 * 2);
    //    modeSwitchTimer->start(1000 *60 * 5);

    //    modeSwitchTimer->start(40*1000);
    //    QTimer::singleShot(1000 * 60 * 60 *2, this, SLOT(slot_pmOn()));
    //        freSwitchTimer->start(FRE_INTERVAL);
    saveDataTimer->start(1000);

}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   scene_init
* Description   :   初始化场景
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::scene_init(void)
{
    //设置场景
    //    this->setScene(&uiScene);

    //注册对象到MQL
    qmlRegisterType<MainViewQML>("MainView", 1, 0, "MainView");
    qmlRegisterType<HistoryPage>("HistoryPage", 1, 0, "HistoryPage");

    //定义用来加载QML脚本文件的实例对象
    view = new QDeclarativeView(this);

    //设置窗口样式为透明
    view->setStyleSheet("background-color:transparent");
    view->hide();

    //获取QML对象并为之设置 名为'MainViewProperty'的自定义属性
    QDeclarativeEngine *engine = view->engine();
    QDeclarativeContext *context = engine->rootContext();
    context->setContextProperty("MainViewProperty",this);

    //加载QML文件
    view->setSource(QUrl(QML_SOURCE_URL));

    //创建历史界面底对象
    historyPage = new HistoryPage();

    historyPage->hide();
    view->show();
    connect(historyPage,SIGNAL(signal_backDefaultPage()),this,SLOT(hide_historyPage()));

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(Qt::black));
    this->setPalette(palette);

    slot_setLightValue(0);
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   get_tempUnit
* Description   :   获取温度单位
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
int MainViewQML::get_tempUnit()
{
    return sysData->get_unitTEMP() == CEL ? 0 :1;
}

/*******************************************************************************
* Function Name  :  slot_getTvocUnit
* Description    :  获取TVOC单位
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString MainViewQML::slot_getTvocUnit()
{
    QString unit;
    switch (sysData->get_unitTVOC()) {
    case MG_M3:
        unit = "mg/m³";
        break;
    case PPM:
        unit = "ppm";
        break;
    case PPB:
        unit = "ppb";
        break;
    default:
        break;
    }
    return unit;
}

/*******************************************************************************
* Author        :   虎正玺@2017-01-19
* Function Name :   slot_mainWifiIsShow
* Description   :   主页面是否显示WiFi页面
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
bool MainViewQML::slot_mainWifiIsShow()
{
    return sysControl->get_value(WIFI_MAIN_PAGE) == "true" ? true : false;
}


void MainViewQML::slot_screenOff()
{
    emit signal_screenoff();
}

/*******************************************************************************
* Author        :   虎正玺@2017-01-26
* Function Name :   slot_isHorizonScreenMode
* Description   :   判断当前是否为竖屏模式
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
bool MainViewQML::slot_isVerticalScreenMode()
{
    return curScreenMode == HORIZONTAL_MODE ? false : true;
}

/*******************************************************************************
* Author        :   虎正玺@2017-02-10
* Function Name :   slot_recoverPageIndex
* Description   :   敲击壳体时，跳转到pm2.5页面并开始检测pm2.5，检测完毕后，数值稳定5秒后返回之前停留的主界面
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_recoverPageIndex()
{
    slot_setCurrentIndex(tap_index);
    tap_index = -1;
}

/*******************************************************************************
* Author        :   虎正玺@2017-03
* Function Name :   slot_mouseMove
* Description   :   鼠标移动事件
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_mouseMove()
{
    emit signal_mouseMove();
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_mouseMarginTouch
* Description   :   屏幕边缘触摸
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_mouseMarginTouch(bool isTouch)
{
    if(isTouch)
        emit signal_marginTouch();
    else
        emit signal_no_marginTouch();
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_getVerticalScreenRefresh
* Description   :   获取竖屏刷新按钮状态
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
bool MainViewQML::slot_getVerticalScreenRefresh()
{
    return sysControl->get_value(VERTICAL_SCREEN_REFRESH) == "true" ? true : false;
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_getAppBindStatus
* Description   :   获取App绑定状态
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
bool MainViewQML::slot_getAppBindStatus()
{
    return iAppBindStatus;
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_timeIsInit
* Description   :   时间是否被初始化
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
bool MainViewQML::slot_timeIsInit()
{
    return sysControl->get_value(TIME_INIT) == "true" ? true : false;
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_timeInit_success
* Description   :   时间初始化成功
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_timeInit_success()
{
    emit signal_timeInit_success();
}

void MainViewQML::slot_clearLogs()
{
    system(qPrintable("rm /usr/bin/qtapp/debugFile/*"));
}

/*******************************************************************************
* Function Name  :  slot_update_battery
* Description    :  刷新电池信息
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainViewQML::slot_update_battery(InfoBattery battery)
{
    this->battery = battery;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_getBatteryCapacity
* Description   :   获取当前电池容量
* Input         :   None
* Output        :   None
* Return        :   当前电池容量
*******************************************************************************/
float MainViewQML::slot_getBatteryCapacity()
{
    float fCapacity  = battery.capacity;
    return fCapacity;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_getBatteryStatusIsCharging
* Description   :   判断当前电池是否充电
* Input         :   None
* Output        :   None
* Return        :   true：充电 false：放电
*******************************************************************************/
bool MainViewQML::slot_getBatteryStatusIsCharging()
{
    bool flag = false;

    if(battery.status == CHARGING || battery.status == FULL)
    {
        flag =  true;
    }
    else
    {
        flag =  false;
    }
    //修改时间标志位
    timeclock++;

    if(flag != ischarging)
    {
        timeclock = 0;
        ischarging  = flag;
    }

    screenOnTime++;
    if(isLightOn != sysControl->is_ScreenOn())
    {
        isLightOn = sysControl->is_ScreenOn();
        screenOnTime = 0;
    }
    return flag;

}

/*******************************************************************************
* Function Name  :  slot_update_airData
* Description    :  刷新页面数据
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainViewQML::slot_update_airData(AirData airData)
{

    if(PM25 == airData.type)
    {
        set_pmValue(airData.value);
    }
    else if(TEMP == airData.type)
    {
        set_tempValue(airData.value);
    }
    else if(HUMI == airData.type)
    {
        set_humValue(airData.value);
    }
    else if(TVOC == airData.type)
    {
        slot_settVOCValue(airData.value);
    }
    else if(CO2E == airData.type)
    {
        slot_setCO2eValue(airData.value);
    }
    else if(PM5 == airData.type)
    {
        set_pm5Value(airData.value);
    }
    else if(PM10 == airData.type)
    {
        set_pm10Value(airData.value);
    }
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   set_pmValue
* Description   :   设置pm2.5值
* Input         :   const float newValue pm2.5值
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::set_pmValue(float newValue)
{
    //如果形参和当前值相等则返回
    if(newValue == fPmValue)
    {
        return;
    }
    //设置当前pm值
    fPmValue = newValue;

    //触发改变信号

    //设置评价信息
    emit signal_pmChanged();

}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   set_tempValue
* Description   :   设置温度值
* Input         :   const float newValue pm2.5值
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::set_tempValue(float newValue)
{   
    fTempValue = newValue;
    emit signal_tempChanged();
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   set_tempValue
* Description   :   设置温度单位
* Input         :   QString str 温度单位
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_updateUnit(UnitType unit)
{   
    if(unit == CEL || unit == KFC)
    {
        emit signal_tempUnitChanged();
    }
    else
    {
        emit signal_tvocUnitChange();
    }

}

void MainViewQML::slot_device_horizontalPrompt()
{
    emit signal_device_horizontalPrompt();
}

void MainViewQML::slot_device_pickUp()
{
    emit signal_device_pickUp();
}


void MainViewQML::slot_update_firmware()
{
    emit signal_update_firmware();
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   set_humValue
* Description   :   设置湿度值
* Input         :   float newValue pm2.5值
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::set_humValue(float newValue)
{
    if(newValue == fHumValue)
    {
        return;
    }
    fHumValue = newValue;
    emit signal_humChanged();
}

void MainViewQML::set_pm5Value(float newValue)
{
    fPm5Value = newValue;
    emit signal_pm5Changed();
}

void MainViewQML::set_pm10Value(float newValue)
{
    fPm10Value = newValue;
    emit signal_pm10Changed();
}

void MainViewQML::slot_setCO2eValue(const float newValue)
{
    fCO2eValue = newValue;
    emit signal_CO2evaluechanged();
}

void MainViewQML::slot_settVOCValue(const float newValue)
{
    read_baseline();
    ftVOCValue = newValue;
    emit signal_tVOCValueChanged();
}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   show_historyPage
* Description   :   显示历史界面
* Input         :   int curIndex:历史界面类型
*                   0--pm 1--温度   2--湿度 3--tVoc   4--co2e
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::show_historyPage(int curIndex)
{
    historyPage->set_index(curIndex);
    historyPage->show();
    view->hide();
    gCurrent_pageIndex = PAGE_HIS;
}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_sysbuttonClick
* Description   :   横屏单机滑动一屏  竖屏单机检测pm2.5
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_sysbuttonClick()
{
    //当前页面是历史页面
    if(view->isHidden() )
    {
        // 隐藏历史界面
        hide_historyPage();
        return;
    }

    //横屏时候滑动一屏
    if(curScreenMode == HORIZONTAL_MODE)
    {
        //复杂模式
        emit signal_nextPage();
    }
    else
    {
        if(sysControl->get_value(VERTICAL_SCREEN_REFRESH) == "true")
        {
            sysControl->set_value(VERTICAL_SCREEN_REFRESH, "false");
        }

        //竖屏时检测pm2.5
        emit singal_reading();
        refresh_pmData();
    }
}


/*******************************************************************************
* Author        :   虎正玺@2016-11-01
* Function Name :   slot_update_wifi
* Description   :   wifi状态更新槽
* Input         :   int status wifi状态
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_update_wifi(WiFi wifi)
{
    this->wifi = wifi;
    slot_getWifiIntensity();
    //发送wifi强度信号
    emit signal_wifiIntensity();
}

/*******************************************************************************
* Function Name  :  slot_update_location
* Description    :  刷新当前位置
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainViewQML::slot_update_location(QString location)
{
    sLocation = location;
    emit signal_updateLocal();
}

/*******************************************************************************
* Function Name  :  slot_update_weather
* Description    :  刷新当前天气
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainViewQML::slot_update_weather(InfoWeather weather)
{
    stWeather = weather;
    emit signal_update_weather();
}

/*******************************************************************************
* Function Name  :  slot_weather_sampling
* Description    :  天气数据正在采集
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void MainViewQML::slot_sampling_weather()
{
    emit signal_sampling_weather();
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_updateAppBindstat
* Description   :   更新App绑定状态
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_updateAppBindstat(int stat)
{
    iAppBindStatus = stat;
    emit signal_updateAppBindstat();
}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   change_direction
* Description   :   横竖屏方向改变
* Input         :   int direction 方向
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::change_direction(DIRECTION direction)
{
    if(gCurrent_pageIndex  != PAGE_MAIN)
    {
        return ;
    }

    int  i = direction - currentDirection;

    if(i == 0)
    {
        return;
    }
    currentDirection = direction;

    //旋转角度
    romationAngle = i*90;
    matrix.rotate(i*90);
    //    this->setMatrix(matrix);

    //横屏
    if(direction == HORIZONTAL)
    {
        //屏幕类型修改为横屏
        curScreenMode = HORIZONTAL_MODE;
        this->setGeometry(QRect(0,0,854,480));
        //发送横屏信号
        emit signal_horizonScreen();

    }
    else
    {
        //屏幕类型改为竖屏
        curScreenMode = VERTICAL_MODE;
        this->setGeometry(QRect(0,-183,480,854));
        emit signal_vertialScreen();
    }
    emit signal_setMatrix(matrix, direction);

}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_poweroff
* Description   :   关机
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_poweroff()
{
    emit signal_device_powerOff();
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_noPoweroff
* Description   :   取消关机
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_noPoweroff()
{
    if(iCurrentPage == OPTION_PAGE)
    {
        emit signal_jumpOptionPage();
    }
    view->show();
}

/*******************************************************************************
* Author        :   虎正玺@2016-12-21
* Function Name :   slot_noPoweron
* Description   :   关机
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_noPoweron()
{
    emit signal_device_powerOff();
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_sysButtonLongPress
* Description   :   系统按钮长按槽
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_sysButtonLongPress()
{
    view->hide();
    emit siganl_startTimer();
}

void MainViewQML::slot_poweroffAnimation()
{
    //    //定义用来加载QML脚本文件的实例对象
    //    powerOffView = new QDeclarativeView(this);
    //    powerOffView->engine()->rootContext()->setContextProperty("MainViewProperty", this);

    //    //加载QML文件
    //    powerOffView->setSource(QUrl(QML_POWEROFF_SOURCE_URL));
    //    powerOffView->show();

}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_getWifiIntensity
* Description   :   获取wifi强度
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
int MainViewQML::slot_getWifiIntensity()
{
    switch (wifi.status) {
    case CONNECTED:
        //信号强度
        iWifiIntensity = wifi.signalNum;
        wifiStatus = wifi.ssid;

        break;
    case CONNECTTING:
        iWifiIntensity = -2;
        wifiStatus = "Disconnect";

        break;
    case UNUSEABLE:
        //连接异常
        iWifiIntensity = -3;
        wifiStatus = "Disconnect";

        break;
    case DISCONNECT:
        //wifi未连接
        iWifiIntensity = -1;
        wifiStatus = "Disconnect";

        break;
    default:
        break;
    }
    return iWifiIntensity;
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_getWifissid
* Description   :   获取ssid
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
QString MainViewQML::slot_getWifissid()
{
    return wifi.status == CONNECTED ? wifi.ssid : "";
}



void MainViewQML::slot_showWifiPage()
{
    // 发送信号，使得系统知道当前进入WiFi界面
    emit signal_jumpOptionPage();
    emit signal_jumpWiFiPage();
}

void MainViewQML::slot_showMiAppPage()
{
    // 发送信号，使得系统知道当前进入米家app界面
    emit signal_jumpOptionPage();
    emit signal_jumpAppPage();
}

void MainViewQML::slot_hideWifiPage()
{
    //    view->show();
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_getNightMode
* Description   :   获取夜间模式参数
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
QString MainViewQML::slot_getNightMode()
{
    if(isNight)
    {
        return "1";
    }
    else
    {
        return "2";
    }
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_setNightMode
* Description   :   设置夜间模式
* Input         :   int mode ==1夜间模式  ==2 白天模式
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_setNightMode(bool isNight)
{
    if(sysControl->get_value(TIME_INIT) == "true")
    {
        this->isNight = isNight;
        emit signal_changeNightMode();
    }
}



void MainViewQML::slot_doubleTap()
{
    //开始检测pm2.5
    emit signal_getPmData();
    tap_index = mainPageIndex;

    //跳转到pm2.5页面
    slot_setCurrentIndex(1);
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_timeModeChange
* Description   :   24小时制修改
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_timeModeChange()
{
    emit signal_timeModeChange();
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_getTimeMode
* Description   :   获取24小时制
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
bool MainViewQML::slot_getTimeMode()
{
    if(sysControl->get_value(TIME_MODE) == DEFAULT_TIME_MODE)
    {
        return true;
    }
    else
    {
        return false;
    }
    return true;
}

/*******************************************************************************
* Author        :   虎正玺@2016-11-08
* Function Name :   slot_getTempValue
* Description   :   获取温度值 精确到小数点后两位
* Input         :   None
* Output        :   None
* Return        :   返回湿度值
*******************************************************************************/
QString MainViewQML::slot_getTempValue()
{
    return  QString::number(fTempValue, 'f',2);
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_getHumValue
* Description   :   获取湿度值，精确到小数点后两位
* Input         :   None
* Output        :   None
* Return        :   返回温度值
*******************************************************************************/
QString MainViewQML::slot_getHumValue()
{
    return  QString::number(fHumValue, 'f',2);
}

QString MainViewQML::slot_getRawTempValue()
{
    return  QString::number(fRawTempValue, 'f',2);

}

QString MainViewQML::slot_getSecondTempValue()
{
    return  QString::number(fRawSecondValue, 'f',2);
}

QString MainViewQML::slot_getRawHumValue()
{
    return  QString::number(fRawHumValue, 'f',2);
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_updatePmData_lastTime
* Description   :   隐藏历史界面
* Input         :   time_t time 时间戳
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_updatePmData_lastTime(time_t time)
{
    pmPrevUpdateTime = time;
    pm25IsOn = true;
    emit signal_startPmAnimation();
}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_pmLastUpdateTime
* Description   :   更新pm2.5更新时间 描述字符串
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_pmLastUpdateTime()
{
    //临时变量
    QString strTmp = "";

    //距离上次更新时间间隔 单位秒
    uint elapsedTime = QDateTime::currentDateTime().toTime_t() - pmPrevUpdateTime;

    //合多少分钟
    uint mins = elapsedTime / 60;

    //合多少小时
    uint hours = elapsedTime/ (60 * 60);

    //合多少天
    uint days = elapsedTime / (60 * 60 * 24);


    if(mins <= 0)
    {
        strTmp = "";
    }
    else if(mins > 0 && hours <= 0 )
    {
        if(mins == 1)
        {
            strTmp = QString("%1 ").arg(mins) + tr("min ago");
        }
        else
        {
            strTmp = QString("%1 ").arg(mins) + tr("mins ago");
        }

    }
    else if(hours >0 && days <=0)
    {
        if(hours == 1)
        {
            strTmp = QString("%1 ").arg(hours) + tr("hour ago");
        }
        else
        {
            strTmp = QString("%1 ").arg(hours) + tr("hours ago");
        }
    }
    else if(days > 0)
    {
        //大于一天 当前日期-天数 为间隔时间
        QDate date = QDate::currentDate().addDays(-days);
        if(sysControl->get_value(LANGUAGE) == "US.qm")
        {
            strTmp = get_englishDate(date);
        }
        else
        {
            strTmp = date.toString("M月d日");
        }
    }

    sElapseTime = strTmp;

    //如果描述不为空 则发送信号
    if(!strTmp.isEmpty())
    {
        emit signal_updatePmElapsed();
    }

}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_getPmLastUpdateTime
* Description   :   获取pm2.5更新时间 描述字符串
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
QString MainViewQML::slot_getpmLastUpdateTime()
{
    return sElapseTime;
}

/*******************************************************************************
* Author        :   虎正玺@2016-11-11
* Function Name :   slot_getLocaltion
* Description   :   获取定位信息
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
QString MainViewQML::slot_getLocaltion()
{
    return  sLocation;
}


/*******************************************************************************
* Author        :   虎正玺@2016-11-11
* Function Name :   slot_getOutDoorTemp
* Description   :   获取室外温度
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
QString MainViewQML::slot_getOutDoorTemp()
{
    return QString::number(stWeather.temp);
}

QString MainViewQML::slot_getOutDoorPm25()
{
    return QString::number(stWeather.pm25);
}

QString MainViewQML::slot_getOutDoorWeatherIcon()
{
    QString skycon = stWeather.skycon;

    //夜间模式
    if(isNight)
    {
        skycon =  skycon.replace("DAY","NIGHT");
    }
    else
    {
        skycon =  skycon.replace("NIGHT","DAY");
    }
    qDebug() << skycon;
    return skycon;
}

/*******************************************************************************
* Author        :   虎正玺@2016-12-05
* Function Name :   slot_showRefresh
* Description   :   显示主界面pm2.5刷新按钮
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_finishPmRefresh()
{
    emit  signal_pmDetected();
    pm25IsOn = false;
    if(tap_index != -1)
    {
        QTimer::singleShot(TAP_INTERVAL, this, SLOT(slot_recoverPageIndex()));
    }
}

/*******************************************************************************
* Author        :   虎正玺@2016-12-15
* Function Name :   slot_isUsLang
* Description   :   当前语言是否为英文
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
bool MainViewQML::slot_isUsLang()
{
    return sysControl->get_value(LANGUAGE).contains("US");
}

/*******************************************************************************
* Author        :   虎正玺@2016-12-19
* Function Name :   slot_setCurrentIndex
* Description   :   设置当前页面
* Input         :   index:当前页小标
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::slot_setCurrentIndex(int index)
{
    int iTmp = mainPageIndex;
    mainPageIndex = index;

    if(iTmp != index)
        emit signal_changeCurrentIndex();
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   hide_historyPage
* Description   :   隐藏历史界面
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::hide_historyPage()
{
    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    view->show();
    historyPage->hide();
    gCurrent_pageIndex = PAGE_MAIN;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   show_optionPage
* Description   :   显示设置
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::show_optionPage()
{
    iCurrentPage = OPTION_PAGE;
    emit signal_jumpOptionPage();
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   refresh_pmData
* Description   :   手动刷点pm2.5
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void MainViewQML::refresh_pmData()
{
    qDebug() << "refresh_pmData---------------------------";
    emit signal_getPmData();
}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   get_currentIndex
* Description   :   获取当前主页index
* Input         :   None
* Output        :   None
* Return        :   当前主页index
*******************************************************************************/
int MainViewQML::slot_getCurrentIndex()
{
    return  mainPageIndex;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   slot_getRomationAngle
* Description   :   获取横竖屏旋转角度
* Input         :   None
* Output        :   None
* Return        :   获取横竖屏旋转角度
*******************************************************************************/
int MainViewQML::slot_getRomationAngle()
{
    return romationAngle;
}



/*******************************************************************************
* Author        :   虎正玺@2016-12-15
* Function Name :   get_englishDate
* Description   :   自动修改夜间模式
* Input         :   QDate date：日期
* Output        :   None
* Return        :   None
*******************************************************************************/
QString MainViewQML::get_englishDate(QDate date)
{
    QString strDay = "";
    QString strDate = "";
    //    switch (date.day()) {
    //    case 1:
    //        strDay = QString("%1st").arg(date.day());
    //        break;
    //    case 2:
    //        strDay = QString("%1nd").arg(date.day());
    //        break;
    //    case 3:
    //        strDay = QString("%1rd").arg(date.day());
    //        break;
    //    case 13:
    //        strDay = QString("%1rd").arg(date.day());
    //        break;
    //    case 21:
    //        strDay = QString("%1st").arg(date.day());
    //        break;
    //    case 22:
    //        strDay = QString("%1nd").arg(date.day());
    //        break;
    //    case 23:
    //        strDay = QString("%1rd").arg(date.day());
    //        break;
    //    case 31:
    //        strDay = QString("%1st").arg(date.day());
    //        break;
    //    default:
    //        strDay = QString("%1th").arg(date.day());
    //        break;
    //    }
    strDate = QString("%1,%2").arg(date.day()).arg(date.toString("MMM"));
    return strDate;
}

QString MainViewQML::get_wifiStatus()
{
    return wifiStatus == "Disconnect" ? wifiStatus : "Connect";
}

void MainViewQML::read_baseline()
{
    QFile file("/sys/devices/platform/sun5i-i2c.2/i2c-2/2-0058/iaq_baseline");
    QString strFile;

    // 以只读方式打开文件成功 或者 文件有内容
    if (file.open(QFile::ReadOnly) && (file.size() != 0))
    {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
        QTextStream stream(&file);

        // 取得 温度值
        strFile     = stream.readAll();

        file.close();
    }
    baseline = "0x" + strFile.trimmed();
}


/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   get_pmValue
* Description   :   获取pm数值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
float MainViewQML::get_pmValue()
{
    return fPmValue;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   get_tempValue
* Description   :   获取温度数值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
float MainViewQML::get_tempValue()
{
    return fTempValue;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   get_humValue
* Description   :   获取湿度数值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
float MainViewQML::get_humValue()
{
    return fHumValue;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   pmValue
* Description   :   获取pm数值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
float MainViewQML::pmValue()
{
    return fPmValue;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   tempValue
* Description   :   获取温度数值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
float MainViewQML::tempValue()
{
    return fTempValue;
}

/*******************************************************************************
* Author        :   虎正玺@2016-10-28
* Function Name :   humValue
* Description   :   获取湿度值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
float MainViewQML::humValue()
{
    return fHumValue;
}

/*******************************************************************************
* Author        :   虎正玺@2017-02-
* Function Name :   slot_getPm5Value
* Description   :   获取pm15值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
float MainViewQML::slot_getPm5Value()
{
    return fPm5Value;
}


/*******************************************************************************
* Author        :   虎正玺@2017-02-
* Function Name :   slot_getPm10Value
* Description   :   获取pm10值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
float MainViewQML::slot_getPm10Value()
{
    return fPm10Value;
}

/*******************************************************************************
* Author        :   虎正玺@2017-03
* Function Name :   slot_getTvocValue
* Description   :   获取tVOC值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
float MainViewQML::slot_getTvocValue()
{
    return ftVOCValue;
}

/*******************************************************************************
* Author        :   虎正玺@2017-03
* Function Name :   slot_getCO2eValue
* Description   :   获取CO2e值
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
float MainViewQML::slot_getCO2eValue()
{
    return fCO2eValue;
}

void MainViewQML::slot_setLightValue(int value)
{

    // 设置滑块滑动的最小值
    if (value <= 0)
    {
        value = 0;
    }
    else if (value >= 100)
    {
        value = 100;
    }
    lightValue = value;

    sysControl->set_value(LIGHT_VALUE,QString::number(lightValue));

    emit signal_set_lightValue(lightValue);

}

int MainViewQML::slot_getCurLightValue()
{
    return lightValue;
}

QString MainViewQML::slot_getWifiStatus()
{
    return wifiStatus;
}


QString MainViewQML::slot_getBatteryCurrentelectricity()
{
    ChargingType way = battery.way;
    QString chargingType = "";
    if(way == AC )
    {
        chargingType = "Dock";
    }
    else if(way == USB)
    {
        chargingType = "USB";
    }
    else
    {
        chargingType = "NONE";
    }
    return ischarging ? QString("%1@%2mA").arg(chargingType).arg(battery.current) : QString("%1mA").arg(battery.current);
}


QString MainViewQML::slot_getBatteryTime()
{
    //小时
    int hours=timeclock / 3600;

    //分钟
    int mins=(timeclock - hours * 3600) / 60;

    //秒
    int secs=timeclock - hours * 3600 - mins * 60;

    return QString("%1:%2:%3").arg(hours,2,10,QLatin1Char('0')).arg(mins,2,10,QLatin1Char('0')).arg(secs,2,10,QLatin1Char('0'));
}


QString MainViewQML::slot_getScreenTime()
{
    //小时
    int hours=screenOnTime / 3600;

    //分钟
    int mins=(screenOnTime - hours * 3600) / 60;

    //秒
    int secs=screenOnTime - hours * 3600 - mins * 60;

    return QString("%1:%2:%3").arg(hours,2,10,QLatin1Char('0')).arg(mins,2,10,QLatin1Char('0')).arg(secs,2,10,QLatin1Char('0'));
}

void MainViewQML::slot_setCpuUseage100()
{
    sysOCC->set_start();
    sysocc1->set_start();
    stsocc2->set_start();
    emit signal_cpuLoad();
}

void MainViewQML::slot_setCpuUsage0()
{
    sysOCC->stop();
    sysocc1->stop();
    stsocc2->stop();
    emit signal_cpuNoLoad();
}

void MainViewQML::slot_set120M()
{
    qDebug() << "slot_set120M,,,,,";
    emit signal_change_fre(false);

}

void MainViewQML::slot_set1G()
{
    qDebug() << "slot_set1G,,,";

    emit signal_change_fre(true);

}

void MainViewQML::slot_update_usage(QString u)
{

    usage = u;
    emit signal_updateUsage();
}

void MainViewQML::slot_update_fre(float f)
{
    qDebug() << "slot_update_fre  f = " << f;
    fre = f;
    emit signal_updateFre();
}

QString MainViewQML::slot_getUsage()
{
    return usage;
}

QString MainViewQML::slot_getFre()
{
    return QString("%1").arg(fre);
}

void MainViewQML::slot_pmOn()
{
    pm25IsOn = true;
    emit signal_pmOn();
}

void MainViewQML::slot_pmOff()
{
    pm25IsOn = false;
    emit signal_pmOff();
}

QString MainViewQML::slot_getBaseLine()
{
    return baseline;
}

void MainViewQML::slot_modeSwitch()
{
    modeIndex++;
    if(modeIndex > 6)
    {

        emit signal_testFinished();
        slot_setCpuUsage0();
        slot_set120M();
        slot_setLightValue(0);
        //        if(pm25IsOn)
        //            slot_pmOff();
        modeSwitchTimer->stop();
        saveDataTimer->stop();

    }
    switch (modeIndex) {
    //基态
    case 0:
        sysControl->slot_screenOn();
        slot_setWifiOff(false);
        break;
    case 1:
    case 3:
        sysControl->slot_screenOn();
        slot_setWifiOff(true);
        break;
    case 2:
        sysControl->slot_screenOff();
        slot_setWifiOff(true);
        break;
    case 4://CPU高频
        slot_setLightValue(0);
        slot_set1G();
        slot_setCpuUsage0();
        sysControl->slot_screenOn();
        slot_setWifiOff(true);
        break;
    case 5://亮度最高
        slot_setLightValue(100);
        slot_setCpuUsage0();
        slot_set1G();
        sysControl->slot_screenOn();
        slot_setWifiOff(true);
        break;
    case 6://CPU满载
        sysControl->slot_screenOn();
        slot_setLightValue(100);
        slot_set1G();
        slot_setCpuUseage100();
        slot_setWifiOff(true);
        break;
    default:
        break;
    }
    emit signal_autoLightChanged();
}

void MainViewQML::slot_switchFre()
{
    qDebug() << "slot_switchFre-----------------------------fre =" << fre;
    if(fre == 1.008f)
    {
        slot_set120M();
    }
    else
    {
        slot_set1G();
    }
}

void MainViewQML::slot_syncRTC()
{
    emit signal_set_timeAuto(true);
}

void MainViewQML::slot_save_data()
{
    if(num< 100)
        num++;

    if(num == 2)
    {
        slot_setWifiOff(true);
    }
//    if(modeIndex == 0 && num == 5)
//    {
        //        pm_off_timer->start();
//        emit signal_change_fre(false);
//    }
    if(modeIndex == 0 && num < 5)
    {
        return ;
    }
    if(modeIndex > 6)
    {
        modeIndex  = 6 ;
        //        return ;
    }
    QDir *debug = new QDir;
    bool exist = debug->exists("./debugFile");
    if(!exist)
    {
        debug->mkdir("./debugFile");
    }

    //    QString path = QString("./debugFile/tempHum-test-%1.csv").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"));
    QString path = QString("./debugFile/wifiAndScreen-test-%1.csv").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"));

    QFile file(filePath);
    if(!file.exists())
    {
        filePath = path;
    }
    QFile file1(filePath);
    g_loginfo.currentMA = battery.current;
    g_loginfo.voltage = battery.voltage;
    g_loginfo.screenOn = sysControl->is_ScreenOn();
    g_loginfo.wifi_status = wifi.status;
    g_loginfo.light = lightValue;
    g_loginfo.charging = slot_getBatteryStatusIsCharging();
    g_loginfo.cpu_fre = qRound(fre);
    g_loginfo.status_chargingOn = status_charging_on;
    g_loginfo.status_cpuLoad = status_CPU_load;
    g_loginfo.status_cpuLoadF = status_CPU_load_CPU_f;
    g_loginfo.status_LCDBrightness = status_LCD_bri;
    g_loginfo.status_cpuF = status_CPU_f;

    g_loginfo.sensirion_hum = get_humValue();
    g_loginfo.sensirion_temp = slot_getTempValue().toFloat();
    g_loginfo.temp_raw = slot_getRawTempValue().toFloat();
    g_loginfo.hum_raw = slot_getRawHumValue().toFloat();
    g_loginfo.second_temp_raw =slot_getSecondTempValue().toFloat();
    QString str = QString("humraw = %1,temp_raw = %2").arg(g_loginfo.hum_raw ).arg(g_loginfo.temp_raw);
    system(qPrintable(QString("echo %1 >> /usr/bin/qtapp/test.log").arg(str)));

    if(file1.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        QTextStream in(&file1);
        if(file1.size() == 0)
        {
            in << QString("Current Version:%1\n").arg(slot_get_version_system());
            in<<QString("%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, %16, %17, %18, %19, %20, %21, %22, %23, %24, %25, %26, %27\n").arg("Flag")
                .arg("status_charging_on").arg("status_CPU_load").arg("status_CPU_f").arg("status_CPU_load_CPU_f").arg("status_LCD_bri")
                .arg("Date Time").arg("pm2.5 sensor is On").arg("Pm2.5")
                .arg("Temperature").arg("Raw Temperature").arg("Sencond Sensor Temp").arg("Humidity (%)").arg("Raw Humidity (%)")
                .arg("Baseline"). arg("tVOC").arg("CO2e").arg("CPU Frequency").arg("CPU Usage (%)").arg("Light")
                .arg("Charging").arg("Voltage (uV)").arg("Current (mA)").arg("Capacity (%)").arg("Wi-Fi Status").arg("Screen On").arg("CG_temp");
        }
        QString line = QString("%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, %16, %17, %18, %19, %20, %21, %22, %23, %24, %25, %26, %27\n").arg(falgModelist[modeIndex])
                .arg(status_charging_on).arg(status_CPU_load).arg(status_CPU_f).arg(status_CPU_load_CPU_f).arg(status_LCD_bri)
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss")).arg(pm25IsOn ? "On" : "Off").arg(fPmValue)
                .arg(slot_getTempValue()).arg(slot_getRawTempValue()).arg(slot_getSecondTempValue()).arg(get_humValue()).arg(slot_getRawHumValue())
                .arg(slot_getBaseLine()).arg(ftVOCValue).arg(fCO2eValue).arg(QString("%1G").arg(fre)).arg(usage).arg(lightValue)
                .arg(slot_getBatteryStatusIsCharging() ? "Charging" : "Discharging").arg(battery.voltage).arg(battery.current).arg(slot_getBatteryCapacity())
                .arg(get_wifiStatus()).arg(sysControl->is_ScreenOn()).arg(fcg_temp);
        in<<line;
        file1.close();
    }
    else
    {
        qDebug()<<"写入文件失败";
    }

}

void MainViewQML::slot_updataRawData(float temp, float hum, float secondTemp, float cg_temp)
{
    fcg_temp = cg_temp;
    fRawTempValue = temp;
    fRawHumValue = hum;
    fRawSecondValue = secondTemp;
}

void MainViewQML::slot_updateTempFlag(int charging_on, int cpu_load,int cpu_f,int cpu_load_CPU_f, int LCD_bri)
{
    status_charging_on = charging_on;
    status_CPU_load = cpu_load;
    status_CPU_f = cpu_f;
    status_CPU_load_CPU_f = cpu_load_CPU_f;
    status_LCD_bri = LCD_bri;
}

/*******************************************************************************
* Function Name  :  slot_get_version_system
* Description    :  获取系统版本
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString MainViewQML::slot_get_version_system()
{
    QFile file("/usr/bin/qtapp/IMG_Version");
    QStringList listStr;
    QString strRead;
    QString version = "999.0";
    if (file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);

        //读取文件内容
        strRead = stream.readAll();
        listStr = strRead.split(" ");
        version = listStr.at(0).trimmed();

        //关闭文件
        file.close();
    }

    version = QString("%1/%2/%3").arg(VERSION_NO).arg(version).arg(get_os_version());
    return version;
}
/*******************************************************************************
* Function Name  :  get_os_version
* Description    :  获取此时的系统版本
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
QString MainViewQML::get_os_version()
{
    QString field = "CLEARGRASS_VERSION";
    QString filePath = "/usr/bin/qtapp/os-release";
    FILE  *stream;
    char  buf[4096];
    QString strCmd = QString("grep \"%1\" %2 | cut -f 2 -d '='").arg(field).arg(filePath);
    QString result;
    QByteArray ba = strCmd.toLatin1();
    char *cmd = ba.data();

    // 获取此时的版本号
    stream = popen(cmd, "r");
    fread(buf, sizeof(char), sizeof(buf),  stream);
    result = buf;
    pclose(stream);

    if(result.contains("\n"))
    {
        result = result.split("\n").at(0);
    }

    if(!result.contains("_")||!result.contains("."))
    {
        result = "unknown";
    }
    return result;
}


void MainViewQML::slot_setInterval(int interval)
{
    modeSwitchTimer->setInterval(interval *1000 *60);
}

void MainViewQML::slot_pm_off_timeout()
{
    pm_off_timer->stop();
    pm_on_timer->start();
    slot_pmOn();
}

void MainViewQML::slot_pm_on_timeout()
{
    pm_off_timer->start();
    pm_on_timer->stop();
    slot_pmOff();
    fPmValue = -1;
}

void MainViewQML::slot_setWifiOff(bool on)
{
    if(on && get_wifiStatus() != "Connect")
    {
        emit signal_wificonnect();
        return ;
    }

    if(on == false)
    {
        emit signal_wifiDisconnect();
        return ;
    }

}
