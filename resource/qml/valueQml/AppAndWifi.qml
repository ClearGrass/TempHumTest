import QtQuick 1.1
import MainView 1.0

Item
{
    property string wifi: "qrc:/resource/images/mainPage/icon/wifi.png"
    property string mijia: "qrc:/resource/images/mainPage/icon/mijia.png"
    property string setting: "qrc:/resource/images/mainPage/icon/setting.png"
    property string wifiPressed: "qrc:/resource/images/mainPage/icon/wifiPressed.png"
    property string mijiaPressed: "qrc:/resource/images/mainPage/icon/mijiaPressed.png"
    property string settingPressed: "qrc:/resource/images/mainPage/icon/settingPressed.png"
    property bool appIsBind: MainViewProperty.slot_getAppBindStatus()
    property int layoutMarigin: appIsBind ? 238 :101
    height: 480
    width: 843

    Image {
        id: wifiimage
        source: wifi
        anchors.top: parent.top
        anchors.topMargin: 166
        anchors.left: parent.left
        anchors.leftMargin: layoutMarigin


        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.bottom
            anchors.topMargin: 20

            id: wifi_ssid
            text: "Wi-Fi"
            //字体
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: "#ffffff"
        }


        MouseArea{
            anchors.fill: parent
            onClicked: {
                MainViewProperty.slot_showWifiPage()
            }
            onPressed: {
                wifiimage.source = wifiPressed
            }
            onReleased: {
                wifiimage.source = wifi
            }
        }

    }


    Image {
        id: mijiaImage
        source: mijia
        visible: !appIsBind
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 166

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.bottom
            anchors.topMargin: 20

            id: mijia_id
            text: qsTr("Connect to Mi Home")
            //字体
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: "#ffffff"
        }


        MouseArea{
            anchors.fill: parent
            onClicked: {
                MainViewProperty.slot_showMiAppPage()
            }
            onPressed: {
                mijiaImage.source = mijiaPressed
            }
            onReleased: {
                mijiaImage.source = mijia
            }
        }
    }


    Image {
        id: settingImage
        source: setting

        anchors.top: parent.top
        anchors.topMargin: 166
        anchors.right: parent.right
        anchors.rightMargin: layoutMarigin

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.bottom
            anchors.topMargin: 20

            text: qsTr("Settings")
            //字体
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: "#ffffff"
        }


        MouseArea {
            id:mouseRegion
            height: 200
            width: 200
            anchors.centerIn: parent
            onClicked: MainViewProperty.show_optionPage();
            onPressed: {
                settingImage.source = settingPressed
            }
            onReleased: {
                settingImage.source = setting
            }
        }
    }

    Connections {

        target:MainViewProperty
        onSignal_wifiIntensity:
        {
            var strSsid = MainViewProperty.slot_getWifissid()
            if(strSsid == "")
            {
                wifi_ssid.text = "Wi-Fi"
            }
            else
            {
                wifi_ssid.text = strSsid

            }
        }
        onSignal_updateAppBindstat:
        {
            appIsBind = MainViewProperty.slot_getAppBindStatus()
            settingImage.visible = false
            wifiimage.visible = false
            if(appIsBind)
            {
                layoutMarigin = 238
            }
            else
            {
                layoutMarigin = 101
            }
            settingImage.visible = true
            wifiimage.visible = true
        }
    }

}
