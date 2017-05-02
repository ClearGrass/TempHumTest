import QtQuick 1.1

Rectangle{
    signal signal_leftButtonOnClicked()
    signal signal_rightButtonOnClicked()
    id:rootRect
    visible: parent.visible
    property alias leftText: leftTextid.text
    property alias rightText: rightTextid.text
    property bool leftButtonBold:false
    height: 390
    width: 694
    radius: 4
    color: "#ffffff"
    anchors.centerIn: parent
    z:23
    Item {
        height: 98
        width: parent.width
        z:24
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        id: doubleButtonItem


        HorizonLine{
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            color: "#80010101"
            opacity: 0.2
        }

        VerticalLine{
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenterOffset: -1
            color: "#80010101"
            opacity: 0.2
        }

        Rectangle
        {
            height: parent.height
            width: parent.width / 2
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            color: "#00000000"
            radius: 4
            z:25
            Text {
                id: leftTextid
                anchors.centerIn: parent
                //字体
                font.family: "MI LANTING"
                font.pixelSize: 25
                font.bold: leftButtonBold
                color:"#cc000000"

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 100
            }

            MouseArea{
                enabled: rootRect.visible
                anchors.fill: parent
                onClicked: {
                    signal_leftButtonOnClicked()
                    console.log("lefftButton onClicked")
                }
                onPressed: {
                    parent.color = "#e6e5e5"
                }
                onReleased: {
                    parent.color = "#00000000"
                }
            }
        }

        Rectangle{
            z:25
            height: parent.height
            width: parent.width / 2
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            color: "#00000000"
            radius: 4
            Text {
                id: rightTextid
                anchors.centerIn: parent
                //字体
                font.family: "MI LANTING"
                font.pixelSize: 25

                color:"#cc000000"


                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 100

            }

            MouseArea{
                enabled: rootRect.visible
                anchors.fill: parent
                onClicked: {
                    signal_rightButtonOnClicked()
                    console.log("rightButton onClicked")

                }
                onPressed: {
                    parent.color = "#e6e5e5"
                }
                onReleased: {
                    parent.color = "#00000000"
                }
            }

        }

    }

}



