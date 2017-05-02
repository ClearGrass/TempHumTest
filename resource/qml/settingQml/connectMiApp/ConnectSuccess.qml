import QtQuick 1.1

Item {
    height: 380
    width: 854
    property string successSource: "qrc:/resource/images/optionPage/label/success.png"
    property string whiteColor: "#ffffff"
    property string fontFamily: "MI LANTING"
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.topMargin: 100
    Column{
        anchors.centerIn: parent
        spacing: 20
        Image {
            source: successSource
            anchors.horizontalCenter: parent.horizontalCenter

        }
        Text {
            text:  qsTr("Connected successfully")
            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color: whiteColor
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
