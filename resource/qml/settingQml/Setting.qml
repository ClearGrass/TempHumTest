import QtQuick 1.0
import "baseQml" as BaseQml
import "reset"   as ResetQml
Rectangle {
    width: 854
    height: 480
    property string transColor: "#00000000"
    property string iconSource: "qrc:resource/images/optionPage/icon/icon_setting.png"
    color:"#292a2c"

    //返回按钮
    BaseQml.BackButton{
        id:backButton
        z:20

        //返回
        MouseArea{
            anchors.fill: backButton
            onClicked:
            {
                SettingProperty.on_page_defalut_back();
            }
            onPressed: {
                backButton.opacity = 0.5
            }
            onReleased: {
                backButton.opacity = 1.0
            }
        }
    }


    ResetQml.ResetPage{
        id:resetpage
        z:22
        anchors.centerIn: parent
        visible: false
    }


    //横向线条
    BaseQml.FrameTop
    {
        z:10
        id:frameTop
        topTitle: qsTr("Settings")
    }

    Flickable {
        id: view
        anchors.fill: parent
        contentWidth: rootRect.width
        contentHeight: rootRect.height
        z:1
        contentY : SettingProperty.set_getScrollValue()

        smooth: true
        BaseSetting{
            color: transColor
            id: rootRect
            onSignal_reset: {
                resetpage.visible = true
            }
        }



        states: State {
            name: "ShowBars"
            when: view.movingVertically
            PropertyChanges { target: verticalScrollBar; opacity: 1 }
        }


        transitions: Transition {
            NumberAnimation { properties: "opacity"; duration: 500

            }
        }


    }

    BaseQml.ScrollBar {
        id: verticalScrollBar
        width: 12; height: view.height-76
        anchors.right: view.right
        anchors.bottom: view.bottom
        opacity: 1
        orientation: Qt.Vertical
        position: view.visibleArea.yPosition
        pageSize: view.visibleArea.heightRatio
    }

    NumberAnimation {
        id:scrollBarAnimation
        running: true
        loops: 1
        target: verticalScrollBar
        property: "opacity"
        from:1.0
        to:0.0
        duration: 1000
    }

    function set_srcollValue()
    {
        SettingProperty.slot_setScrollValue(view.contentY)
    }

    function init_scollValue()
    {
        view.contentY = 0;
    }

    Connections{
        target: SettingProperty
        onSingal_changeScrollValue:
        {
            set_srcollValue();
        }
        onSignal_initScrllValue:
        {
            init_scollValue();
        }
        onSignal_showScrollBarAnimatio:
        {
            scrollBarAnimation.start()
        }

        onSignal_back:
        {
            if(resetpage.visible)
            {
                resetpage.visible = false
                return;
            }
            if(SettingProperty.slot_getCurPageIndex() == 0)
            {
                SettingProperty.on_page_defalut_back();
            }

        }
    }
}
