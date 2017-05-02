import QtQuick 1.1
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle
{
    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_light.png"
    property string sourcelight1: "qrc:resource/images/optionPage/label/label_light1.png"
    property string sourcelight2: "qrc:resource/images/optionPage/label/label_light2.png"
    property string sourcesliderRelease: "qrc:/resource/images/optionPage/tools/slider_release.png"
    property string sourcesliderPress: "qrc:/resource/images/optionPage/tools/slider_press.png"

    property string blueEnterSource: "qrc:/resource/images/optionPage/button/button_enter_blue.png"

    property string whiteColor: "#ffffff"
    property int lightValue: 0
    property string fontFamily: "MI LANTING"
    signal signal_updateScroll()
    //页面宽度和高度
    height: 564
    width:  854
    color:"#292a2c"
    z:11


    Item{
        id:brightnessitem
        height: 94
        width: 854
        anchors{
            top:parent.top
            left: parent.left
            topMargin: 0
        }

        Text {
            text: qsTr("Brightness")

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
    }
    BaseQml.HorizonLine{
        anchors.top:brightnessitem.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 774
    }

    Item{
        height: 94
        width: 854
        id:progress
        anchors.top: brightnessitem.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        Image {
            id: light1
            source: sourcelight1
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter
        }


        Image {
            id: light2
            source: sourcelight2
            anchors.right: parent.right
            anchors.rightMargin: 40
            anchors.verticalCenter: parent.verticalCenter
        }

        Rectangle{
            id:sliderBack
            height: 6
            width: 660
            radius: 3
            color: "#494A4B"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -13

            Rectangle{
                id:sliderRect
                height: parent.height
                radius: parent.radius
                color: "#B6B6B7"
                width: slider.x +25
            }

            Image
            {
                id:slider
                x:-25
                y:-20
                source: sourcesliderRelease
            }
            MouseArea{
                height: 94
                width: 94
                id:sliderMousarea
                anchors.centerIn: slider
                drag.target: slider;
                drag.axis: Drag.XAxis;
                drag.minimumX: -25;
                drag.maximumX: 635;
                onMouseXChanged:   {
                    if(hourSwitchImage.on == false)
                    {
                        var tmp = Math.round((slider.x+25)/660 *100);
                        if(tmp != lightValue)
                        {
                            lightValue = tmp
                            console.log(tmp)
                            SettingProperty.slot_setLightValue(tmp)
                        }
                    }
                }
                onPressed: {
                    slider.source = sourcesliderPress
                }
                onReleased: {
                    slider.source = sourcesliderRelease
                }

            }

        }

    }
    BaseQml.HorizonLine{
        anchors.top:progress.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 774
    }

    Rectangle{
        height: 94
        width: 854
        anchors.top: progress.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        color: "#00000000"
        id:autoAdjust
        Text {
            text: qsTr("Auto-adjust")
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 40

            //字体
            font.family: fontFamily
            font.pixelSize: 26

            //加粗
            font.bold: false
            color:whiteColor
        }


        BaseQml.Switch {
            id:hourSwitchImage
            anchors.right:parent.right
            anchors.rightMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            on:SettingProperty.slot_isAuto()
            onClicked:
            {
                sliderMousarea.visible =!flag
                SettingProperty.slot_setAutoChangeLight(flag);
            }
        }
    }
    BaseQml.HorizonLine{
        anchors.top:autoAdjust.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 774
    }

    Item{
        height: 94
        width: 600
        anchors{
            right:brightnessitem.right
            top:brightnessitem.top
            rightMargin: 0
            topMargin: 0
        }

        Text {
            //字体
            font.family: fontFamily
            font.pixelSize: 26

            color: whiteColor
            id: outValue
            text: qsTr("外界亮度:")
            anchors.left: parent.left
            anchors.leftMargin: 60
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            //字体
            font.family: fontFamily
            font.pixelSize: 26

            color: whiteColor
            id: inValue
            text: qsTr("传感器设定亮度:")
            anchors.right:  parent.right
            anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
        }
    }



    Item{
        id:screenAutoOffItem
        height: 94
        width: 854
        anchors{ top:autoAdjust.bottom;left: parent.left;}

        Text {
            text: qsTr("Screen Auto-off")

            //字体
            font.family: fontFamily
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
    }
    //横线
    BaseQml.HorizonLine{
        anchors.top:screenAutoOffItem.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 774
    }

    //连接电源时
    Item
    {
        id:powerAdaptorItem
        height: 94
        width: 854
        anchors{left: parent.left;leftMargin: 0;top:screenAutoOffItem.bottom;topMargin: 0}
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
            text: get_srceenAutoInterval(0)
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
            text: get_srceenAutoInterval(1)
            anchors{right:batteryImage.left;rightMargin: 0;verticalCenter: parent.verticalCenter}

            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color:"#80ffffff"
        }
    }


    Connections{
        target: SettingProperty
        onSignal_autoChangeValue:
        {
            if(hourSwitchImage.on)
                slider.x = SettingProperty.slot_getLightValue() /100 *660 -25
        }
        onSignal_isAutoChange:
        {
            var tmp =SettingProperty.slot_isAuto();
            if(tmp == true)
            {
                sliderMousarea.visible =false
//                hourSwitchImage.setSwitchOn()
            }
            slider.x =  SettingProperty.slot_getLightValue() /100 *660 -25
        }
        onSignal_outLight:{
            outValue.text = qsTr("外界亮度：") + SettingProperty.slot_getOutOrInLightValue(1)
        }
        onSignal_inLight:
        {
            inValue.text = qsTr("传感器设定亮度：") + SettingProperty.slot_getOutOrInLightValue(2)
        }
    }

    function get_srceenAutoInterval(type)
    {
        var value = SettingProperty.slot_getScreenAutoInterval(type);
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
            signal_updateScroll()
            SettingProperty.slot_setScreenAutoIndex(0)
            SettingProperty.slot_setQMl(20)
        }
    }
    MouseArea{
        anchors.fill: batteryItem
        onClicked: {
            signal_updateScroll()
            SettingProperty.slot_setScreenAutoIndex(1)
            SettingProperty.slot_setQMl(20)
        }
    }

}
