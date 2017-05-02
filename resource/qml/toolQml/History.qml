import QtQuick 1.1

Image{

    //资源定义
    property string historySource: "qrc:/resource/images/mainPage/button/history.png"
    property string historyPressedSource: "qrc:/resource/images/mainPage/button/historyPressed.png"

    //按下信号
    signal signal_onClicked()

    source: historySource

    MouseArea{
        height: 100
        width: 100
        anchors.centerIn: parent

        onClicked: {
            signal_onClicked()
            console.log("history onClicked")
        }
        onPressed: {
            source = historyPressedSource
        }
        onReleased: {
            source = historySource
        }
    }

}
