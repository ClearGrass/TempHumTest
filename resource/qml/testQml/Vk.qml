import QtQuick 1.0
Rectangle {
    id:rootRect
    property alias text: num.text
    property int currentIndex: 0
    width:    854; height:     480
    color: "#212121"

    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }

    Rectangle{
        height: 2
        width: 150
        color: "#ffffff"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 100
        Text {
            id: num
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            font.family: "MI LANTING"
            font.pixelSize: 28
            color: "#ffffff"
        }
        Text {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            anchors.right:  parent.right
            font.family: "MI LANTING"
            font.pixelSize: 28
            color: "#ffffff"
            text: "min"
        }

        Rectangle{
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            anchors.left: parent.right
            anchors.leftMargin: 10
            height: 30
            width: 30
            smooth: true
            radius: 15
            color: "#ffffff"
            opacity: 0.5
            Text {
                text: "+"
                anchors.centerIn: parent
                font.family: "MI LANTING"
                font.pixelSize: 28
                color: "#000000"
                rotation: 50
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    num.text = ""
                }
                onPressed: {
                    parent.opacity = 0.4
                }
                onReleased: {
                    parent.opacity = 0.5
                }
            }
        }
    }


    Column{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 60
        spacing: 30
        Row{

            spacing: 30
            KeyButton{
                text: "0"
                onSignal_clicked:  {
                    num.text = num.text + "0"
                }
            }
            KeyButton{
                text: "1"
                onSignal_clicked: {
                    num.text = num.text + "1"

                }
            }
            KeyButton{
                text: "2"
                onSignal_clicked: {
                    num.text = num.text + "2"

                }
            }
            KeyButton{
                text: "3"
                onSignal_clicked: {
                    num.text = num.text + "3"

                }
            }
            KeyButton{
                text: "4"
                onSignal_clicked: {
                    num.text = num.text + "4"

                }
            }
            KeyButton{
                text: "5"
                onSignal_clicked: {
                    num.text = num.text + "5"

                }
            }

        }

        Row{
            spacing: 30

            KeyButton{
                text: "Cancel"
                onSignal_clicked: {
                    rootRect.visible = false
                }
            }
            KeyButton{
                text: "6"
                onSignal_clicked: {
                    num.text = num.text + "6"

                }
            }
            KeyButton{
                text: "7"
                onSignal_clicked: {
                    num.text = num.text + "7"
                }
            }
            KeyButton{
                text: "8"
                onSignal_clicked: {
                    num.text = num.text + "8"
                }
            }
            KeyButton{
                text: "9"
                onSignal_clicked: {
                    num.text = num.text + "9"
                }
            }

            KeyButton{
                text: "Ok"
                onSignal_clicked: {
//                    console.log(Number(num.text))
                    var time = Number(num.text)
                    if(0 != time)
                    {
                        MainViewProperty.slot_setInterval(time)
                    }
                    rootRect.visible = false
                }
            }
        }
    }



}
