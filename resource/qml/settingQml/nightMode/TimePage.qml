import QtQuick 1.1
import "../baseQml" as BaseQml
import "../../toolQml" as ToolQml
import Setting 1.0

Rectangle{
    property bool isUSlan: SettingProperty.slot_isUsLang();
    property bool isHour24: SettingProperty.slot_get24Hours()
    height: 480
    width:  854
    color:"#292a2c"
    property string whiteColor: "#ffffff"
    property string fontFamily: "MI LANTING"
    property string blueEnterSource: "qrc:/resource/images/optionPage/button/button_enter_blue.png"
    property string timeInterVal: SettingProperty.slot_getNightInterver()
    z:9
    BaseQml.BackButton{
        id:backButton
    }

    //横向线条
    BaseQml.FrameTop
    {
        id:frameTop
        topTitle: qsTr("Custom")
    }

    TimeWidget{
        z:12
        anchors.centerIn: parent
        id:timeWidget
        visible: false
        onSignal_ok: {
            timeInterVal = SettingProperty.slot_getNightInterver()
        }
    }

    Column{
        anchors.top: frameTop.bottom
        anchors.left: parent.left
        Item{
            height: 94
            width: 854

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter

                text: qsTr("Start")
                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color: whiteColor

            }

            Image {
                id:starttimeImage
                anchors{right:parent.right;verticalCenter: parent.verticalCenter}
                source: blueEnterSource
            }

            Text {
                id: startTimeText
                text: timeInterVal.substring(0, timeInterVal.indexOf("-"))

                font.family: fontFamily
                font.pixelSize: 26
                color: whiteColor

                anchors.right: starttimeImage.left
                anchors.verticalCenter: parent.verticalCenter

            }

            //横线
            BaseQml.HorizonLine{
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: 774
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    timeWidget.text = qsTr("Start")
                    timeWidget.isStartTime = true
                    timeWidget.initFunction()
                    timeWidget.visible = true
                }
            }
        }


        Item{
            height: 94
            width: 854
            Text {
                anchors.left: parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter

                text: qsTr("End")
                //字体
                font.family: fontFamily
                font.pixelSize: 26
                color: whiteColor

            }

            Image {
                id:endtimeImage
                anchors{right:parent.right;verticalCenter: parent.verticalCenter}
                source: blueEnterSource
            }

            Text {
                id: endTimeText
                text: timeInterVal.substring(timeInterVal.lastIndexOf("-")+1)

                font.family: fontFamily
                font.pixelSize: 26
                color: whiteColor

                anchors.right: endtimeImage.left
                anchors.verticalCenter: parent.verticalCenter

            }

            //横线
            BaseQml.HorizonLine{
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: 774
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    timeWidget.text = qsTr("End")
                    timeWidget.isStartTime = false
                    timeWidget.initFunction()
                    timeWidget.visible = true
                }
            }
        }

    }


    MouseArea{
        anchors.fill: backButton
        onClicked:
        {

            SettingProperty.slot_setQMl(4)
        }
        onPressed: {
            backButton.opacity = 0.5
        }
        onReleased: {
            backButton.opacity = 1.0
        }
    }

    Connections{
        target: SettingProperty
        onSignal_back:
        {
            if(timeWidget.visible)
            {
                timeWidget.visible = false
                return ;
            }

            SettingProperty.slot_setQMl(4)
        }

    }


}
