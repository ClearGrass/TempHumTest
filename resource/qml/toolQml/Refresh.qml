import QtQuick 1.1

Image{

    //资源定义
    property string refreshSource: "qrc:/resource/images/mainPage/button/refresh.png"
    property string refreshPressedSource: "qrc:/resource/images/mainPage/button/refreshPressed.png"

    //按下信号
    signal signal_onClicked()

    source: refreshSource
    smooth: true

    NumberAnimation on rotation {
        id:animationRefresh ; from: 0; to: 360; running: false; loops: Animation.Infinite; duration: 2000
    }

    MouseArea{
        height: 100
        width: 100
        anchors.centerIn: parent

        onClicked: {
            signal_onClicked()
            console.log("refresh onClicked")
        }
        onPressed: {
            source = refreshPressedSource
        }
        onReleased: {
            source = refreshSource
        }
    }


    function start_animation()
    {
        animationRefresh.start();
    }

    function stop_animation()
    {
        animationRefresh.stop();
    }


}
