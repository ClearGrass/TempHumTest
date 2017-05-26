import QtQuick 1.1

Rectangle{
    signal signal_clicked()
    property alias text: keyText.text
    height: 100
    width: 100
    radius: 100
    smooth: true
    color: "#ffffff"
    opacity: 0.5

    Text {
        id: keyText
        anchors.centerIn: parent
        font.family: "MI LANTING"
        font.pixelSize: 28
        color: "#000000"
    }
    MouseArea{
        anchors.fill: parent
        onClicked: {
            signal_clicked()
        }
        onPressed: {
            parent.opacity = 0.4
        }
        onReleased: {
            parent.opacity = 0.5
        }
    }
}


