import QtQuick 1.1
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle{
    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_pmFrequency.png"
    property string blueEnterSource: "qrc:/resource/images/optionPage/button/button_enter_blue.png"
    height: 480
    width: 854
    color:"#292a2c"
    property string fontFamily: "MI LANTING"
    property string whiteColor: "#ffffff"

    BaseQml.FrameTop
    {
        id:frameTop
        iconSource: sourceIcon
        topTitle: qsTr("PM2.5 Detecting Frequency")
    }

    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }

    //双击设备顶部触发检测
    Item{
        id:doubleTapItem
        height: 94
        width: 854
        anchors{left: parent.left;leftMargin: 0;top:frameTop.bottom;topMargin: 0}

        Text {
            anchors{left: parent.left;leftMargin: 40;verticalCenter: parent.verticalCenter}
            text: qsTr("Double Tap the Top of the Device to Detect PM2.5")

            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color: whiteColor
        }

        BaseQml.Switch {
            id:checkingSwitch
            anchors.right:parent.right
            anchors.rightMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            on:SettingProperty.slot_getDoubleTapChecking()
            onClicked:
            {
                SettingProperty.slot_setDoubleTapChecking(flag);
            }
        }

        BaseQml.HorizonLine{
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 774
        }
    }


    Item{
        id:autoDetectingItem
        height: 94
        width: 854
        anchors{left: parent.left;leftMargin: 0;top:doubleTapItem.bottom;topMargin: 0}

        Text {
            text: qsTr("Auto Detecting")

            //字体
            font.family:fontFamily
            font.pixelSize: 24
            //加粗
            font.bold: false
            color:"#80ffffff"
            anchors{
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 20
            }
        }
        BaseQml.HorizonLine{
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 774
        }
    }


    //连接电源时
    Item
    {
        id:powerAdaptorItem
        height: 94
        width: 854
        anchors{left: parent.left;leftMargin: 0;top:autoDetectingItem.bottom;topMargin: 0}
        Text {
            anchors{left: parent.left;leftMargin: 40;verticalCenter: parent.verticalCenter}
            text: qsTr("Power Adaptor")

            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color: whiteColor
        }

        Image {
            id:powerAdaptorImage
            anchors{right:parent.right;rightMargin: 0;verticalCenter: parent.verticalCenter}
            source: blueEnterSource
        }

        Text {
            id: powerAdaptorText
            text: get_pmInterval(0)
            anchors{right:powerAdaptorImage.left;rightMargin: 0;verticalCenter: parent.verticalCenter}
            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color:"#80ffffff"
        }
    }

    //横线
    BaseQml.HorizonLine{
        anchors.top:powerAdaptorItem.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 774
    }
    //使用电池时
    Item
    {
        id:batteryItem
        height: 94
        width: 854
        anchors{left: parent.left;leftMargin: 0;top:powerAdaptorItem.bottom;topMargin: 0}
        Text {
            anchors{left: parent.left;leftMargin: 40;verticalCenter: parent.verticalCenter}
            text: qsTr("Battery")

            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color: whiteColor
        }

        Image {
            id:batteryImage
            anchors{right:parent.right;rightMargin: 0;verticalCenter: parent.verticalCenter}
            source: blueEnterSource
        }

        Text {
            id: batteryText
            text: get_pmInterval(1)
            anchors{right:batteryImage.left;rightMargin: 0;verticalCenter: parent.verticalCenter}

            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color:"#80ffffff"
        }
    }
    //横线
    BaseQml.HorizonLine{
        anchors.top:batteryItem.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 774
    }

    MouseArea{
        anchors.fill: backButton
        onClicked: {
            SettingProperty.slot_setQMl(0)
        }
        onPressed: {
            backButton.opacity = 0.5
        }
        onReleased: {
            backButton.opacity = 1.0
        }
    }

    Connections{
        target: SettingProperty
        onSignal_back:
        {
            SettingProperty.slot_setQMl(0)
        }
    }

    function get_pmInterval(type)
    {
        var value = SettingProperty.slot_getPmDetectingFrequency(type);
        if("5" == value)
        {
            return qsTr("5 mins")
        }
        else if("15" == value)
        {
            return qsTr("15 mins")
        }
        else if("30" == value)
        {
            return qsTr("30 mins")
        }
        else
        {
            return qsTr("Never")
        }
    }

    MouseArea{
        anchors.fill: powerAdaptorItem
        onClicked: {
            SettingProperty.slot_setPmDetetingSettingType(0)
            SettingProperty.slot_setQMl(22)
        }
    }
    MouseArea{
        anchors.fill: batteryItem
        onClicked: {
            SettingProperty.slot_setPmDetetingSettingType(1)
            SettingProperty.slot_setQMl(22)
        }
    }
}
