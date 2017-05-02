import QtQuick 1.1
Rectangle {
    property string backbottonSource: "qrc:resource/images/poweroff/closeButton.png"
    property string transColor: "#00000000"

    id: backButton
    height: 76
    width: 150
    color: transColor
    anchors.top: parent.top
    anchors.topMargin: 0
    anchors.left: parent.left
    anchors.leftMargin: 0
    z:10
    Image {
        source: backbottonSource
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 24
        anchors.leftMargin:  30
    }


}
