import QtQuick 1.1
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle
{
    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_reset.png"
    property string sourceButton: "qrc:resource/images/optionPage/button/button_red.png"
    property bool wifiIsForget: false
    //页面宽度和高度
    property int itemHeight: 480
    property int itemWidth:  854
    property string whiteColor: "#ffffff"

    height: itemHeight
    width:  itemWidth
    color:"#00000000"
    z:22
    id:resetpage

    Rectangle{
        id:waittingRect
        height: parent.height
        width: parent.width
        z:24
        color:"#292a2c"
        visible: false
        BaseQml.Waitting{
            z:24
            anchors.centerIn: parent
            run:parent.visible
        }
    }

    Rectangle{
        z:23
        id:waitRect
        height: itemHeight
        width:  itemWidth
        color:"#292a2c"
        anchors.centerIn: parent
        visible: false
        BaseQml.Waitting{
            id:waitAnimation
            anchors.centerIn: parent
            run:false
        }

    }

    BaseQml.Messagebox{
        z:23
        id:box1
        visible: !box2.visible
        anchors.centerIn: parent
        rightText: qsTr("Yes")
        leftText:  qsTr("Cancel")
        Text {
            z:23
            id:text1
            text: qsTr("Restore to factory settings now?")
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: "#cc010101"

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 109
        }

        Text {
            z:23
            id: tex
            text: qsTr("After restore to factory settings, all the user data will be erased.")

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: text1.bottom
            anchors.topMargin: 28
            //字体
            font.family: "MI LANTING"
            font.pixelSize: 26

            color: "#ab010101"
        }

        //取消
        onSignal_leftButtonOnClicked: {
            resetpage.visible = false
        }


        //确定
        onSignal_rightButtonOnClicked: {
            box2.visible = true
        }
    }

    BaseQml.Messagebox{
        z:23
        id:box2
        visible: false
        anchors.centerIn: parent
        rightText: qsTr("Confirm")
        leftText:  qsTr("Cancel")
        leftButtonBold: true

        Item{
            height: 292
            width: parent.width
            z:23
            anchors.left: parent.left
            anchors.top: parent.top
            Text {
                width: 500
                anchors.centerIn: parent
                text: qsTr("Please confirm that you want to restore to factory settings.")
                horizontalAlignment: Text.AlignHCenter
                //字体
                font.family: "MI LANTING"
                font.pixelSize: 26
                wrapMode: Text.Wrap
                color: "#ab010101"
            }
        }



        //取消
        onSignal_leftButtonOnClicked: {
            box2.visible = false
            resetpage.visible = false
        }


        //确定
        onSignal_rightButtonOnClicked: {
            waitRect.visible = true
            waitAnimation.startAnimation()
            wifiIsForget = true
            waittingRect.visible = true
            timer.start()
            SettingProperty.slot_reset()
        }
    }


    Timer{
        interval: 3000;
        repeat: true;
        running: false;
        id:timer
        onTriggered: {
            slot_timeout();
        }

    }


    function slot_timeout()
    {
        if(wifiIsForget == false)
            SettingProperty.slot_restart()
    }

    Connections{
        target: SettingProperty

        onSignal_showWifiList:{
            if(wifiIsForget == true)
            {
                wifiIsForget = false
            }
        }
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }
}
