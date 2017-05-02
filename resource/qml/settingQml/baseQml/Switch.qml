import QtQuick 1.0

Item {
    id: toggleswitch
    width: background.width; height: background.height

    property string sourceOn: "#33b4ff"
    property string sourceOff: "#bfc0c0"
    property string sourceBackground: "qrc:/resource/images/optionPage/tools/switch_background.png"
    property bool on: false
    signal clicked(bool flag)

    function toggle() {
        if (toggleswitch.state == "on")
        {
            toggleswitch.state = "off";
            toggleswitch.clicked(false)

        }
        else 
        {
            toggleswitch.state = "on";
            toggleswitch.clicked(true)

        }
    }

    function releaseSwitch() {
        if (knob.x == 7) {
            if (toggleswitch.state == "off") return;
        }
        if (knob.x == 42) {
            if (toggleswitch.state == "on") return;
        }
        toggle();

    }

    Image {
        id: background
        source: sourceBackground
        MouseArea { anchors.fill: parent; onClicked: toggle() }
    }

    Rectangle {
        id: knob
        x: 7; y: 7
        radius: 15.5
        height: 31
        width: 31
        color: sourceOff

        MouseArea {
            anchors.fill: parent
            drag.target: knob; drag.axis: Drag.XAxis; drag.minimumX: 7; drag.maximumX: 42
            onClicked: toggle()
            onReleased: releaseSwitch()
        }
    }
    state: on == true ? "on" : "off"
    states: [
        State {
            name: "on"
            PropertyChanges { target: knob; x: 42 }
            PropertyChanges { target: toggleswitch; on: true }
            PropertyChanges { target: knob; color:  sourceOn}

        },
        State {
            name: "off"
            PropertyChanges { target: knob; x: 7 }
            PropertyChanges { target: toggleswitch; on: false }
            PropertyChanges { target: knob; color:  sourceOff}

        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "x"; easing.type: Easing.InOutQuad; duration: 200 }
    }
}
