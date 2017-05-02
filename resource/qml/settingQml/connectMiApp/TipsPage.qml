import QtQuick 1.1

Item {
    height: 380
    width: 854
    property string listImageSource: "qrc:/resource/images/optionPage/label/list_symbol.png"
    property string buttonSource: "qrc:/resource/images/optionPage/button/buttonblue.png"
    property string buttonPressedSource: "qrc:/resource/images/optionPage/button/buttonbuluPressed.png"
    property string whiteColor: "#ffffff"
    property string fontFamily: "MI LANTING"
    signal signal_next()

    anchors.top: parent.top
    anchors.topMargin: 100
    anchors.left: parent.left
    id:rootItem
    Column{
        anchors.top: parent.top
        anchors.topMargin: 60
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20
        Row{
            spacing: 5
            Image {
                source: listImageSource
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text:  qsTr("Check the data at anytime and anywhere.")
                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color: whiteColor

            }
        }
        Row{
            spacing: 5
            Image {
                source: listImageSource
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text:  qsTr("Link with Mi Air Purifier and Other Devices.")
                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color: whiteColor

            }
        }
    }


    Image {
        source: buttonSource
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30

        Text {
            anchors.centerIn: parent
            text:  qsTr("Next")
            //字体
            font.family: fontFamily
            font.pixelSize: 26
            color: whiteColor

        }

        MouseArea{
            anchors.fill: parent
            enabled: rootItem.visible
            onClicked: {
                signal_next()
            }
            onPressed: {
                parent.source = buttonPressedSource
            }
            onReleased: {
                parent.source = buttonSource
            }
        }
    }


}
