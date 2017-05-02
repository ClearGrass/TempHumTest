import QtQuick 1.1
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle
{
    property string sourceicon: "qrc:resource/images/optionPage/icon/icon_night.png"
    property string sourceSelected: "qrc:/resource/images/optionPage/label/label_selected.png"
    property string sourceEnter: "qrc:/resource/images/optionPage/button/buttonEnter.png"
    property int nightMode: 2
    property string autoNightMode: SettingProperty.slot_getAotoNight() == "true" ? true : false
    property int  curNightmode: SettingProperty.slot_getNightMode()
    property string sunrise: ""
    //页面宽度和高度
    property int itemHeight: 480
    property int itemWidth:  854
    property string whiteColor: "#ffffff"
    property string blueColor:  "#33b4ff"
    property string fontFamily: "MI LANTING"
    property string  sun_init: "true"
    height: itemHeight
    width:  itemWidth
    color:"#292a2c"

    //横向线条
    BaseQml.FrameTop
    {
        id:frameTop
        iconSource: sourceicon
        topTitle: qsTr("Night Mode")
    }

    //返回那妞
    BaseQml.BackButton{

        id:backButton
    }

    //手动开启夜间模式
    Item
    {
        id:openNightModeItem
        height: 94
        width: 854
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top:frameTop.bottom

        Text {
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            text: curNightmode == 1 ? qsTr("Quit Night Mode Now") :qsTr("Enter Night Mode Now")
            color: whiteColor

            //字体
            font.family: fontFamily
            font.pixelSize: 26

        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                SettingProperty.slot_setNightMode(curNightmode == 1 ? 2 : 1)
                SettingProperty.slot_setQMl(0)
            }
        }

        BaseQml.HorizonLine{
            width: 774
            anchors.horizontalCenter:parent.horizontalCenter
            anchors.bottom: parent.bottom
        }
    }

    //自动调节亮度
    Item
    {
        id:autoLight
        height: 94
        width: 854
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top:openNightModeItem.bottom
        anchors.topMargin: 0
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            text: qsTr("Scheduled")
            color: whiteColor

            //字体
            font.family: fontFamily
            font.pixelSize: 26

        }

        BaseQml.Switch {
            id:autoLightSwitchImage
            anchors.right:parent.right
            anchors.rightMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            on:autoNightMode
            onClicked:
            {
                sunriseTosundown.visible = flag
                setInterval.visible = flag
                setInterValMousearea.visible = flag
                SettingProperty.slot_setAotoNight(flag)
            }
        }
        BaseQml.HorizonLine{
            width: 774
            anchors.horizontalCenter:parent.horizontalCenter
            anchors.bottom: parent.bottom
        }

    }

    //从日出到日落
    Item
    {
        id:sunriseTosundown
        height: 94
        width: 854
        anchors.left: parent.left
        anchors.top:autoLight.bottom
        visible: false
        opacity: sun_init == "true" ? 1.0 : 0.0
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            text: qsTr("Sunset to Sunrise")
            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color: sunraiseImage.visible ? blueColor : whiteColor

        }


        Image {
            id:sunraiseImage
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            source: sourceSelected
            visible: false

        }
        MouseArea{
            anchors.fill: sunriseTosundown
            onClicked: {
                SettingProperty.slot_setSunRise("true");
                sunraiseImage.visible = true
                customImage.visible = false
            }
        }
        BaseQml.HorizonLine{
            width: 774
            anchors.horizontalCenter:parent.horizontalCenter
            anchors.bottom: parent.bottom
        }

    }

    //指定开启时段
    Item
    {
        id:setInterval
        height: 94
        width: 854
        anchors.left: parent.left
        anchors.top:sunriseTosundown.bottom
        visible: false
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            text: qsTr("Custom")
            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color:  customImage.visible ? blueColor  :whiteColor

        }


        Image {
            id: customImage
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            source: sourceSelected
            visible: false

        }

        Image {
            id: imageChangeInterval
            source: sourceEnter
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
        }
        Text {
            id: customText
            anchors.right: imageChangeInterval.left
            anchors.verticalCenter: parent.verticalCenter
            text: SettingProperty.slot_getNightInterver()

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
        anchors.fill:setInterval
        id:setInterValMousearea
        visible: true
        onClicked: {
            if( customImage.visible == true)
            {
                SettingProperty.slot_setQMl(5)
            }
            else
            {
                sunraiseImage.visible = false
                 customImage.visible = true
                SettingProperty.slot_setSunRise("false");
            }
        }
        onPressed: {
            if( customImage.visible == true)
                 customText.opacity = 0.5
        }
        onReleased: {
            if( customImage.visible == true)
                 customText.opacity = 1.0
        }

    }

    Connections{
        target: SettingProperty
        onSignal_nightInit:
        {
            nightMode  = SettingProperty.slot_getNightMode();
            console.log(nightMode)


            autoNightMode = SettingProperty.slot_getAotoNight()
            if("true" == autoNightMode)
            {
                sunriseTosundown.visible = true
                setInterval.visible = true
                setInterValMousearea.visible = true

            }

            sunrise = SettingProperty.slot_getSunRise();
            if("mode_sun" == sunrise)
            {
                sunraiseImage.visible = true
                 customImage.visible = false
            }
            else
            {
                sunraiseImage.visible = false
                 customImage.visible = true
            }

        }
        onSignal_back:
        {
            if(SettingProperty.slot_getCurPageIndex() == 4)
                SettingProperty.slot_setQMl(0)

        }
    }

}
