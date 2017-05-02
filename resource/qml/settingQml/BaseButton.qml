
import QtQuick 1.0

Rectangle{

    property string iconSource: "qrc:resource/images/optionPage/logo/logo_wifi.png"
    property string nameText: ""
    property string variText: ""


    id:rootRect
    width: 427
    height: 126
    color: "#00000000"
    property string whiteColor: "#ffffff"
    property string grayColor: "#80ffffff"
    Image {
        id:buttonImage
        source: iconSource

        anchors.left: parent.left
        anchors.leftMargin: 58
        anchors.verticalCenter: parent.verticalCenter
    }

    Column{
        anchors.left: buttonImage.right
        anchors.leftMargin: 40
        anchors.verticalCenter: parent.verticalCenter
        spacing: 5
        Text {
            text: nameText

            //字体
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: whiteColor

        }

        Text {
            text: variText

            //字体
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: grayColor
            visible: variText == "" ? false : true
        }
    }

}
