import QtQuick 1.0


Rectangle {
    id: button
    color: "#00000000"
    property alias source:  buttonImage.source
    property string op: ""
    property string keyPress2: "qrc:/resource/images/optionPage/keyboard/keyPress2.png"
    property string keyRelease2: "qrc:/resource/images/optionPage/keyboard/keyRelease2.png"

    signal clicked(string op)
    radius: 8


    width: 121
    height: 64

    Image {
        anchors.fill: parent

        id: backgroundRect
        source: keyRelease2
    }

    Image {
        id: buttonImage
        anchors { centerIn: parent; }
        smooth: true
    }
    MouseArea {
        id: mouseAreas
        anchors.fill: parent
        onClicked: button.clicked(op)
        onPressed: backgroundRect.source = keyPress2
        onReleased: backgroundRect.source = keyRelease2
    }

}
