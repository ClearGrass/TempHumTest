#-------------------------------------------------
#
# Project created by QtCreator 2016-07-08T22:23:19
#
#-------------------------------------------------

QT          += core gui sql thread declarative qml
QT          += network script
include(code/3rdparty/qextserialport/src/qextserialport.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#加载国际化文件 ZH_SC为中文简体  ZH_TC为中文繁体 US为英文
TRANSLATIONS += ZH_SC.ts ZH_TC.ts US.ts

#添加对QML文件的翻译识别
lupdate_only{
    SOURCES += resource/qml/*.qml \
              resource/qml/blockHorizonQml/*.qml \
              resource/qml/blockQml/*.qml \
              resource/qml/cardQml/*.qml \
              resource/qml/charging/*.qml \
              resource/qml/content/*.qml \
              resource/qml/Dropdown/*.qml \
              resource/qml/labelQml/*.qml \
              resource/qml/poweroff/*.qml \
              resource/qml/toolQml/*.qml \
              resource/qml/valueQml/*.qml \
              resource/qml/weatherQml/*.qml \
              resource/qml/verticalScreen/*.qml \
              resource/qml/settingQml/*.qml \
              resource/qml/settingQml/baseQml/*.qml \
              resource/qml/settingQml/connectApp/*.qml \
              resource/qml/settingQml/connectMiApp/*.qml \
              resource/qml/settingQml/dateAndTime/*.qml \
              resource/qml/settingQml/keyboard/*.qml \
              resource/qml/settingQml/language/*.qml \
              resource/qml/settingQml/light/*.qml \
              resource/qml/settingQml/location/*.qml \
              resource/qml/settingQml/nightMode/*.qml \
              resource/qml/settingQml/reset/*.qml \
              resource/qml/settingQml/tVOC/*.qml \
              resource/qml/settingQml/VersionUpgrade/*.qml \
              resource/qml/settingQml/wifi/*.qml \
              resource/qml/settingQml/pmFrequency/*.qml \
              resource/qml/settingQml/about/*.qml

}

TARGET      = BranQt4
TEMPLATE    = app
DESTDIR     += ../TempHumTest

CONFIG      +=  qaxcontainer

# 定义宏变量Bran_R8 以区分 QT编译的code是运行于R8还是PC
DEFINES += Bran_R8
DEFINES += QML

#条件编译
if(contains(DEFINES, Bran_R8)){
    LIBS        += -L../TempHumTest/lib_R8 -lwifimg
    LIBS        += -L../TempHumTest/lib_R8 -lwilddog
    LIBS        += -L../TempHumTest/lib_R8 -lqrencode
}else{

# 增加野狗库
    LIBS        += -L../TempHumTest/lib -lwilddog
    LIBS        += -L../TempHumTest/lib -lqrencode
}

SOURCES += \
    code/view/historyPage/colorGenerator.cpp \
    code/view/historyPage/historyChartBase.cpp \
    code/view/historyPage/historyPage.cpp \
    code/main.cpp \
    code/view/mainViewQml.cpp \
    code/control/system/language/sysLanguage.cpp \
    code/control/system/light/sysLight.cpp \
    code/control/system/time/sysTime.cpp \
    code/drivers/pm25/driverPM25.cpp \
    code/drivers/light/driverLight.cpp \
    code/drivers/gravity/driverGravity.cpp \
    code/drivers/tempHum/driverTempHum.cpp \
    code/drivers/button/driverButton.cpp \
    code/drivers/battery/driverBattery.cpp \
	code/drivers/Wi-Fi/driverWiFi.cpp \
    code/control/system/button/sysButton.cpp \
    code/control/system/Wi-Fi/sysWiFi.cpp \
    code/control/system/gravity/sysGravity.cpp \
    code/view/settingPage/pageSetting.cpp \
    code/database/database.cpp \
    code/drivers/time/driverTime.cpp \
    code/3rdparty/cJSON/cJSON.c \
    code/view/settingPage/time/dataobject.cpp \
    code/view/settingPage/Wi-Fi/wifiDataobject.cpp \
    code/control/airData/dataPM25.cpp \
    code/control/airData/dataTempHumi.cpp \
    code/control/airData/dataTvocCo2e.cpp \
    code/control/system/sysControl.cpp \
    code/control/system/battery/sysBattery.cpp \
    code/drivers/driversInit.cpp \
    code/file/config/fileConfig.cpp \
    code/file/timeZone/fileTimeZone.cpp \
    code/view/settingPage/localtion/cityDataobject.cpp \
    code/database/tableAirData.cpp \
    code/wilddog/sysData/wilddogWeather.cpp \
    code/server/networkManage.cpp \
    code/server/sysData/serverApp.cpp \
    code/server/sysData/serverCity.cpp \
    code/server/sysData/serverDevice.cpp \
    code/server/sysData/serverFirmware.cpp \
    code/server/sysData/serverWeather.cpp \
    code/server/airData/serverAirData.cpp \
    code/view/setupWizard/setupwizard.cpp \
    code/view/historyPage/waiting/waiting.cpp \
    code/view/historyPage/waiting/labelRotate.cpp \
    code/database/tableCache.cpp \
    code/server/serverControl.cpp \
    code/view/setupWizard/wizardwait.cpp \
    code/wilddog/wilddogControl.cpp \
    code/wilddog/airData/wilddogAirData.cpp \
    code/control/system/power/sysPower.cpp \
    code/control/system/data/sysData.cpp \
    code/control/airData/dataAirControl.cpp \
    code/control/function/weather/funcWeather.cpp \
    code/control/function/city/funcCity.cpp \
    code/control/function/firmware/funcFirmware.cpp \
    code/control/function/app/funcApp.cpp \
    code/control/system/device/sysDevice.cpp \
    code/control/system/cpu/sysCpu.cpp \
    code/control/mainControl.cpp \
    code/view/powerOn/pagePowerOn.cpp \
    code/view/power/pagePowerOff.cpp \
    code/server/sysData/serverTime.cpp \
    code/wilddog/sysData/wilddogCity.cpp \
    code/wilddog/sysData/wilddogPower.cpp \
    code/wilddog/sysData/wilddogStatus.cpp \
    code/wilddog/sysData/wilddogApp.cpp \
    code/server/sysData/serverDebug.cpp \
    code/control/debugControl.cpp \
    code/view/historyPage/SwitchControl.cpp \
    code/view/historyPage/weatherChartBase.cpp \
    code/miio/miioControl.cpp \
    code/drivers/tvocCo2e/driverTvocCo2e.cpp \
    code/view/dropDown/dropDown.cpp \
    code/wilddog/sysData/wilddogDevice.cpp \
    code/drivers/tempHum/sensirion_configuration.c \
    code/drivers/tempHum/sht3x.c \
    code/sysOCC.cpp
	
HEADERS  += \
    code/view/historyPage/colorGenerator.h \
    code/view/historyPage/historyChartBase.h \
    code/view/historyPage/historyChartDefine.h \
    code/view/historyPage/historyPage.h \
    code/view/mainViewQml.h \
    code/globalDefine.h \
    code/control/system/language/sysLanguage.h \
    code/control/system/light/sysLight.h \
    code/control/system/time/sysTime.h \
    code/drivers/pm25/driverPM25.h \
    code/drivers/time/driverTime.h \
    code/drivers/light/driverLight.h \
    code/drivers/gravity/driverGravity.h \
    code/drivers/button/driverButton.h \
    code/drivers/tempHum/driverTempHum.h \
    code/control/system/Wi-Fi/sysWiFi.h \
    code/drivers/battery/driverBattery.h \
	code/drivers/Wi-Fi/Wi-Fimanager/network_manager.h \
    code/drivers/Wi-Fi/Wi-Fimanager/wifi_event.h \
    code/drivers/Wi-Fi/Wi-Fimanager/wifi_intf.h \
    code/drivers/Wi-Fi/Wi-Fimanager/wifi_state_machine.h \
    code/drivers/Wi-Fi/Wi-Fimanager/wifi.h \
    code/drivers/Wi-Fi/Wi-Fimanager/wpa_ctrl.h \
    code/drivers/Wi-Fi/Wi-Fimanager/wpa_supplicant_conf.h \
    code/drivers/Wi-Fi/driverWiFi.h \
    code/control/system/button/sysButton.h \
    code/control/system/gravity/sysGravity.h \
    code/view/settingPage/pageSetting.h \
    code/database/database.h \
    code/3rdparty/cJSON/cJSON.h \
    code/3rdparty/wilddog/wilddog.h \
    code/3rdparty/wilddog/wilddog_api.h \
    code/3rdparty/wilddog/wilddog_config.h \
    code/3rdparty/wilddog/wilddog_debug.h \
    code/3rdparty/wilddog/wilddog_port.h \
    code/view/settingPage/time/dataobject.h \
    code/view/settingPage/Wi-Fi/wifiDataobject.h \
    code/control/airData/dataPM25.h \
    code/control/airData/dataTempHumi.h \
    code/control/airData/dataTvocCo2e.h \
    code/control/airData/dataAirDefine.h \
    code/database/databaseDefine.h \
    code/globalStruct.h \
    code/globalVariable.h \
    code/control/system/sysControl.h \
    code/control/system/battery/sysBattery.h \
    code/drivers/driversInit.h \
    code/file/config/fileConfig.h \
    code/file/timeZone/fileTimeZone.h \
    code/view/settingPage/localtion/cityDataobject.h \
    code/database/tableAirData.h \
    code/wilddog/wilddogDefine.h \
    code/wilddog/sysData/wilddogWeather.h \
    code/server/networkManage.h \
    code/server/serverDefine.h \
    code/server/sysData/serverApp.h \
    code/server/sysData/serverCity.h \
    code/server/sysData/serverDevice.h \
    code/server/sysData/serverFirmware.h \
    code/server/sysData/serverWeather.h \
    code/server/airData/serverAirData.h \
    code/view/setupWizard/setupwizard.h \
    code/file/config/globalConfigDefine.h \
    code/view/historyPage/waiting/waiting.h \
    code/view/historyPage/waiting/labelRotate.h \
    code/database/tableCache.h \
    code/server/serverControl.h \
    code/view/setupWizard/wizardwait.h \
    code/wilddog/wilddogControl.h \
    code/wilddog/airData/wilddogAirData.h \
    code/control/system/sysDefine.h \
    code/control/system/power/sysPower.h \
    code/control/system/data/sysData.h \
    code/control/airData/dataAirControl.h \
    code/control/function/weather/funcWeather.h \
    code/control/function/city/funcCity.h \
    code/control/function/firmware/funcFirmware.h \
    code/control/function/app/funcApp.h \
    code/control/system/device/sysDevice.h \
    code/control/system/cpu/sysCpu.h \
    code/control/mainControl.h \
    code/view/powerOn/pagePowerOn.h \
    code/view/power/pagePowerOff.h \
    code/server/sysData/serverTime.h \
    code/wilddog/sysData/wilddogCity.h \
    code/wilddog/sysData/wilddogPower.h \
    code/wilddog/sysData/wilddogStatus.h \
    code/wilddog/sysData/wilddogApp.h \
    code/server/sysData/serverDebug.h \
    code/control/debugControl.h \
    code/view/historyPage/SwitchControl.h \
    code/view/historyPage/weatherChartBase.h \
    code/miio/miioControl.h \
    code/drivers/tvocCo2e/driverTvocCo2e.h \
    code/3rdparty/qrcode/qrencode.h \
    code/view/dropDown/dropDown.h \
    code/wilddog/sysData/wilddogDevice.h \
    code/drivers/tempHum/sensirion_configuration.h \
    code/drivers/tempHum/sht3x.h \
    code/sysOCC.h




FORMS    += \
    ui/mainPage/mainwindow.ui \
    ui/historyPage/historyPage.ui \
    code/view/setupWizard/setupwizard.ui \
    code/view/historyPage/waiting/waiting.ui \
    code/view/setupWizard/wizardwait.ui

INCLUDEPATH +=code/ \
    code/view \
    code/drivers \
    code/drivers/battery \
    code/drivers/button \
    code/drivers/gravity \
    code/drivers/light \
    code/drivers/pm25 \
    code/drivers/tempHum \
    code/drivers/Wi-Fi \
    code/drivers/time \
    code/drivers/Wi-Fi/Wi-Fimanager\
    code/view/base \
    code/view/mainPage/temp \
    code/view/mainPage/pm2.5 \
    code/view/mainPage/hum \
    code/view/historyPage \
    code/control/system/button \
    code/control/system/language \
    code/control/system/light/ \
    code/control/system/time/ \
    code/control/system/Wi-Fi/ \
    code/control/system/gravity/ \
    code/control/system/location/ \
    code/control/system/config/ \
    code/control/function/app/ \
    code/control/system/firmware/ \
    code/control/system/weather/ \
    code/control/system/power/ \
    code/control/system/screen/ \
    code/control/system/unit/ \
    code/control/system \
    code/control/function \
    code/control/airData \
    code/database \
    code/3rdparty/qextserialport/src \
    code/3rdparty/wilddog \
    code/view/settingPage \
    code/view/setupWizard \
    code/view/settingPage/time \
    code/view/settingPage/Wi-Fi \
    code/view/settingPage/localtion \
    code/server/weather \
    code/server \
    code/view/historyPage/waiting \
    code/view/dropDown/


RESOURCES += \
    BranQt.qrc

DISTFILES +=











