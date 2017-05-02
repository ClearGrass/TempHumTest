import QtQuick 1.0

Rectangle {
    id: powerSwitch

    width: 854; height: 480
    color: "#000000"
    opacity: 0.95
    z:0

    property string whiteColor: "#ffffff"
    property string poweroffSource: "qrc:/resource/images/poweroff/poweroff.png"
    property string poweroffPressedSource: "qrc:/resource/images/poweroff/poweroffPressed.png"
    property string screenoffSource: "qrc:/resource/images/poweroff/screenoff.png"
    property string screenoffPressedSource: "qrc:/resource/images/poweroff/screenoffPressed.png"

    Image {
        z:1
        id: closeScreenImage
        source: screenoffSource
        anchors.left: parent.left
        anchors.leftMargin: 238
        anchors.verticalCenter: parent.verticalCenter

        Text {
            id:screenoffText
            text: qsTr("Screen Off")
            font.family: "MI LANTING"
            font.pixelSize: 24
            color: whiteColor

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.bottom
            anchors.topMargin: 20
        }


        MouseArea{
            anchors.fill: parent
            onClicked: {

                //关闭屏幕
                PagePowerOff.slot_screen_off();
                timerStop_func()
            }
            onPressed:
            {
                closeScreenImage.source = screenoffPressedSource
            }
            onReleased: {
                closeScreenImage.source = screenoffSource
            }
        }
    }

    Image {
        z:1
        id: poweroffImage
        source: poweroffSource
        anchors.right: parent.right
        anchors.rightMargin: 238
        anchors.verticalCenter: parent.verticalCenter

        Text {
            id:powerOffText
            text: qsTr("Power Off")
            font.family: "MI LANTING"
            font.pixelSize: 24
            color: whiteColor
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.bottom
            anchors.topMargin: 20
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                PagePowerOff.slot_power_off();
                timerStop_func()
            }
            onPressed:
            {
                poweroffImage.source = poweroffPressedSource
            }
            onReleased: {
                poweroffImage.source = poweroffSource
            }
        }
    }


    Connections{
        target: PagePowerOff
        onSignal_horrizonScreen:
        {
            console.log("onSignal_horrizonScreen")
            closeScreenImage.rotation = 0;
            poweroffImage.rotation = 0;
        }
        onSignal_vertialScreen:
        {
            console.log("onSignal_vertialScreen")

            closeScreenImage.rotation = -90;
            poweroffImage.rotation = -90;
        }
        onSignal_startTimer:
        {
//            timer_off.start()
        }

    }

    //时间定时器 一秒执行一次
    Timer{
        id:timer_off
        interval: 500;
        repeat: true;

        triggeredOnStart: false;
        running: false;

        onTriggered: {
            timeout_func();
        }

    }

    function timeout_func()
    {


    }

    function timerStop_func()
    {
        timer_off.stop()
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            PagePowerOff.slot_cancel();
        }
    }
}


