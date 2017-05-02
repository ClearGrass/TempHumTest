import QtQuick 1.0


Item {
    id: button
    property string op: ""
    property string keyPress2: "qrc:/resource/images/optionPage/keyboard/keyPress2.png"
    property string keyRelease2: "qrc:/resource/images/optionPage/keyboard/keyRelease2.png"
    property string fontFamily: "MI LANTING"
    property alias text: symbolText.text
    signal clicked(string op)


    width: 121
    height: 64

    Image {
        anchors.fill: parent

        id: backgroundRect
        source: keyRelease2
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
        onPressed: backgroundRect.source = keyPress2
        onReleased: backgroundRect.source = keyRelease2
    }

    function set_color(color)
    {
        symbolText.color = color
    }

    function set_enable()
    {
        symbolText.color = "#ffffff"
        mouseAreas.visible = true
    }

    function set_disable()
    {
        symbolText.color = "#80ffffff"
        mouseAreas.visible = false
    }
}
