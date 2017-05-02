/*******************************************************************************
* File Name     :   PageControl.qml
* Description   :   用于主界面循环管理
* Author        :   Jason
* Date          :   2016-10-1
*******************************************************************************/
import QtQuick 1.0
import MainView 1.0
Rectangle {

    //图标资源定义
    property string curPageIndicator: "qrc:/resource/images/mainPage/icon/curPageIndicator.png"
    property string otherPageIndicator: "qrc:/resource/images/mainPage/icon/otherpageIndicator.png"
    property int pmIndex: 1
    property int outPmIndex: 1
    property int co2Index: 1
    property int tVOCIndex: 1
    property string backgroundSource: get_source()

    property int curPageSoueceIndex: 1
    property int  curIndex: 0
    id: pageControl
    width: 854
    height: 480
    //    clip: false
    anchors.centerIn: parent
    signal signal_mouseMove();
    //模型定义
    property VisualItemModel listModel

    //页面变动信号定义
    signal pageChanged(int page)


    Image {
        id: back
        fillMode: Image.TileHorizontally
        source: backgroundSource
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

    }

    //基础界面定义
    BaseFrame {
        id:baseFrame
    }

    //当前页面变动时，触发信号
    onPageChanged:
    {
        baseFrame.currentIndex = page
        curIndex = page
        MainViewProperty.slot_setCurrentIndex(curIndex)


        set_source()
    }

    PathView {
        id: view

        width: parent.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        flickDeceleration: 50

        Keys.onRightPressed: {
            if (!moving && interactive) incrementCurrentIndex()
        }
        Keys.onLeftPressed:
        {
            if (!moving && interactive) decrementCurrentIndex()
        }

        onCurrentIndexChanged: pageChanged(currentIndex)

        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        focus: true
        interactive: true
        model: listModel
        path: Path {
            startX: - pageControl.width * listModel.count / 2 + pageControl.width / 2
            startY: pageControl.height / 2
            PathLine {
                x: pageControl.width * listModel.count / 2 + pageControl.width / 2
                y: pageControl.height / 2
            }
        }
        Connections {
            target:MainViewProperty
            onSignal_nextPage:{
                if (!view.moving && view.interactive) view.incrementCurrentIndex();
            }

            //夜间模式
            onSignal_changeNightMode:
            {
               set_source()
            }
            onSignal_changeCurrentIndex:
            {
                view.currentIndex = MainViewProperty.slot_getCurrentIndex()
            }
            onSignal_mouseMove:
            {
                signal_mouseMove();
            }

        }

    }


    //页面变动指示器定义
    Rectangle {
        id: pageIndicator
        opacity: 1
        width:100
        height: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 22
        anchors.left: parent.left
        anchors.leftMargin: 22
        color: "#00000000"
        Row {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            spacing: 10
            Repeater {
                model: listModel.count
                delegate: Rectangle {
                    width: 10
                    height: pageIndicator.height
                    color: "#00000000"
                    Image {
                        anchors.centerIn: parent
                        width: 10
                        height: 10
                        source:
                        {
                            if(view.currentIndex == index)
                            {
                                curPageIndicator
                            }
                            else
                            {
                                otherPageIndicator
                            }
                        }

                        smooth: true

                    }// End of Image

                } //End of Rectangle

            }// End of Repeater

        }// End of Row

    } // End of Rectangle


    function get_source()
    {
        if("2" == MainViewProperty.slot_getNightMode())
        {
            return   "qrc:/resource/images/mainPage/background/good.png"
        }
        else
        {
            return  "qrc:/resource/images/mainPage/background/nightmode.png"

        }
    }


    function set_source()
    {
        var nightmode =  MainViewProperty.slot_getNightMode()

        switch(curIndex)
        {
        case 0: //时间天气
            curPageSoueceIndex = pmIndex > outPmIndex ? pmIndex : outPmIndex;
            if(co2Index > curPageSoueceIndex)
                curPageSoueceIndex = co2Index;
            if(tVOCIndex > curPageSoueceIndex)
                curPageSoueceIndex = tVOCIndex;

            break;
        case 1: //pm2.4
            curPageSoueceIndex = pmIndex;
            break;
        case 2: // tVOC CO2
            curPageSoueceIndex = co2Index > tVOCIndex ? co2Index : tVOCIndex
            break;
        case 3: //温湿度
            curPageSoueceIndex = 1
            break;
        case 4: //其他
            curPageSoueceIndex = 1
            break;
        }

        //白天模式
        if(2 == nightmode)
        {
            switch(curPageSoueceIndex)
            {
            case 1:
                backgroundSource = "qrc:/resource/images/mainPage/background/good.png"
                break;
            case 2:
                backgroundSource = "qrc:/resource/images/mainPage/background/lightly.png"
                break;
            case 3:
                backgroundSource = "qrc:/resource/images/mainPage/background/moderately.png"
                break;
            case 4:
                backgroundSource = "qrc:/resource/images/mainPage/background/heavily.png"
                break;
            case 5:
                backgroundSource = "qrc:/resource/images/mainPage/background/severely.png"
                break;
            }
        }
        else
        {
            switch(curPageSoueceIndex)
            {
            case 1:
            case 2:
                backgroundSource = "qrc:/resource/images/mainPage/background/nightmode.png"
                break;
            case 3:
                backgroundSource = "qrc:/resource/images/mainPage/background/moderatelyNightmode.png"
                break;
            case 4:
                backgroundSource = "qrc:/resource/images/mainPage/background/heavilyNightmode.png"
                break;
            case 5:
                backgroundSource = "qrc:/resource/images/mainPage/background/severelyNightmode.png"
                break;
            }
        }
    }


    onPmIndexChanged: {
        set_source();
    }

    onCo2IndexChanged: {
        set_source();
    }

    onTVOCIndexChanged: {
        set_source();
    }

}
