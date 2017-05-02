import QtQuick 1.0
import "baseQml" as BaseQml
import Setting 1.0
Rectangle{

    //wifi
    property string iconWifi: "qrc:resource/images/optionPage/logo/logo_wifi.png"

    //固件
    property string iconfirmware: "qrc:resource/images/optionPage/logo/logo_firmware.png"

    //app
    property string iconApp: "qrc:resource/images/optionPage/logo/logo_app.png"

    //连接米家
    property string iconConnMi: "qrc:resource/images/optionPage/logo/logo_mijia.png"

    //屏幕
    property string iconLight: "qrc:resource/images/optionPage/logo/logo_light.png"

    //夜间模式
    property string iconNight: "qrc:resource/images/optionPage/logo/logo_night.png"

    //所在地
    property string iconLocation: "qrc:resource/images/optionPage/logo/logo_location.png"

    //日期
    property string iconDatetime: "qrc:resource/images/optionPage/logo/logo_datatime.png"

    //温度单位
    property string iconTempUnit:"qrc:/resource/images/optionPage/logo/logo_tempUnit.png"

    //语言
    property string iconLang: "qrc:resource/images/optionPage/logo/logo_language.png"

    //恢复出厂设置
    property string iconReset: "qrc:resource/images/optionPage/logo/logo_reset.png"

    //pm2.5检测频率
    property string iconPmFrequency: "qrc:resource/images/optionPage/logo/logo_pm25.png"

    property string iconTVOC: "qrc:resource/images/optionPage/logo/logo_tVOC.png"

    //关于
    property string iconAbout: "qrc:/resource/images/optionPage/logo/logo_about.png"

    property bool  timeIsInit: SettingProperty.slot_timeIsInit()
    property string whiteColor: "#ffffff"
    property string blueColor: "#00e5ff"

    property int mousPositionFlag: 0
    height: timeIsInit ? 982 :856
    width: 854
    id: rootRect
    signal signal_reset()


    Column{
        width: 427
        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.left: parent.left
        //wifi
        BaseButton{
            id:wifi
            iconSource:iconWifi
            nameText: "Wi-Fi"
            variText: get_wifiStatu(SettingProperty.slot_getWifiConnStatu())
            MouseArea{
                anchors.fill:wifi
                onClicked:
                {
                    SettingProperty.slot_setQMl(15)
                }
                onPressed: {
                    wifi.opacity = 0.5
                }
                onReleased: {
                    wifi.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        wifi.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }
        }

        //连接米家
        BaseButton{
            id:connMi
            iconSource: iconConnMi
            nameText: qsTr("Connect to Mi Home")
            variText: qsTr("Not Connected")
            MouseArea{
                anchors.fill: connMi
                onClicked: {
                    SettingProperty.slot_setQMl(6);
                }
                onPressed: {
                    connMi.opacity = 0.5
                }
                onReleased: {
                    connMi.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        connMi.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }
        }

        //夜间模式
        BaseButton{
            id:night
            iconSource: iconNight
            nameText: qsTr("Night Mode")
            variText: get_nightmode_interval()
            visible:timeIsInit
            MouseArea{
                anchors.fill:night
                onClicked:
                {
                    SettingProperty.slot_setQMl(4)

                }
                onPressed: {
                    night.opacity = 0.5
                }
                onReleased: {
                    night.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        night.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }
        }

        //所在地
        BaseButton{
            id:local
            iconSource: iconLocation
            nameText: qsTr("Location")
            variText: get_localCity()
            MouseArea{
                anchors.fill:local
                onClicked:
                {
                    SettingProperty.slot_setQMl(12)
                }
                onPressed: {
                    local.opacity = 0.5
                }
                onReleased: {
                    local.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        local.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }
        }

        //温度单位
        BaseButton{
            id:unit
            iconSource: iconTempUnit
            nameText: qsTr("TempUnit")
            variText: SettingProperty.slot_curTempUnitIsFahrenheit() ?  "℉" : "℃"
            MouseArea{
                anchors.fill:unit
                onClicked:
                {
                    SettingProperty.slot_setQMl(23)
                }
                onPressed: {
                    unit.opacity = 0.5
                }
                onReleased: {
                    unit.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        unit.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }
        }

        //系统语言
        BaseButton{
            id:languge
            iconSource: iconLang
            nameText: qsTr("Language")
            variText: get_lang()

            MouseArea{
                anchors.fill:languge
                onClicked:
                {
                    SettingProperty.slot_setQMl(11)
                }
                onPressed: {
                    languge.opacity = 0.5
                }
                onReleased: {
                    languge.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        languge.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }
        }

        //关于
        BaseButtonTwo{
            id:about
            iconSource: iconAbout
            nameText: qsTr("About")

            MouseArea{
                anchors.fill: about
                onClicked: {
                    SettingProperty.slot_setQMl(21);
                }
                onPressed: {
                    about.opacity = 0.5
                }
                onReleased: {
                    about.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        about.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }

            }
        }

    }

    BaseQml.VerticalLine{
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
    }

    Column
    {
        width: 427

        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.right: parent.right
        //固件版本
        BaseButton{
            id:firmware

            iconSource:iconfirmware
            nameText: qsTr("Software Version")
            variText:get_versionInfo()

            BaseQml.BadgeImage{
                id:versionSign
                visible: false
            }

            MouseArea{
                anchors.fill: firmware
                onClicked: {
                    SettingProperty.slot_setQMl(16);
                }
                onPressed: {
                    firmware.opacity = 0.5
                }
                onReleased: {
                    firmware.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        firmware.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }
        }
        //分割线
        BaseQml.HorizonLine{
            anchors.right: parent.right

        }
        //屏幕
        BaseButtonTwo{
            id:light

            iconSource: iconLight
            nameText: qsTr("Screen")

            MouseArea{
                anchors.fill:light
                onClicked:
                {
                    SettingProperty.slot_setQMl(9)
                }
                onPressed: {
                    light.opacity = 0.5
                }
                onReleased: {
                    light.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        light.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }
        }
        //分割线
        BaseQml.HorizonLine{
             anchors.right: parent.right
        }



        //pm2.5检测频率
        BaseButtonTwo{
            id:pmFrequency
            iconSource: iconPmFrequency
            nameText: qsTr("PM2.5 Detecting Frequency")
            MouseArea{
                anchors.fill: pmFrequency
                onClicked: {
                    SettingProperty.slot_setQMl(19);
                }
                onPressed: {
                    pmFrequency.opacity = 0.5
                }
                onReleased: {
                    pmFrequency.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        pmFrequency.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }
        }
        //分割线
        BaseQml.HorizonLine{
             anchors.right: parent.right
        }


        //日期
        BaseButtonTwo{
            id:datetime
            iconSource: iconDatetime
            nameText: qsTr("Date and Time")
            MouseArea{
                anchors.fill:datetime
                onClicked:
                {
                    SettingProperty.slot_setQMl(7)
                }
                onPressed: {
                    datetime.opacity = 0.5
                }
                onReleased: {
                    datetime.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        datetime.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }
        }
        //分割线
        BaseQml.HorizonLine{
             anchors.right: parent.right
        }


        //tvoc
        BaseButton{
            id:tvoc
            iconSource: iconTVOC
            nameText: qsTr("tVOC Unit")
            variText:
            {

                var unit =  SettingProperty.slot_getTvocUnitName();
                if(unit == "mg/m3")
                {
                    "mg/m³"
                }
                else
                {
                    unit
                }
            }
            MouseArea{
                anchors.fill:tvoc
                onClicked:
                {
                    SettingProperty.slot_setQMl(14)
                }
                onPressed: {
                    tvoc.opacity = 0.5
                }
                onReleased: {
                    tvoc.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        tvoc.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }

        }
        //分割线
        BaseQml.HorizonLine{
             anchors.right: parent.right
        }

        //恢复出厂设置
        BaseButtonTwo{
            id:reset
            iconSource: iconReset
            nameText: qsTr("Restore to Factory Settings")
            MouseArea{
                anchors.fill:reset
                onClicked:
                {
                    signal_reset()
                }
                onPressed: {
                    reset.opacity = 0.5
                }
                onReleased: {
                    reset.opacity = 1.0
                }
                onMousePositionChanged: {
                    if(mousPositionFlag >= 1)
                    {
                        mousPositionFlag= 0;
                        reset.opacity = 1.0
                        return ;
                    }
                    mousPositionFlag ++;
                }
            }
        }

        //分割线
        BaseQml.HorizonLine{
             anchors.right: parent.right
        }

    }

    function get_lang()
    {
        var tmp =SettingProperty.slot_getLanguage()
        if(tmp == 0)
        {
            return "中文（简体）"
        }
        else if(1 == tmp)
        {
            return "中文（繁體）"
        }
        else
        {
            return "English"

        }
    }



    //分割线
    BaseQml.HorizonLine{
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        visible: timeIsInit
    }


    Connections{
        target: SettingProperty
        onSignal_wifi_connStatChange:
        {
            wifi.variText = get_wifiStatu(SettingProperty.slot_getWifiConnStatu())
        }
        onSignal_update_firmware:
        {
            firmware.variText = qsTr("Update available")
            versionSign.visible  =true
        }
        onSignal_timeInit_success:
        {
            timeIsInit = true
        }

    }

    function get_wifiStatu(value)
    {
        switch(value)
        {
        case 0:
            return SettingProperty.slot_getCurConnSSid();
        case 1:
            return qsTr("Not Connected")
        case 2:
            return qsTr("Cannot connect to internet")
        case 3:
            return qsTr("Connecting")
        }

    }
    function get_versionInfo()
    {
        if(SettingProperty.slot_getCurVersion(false) == "")
        {
            versionSign.visible = false
            firmware.variText = SettingProperty.slot_getCurVersion(true)
        }
        else
        {
            versionSign.visible = true
            firmware.variText = qsTr("Update available")
        }
    }


    function get_localCity()
    {
        var city = SettingProperty.slot_getLocaltionCity()
        console.log("city = -------------------------:")
        console.log(city)
        if(city == "")
        {
            return qsTr("UnknownCity")
        }

        return city;
    }


    function get_nightmode_interval()
    {
        if(SettingProperty.slot_getAotoNight() == "true")
        {
            if(SettingProperty.slot_getSunRise() == "mode_sun")
            {
                return qsTr("Sunset to Sunrise")
            }
            else
            {
                return SettingProperty.slot_getNightInterver()
            }
        }
        else
        {
            return ""
        }
    }


}
