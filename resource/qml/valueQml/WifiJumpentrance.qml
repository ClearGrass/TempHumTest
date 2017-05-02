import QtQuick 1.1
import MainView 1.0

Item{
    id:wifi
    width: 385
    height: 386
    Image {
        id:img
        source:"qrc:/resource/images/mainPage/backgroud/wifiJumpentrance.png";
    }
    Text {
        anchors.top: parent.top
        anchors.topMargin: 184
        anchors.horizontalCenter: parent.horizontalCenter
        id: connect
        text: qsTr("Connect to the Cloud")
        font.pointSize: 30
        font.family: "DINCondensedC"
        color : "#FFFFFF"
    }

    MouseArea{
        anchors.fill: wifi
        onClicked: {
            MainViewProperty.slot_showWifiPage()
        }
        onPressed: {
            connect.opacity = 0.5
        }
        onReleased: {
            connect.opacity = 1.0
        }
    }
    function set_opacity(value)
    {
        connect.opacity = value
    }

}

