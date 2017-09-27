#include <QScrollBar>
#include "pageSetting.h"

#include "sysControl.h"
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QtDeclarative>
PageSetting::PageSetting(QWidget *parent) :
    QWidget(parent)
{
    control_init();
    //    page_init();
    connect_init();
    page_init();
}

PageSetting::~PageSetting()
{
}

void PageSetting::set_wifiEntery(int entry)
{
    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@$$$$$$$$$$$";
    wifiEntry = entry;
    emit signal_changeEntry();
}

void PageSetting::set_wifiWizardLanguage(QString lang)
{
    wifiPageWizardLanguage = lang;
}

int PageSetting::slot_getCurPageIndex()
{
    return iCurPage;
}

/*******************************************************************************
* Author        :   虎正玺@2017-01-18
* Function Name :   slot_getAutoTimezone
* Description   :   获取自动定位时区
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
QString PageSetting::slot_getAutoTimezone()
{
    QString netTimeZone = sysTime->get_timeZoneAuto();
    if(netTimeZone.isEmpty())
    {
        return netTimeZone;
    }

    QList<TimeZone> *list = sysTime->get_listTimeZone();

    for(int i  = 0; i < list->size(); i++)
    {
        if(list->at(i).name == netTimeZone)
        {
            netTimeZone = list->at(i).description +" " + list->at(i).gmt;
            emit signal_set_timeZone(list->at(i).gmt);
            sysControl->set_value(TIME_ZONE,list->at(i).name);

            break;
        }
    }
    return netTimeZone;

}

/*******************************************************************************
* Author        :   虎正玺@2017-01-19
* Function Name :   slot_setMainWifiPageIsHide
* Description   :   修改标志位，主页面连接云端页面不被显示
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PageSetting::slot_setMainWifiPageIsHide()
{
    sysControl->set_value(WIFI_MAIN_PAGE, "false");
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_getAppQrcode
* Description   :   获取米家App二维码
* Input         :   None
* Output        :   None
* Return        :   成功时返回二维码相对地址，失败时返回空字符串
*******************************************************************************/
QString PageSetting::slot_getAppQrcode()
{
    qrCode = "";
    QString tmpStr = "";
    if(funcApp->get_bindkey_qrcode(tmpStr))
    {
        qDebug() << tmpStr;
        return tmpStr;
    }
    else
    {
        return "";
    }
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_refresh_appQrcode
* Description   :   重新获取二维码
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PageSetting::slot_refresh_appQrcode()
{
    emit signal_refreshAppQrcode();
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_removeQrcode
* Description   :   删除二维码
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PageSetting::slot_removeQrcode()
{
    emit signal_removeQrcode();
}

void PageSetting::slot_setQrcode(QString qrcode)
{
    qrCode = qrcode;
    emit signal_setAppQrcode();
}

QString PageSetting::slot_getQrcode()
{
    return qrCode;
}

void PageSetting::slot_qrcodeTimeout()
{
    emit signal_setQrcodeTimeout();
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_getMiAppBindStatus
* Description   :   获取米加app 绑定状态
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/

bool PageSetting::slot_getAppBindStat()
{
    return appBindStatus;
}

void PageSetting::slot_updatebindstat(int stat)
{
    appBindStatus = stat;
    emit signal_updateAppBindStatus(stat);
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_getDoubleTapChecking
* Description   :   双击设备顶部监测 开关状态获取
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
bool PageSetting::slot_getDoubleTapChecking()
{
    if(sysControl->get_value(DOUBLE_CLICK_CHECKING) == "true")
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_setDoubleTapChecking
* Description   :   双击设备顶部监测 开关状态设置
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PageSetting::slot_setDoubleTapChecking(bool checked)
{
    if(checked)
    {
        sysControl->set_value(DOUBLE_CLICK_CHECKING, "true");
    }
    else
    {
        sysControl->set_value(DOUBLE_CLICK_CHECKING, "false");
    }
}

void PageSetting::slot_net_abnormal()
{
    emit  signal_net_abnormal();
}

void PageSetting::slot_net_disconnect()
{
    emit signal_net_disconnect();
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_timeInit_success
* Description   :   时间初始化成功
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PageSetting::slot_timeInit_success()
{
    emit signal_timeInit_success();

}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   slot_timeIsInit
* Description   :   时间是否被初始化
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
bool PageSetting::slot_timeIsInit()
{
    return sysControl->get_value(TIME_INIT) == "true" ? true : false;
}

void PageSetting::slot_wificonnect()
{
    timerCounter = 0;
    slot_wifiConnect("Cleargrass_SZ", "cleargrass2015");
    wifiTimer->start();
}

void PageSetting::slot_wifiDisconnect()
{
    timerCounter = 0;
    emit signal_remove_allCookie();
    wifiDisconnectTimer->start();
}

/*******************************************************************************
* Author        :   虎正玺@2017-04
* Function Name :   set_otaError
* Description   :
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PageSetting::set_otaError(int errorCode)
{
    otaErrorCode = errorCode;
    emit  signal_errorInfo();
}


/*******************************************************************************
* Author        :   虎正玺@2017-01-20
* Function Name :   show_scrollBarAniation
* Description   :   显示滚动条动画
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PageSetting::show_scrollBarAniation()
{
    emit signal_showScrollBarAnimatio();
}

void PageSetting::page_init()
{

    newVersion = "";
    newVersionDes= "";
    wifiListAllowRefresh  = true;
    wifiConnStat = WIFI_DISCONNECT;
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    iScrollValue =0;
    curTimeZoneIndex = 0;
    appBindStatus = funcApp->get_bindState();
    //默认从设置界面进入
    wifiEntry = 2;
    iCurPage = 0;
    isNight = sysControl->get_night();

    //屏幕自动熄灭频率 设置页面默认显示子页面index
    initScreenIndex = 0;
    pmDetectingFrequencySettingType = 0;
    qmlRegisterType<PageSetting>("Setting", 1, 0, "Setting");

    view  = new QDeclarativeView(this);
    view->engine()->rootContext()->setContextProperty("SettingProperty", this);
    view->setSource(QUrl("qrc:/resource/qml/settingQml/Setting.qml"));
}

void PageSetting::connect_init()
{
    connect(sysWiFi, SIGNAL(signal_update_wifi(WiFi)), this, SLOT(slot_wifi_status(WiFi)));
    connect(sysWiFi, SIGNAL(signal_update_listWiFi(QList<WiFi>*)), this, SLOT(refresh_page(QList<WiFi>*)));
    connect(sysWiFi, SIGNAL(signal_connect_result(int)), this, SLOT(slot_connect_result(int)));

    connect(this, SIGNAL(signal_refresh_listWiFi()), sysWiFi, SLOT(slot_refresh_listWiFi()));
    connect(this, SIGNAL(signal_connect_withKey(QString,QString)), sysWiFi, SLOT(slot_connect_withKey(QString,QString)));
    connect(this, SIGNAL(signal_connect_withNetID(QString)), sysWiFi, SLOT(slot_connect_withNetID(QString)));
    connect(this, SIGNAL(signal_connectPublicWifi(QString)), sysWiFi, SLOT(slot_connect_publicWiFi(QString)));
    connect(this, SIGNAL(signal_remove_cookie(QString)), sysWiFi, SLOT(slot_remove_cookie(QString)));
    connect(this, SIGNAL(signal_remove_allCookie()), sysWiFi, SLOT(slot_remove_allCookies()));
    connect(this, SIGNAL(signal_set_timeZone(QString)), sysTime, SLOT(slot_set_timeZone(QString)));
    connect(this, SIGNAL(signal_set_timeAuto(bool)), sysTime, SLOT(slot_set_timeAuto(bool)));
    connect(this, SIGNAL(signal_set_dateTime(QString)), sysTime, SLOT(slot_set_time(QString)));
    connect(this, SIGNAL(signal_set_night(bool)), sysTime, SLOT(slot_set_night(bool)));
    connect(this, SIGNAL(signal_set_nightAuto(bool)), sysTime, SLOT(slot_set_nightAuto(bool)));
    connect(this, SIGNAL(signal_set_nightMode(QString)), sysTime, SLOT(slot_set_nightMode(QString)));
    connect(this, SIGNAL(signal_set_nightPeriod(QString,bool)), sysTime, SLOT(slot_set_nightPeriod(QString,bool)));
    connect(sysTime, SIGNAL(signal_timeInit_success()), this, SLOT(slot_timeInit_success()));

    connect(this, SIGNAL(signal_set_cityAuto()), funcCity, SLOT(slot_set_cityAuto()));
    connect(this, SIGNAL(signal_set_cityManual(QString)), funcCity, SLOT(slot_set_cityManual(QString)));
    connect(this, SIGNAL(signal_search_city(QString)), funcCity, SLOT(slot_search_city(QString)));

    connect(sysControl, SIGNAL(signal_in_night(bool)), this, SLOT(slot_in_night(bool)));
    connect(sysControl, SIGNAL(signal_light_value(int)),this,SLOT(auto_changeValue(int)));
    connect(sysControl, SIGNAL(signal_outLight_value(int)),this,SLOT(slot_outLight_value(int)));
    connect(sysControl, SIGNAL(signal_inLight_value(int)),this,SLOT(slot_intLight_value(int)));
    connect(sysControl, SIGNAL(signal_net_ip(QString)),this,SLOT(slot_netIp(QString)));
    connect(this, SIGNAL(signal_set_lightMode(bool)), sysControl, SLOT(slot_set_lightMode(bool)));
    connect(this, SIGNAL(signal_set_lightValue(int)), sysControl, SLOT(slot_set_lightValue(int)));
    connect(this, SIGNAL(signal_offInterval_charging(IntervalType)), sysControl, SLOT(slot_offInterval_charging(IntervalType)));
    connect(this, SIGNAL(signal_offInterval_discharging(IntervalType)), sysControl, SLOT(slot_offInterval_discharging(IntervalType)));


    connect(funcFirmware, SIGNAL(signal_download_success()), this, SLOT(slot_update_download_success()));
    connect(funcFirmware, SIGNAL(signal_checkFirmware_success(QString,QString)), this, SLOT(slot_update_firmware(QString,QString)));
    connect(funcFirmware, SIGNAL(signal_checkFirmware_success()), this, SLOT(slot_update_firmware()));
    connect(funcFirmware, SIGNAL(signal_download_process(qint64,qint64)), this, SLOT(slot_updateProgress(qint64,qint64)));
    connect(funcFirmware, SIGNAL(signal_net_abnormal()), this, SLOT(slot_ota_net_abnormal()));
    connect(funcFirmware, SIGNAL(signal_net_disconnect()), this, SLOT(slot_ota_net_disconnect()));
    connect(this, SIGNAL(signal_start_download()), funcFirmware, SLOT(slot_download_manual()));
    connect(this, SIGNAL(signal_check_update()), funcFirmware, SLOT(slot_checkRelease_manual()));
    connect(this, SIGNAL(signal_check_updateDebug()), funcFirmware, SLOT(slot_checkDebug_manual()));

    connect(funcCity, SIGNAL(signal_searchCity_success(QList<InfoCity>*)), this, SLOT(slot_searchRecCityList(QList<InfoCity>*)));
    connect(funcCity, SIGNAL(signal_net_abnormal()), this, SLOT(slot_local_net_abnormal()));
    connect(funcCity, SIGNAL(signal_net_disconnect()), this, SLOT(slot_local_net_disconnect()));
    connect(funcCity, SIGNAL(signal_locate_success(QString)), this, SLOT(slot_locate_success(QString)));
    connect(funcCity, SIGNAL(signal_locate_failed()), this, SLOT(slot_locate_failed()));

    connect(this, SIGNAL(signal_change_unitTEMP(UnitType)), sysData, SLOT(slot_change_unitTEMP(UnitType)));
    connect(this, SIGNAL(signal_change_unitTVOC(UnitType)), sysData, SLOT(slot_change_unitTVOC(UnitType)));
    connect(this, SIGNAL(signal_intervalPM25_charging(IntervalType)), sysData, SLOT(slot_intervalSampling_charging(IntervalType)));
    connect(this, SIGNAL(signal_intervalPM25_discharging(IntervalType)), sysData, SLOT(slot_intervalSampling_discharging(IntervalType)));

    connect(this, SIGNAL(signal_change_language(LanguageType)), sysLanguage, SLOT(slot_change_language(LanguageType)));
    connect(this, SIGNAL(signal_refreshAppQrcode()), funcApp, SLOT(slot_refresh_qrcode()));
    connect(this, SIGNAL(signal_removeQrcode()), funcApp, SLOT(slot_remove_qrcode()));
    connect(funcApp, SIGNAL(signal_update_qrcode(QString)), this, SLOT(slot_setQrcode(QString)));
    connect(funcApp, SIGNAL(signal_update_bindStatus(int)), this, SLOT(slot_updatebindstat(int)));
    connect(funcApp, SIGNAL(signal_qrcode_timeout()), this, SLOT(slot_qrcodeTimeout()));

    connect(funcApp, SIGNAL(signal_net_abnormal()), this, SLOT(slot_net_abnormal()));
    connect(funcApp, SIGNAL(signal_net_disconnect()), this, SLOT(slot_net_disconnect()));
    connect(wifiTimer, SIGNAL(timeout()), this, SLOT(slot_reconnectWifi()));
    connect(wifiDisconnectTimer, SIGNAL(timeout()), this, SLOT(slot_disconnectWifi()));


}

void PageSetting::control_init()
{
    funcFirmware = FuncFirmware::getInstance();
    funcCity    = FuncCity::getInstance();
    sysControl = SysControl::getInstance();
    sysWiFi    = SysWiFi::getInstance();
    sysTime    = SysTime::getInstance();
    sysData    = SysData::getInstance();
    sysLanguage = SysLanguage::getInstance();
    funcApp    = FuncApp::getInstance();

    wifiTimer = new QTimer(this);
    wifiTimer->setInterval(60 *1000);

    wifiDisconnectTimer = new QTimer(this);
    wifiDisconnectTimer->setInterval(60 *1000);
}

/*******************************************************************************
* Author        :   虎正玺@2016-11-04
* Function Name :   slot_setNightMode
* Description   :   设置夜间模式
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PageSetting::slot_setNightMode(int mode)
{
    if(mode == 1)
    {
        emit signal_set_night(true);
    }
    else
    {
        emit signal_set_night(false);
    }
    //自动返回
    on_page_defalut_back();
}

/*******************************************************************************
* Author        :   虎正玺@2016-11-04
* Function Name :   slot_setNightMode
* Description   :   设置夜间模式
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PageSetting::slot_timeModeChange()
{
    emit signal_timeModeChange();
}

/*******************************************************************************
* Author        :   虎正玺@2016-11-04
* Function Name :   slot_setScrollValue
* Description   :   设置滚动条位置
* Input         :   value：滚动条位置
* Output        :   None
* Return        :   None
*******************************************************************************/
void PageSetting::slot_setScrollValue(int value)
{
    iScrollValue = value;
}

int PageSetting::set_getScrollValue()
{
    return iScrollValue;
}


/*******************************************************************************
* Function Name :   slot_setTempUnit
* Description   :   设置温度单位
* Input         :   falg == true 为华氏度  false 为摄氏度
* Output        :   对事件进行反应
* Return        :   None
*******************************************************************************/
void PageSetting::slot_setTempUnit(bool flag)
{
    if(!flag)
    {
        qDebug("tempture is ℃...");
        emit signal_change_unitTEMP(CEL);
    }
    //开关关闭
    else
    {
        // 此处缺少将温度单位设置成 华摄度的函数
        qDebug("tempture is ℉...");
        emit signal_change_unitTEMP(KFC);
    }

}

/*******************************************************************************
* Function Name :   slot_curTempUnitIsFahrenheit
* Description   :   判断当前温度单位是否为华氏度
* Input         :   None
* Output        :   None
* Return        :   单位为华氏度时返回true；单位为摄氏度时返回false
*******************************************************************************/
bool PageSetting::slot_curTempUnitIsFahrenheit()
{
    qDebug() << sysData->get_unitTEMP();
    return (sysData->get_unitTEMP() == KFC) ? true : false;
}

void PageSetting::auto_changeValue(int value)
{
    curLightValue = value;
    emit signal_autoChangeValue();
}

void PageSetting::slot_setAutoChangeLight(bool checked)
{

    //    sysLight->set_autoAdjust(checked);
    emit signal_set_lightMode(checked);

    if(checked)
    {
        sysControl->set_value(LIGHT_AUTO,"true");
    }
    else
    {
        sysControl->set_value(LIGHT_AUTO,"false");
    }
}

void PageSetting::slot_setLightValue(int value)
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
    sysControl->set_value(LIGHT_VALUE,QString::number(value));

    emit signal_set_lightValue(value);

}

int PageSetting::slot_getLightValue()
{
    return curLightValue;
}

void PageSetting::slot_setWifiStatus(int index, int status)
{
    if(wifiDataList.size() > index)
    {
        for(int i = 0; i < wifiDataList.size(); i++)
        {
            WifiDataObject * dataTmp =  (WifiDataObject*)wifiDataList[i];

            //全部断开
            dataTmp->set_status(1);
            //           dataTmp->deleteLater();
        }
        WifiDataObject * data = (WifiDataObject*)wifiDataList[index];
        data->set_status(status);
        //        data->deleteLater();
    }
}

/*******************************************************************************
* Author        :   虎正玺@2017-03
* Function Name :   slot_getSerialNumber
* Description   :   获取设备序列号
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
QString PageSetting::slot_getSerialNumber()
{
    return  sysControl->get_value(DEVICE_SN);
}

/*******************************************************************************
* Author        :   虎正玺@2017-03
* Function Name :   slot_getDeviceMAC
* Description   :   获取设备MAC
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
QString PageSetting::slot_getDeviceMAC()
{
    return  sysControl->get_value(DEVICE_MAC);
}

bool PageSetting::slot_isAuto()
{
    if(sysControl->get_value(LIGHT_AUTO) == "true")
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*******************************************************************************
* Function Name  :  slot_in_night
* Description    :  夜间模式被改变
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void PageSetting::slot_in_night(bool isNight)
{
    this->isNight = isNight;
}

int PageSetting::slot_getNightMode()
{
    if(isNight)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

QString PageSetting::slot_getAotoNight()
{
    QString nightAuto =  sysControl->get_value(NIGHT_AUTO);

    return  nightAuto;

}

QString PageSetting::slot_getSunRise()
{
    QString nightMode =  sysControl->get_value(NIGHT_MODE);
    if(nightMode == "mode_sun"  && slot_getSunRiseInit() == "false")
    {
        nightMode = "mode_manual";
    }
    return nightMode;

}

QString PageSetting::slot_getSunRiseInit()
{
    return sysControl->get_value(SUN_INIT);

}

void PageSetting::slot_setLanguage(int index)
{
    if(slot_getLanguage() == index)
        return ;
    switch (index)
    {
    case 0:
        emit signal_change_language(LANG_CN);
        break;
    case 1:
        emit signal_change_language(LANG_TW);
        break;
    case 2:
        emit signal_change_language(LANG_EN);
        break;
    default:
        break;
    }
}

int PageSetting::slot_getLanguage()
{
    QString lang =sysControl->get_value(LANGUAGE);
    int index;
    if("ZH_SC.qm" == lang)
    {
        index = 0;
    }
    else if("ZH_TC.qm" == lang)
    {
        index = 1;
    }
    else
    {
        index = 2;
    }
    return index;
}

int PageSetting::slot_getTvocUnit()
{
    UnitType lang = sysData->get_unitTVOC();
    int index;
    if(MG_M3 == lang)
    {
        index = 0;
    }
    else if(PPM == lang)
    {
        index = 1;
    }
    else
    {
        index = 2;
    }
    return index;
}

QString PageSetting::slot_getTvocUnitName()
{
    return sysControl->get_value(TVOC_UNIT);
}

void PageSetting::slot_setTvocUnit(int index)
{
    switch (index) {
    case 0:
        emit signal_change_unitTVOC(MG_M3);
        break;
    case 1:
        emit signal_change_unitTVOC(PPM);
        break;
    case 2:
        emit signal_change_unitTVOC(PPB);
        break;
    default:
        break;
    }
}

void PageSetting::slot_setAotoNight(bool check)
{
    emit signal_set_nightAuto(check);
}

void PageSetting::slot_setSunRise(QString check)
{
    if(check == "true")
    {
        emit signal_set_nightMode("mode_sun");
    }
    else
    {
        emit signal_set_nightMode("mode_manual");
    }
}

QString PageSetting::slot_getNightInterver()
{
    QString timeInterver;
    QString startTime= sysControl->get_value(NIGHT_START_TIME);
    QString endTime  = sysControl->get_value(NIGHT_END_TIME);

    bool isUs = slot_isUsLang();
    if(!slot_get24Hours())
    {
        if(isUs)
        {
            startTime = QDateTime::fromString(startTime, "h:mm").toString("h:mm AP");
            endTime = QDateTime::fromString(endTime, "h:mm").toString("h:mm AP");
        }
        else
        {
            startTime = QDateTime::fromString(startTime, "h:mm").toString("AP h:mm").replace("AM","上午").replace("PM","下午");
            endTime = QDateTime::fromString(endTime, "h:mm").toString("AP h:mm").replace("AM","上午").replace("PM","下午");
        }
    }

    timeInterver = startTime +" - " +endTime;
    return timeInterver;
}

//1 =  开始时间   2= 结束时间
QString PageSetting::slot_getNightInterTime(int flag)
{
    QString strTime = "";
    if(flag == 1)
    {
        strTime = sysControl->get_value(NIGHT_START_TIME);
    }
    else
    {
        strTime = sysControl->get_value(NIGHT_END_TIME);
    }
    if(!slot_get24Hours())
    {
        strTime = QDateTime::fromString(strTime, "h:mm").toString("APh:mm").replace("AM","").replace("PM","");

    }
    return strTime;
}

QString PageSetting::slot_getAmp(int flag)
{
    QString strTime = "";
    if(flag == 1)
    {
        strTime = sysControl->get_value(NIGHT_START_TIME);
    }
    else
    {
        strTime = sysControl->get_value(NIGHT_END_TIME);
    }
    strTime = QDateTime::fromString(strTime, "h:mm").toString("AP");
    return strTime;
}

void PageSetting::slot_setNightIntrTime(QString timeInterver, bool isStartTime)
{
    if(!slot_get24Hours())
    {
        timeInterver = QDateTime::fromString(timeInterver, "h:mm AP").toString("h:mm");
    }

    emit signal_set_nightPeriod(timeInterver,isStartTime);
}

void PageSetting::slot_set24Hours(bool flag)
{
    if(flag)
    {
        sysControl->set_value(TIME_MODE,DEFAULT_TIME_MODE);

    }
    else
    {
        sysControl->set_value(TIME_MODE,"mode_12");

    }
    emit signal_timeModeChange();

}

bool PageSetting::slot_get24Hours()
{
    if(sysControl->get_value(TIME_MODE) == DEFAULT_TIME_MODE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PageSetting::slot_setAutoGetTime(bool flag)
{
    emit signal_set_timeAuto(flag);
}

bool PageSetting::slot_getAutoGetTime()
{
    if("true" == sysControl->get_value(TIME_AUTO))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString PageSetting::slot_getCurDateTime()
{
    QString curTime = "";

    //当前语言是否为英文
    bool isUs = slot_isUsLang();
    if(slot_get24Hours())
    {
        if(isUs)
        {
            curTime =  QDateTime::currentDateTime().toString("dd/MMM/yyyy h:mm");
        }
        else
        {
            curTime =  QDateTime::currentDateTime().toString("yyyy-MM-dd h:mm");
        }
    }
    else
    {
        if(isUs)
        {
            curTime = QDateTime::currentDateTime().toString("dd/MMM/yyyy h:mm AP");
        }
        else
        {
            curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd AP h:mm ");
            curTime.replace("AM","上午").replace("PM","下午");
        }
    }
    return curTime;

}

QString PageSetting::slot_getDatetime(QString strDatetime)
{
    QString datetime = "";

    //当前语言是否为英文
    bool isUs = slot_isUsLang();
    if(slot_get24Hours())
    {
        if(isUs)
        {
            datetime = QDateTime::fromString(strDatetime, "yyyy-MM-dd hh:mm:ss").toString("dd/MMM/yyyy h:mm");
        }
        else
        {
            datetime = QDateTime::fromString(strDatetime, "yyyy-MM-dd hh:mm:ss").toString("yyyy-MM-dd h:mm");
        }
    }
    else
    {
        if(isUs)
        {
            datetime = QDateTime::fromString(strDatetime, "yyyy-MM-dd hh:mm:ss").toString("dd/MMM/yyyy h:mm AP");
        }
        else
        {
            datetime = QDateTime::fromString(strDatetime, "yyyy-MM-dd hh:mm:ss").toString("yyyy-MM-dd AP h:mm ");
            datetime.replace("AM","上午").replace("PM","下午");
        }
    }
    return datetime;
}

void PageSetting::slot_setZoneTime(QString zoneTime)
{

    sysControl->set_value(TIME_ZONE,zoneTime);
    QList<TimeZone> *list = sysTime->get_listTimeZone();

    //设置时区
    for(int i = 0; i < list->size(); i++)
    {
        if(list->at(i).name == zoneTime)
        {
            emit signal_set_timeZone(list->at(i).gmt);
            break;
        }
    }

}

int PageSetting::slot_getCurZoneIndex()
{
    return curTimeZoneIndex;
}

QString PageSetting::slot_getTimeZone()
{
    QString timeZone = "";
    QString curname =  sysControl->get_value(TIME_ZONE);
    QList<TimeZone> *list = sysTime->get_listTimeZone();
    for(int i  = 0; i < list->size(); i++)
    {
        if(list->at(i).name == curname)
        {
            timeZone = list->at(i).description +" " + list->at(i).gmt;
            break;
        }
    }
    return timeZone;
}

bool PageSetting::slot_wifiListIsEmpty()
{
    return wifiDataList.isEmpty();
}

void PageSetting::slot_wifiConnect(QString ssid, QString pwd)
{
    iWifiConnectResult = WIFI_DISCONNECT;
    qDebug() << "pwd = " << pwd << ",ssid = " << ssid;
    emit signal_connect_withKey(ssid,pwd);
}

void PageSetting::slot_connect_result(int result)
{
    qDebug() << "slot_connect_result-------------";

    qDebug() << result;
    iWifiConnectResult = result;
    emit signal_connRes();
}

int PageSetting::slot_getWifiConnRes()
{
    return iWifiConnectResult;
}

int PageSetting::slot_get_keyType(QString ssid)
{
    int i = sysWiFi->get_keyType(ssid);
    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@!!!!!!!!!!!!!!!!!!!!!!"<<i;
    return i;
}

void PageSetting::slot_connect_withNetID(QString ssid)
{
    emit signal_connect_withNetID(ssid);
}

void PageSetting::slot_connect_publicWiFi(QString ssid)
{
    emit signal_connectPublicWifi(ssid);
}

void PageSetting::slot_wifiListAllowRefresh(bool flag)
{
    wifiListAllowRefresh = flag;
}

void PageSetting::slot_forgetWifi(QString ssid)
{
    emit signal_remove_cookie(ssid);
}

void PageSetting::slot_forgetAllWifi()
{
    emit signal_remove_allCookie();
}

int PageSetting::slot_getWifiConnStatu()
{
    return wifiConnStat;
}

QString PageSetting::slot_getCurConnSSid()
{
    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@"<<sysControl->get_currentWiFi().ssid;
    return sysControl->get_currentWiFi().ssid;
}


void PageSetting::slot_wifi_status(WiFi wifi)
{
    switch (wifi.status) {
    case CONNECTED:
        wifiConnStat = 0;
        break;
    case DISCONNECT:
        wifiConnStat = 1;
        break;
    case UNUSEABLE:
        wifiConnStat = 2;
        break;
    case CONNECTTING:
        wifiConnStat = 3;
        break;
    default:
        break;
    }
    emit signal_wifi_connStatChange();
}

void PageSetting::slot_setDateTime(QString datime)
{
    //设置时间
    emit signal_set_dateTime(datime);
}

void PageSetting::slot_showWifiPage()
{
    slot_setQMl(15);
    wifiEntry = 1;
    emit signal_changeEntry();
}
void PageSetting::slot_showMiAppPage()
{
    slot_setQMl(6);
    wifiEntry = 1;
    emit signal_changeEntry();
}

int PageSetting::slot_getEntry()
{
    return wifiEntry;
}

/*******************************************************************************
*                           虎正玺@2016-12-01
* Function Name  :  slot_getCurVersion
* Description    :  获取当前固件版本信息
* Input          :  bool flag :为true时返回当前版本，为false时返回新版本
* Output         :  None
* Return         :  None
*******************************************************************************/
QString PageSetting::slot_getCurVersion(bool flag)
{
    if(flag)
    {
        return FIRMWARE_VERSION;
    }
    else
    {
        return newVersion;
    }
}

QString PageSetting::slot_getCurSystem(bool flag)
{
    if(flag)
    {
        return funcFirmware->get_version_system();
    }
    else
    {
        return newVersion;
    }
}

QString PageSetting::slot_getVersionDes()
{
    return newVersionDes;
}

/*******************************************************************************
*                           虎正玺@2016-12-01
* Function Name  :  slot_start_download
* Description    :  开始下载固件
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
int PageSetting::slot_start_download()
{
    emit signal_start_download();
}

/*******************************************************************************
*                           虎正玺@2016-12-01
* Function Name  :  slot_start_checkUpdate
* Description    :  检查更新
* Input          :  int type:检测类型 发布还是调试 默认为0 发布  1为调试
* Output         :  None
* Return         :  None
*******************************************************************************/
void PageSetting::slot_start_checkUpdate(int type)
{
    qDebug() << "------------slot_start_checkUpdate begin-------------------------------------------------t-------" ;
    qDebug()<< "type = " << type << endl;
    if(0 == type)
    {

        emit signal_check_update();
    }
    else
    {
        emit signal_check_updateDebug();
    }
    qDebug() << "------------slot_start_checkUpdate end--------------------------------------------------------" ;

}

int PageSetting::slot_getProgress()
{
    return downLoadProcess;
}

void PageSetting::slot_updateProgress(qint64 bytesGot, qint64 bytesTotal)
{
    if(bytesGot >= bytesTotal)
        downLoadProcess = 100;
    else
        downLoadProcess = bytesGot / (bytesTotal * 1.0) *100;

    emit signal_updateProgress();
}

void PageSetting::slot_update_firmware(QString versionInfo, QString versionDes)
{
    newVersion  = versionInfo;
    newVersionDes = versionDes;
    emit signal_update_firmware();
}

void PageSetting::slot_update_firmware()
{
    emit  signal_no_firmware();
}

void PageSetting::slot_update_download_success()
{
    //修改配置文件标志位
    sysControl->set_value(SOFTWARE_UPGRADE,DEFAULT_SOFTWARE_UPGRADE);
    emit signal_update_download_success();
}

void PageSetting::slot_ota_net_abnormal()
{
    otaErrorCode = NET_ABNORMAL;
    emit signal_update_download_fail();
    emit  signal_errorInfo();
}

void PageSetting::slot_ota_net_disconnect()
{
    otaErrorCode = NET_DISCONNECT;
    emit signal_update_download_fail();
    emit  signal_errorInfo();


}

int PageSetting::slot_getErrorCode()
{
    return otaErrorCode;
}

void PageSetting::slot_setAutoLocal()
{
    emit signal_set_cityAuto();
}

QString PageSetting::slot_get_locationAuto()
{
    return funcCity->get_locationAuto();
}

QString PageSetting::slot_get_locationMannual()
{
    return funcCity->get_locationManual();
}

QString PageSetting::slot_getLocaltionCity()
{
    return funcCity->get_location();
}

bool PageSetting::slot_getLocalMode()
{
    return funcCity->get_locationMode();
}

void PageSetting::set_cityManual(QString strCity)
{
    qDebug() << "--------------set_cityManual------------------------------------------------------";
    qDebug() << strCity;
    emit signal_set_cityManual(strCity);
}

void PageSetting::slot_searchCity(QString str)
{
    qDebug() << "--------------slot_searchCity------------------------------------------------------";
    qDebug() << str;
    emit signal_search_city(str);
    cityDataList.clear();
}

void PageSetting::slot_searchRecCityList(QList<InfoCity> *cityList)
{
    qDebug() << "--------------slot_searchRecCityList------------------------------------------------------";
    qDebug() << cityList->size();
    cityDataList.clear();

    QString strTmp = "";
    for(int i = 0; i < cityList->size(); i++)
    {
        if(!cityList->at(i).area.isEmpty())
        {
            strTmp = cityList->at(i).name + ", " + cityList->at(i).area;
        }
        if(!cityList->at(i).country.isEmpty())
        {
            strTmp  = strTmp + ", " + cityList->at(i).country;
        }
        cityDataList.append(new CityDataObject(cityList->at(i).name, strTmp));
    }
    ctxt = view->rootContext();

    ctxt->setContextProperty("cityModel", QVariant::fromValue(cityDataList));
    emit signal_showCityList();

}

int PageSetting::slot_getSearchCityNum()
{
    return cityDataList.size();
}

void PageSetting::slot_local_net_abnormal()
{
    localErrorCode = NET_ABNORMAL;
    emit signal_localnet_abnormal();
}

void PageSetting::slot_local_net_disconnect()
{
    localErrorCode = NET_DISCONNECT;
    emit signal_loaclnet_disconnect();
}

void PageSetting::slot_netIp(QString ip)
{
    strIp = ip;
    emit signal_localNetIpChange();
}

int PageSetting::slot_getLocalErrorCode()
{
    return localErrorCode;
}

void PageSetting::slot_locate_success(QString strCity)
{

    autoCity = strCity;
    emit signal_autoLocalRes();
}

void PageSetting::slot_locate_failed()
{
    //    autoCity = "";
    emit signal_autoLocalRes();
}

QString PageSetting::slot_getLocalIp()
{
    return strIp;
}

QString PageSetting::slot_getAutoLocalRes()
{
    return  autoCity;
}


void PageSetting::slot_outLight_value(int value)
{

    outLightValue = value;
    emit signal_outLight();
}

void PageSetting::slot_intLight_value(int value)
{
    inLightValue = value;
    emit signal_inLight();
}

int PageSetting::slot_getOutOrInLightValue(int type)
{
    if(1 == type)
    {
        return outLightValue;
    }
    else if(2 == type)
    {
        return inLightValue;
    }
}

void PageSetting::slot_reset()
{
    //重置数据库
    emit signal_resetDb();

    //重置数据库
    sysControl->reset();

    //清空wifi
    emit signal_remove_allCookie();
}

void PageSetting::slot_restart()
{
    emit signal_device_reboot();
}

void PageSetting::slot_jumpLanWizard()
{
    emit signal_jumpLangeWizard();
}

void PageSetting::slot_wizardFinish()
{
    emit signal_wizardFinish();
}

bool PageSetting::slot_isUsLang()
{
    return sysControl->get_value(LANGUAGE).contains("US");
}

QString PageSetting::slot_getWifiWizardLanguage()
{
    return wifiPageWizardLanguage;
}

QString PageSetting::slot_getPmDetectingFrequency(int type)
{
    return type == 0 ? sysControl->get_value(PM25_SAMPLING_CHARGING) : sysControl->get_value(PM25_SAMPLING_DISCHARGING);
}

QString PageSetting::slot_getScreenAutoInterval(int type)
{
    return type == 0 ? sysControl->get_value(SCREEN_OFF_CHARGING) : sysControl->get_value(SCREEN_OFF_DISCHARGING);
}

void PageSetting::slot_setPmDetetingFrequency(int type, QString value)
{
    IntervalType interval;
    if("30" == value)
    {
        interval = MIN30;
    }
    else if("15" == value)
    {
        interval = MIN15;
    }
    else
    {
        interval = MIN5;
    }

    if(0 == type)
    {
        emit signal_intervalPM25_charging(interval);
    }
    else
    {
        emit signal_intervalPM25_discharging(interval);
    }
}

void PageSetting::slot_setScreenAutoInterval(int type, QString value)
{
    IntervalType offInterval;
    if("Never" == value)
    {
        offInterval = NEVER;
    }
    else if("30" == value)
    {
        offInterval = MIN30;
    }
    else if("15" == value)
    {
        offInterval = MIN15;
    }
    else
    {
        offInterval = MIN5;
    }

    if(0 == type)
    {
        emit signal_offInterval_charging(offInterval);
    }
    else
    {
        emit signal_offInterval_discharging(offInterval);
    }
}

/*******************************************************************************
* Author        :   虎正玺@2017-03
* Function Name :   slot_setScreenAutoIndex
* Description   :   设置
* Input         :   None
* Output        :   None
* Return        :   None
*******************************************************************************/
void PageSetting::slot_setScreenAutoIndex(int index)
{
    initScreenIndex = index;
}

int PageSetting::slot_getScreenAutoIndex()
{
    return initScreenIndex;
}

void PageSetting::slot_setPmDetetingSettingType(int type)
{
    pmDetectingFrequencySettingType = type;
}

int PageSetting::slot_getPmDetetingSettingType()
{
    return pmDetectingFrequencySettingType;
}

int PageSetting::slot_getScreenSettingScrollValue()
{
    return iScreenSettingScrollValue;
}

void PageSetting::slot_setSceenSettingScrollvalue(int value)
{
    iScreenSettingScrollValue = value;
}

void PageSetting::slot_setQMl(int i)
{
    //如果是打开时区页面，则进行判空操作

    if(i == 8 && sysTime->get_listTimeZone()->size() <=0)
    {
        return ;
    }
    iCurPage = i;
    if(view != NULL)
    {
        view->deleteLater();
    }
    view  = new QDeclarativeView(this);
    view->engine()->rootContext()->setContextProperty("SettingProperty", this);
    switch (i) {
    case 0:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/Setting.qml"));
        break;
    case 2:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/connectApp/MatchesApp.qml"));
        break;
    case 3:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/connectApp/ConnAppPage.qml"));
        break;
    case 4:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/nightMode/NightModePage.qml"));
        emit signal_nightInit();
        break;
    case 5:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/nightMode/TimePage.qml"));
        emit signal_intNightTime();
        break;
    case 6:
        wifiEntry = 2;
        view->setSource(QUrl("qrc:/resource/qml/settingQml/connectMiApp/ConnectMiHomeManager.qml"));
        break;
    case 7:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/dateAndTime/DatetimePage.qml"));
        emit signal_initTimeDate();
        break;
    case 8:
        set_zoneInfo();
        view->setSource(QUrl("qrc:/resource/qml/settingQml/dateAndTime/Timezone.qml"));
        emit signal_initTimeZoneListPos();
        break;
    case 9:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/light/LightPage.qml"));
        init_light();
        break;
    case 10:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/reset/ResetPage.qml"));
        break;
    case 11:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/language/LanguagePage.qml"));
        emit signal_initLang();
        break;
    case 12:
        ctxt = view->rootContext();
        ctxt->setContextProperty("cityModel", QVariant::fromValue(cityDataList));
        view->setSource(QUrl("qrc:/resource/qml/settingQml/location/LocalManagerPage.qml"));
        emit signal_initLocalpage();
        break;
    case 14:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/tVOC/TvocPage.qml"));
        emit signal_initTvocUnit();
        break;
    case 15:
        wifiEntry = 2;
        ctxt = view->rootContext();
        emit signal_refresh_listWiFi();
        ctxt->setContextProperty("wifiModel", QVariant::fromValue(wifiDataList));
        view->setSource(QUrl("qrc:/resource/qml/settingQml/wifi/WifiManagerPage.qml"));
        break;
    case 16:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/VersionUpgrade/VersionsUpgrade.qml"));
        emit signal_initFirwarePage();
        break;
    case 18:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/dateAndTime/DateTime.qml"));
        emit signal_initDateTimeSpinner();
        break;
    case 19:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/pmFrequency/PmFrequency.qml"));
        break;
    case 20:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/light/ScreenAutoOff.qml"));
        break;
    case 21:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/about/AboutPage.qml"));
        break;
    case 22:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/pmFrequency/BasePmFrequency.qml"));
        break;
    case 23:
        view->setSource(QUrl("qrc:/resource/qml/settingQml/tempUnit/TempUnit.qml"));
        emit signal_initTempUnit();
        break;
    default:
        break;
    }
    emit singal_changeScrollValue();

    view->show();
}

void PageSetting::refresh_page(QList<WiFi> *wifiList)
{
    if(wifiListAllowRefresh)
    {

        wifiDataList.clear();

        qDebug() << "----------------refresh_page begin-------------------------------------------------------" << endl;
        for(int i = 0; i < wifiList->size(); i++)
        {
            wifiDataList.append(new WifiDataObject(wifiList->at(i)));
            qDebug() << wifiList->at(i).ssid << ", " << wifiList->at(i).status;
        }
        ctxt = view->rootContext();

        ctxt->setContextProperty("wifiModel", QVariant::fromValue(wifiDataList));
        emit signal_showWifiList();
        qDebug() << "----------------refresh_page end-------------------------------------------------------" << endl;

    }

}

void PageSetting::slot_back()
{
    emit signal_back();

}

void PageSetting::slot_reconnectWifi()
{
    timerCounter++;
    if(iWifiConnectResult != WIFI_CONNECT_SUCCESS && timerCounter < 3)
    {
        slot_wifiConnect("Cleargrass_SZ", "cleargrass2015");
    }else
    {
        wifiTimer->stop();
    }
}

void PageSetting::slot_disconnectWifi()
{
    timerCounter++;
    if(iWifiConnectResult == WIFI_CONNECT_SUCCESS && timerCounter < 3)
    {
        emit signal_remove_allCookie();
    }
    else
    {
        wifiDisconnectTimer->stop();
    }
}

QString PageSetting::get_sysVersion()
{
    QString strReadLine;
    QFile file(FILE_SYSVERSION);
    if (file.open(QFile::ReadOnly))
    {
        QTextStream stream(&file);

        //读取文件内容
        strReadLine = stream.readLine();

        //关闭文件
        file.close();
    }
    else
    {
        qDebug() << file.errorString();
        return "NO SYSTEM VERSION";
    }

    return strReadLine;
}

void PageSetting::set_zoneInfo()
{
    curTimeZoneIndex = 0;
    QString curZoneTime = sysControl->get_value(TIME_ZONE);
    QList<TimeZone> *list = sysTime->get_listTimeZone();
    QString netTimeZone = sysTime->get_timeZoneAuto();
    dataZoneList.clear();
    for(int i = 0; i < list->size(); i++)
    {

        if(netTimeZone == list->at(i).name)
        {
            dataZoneList.push_front(new DataObject(list->at(i).description, list->at(i).gmt, list->at(i).name, true));

        }
        if(curZoneTime == list->at(i).name)
        {
            curTimeZoneIndex = i+1;
        }
        dataZoneList.append(new DataObject(list->at(i).description, list->at(i).gmt, list->at(i).name, false));
    }

    QDeclarativeContext *ctxt = view->rootContext();
    if(list->size() == dataZoneList.size())
    {
        curTimeZoneIndex--;
    }
    ctxt->setContextProperty("zoneModel", QVariant::fromValue(dataZoneList));

}

void PageSetting::init_light()
{
    bool bAuto = false;

    if(sysControl->get_value(LIGHT_AUTO) == "true")
    {
        bAuto = true;

    }
    else
    {
        bAuto = false;

    }
    curLightValue = sysControl->get_value(LIGHT_VALUE).toInt();

    //    sysLight->set_autoAdjust(bAuto);
    emit signal_isAutoChange();

}


void PageSetting::on_page_defalut_back()
{
    emit signal_initScrllValue();
    emit signal_jumpMainPage();

}
