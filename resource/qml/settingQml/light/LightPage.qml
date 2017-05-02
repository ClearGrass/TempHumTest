import QtQuick 1.1
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle
{
    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_light.png"
    property string sourcelight1: "qrc:resource/images/optionPage/label/label_light1.png"
    property string sourcelight2: "qrc:resource/images/optionPage/label/label_light2.png"
    property string sourceslider: "qrc:resource/images/optionPage/tools/slide_point.png"

    //页面宽度和高度
    property int itemHeight: 480
    property int itemWidth:  854
    property string whiteColor: "#ffffff"
    property int lightValue: 0
    height: itemHeight
    width:  itemWidth
    color:"#292a2c"
    z:11
    //横向线条
    BaseQml.FrameTop
    {
        id:frameTop
        iconSource: sourceIcon
        topTitle: qsTr("Screen")
    }

    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }
    Rectangle{
        id:fillRect
        height: 380
        width: 854
        z:1

        color:"#00000000"
        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.left: parent.left
        anchors.leftMargin: 0
    }


    Flickable {
        id: view

        height: 380
        width: 854
        anchors.fill: fillRect
        contentWidth: rootRect.width
        contentHeight: rootRect.height
        contentY : SettingProperty.slot_getScreenSettingScrollValue()
        clip: true
        BaseScreen{
            onSignal_updateScroll: {

                SettingProperty.slot_setSceenSettingScrollvalue(view.contentY)
            }

            id:rootRect

        }

        states: State {
            name: "ShowBars"
            when: view.movingVertically
            PropertyChanges { target: verticalScrollBar; opacity: 1 }
        }

        transitions: Transition {
            NumberAnimation { properties: "opacity"; duration: 500 }
        }
    }

    BaseQml.ScrollBar {
        id: verticalScrollBar
        width: 12; height: view.height
        anchors.right: view.right
        anchors.top: view.top
        opacity: 1.0
        orientation: Qt.Vertical
        position: view.visibleArea.yPosition
        pageSize: view.visibleArea.heightRatio
    }

    NumberAnimation {
        running: true
        loops: 1
        target: verticalScrollBar
        property: "opacity"
        from:1.0
        to:0.0
        duration: 1000
    }


    MouseArea{
        anchors.fill: backButton
        onClicked:
        {
            SettingProperty.slot_setSceenSettingScrollvalue(0)
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
        onSignal_back:
        {
            if(SettingProperty.slot_getCurPageIndex() == 9)
                SettingProperty.slot_setQMl(0)
        }

    }
}
