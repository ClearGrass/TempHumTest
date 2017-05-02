
import QtQuick 1.0

Rectangle{

    property string iconSource: "qrc:resource/images/optionPage/logo/logo_wifi.png"
    property string nameText: ""
    id:rootRect

    width: 427
    height: 126
    color: "#00000000"
    property string whiteColor: "#ffffff"
    Image {
        id:buttonImage
        source: iconSource
        anchors.left: parent.left
        anchors.leftMargin: 58
        anchors.verticalCenter: parent.verticalCenter

    }

    Text {
        text: nameText

        //字体
        font.family: "MI LANTING"
        font.pixelSize: 26
        color: whiteColor


        anchors.left: buttonImage.right
        anchors.leftMargin: 40
        anchors.verticalCenter: parent.verticalCenter
    }

}
