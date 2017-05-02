import QtQuick 1.1
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle
{
    //页面宽度和高度
    property int itemHeight: 537
    property int itemWidth:  854
    property string whiteColor: "#ffffff"
    property string fontFamily: "MI LANTING"

    height: itemHeight
    width:  itemWidth
    color:"#292a2c"


    Column{
        anchors.top:parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        Item{
            height: 80
            width: 854

            Text {
                text: qsTr("Device Name")

                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color:whiteColor

                anchors.left:parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: qsTr("Bran")

                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color:"#808080"

                anchors.right:parent.right
                anchors.rightMargin: 40
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        BaseQml.HorizonLine{
            width: 774
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Item{
            height: 80
            width: 854

            Text {
                text: qsTr("Model")

                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color:whiteColor

                anchors.left:parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: "KQJCY01QP"

                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color:"#808080"

                anchors.right:parent.right
                anchors.rightMargin: 40
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        BaseQml.HorizonLine{
            width: 774
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Item{
            height: 80
            width: 854

            Text {
                text: qsTr("Serial Number")

                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color:whiteColor

                anchors.left:parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: SettingProperty.slot_getSerialNumber()

                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color:"#808080"

                anchors.right:parent.right
                anchors.rightMargin: 40
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        BaseQml.HorizonLine{
            width: 774
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Item{
            height: 80
            width: 854

            Text {
                text: "Wi-Fi MAC"

                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color:whiteColor

                anchors.left:parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: SettingProperty.slot_getDeviceMAC()

                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color:"#808080"

                anchors.right:parent.right
                anchors.rightMargin: 40
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        BaseQml.HorizonLine{
            width: 774
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }


    Column{
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "cleargrass.com/bran"
            font.family: fontFamily
            font.pixelSize: 26
            color: whiteColor
            opacity: 0.5
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "support@cleargrass.com"
            font.family: fontFamily
            font.pixelSize: 26
            color: whiteColor
            opacity: 0.5
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("ClearGrass LLC")
            font.family: fontFamily
            font.pixelSize: 26
            color: whiteColor
            opacity: 0.5
        }

    }
}
