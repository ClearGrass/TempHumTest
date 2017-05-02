
import QtQuick 1.0
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle {
    width: 854; height: 480

    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_datetime.png"
    property string sourceIma: "qrc:/resource/images/optionPage/label/label_selected.png"
    color:"#292a2c"
    z:11
    property string fontFamily: "MI LANTING"

    BaseQml.FrameTop
    {
        id:frameTop
        iconSource: sourceIcon
        topTitle: qsTr("Time Zone")
    }

    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }

    Component {
        id: petDelegate
        Item {
            id: wrapper
            width: 854; height: 94
            Text {
                id:zoneName
                text:  (curIndex == true) ? qsTr("Automatically-")  + name : name

                //字体
                font.family:fontFamily
                font.pixelSize: 26
                font.bold: false
                color: "#ffffff"

                anchors.left: parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter

            }
            Text {
                id:zoneGMT
                text:  gmt
                //字体
                font.family: fontFamily
                font.pixelSize: 26
                font.bold: false
                color: "#80ffffff"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 40
            }

            Image {
                id: ima
                visible: false
                source:sourceIma
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            states: State {
                name: "Current"
                when: wrapper.ListView.isCurrentItem

                PropertyChanges { target: zoneName; color: "#33b4ff" }
                PropertyChanges { target: zoneGMT; color: "#33b4ff" }

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

        model: zoneModel
        delegate: petDelegate
        focus: true
        clip: true
        highlightFollowsCurrentItem: true
    }

    MouseArea{
        anchors.fill: backButton
        onClicked:
        {
            var tmp =  zoneModel[listView.currentIndex].num
            console.log(tmp)
            if(tmp != "")
                SettingProperty.slot_setZoneTime(tmp)
            SettingProperty.slot_setQMl(7)

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
        onSignal_initTimeZoneListPos:{
            listView.contentY = SettingProperty.slot_getCurZoneIndex() * 94
            listView.currentIndex = SettingProperty.slot_getCurZoneIndex()

        }
        onSignal_back:
        {
            SettingProperty.slot_setQMl(7)
        }
    }

}
