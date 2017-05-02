import QtQuick 1.1
import MainView 1.0

Rectangle {
    height: 480
    width: 854
    color: "#212121"
    property bool pmisOn: false
    Column{
        anchors.centerIn: parent

        spacing: 3
        Battery{
            id:battery
        }

        HumitureBlock{

        }

        ProgressBar{
            id:light
            logoSource: "qrc:/resource/images/test/light.png"
            value:MainViewProperty.slot_getCurLightValue()
            onValueChanged:
            {
                MainViewProperty.slot_setLightValue(light.value)
            }
        }

        CpuItem{

        }

        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 4
            BaseButtonPlus{
                id:pm25
                logoSource:"qrc:/resource/images/test/pm25.png"
                maintext: pmisOn ? "PM2.5 On" : "PM2.5 Off"
                onClick:
                {
                   if(pmisOn)
                   {
                       pmisOn = false
                       MainViewProperty.slot_pmOff()
                       pm25.maintext = "PM2.5 Off"
                   }
                   else
                   {
                       pmisOn = true
                       MainViewProperty.slot_pmOn()
                       pm25.maintext = "PM2.5 On"

                   }
                }

            }
            BaseButton{
                id:wifibutton
                logoSource:"qrc:/resource/images/test/wifiEnable.png"
                maintext: "Wi-Fi"
                subText: "Disconnect"
                onClick: {
                    MainViewProperty.slot_showWifiPage()

                }
            }

//            BaseButton{
//                id:screen
//                logoSource:"qrc:/resource/images/test/screen.png"
//                maintext: "Screen Off"
//                subText: "00:00:00"
//                onClick: {
//                    MainViewProperty.slot_screenOff();
//                }
//            }
        }

    }




    Connections {

        target:MainViewProperty
        onSignal_pmDetected:
        {
//            pm25.stop_ani()
        }
        onSignal_startPmAnimation:
        {
//            pm25.start_ani()
        }
        onSignal_wifiIntensity:
        {
            wifibutton.subText = MainViewProperty.slot_getWifiStatus()
        }
    }

    Timer{
        interval: 1000;
        repeat: true;
        triggeredOnStart: true;
        running: true;

        onTriggered: {
            slot_timeout();
        }

    }

    function slot_timeout()
    {
        battery.levelWidth = 28*(MainViewProperty.slot_getBatteryCapacity()/100.0);
        battery.percent = MainViewProperty.slot_getBatteryCapacity()
        battery.isCharging = MainViewProperty.slot_getBatteryStatusIsCharging()
        if(!battery.isCharging)
        {
            battery.powerColor = "#ffffff"

        }
        else
        {
            battery.powerColor = "#24ff00";

        }
        battery.currentelectricity =  MainViewProperty.slot_getBatteryCurrentelectricity()

        battery.timestring = MainViewProperty.slot_getBatteryTime()

    }

}
