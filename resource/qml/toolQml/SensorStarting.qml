import QtQuick 1.1

Item{
    height: 30
    width: 200
    property string fontColor: "#ffffff"
    property int fontsize: 24
    Row{
        Text {
            text: qsTr("Starting")
            font.family: "MI LANTING"
            font.pixelSize: fontsize
            color: fontColor
        }
        Text {
            id:timerText
            text: "."
            font.family: "MI LANTING"
            font.pixelSize: fontsize
            color: fontColor
            horizontalAlignment: Text.AlignLeft
            width: 20
        }
    }
    Timer{
        id:timer
        interval:300;
        repeat: true;
        running: parent.visible;

        onTriggered: {
            time_function();
        }

    }

    function time_function()
    {
        if(timerText.text == ".")
        {
            timerText.text = ".."
        }
        else if(timerText.text == "..")
        {
            timerText.text = "..."
        }
        else
        {
            timerText.text = "."
        }
    }
}
