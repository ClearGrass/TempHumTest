import QtQuick 1.1

Rectangle {
    width: 854
    height: 480
    color: "#212121"


    Text
    {
        text: "Test done"
        color: "#ffffff"

        //字体
        font.family: "KMedium"
        font.pixelSize: 36
        anchors.centerIn: parent

    }

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }
}
