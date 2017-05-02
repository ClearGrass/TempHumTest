import QtQuick 1.0
import "../baseQml" as BaseQml
import Setting 1.0

Item {
    height: 380
    width: 854
    property string whiteColor: "#ffffff"

    property string labelMiLogoSource: "qrc:/resource/images/optionPage/label/label_mijia.png"
    property string qrcodeMiHomeAppSource: "qrc:/resource/images/optionPage/label/miHomeApp.png"
    property string labelStoreSource: "qrc:resource/images/optionPage/label/label_store.png"
    property string labelPlaySource: "qrc:resource/images/optionPage/label/label_play.png"
    property string buttonPress: "qrc:/resource/images/optionPage/button/buttonblueSmallPressed.png"
    property string buttonRelease: "qrc:/resource/images/optionPage/button/buttoblueSmall.png"
    signal signal_next()
    id:rootItem

    anchors.top: parent.top
    anchors.topMargin: 100
    anchors.left: parent.left

    Image {
        source:qrcodeMiHomeAppSource
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.verticalCenter: parent.verticalCenter
    }



    Column{
        anchors.top: parent.top
        anchors.topMargin:30
        anchors.right: parent.right
        anchors.rightMargin: 107

        spacing: 20
        Image {
            source: labelMiLogoSource

            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
            Image {
                source: labelStoreSource
            }
            Image {
                source: labelPlaySource
            }
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "home.mi.com"
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: whiteColor

        }
    }


    Image {
        source: buttonRelease
        anchors.right: parent.right
        anchors.rightMargin: 87
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30

        Text {
            anchors.centerIn: parent
            text:  qsTr("Installed Already")
            //字体
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: whiteColor

        }

        MouseArea{
            enabled: rootItem.visible
            anchors.fill: parent
            onClicked: {
                signal_next()
            }
            onPressed: {
                parent.source = buttonPress
            }
            onReleased: {
                parent.source = buttonRelease
            }
        }
    }

}
