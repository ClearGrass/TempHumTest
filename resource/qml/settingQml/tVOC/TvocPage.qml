import QtQuick 1.1
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle
{
    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_language.png"
    property string sourceIma: "qrc:/resource/images/optionPage/label/label_selected.png"


    property string seleSSic: ""
    //页面宽度和高度
    property int itemHeight: 480
    property int itemWidth:  854
    property string whiteColor: "#ffffff"
    property string fontFamily: "MI LANTING"

    height: itemHeight
    width:  itemWidth
    color:"#292a2c"
    z:11
    //横向线条
    BaseQml.FrameTop
    {
        id:frameTop
        iconSource: sourceIcon

        topTitle: qsTr("tVOC Unit")
        iconShow:false
    }

    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }

    Component {
        id: tVOCDelegate
        Item {
            id: wrapper
            width: 854; height: 94

            Text {
                id:tVocName
                text:  unitname
                //字体
                font.family: fontFamily
                font.pixelSize: 26
                font.bold: false
                color: "#ffffff"

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 40
            }


            Image {
                id: ima
                visible: false
                source:sourceIma
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            BaseQml.HorizonLine{
                id:line2
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 40
                width: 774
            }

            states: State {
                name: "Current"
                when: wrapper.ListView.isCurrentItem

                PropertyChanges { target: tVocName; color: "#33b4ff" }

                PropertyChanges { target: ima; visible: true }

            }

            MouseArea{
                anchors.fill: wrapper
                onClicked: {
                    wrapper.ListView.view.currentIndex = index
                }
            }

        }
    }



    ListView {
        id: listView

        width: 854; height: 380
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 100

        model: tVOCModel
        delegate: tVOCDelegate
        focus: true
        clip: true
        highlightFollowsCurrentItem: true
    }

    ListModel {
        id:tVOCModel
        ListElement {
            unitname: "mg/m³"

        }
        ListElement {
            unitname: "ppm"

        }
        ListElement {
            unitname: "ppb"

        }

    }


    MouseArea{
        anchors.fill: backButton
        onClicked:
        {
            SettingProperty.slot_setTvocUnit(listView.currentIndex)
            SettingProperty.slot_setQMl(0)
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
        onSignal_initTvocUnit:
        {
            listView.currentIndex = SettingProperty.slot_getTvocUnit()
        }
        onSignal_back:
        {
            SettingProperty.slot_setQMl(0)

        }
    }
}
