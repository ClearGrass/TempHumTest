import QtQuick 1.1
import MainView 1.0

Item {
    property string wifiIntensity1Source: "qrc:/resource/images/mainPage/icon/wifi_1.png"
    property string wifiIntensity2Source: "qrc:/resource/images/mainPage/icon/wifi_2.png"
    property string wifiIntensity3Source: "qrc:/resource/images/mainPage/icon/wifi_3.png"
    property string wifiIntensity4Source: "qrc:/resource/images/mainPage/icon/wifi_4.png"
    property string wifiIntensity5Source: "qrc:/resource/images/mainPage/icon/wifi_5.png"
    property string  batteryBorder: "qrc:/resource/images/mainPage/icon/batteryBorder.png"
    property alias levelWidth: level.width
    property string powerColor: "#ffffff"
    property int wifilen: 1

    id: wifiAndPower
    height: 18
    width: 82
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.topMargin: 20
    anchors.leftMargin: 20
    Image {
        id: wifi
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        source: get_wifiIntensity()
        opacity: 1.0
    }
    Image {
        id: power
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        source: batteryBorder

        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 3
            anchors.leftMargin: 3
            id:level
            height: 10
            color: powerColor
            radius: 5
        }
    }

    Connections {

        target:MainViewProperty
        onSignal_wifiIntensity:
        {
            wifi.source = get_wifiIntensity()
        }
    }

    Timer{
        interval: 1100;
        repeat: true;
        triggeredOnStart: true;
        running: true;

        onTriggered: {
            slot_timeout();
        }

    }


    function slot_timeout()
    {
        levelWidth = 40*(MainViewProperty.slot_getBatteryCapacity()/100.0);

        if(!MainViewProperty.slot_getBatteryStatusIsCharging())
        {
            powerColor = "#ffffff"
        }
        else
        {
            powerColor = "#00e676";
        }
    }

    function get_wifiIntensity()
    {
        var wifiIntensity;
        switch(MainViewProperty.slot_getWifiIntensity())
        {
        case 1:
            wifiAnmation.stop()
            wifi.opacity = 1.0
            wifiIntensity =  wifiIntensity1Source;
            break;
        case 2:
            wifiAnmation.stop()
            wifi.opacity = 1.0
            wifiIntensity =  wifiIntensity2Source;
            break;
        case 3:
            wifiAnmation.stop()
            wifi.opacity = 1.0
            wifiIntensity =  wifiIntensity3Source;
            break;
        case 4:
            wifiAnmation.stop()
            wifi.opacity = 1.0
            wifiIntensity =  wifiIntensity4Source;
            break;
        case -2:
            //正在连接wifi 显示动画
            wifiAnmation.start()
            wifi.opacity = 1.0
            break;
        case -3:
            //连接不可用
            wifiAnmation.stop()
            wifi.opacity = 1.0
            wifiIntensity =  wifiIntensity5Source;
            break;
        default:
            wifiAnmation.stop()
            wifi.opacity = 0.2
            wifiIntensity =  wifiIntensity4Source;
            break;
        }
        return wifiIntensity;
    }


    //wifi正在连接动画
    Timer{
        id:wifiAnmation
        interval: 200;
        repeat: true;
        triggeredOnStart: true;
        running: false;

        onTriggered: {
            slot_wifiTimeout();
        }
    }
    function slot_wifiTimeout()
    {
        if(wifilen == 1)
        {
            wifi.source = wifiIntensity1Source;
        }
        else if(2 == wifilen)
        {
            wifi.source = wifiIntensity2Source;

        }
        else if(3 == wifilen)
        {
            wifi.source = wifiIntensity3Source;

        }
        else if(4 == wifilen)
        {
            wifi.source = wifiIntensity4Source;

        }

        wifilen += 1
        if(wifilen == 5)
        {
            wifilen = 1
        }
    }

}
