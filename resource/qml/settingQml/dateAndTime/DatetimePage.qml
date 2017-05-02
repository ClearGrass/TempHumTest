import QtQuick 1.1
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle
{
    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_datetime.png"

    //页面宽度和高度
    property int itemHeight: 480
    property int itemWidth:  854
    property string whiteColor: "#ffffff"
    property string blueColor: "#00E5FF"
    property string fontFamily: "MI LANTING"
    property string blueEnterSource: "qrc:/resource/images/optionPage/button/button_enter_blue.png"

    property bool is24Hours: SettingProperty.slot_get24Hours()
    property bool  isAutoGetTime: SettingProperty.slot_getAutoGetTime()
    height: itemHeight
    width:  itemWidth
    color:"#292a2c"
    z:9
    //横向线条
    BaseQml.FrameTop
    {
        id:frameTop
        iconSource: sourceIcon
        topTitle: qsTr("Date and Time")
    }

    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }

    DateTime{
        anchors.centerIn: parent
        id:datetimeWidget
        visible: false
        onSignal_ok: {
            datetimeText.text =  SettingProperty.slot_getDatetime(datetimeWidget.strSetDatetime)
        }
    }

    //24小时
    Item
    {
        id:hourMode
        height: 94
        width: 854
        anchors.left: parent.left
        anchors.top:frameTop.bottom
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            text: qsTr("24-hour Time")
            color: whiteColor

            //字体
            font.family: fontFamily
            font.pixelSize: 26
        }



        BaseQml.Switch {
            id:hourSwitchImage
            anchors.right:parent.right
            anchors.rightMargin: 40
            anchors.verticalCenter: parent.verticalCenter
            on:is24Hours
            onClicked:
            {
                SettingProperty.slot_set24Hours(flag);
                datetimeText.text = SettingProperty.slot_getCurDateTime()
            }

        }
        BaseQml.HorizonLine{
            width: 774
            anchors.horizontalCenter:parent.horizontalCenter
            anchors.bottom: parent.bottom
        }

    }

    //自动获取时间
    Item
    {
        id:autoGetTime
        height: 94
        width: 854
        anchors.left: parent.left
        anchors.top:hourMode.bottom
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            text: qsTr("Set Time Automatically")
            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color: whiteColor

        }


        BaseQml.Switch {
            id:autoSwitchImage
            anchors.right:parent.right
            anchors.rightMargin: 40
            anchors.verticalCenter: parent.verticalCenter
            on:isAutoGetTime
            Connections{
                target: autoSwitchImage
                onClicked:
                {
                    datetimeItem.visible = !flag
                    datetimeMousarea.visible = !flag

                    SettingProperty.slot_setAutoGetTime(flag)
                    mouseAreaTimezone.visible  = !flag
                    timezoneImage.visible = !flag
                    if(true == flag)
                    {
                        var str = SettingProperty.slot_getAutoTimezone()
                        if(str != "" && str != timezoneText.text)
                        {
                            timezoneText.text = str
                        }

                    }
                }
            }

        }
        BaseQml.HorizonLine{
            width: 774
            anchors.horizontalCenter:parent.horizontalCenter
            anchors.bottom: parent.bottom
        }
    }


    //时区
    Item
    {
        id:timezone
        height: 94
        width: 854
        anchors.left: parent.left
        anchors.top:autoGetTime.bottom
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            text: qsTr("Time Zone")
            //字体
            font.family: "MI LANTING"
            font.pixelSize: 24
            color: whiteColor

        }



        Image {
            id:timezoneImage
            anchors.right:parent.right
            anchors.rightMargin: 0
            anchors.verticalCenter: parent.verticalCenter
            source: blueEnterSource
        }

        Text {
            id:timezoneText
            anchors.right: timezoneImage.left
            anchors.verticalCenter: parent.verticalCenter
            text: SettingProperty.slot_getTimeZone()

            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color:"#80ffffff"
        }
        BaseQml.HorizonLine{
            width: 774
            anchors.horizontalCenter:parent.horizontalCenter
            anchors.bottom: parent.bottom
        }
    }

    //日期时间
    Item
    {
        id:datetimeItem
        height: 94
        width: 854
        anchors.left: parent.left
        anchors.top:timezone.bottom
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            text: qsTr("Date and Time")
            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color: whiteColor

        }



        Image {
            id:datetimeImage
            anchors.right:parent.right
            anchors.rightMargin: 0
            anchors.verticalCenter: parent.verticalCenter
            source: blueEnterSource
        }

        Text {
            id:datetimeText
            anchors.right: datetimeImage.left
            anchors.verticalCenter: parent.verticalCenter
            text: SettingProperty.slot_getCurDateTime()

            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color: "#80ffffff"

        }
    }

    MouseArea{
        anchors.fill: backButton
        onClicked:
        {
            SettingProperty.slot_setQMl(0)

        }
        onPressed: {
            backButton.opacity = 0.5
        }
        onReleased: {
            backButton.opacity = 1.0
        }
    }


    MouseArea{
        id:mouseAreaTimezone
        anchors.fill: timezone
        onClicked: {
            SettingProperty.slot_setQMl(8)

        }
    }
    MouseArea{
        id:datetimeMousarea
        anchors.fill: datetimeItem
        visible: true
        onClicked: {
            datetimeWidget.initFunction()
            datetimeWidget.visible   = true
        }
    }
    Connections{
        target: SettingProperty
        onSignal_initTimeDate:
        {
            if(isAutoGetTime)
            {
                datetimeItem.visible = false
                datetimeMousarea.visible = false
                mouseAreaTimezone.visible  = false
                timezoneImage.visible = false
            }
            else{

                datetimeItem.visible = true
                datetimeMousarea.visible = true
                mouseAreaTimezone.visible  = true
                timezoneImage.visible = true
            }
        }
        onSignal_back:
        {
            if(datetimeWidget.visible)
            {
                datetimeWidget.visible   = false
                return ;
            }
            if(SettingProperty.slot_getCurPageIndex() == 7)
                SettingProperty.slot_setQMl(0)
        }
    }



}

