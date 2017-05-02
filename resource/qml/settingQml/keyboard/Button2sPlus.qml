import QtQuick 1.0


Item {
    id: button
    property string op: " "
    property string keyPress3: "qrc:/resource/images/optionPage/keyboard/keyPress3.png"
    property string keyRelease3: "qrc:/resource/images/optionPage/keyboard/keyRelease3.png"
    property string fontFamily: "MI LANTING"
    property alias text: symbolText.text
    signal clicked(string op)


    width: 451
    height: 64

    Image {
        anchors.fill: parent

        id: backgroundRect
        source: keyRelease3
    }

    Text {
        id:symbolText
        anchors.centerIn: parent
        font.family: fontFamily
        font.pixelSize: 24
        color: "#ffffff"
    }

    MouseArea {
        id: mouseAreas
        anchors.fill: parent
        onClicked: button.clicked(op)
        onPressed: backgroundRect.source = keyPress3
        onReleased: backgroundRect.source = keyRelease3
    }

}
