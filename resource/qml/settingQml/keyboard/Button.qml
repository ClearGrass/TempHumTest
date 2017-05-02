import QtQuick 1.0


Item {
    id: button
    property string keyEnlarge: "qrc:/resource/images/optionPage/keyboard/keyboardEnlarge.png"

    property string keyPress: "qrc:/resource/images/optionPage/keyboard/keyPress.png"
    property string keyRelease: "qrc:/resource/images/optionPage/keyboard/keyRelease.png"
    property string lettertext: ""
    property string op: lettertext
    property string shiftText: ""
    property string shiftOp: ""
    property string fontFamily: "MI LANTING"

    property int timerInterVal: 50
    signal clicked(string op)
    width: 76
    height: 64

    Image {
        anchors.fill: parent
        id: backgroundRect
        source: keyRelease
    }

    Image {
        id: enlage
        source: keyEnlarge
        opacity:0
        anchors.bottom: parent.top
        anchors.bottomMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            text: buttonText.text
            anchors.centerIn: parent

            font.family: fontFamily
            font.pixelSize: 60
            color: "#292a2c";
            smooth: true
        }

    }
    Text {
        id: buttonText
        text :lettertext
        anchors { centerIn: parent; }
        font.family: fontFamily
        font.pixelSize: 30
        color: "#ffffff";
        smooth: true
        opacity: 1
    }


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: button.clicked(op)
        onPressed: {
            timer.stop()
            enlage.opacity = 1
            backgroundRect.source = keyPress
        }
        onReleased: {
            timer.start()
        }

    }

//    states: State {
//        name: "pressed"; when: mouseArea.pressed == true
//        PropertyChanges { target: enlage; opacity:1 ;explicit:true}
//    }


    function use_sysmbol()
    {
        console.log("use_sysmbol")
        buttonText.text = shiftText
        op = shiftText
    }

    function use_letter()
    {
        console.log("use_letter")

        buttonText.text = lettertext
        op = lettertext

    }


    function set_upper()
    {
        buttonText.text = buttonText.text.toUpperCase()
        op = op.toUpperCase()
    }
    function set_lower()
    {
        buttonText.text = buttonText.text.toLowerCase()
        op = op.toLowerCase()

    }

    Timer{
        id:timer
        interval: timerInterVal;
        repeat: false;
        running: false;

        onTriggered: {
            timer_function();
        }

    }

    function timer_function()
    {
        enlage.opacity = 0
        backgroundRect.source = keyRelease
    }

}
