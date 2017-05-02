import QtQuick 1.1
import "../baseQml" as BaseQml
import Setting 1.0

Item {
    height: 380
    width: 854
    property string whiteColor: "#ffffff"
    property string labelStoreSource: "qrc:resource/images/optionPage/label/label_store.png"
    property string labelPlaySource: "qrc:resource/images/optionPage/label/label_play.png"
    property string refreshSource: "qrc:/resource/images/optionPage/button/refresh.png"
    property string refreshPressedSource: "qrc:/resource/images/optionPage/button/refreshPressed.png"
    property string  fileName: SettingProperty.slot_getAppQrcode() == "" ? "" : "file:///" +SettingProperty.slot_getAppQrcode()
    id:rootItem

    anchors.top: parent.top
    anchors.topMargin: 100
    anchors.left: parent.left


    Image {
        id: qcodeImage
        source:fileName
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 30
        visible: fileName == "" ? false : true
        cache: false

    }

    Rectangle{
        height: 320
        width: 320
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 30
        visible: fileName == "" ? true : false
        color: "#3F4042"
        Column{
            anchors.centerIn: parent
            spacing: 30
            Image {
                smooth: true
                id:refreshImage
                source: refreshSource

                anchors.horizontalCenter: parent.horizontalCenter
                MouseArea{
                    height: 100
                    width: 100
                    anchors.centerIn: parent
                    enabled: rootItem.visible
                    onClicked: {
                        animationRefresh.start()
                        SettingProperty.slot_refresh_appQrcode()
                    }
                    onPressed: {
                        refreshImage.source = refreshPressedSource
                    }
                    onReleased: {
                        refreshImage.source = refreshSource
                    }
                }
                NumberAnimation on rotation {
                    id:animationRefresh ; from: 0; to: 360; running: false; loops: Animation.Infinite; duration: 2000
                }
            }

            Text {
                id:tipText
                text: qsTr("Unable to get the QR code")
                horizontalAlignment: Text.AlignHCenter
                font.family: "MI LANTING"
                font.pixelSize: 26
                color: whiteColor
                width: 230
                wrapMode: Text.Wrap
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

    }


    Column{
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 60
        spacing: 20
        Text {
            text: qsTr("1. Open the Mi Home app on your mobile.")
            horizontalAlignment: Text.AlignLeft
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: whiteColor
            width: 384
            wrapMode: Text.Wrap
            anchors.left: parent.left
        }
        Text {
            text: qsTr("2. Add device.")
            horizontalAlignment: Text.AlignLeft
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: whiteColor
            width: 384
            wrapMode: Text.Wrap
            anchors.left: parent.left
        }
        Text {
            text: qsTr("3. Select Air Monitor.")
            horizontalAlignment: Text.AlignLeft
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: whiteColor
            width: 384
            wrapMode: Text.Wrap
            anchors.left: parent.left
        }
        Text {
            text: qsTr("4. Enter the verification")
            horizontalAlignment: Text.AlignLeft
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: whiteColor
            width: 384
            wrapMode: Text.Wrap
            anchors.left: parent.left
        }

    }


    Connections{
        target: SettingProperty

        onSignal_setAppQrcode:
        {
            qcodeImage.sourceSize = Qt.size(1, 1)
            fileName =SettingProperty.slot_getQrcode() == "" ? "" : "file:///" + SettingProperty.slot_getQrcode()
            qcodeImage.sourceSize = Qt.size(320, 320)

            animationRefresh.stop()
        }
        onSignal_setQrcodeTimeout:
        {
            animationRefresh.stop()
            fileName = ""
            tipText.text = qsTr("Timed out, please try again.")
        }
    }

}
