import QtQuick 1.1
import MainView 1.0
import "../toolQml" as ToolQml


Item {
    id:thisItem
    property string backSource: "qrc:/resource/images/verticalScreen/waringBack.png"
    height: 854
    width: 480
    z:21
    Image {
        anchors.fill: parent
        source:backSource
    }


    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 60
        anchors.horizontalCenter: parent.horizontalCenter
        id: des
        width:  420

        wrapMode: Text.Wrap
        color:"#ffffff"
        font.family: "MI LANTING"
        font.pixelSize: 24
        horizontalAlignment: Text.AlignHCenter
        text:qsTr("Placing the device vertically on a flat surface will block the ventilation holes")
    }

    Text {
        anchors.bottom: des.top
        anchors.bottomMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter

        color:"#ffffff"
        font.family: "MI LANTING"
        font.pixelSize: 30
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Please place the air monitor horizontally")
    }


    Connections{
        target: MainViewProperty
        onSignal_device_horizontalPrompt:
        {
            thisItem.visible = true
        }
        onSignal_device_pickUp:
        {
            thisItem.visible = false
        }
    }

}
