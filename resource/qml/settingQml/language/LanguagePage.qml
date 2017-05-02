import QtQuick 1.0
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle {
    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_language.png"
    property string sourceIma: "qrc:/resource/images/optionPage/label/label_selected.png"

    //页面宽度和高度
    property int itemHeight: 480
    property int itemWidth:  854
    property string whiteColor: "#ffffff"
    property int  initIndex: 0
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
        topTitle: qsTr("Select the Language")
    }

    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }

    BaseQml.Messagebox{
        anchors.centerIn: parent
        z:23
        id:messageBox
        visible: false

        Text {
            z:23
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:  parent.verticalCenter
            anchors.verticalCenterOffset:  -50
            width: 450
            font.family: "MI LANTING"
            wrapMode: Text.Wrap
            font.pixelSize: 30
            horizontalAlignment: Text.AlignHCenter
            color: "#212121"
            text: qsTr("Are you sure you want to change the system language? The device will restart after confirmation.")
        }
        leftText: qsTr("Yes")
        rightText:  qsTr("Cancel")

        //确定
        onSignal_leftButtonOnClicked: {
            messageBox.visible = true
            SettingProperty.slot_setLanguage( listView.currentIndex)
            SettingProperty.slot_setQMl(0)
            SettingProperty.slot_reset()

        }


        //取消
        onSignal_rightButtonOnClicked: {
            messageBox.visible = false
        }
    }

    Component {
        id: langDelegate
        Item {
            id: wrapper
            width: 854; height: 94
            Text {
                id: id
                text: languageid
                visible: false
            }
            Text {
                id:langename
                text:  languagename
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

                PropertyChanges { target: langename; color: "#33b4ff" }

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

        model: laguageModel
        delegate: langDelegate
        focus: true
        clip: true
        highlightFollowsCurrentItem: true
    }

    ListModel {
        id:laguageModel
        ListElement {
            languagename: "中文（简体）"
            languageid: "ZH_SC"

        }
        ListElement {
            languagename: "中文（繁體）"
            languageid: "ZH_TC"

        }
        ListElement {
            languagename: "English"
            languageid: "US"

        }

    }
    MouseArea{
        anchors.fill: backButton
        onClicked:
        {
            if(initIndex == listView.currentIndex)
            {
                SettingProperty.slot_setQMl(0)
            }
            else
            {
                messageBox.visible = true
            }
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
        onSignal_initLang:
        {
            listView.currentIndex = SettingProperty.slot_getLanguage()
            initIndex = listView.currentIndex
        }
        onSignal_back:
        {
            if(messageBox.visible)
            {
                messageBox.visible = false
                return ;
            }
            SettingProperty.slot_setQMl(0)
        }
    }

}

