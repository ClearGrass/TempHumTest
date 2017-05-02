import QtQuick 1.1

Rectangle{
    property string errorSource: "qrc:/resource/images/optionPage/label/fail.png"
    property string whiteColor: "#ffffff"

    height: 380
    width: 854
    anchors.top: parent.top
    anchors.topMargin: 100
    anchors.left: parent.left
    color:"#292a2c"
    z:30

    Column{
        anchors.centerIn: parent
        spacing: 20
        Image {
            source: errorSource
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            id:tipText
            text: qsTr("Wi-Fi not connected")
            horizontalAlignment: Text.AlignHCenter
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: whiteColor
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }


}
