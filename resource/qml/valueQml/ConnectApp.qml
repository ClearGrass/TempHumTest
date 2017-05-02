import QtQuick 1.1
import MainView 1.0

Item{
    width: 385
    height: 386
    Image {
        id:img
        source:"qrc:/resource/images/mainPage/backgroud/connectApp.png";
    }

    Text {
        anchors.top: parent.top
        anchors.topMargin: 194
        anchors.horizontalCenter: parent.horizontalCenter
        id: app
        text: qsTr("ConnectMiHomeApp")
        font.pointSize: 30
        font.family: "DINCondensedC"
        color : "#FFFFFF"
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
                MainViewProperty.slot_showMiAppPage()
        }
        onPressed: {
            app.opacity = 0.5
        }
        onReleased: {
            app.opacity = 1.0
        }
    }
    function set_opacity(value)
    {
        app.opacity = value
    }
}
