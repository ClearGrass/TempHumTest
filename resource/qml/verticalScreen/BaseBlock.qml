import QtQuick 1.1
import "../toolQml" as Tool

Item {
    height: 102
    width: 480
    property alias dataName: envName.text
    property alias dataDescribe: envDescribe.text
    property alias dataValue: evnValue.text
    property alias dataColor: evnValue.color
    property alias dataUnit: envUnit.text
    property bool  sensorstarting: false

    Row{
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        spacing: 15
        anchors.verticalCenterOffset: dataValue.indexOf(".") < 0 ?  -12 :-7
        Text {
            id:envName

            //字体
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: "#585858"
        }
        Text {
            text: "|"

            //字体
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: "#000000"
            opacity: 0.3
            visible: envDescribe.text == "" ? false : true
        }

        //环境评价
        Text {
            id:envDescribe

            //字体
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: "#585858"
        }

        Text {
            text: "|"

            //字体
            font.family: "MI LANTING"
            font.pixelSize: 26
            color: "#000000"
            opacity: 0.3
            visible: sensorstart.visible
        }

        Tool.SensorStarting{
            id:sensorstart
            fontColor: "#585858"
            fontsize: 26
            visible: sensorstarting
        }

    }

    Column{
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 2
        anchors.right: parent.right
        anchors.rightMargin: 20
        spacing: -15

        //值
        Text {
            id:evnValue
            //字体
            font.family: "KMedium"
            font.pixelSize: 50
            horizontalAlignment: Text.AlignRight
            anchors.right: parent.right
        }

        Text{
            id:envUnit
            font.family: "MI LANTING"
            font.pixelSize: 24
            color: "#a3a3a3"
            anchors.right: parent.right
        }
    }


}
