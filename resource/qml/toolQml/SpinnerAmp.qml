import QtQuick 1.0

import "../content"
Rectangle {
    property int rootWidth: 230
    function set_curAmp()
    {
        var d = new Date()
        var hour = d.getHours()
        if(hour > 12)
        {
            spinner.currentIndex = 0
        }
        else
        {
            spinner.currentIndex = 1

        }
    }

    function get_curValue()
    {
        if(spinner.currentIndex == 0 || spinner.currentIndex == 2)
        {
            return "PM"
        }
        else{
            return "AM"
        }
    }

    function set_curValue(value)
    {
        if(value  == "AM")
        {
            spinner.currentIndex = 1
        }
        else
        {
            spinner.currentIndex = 0
        }
    }


    function updateX(x)
    {
        spinner.x= x;
    }
    function updateWidth(w)
    {
        Rectangle.width = w;
        spinner.width = w;
    }
    clip: true
    width: rootWidth; height: 194
    color: "#00FFFFFF"
    Rectangle{
        height: 194
        width: 2
        color: "#80010101"
        opacity: 0.2

        anchors.right: parent.right
        anchors.top: parent.top
    }

    Spinner {
        anchors.centerIn: parent

        id: spinner
        width: rootWidth; height: 194
        focus: true
        model: 4
        itemHeight: 45
        delegate: Text {
            font.pixelSize:
            {
                if(index == spinner.currentIndex)
                    36
                else
                    26
            }
            text:
            {
                if(0 == index || 2 == index)  qsTr("PM")
                else           qsTr("AM")


            }
            color:
            {
                if(index == spinner.currentIndex)
                    "#22aaff"
                else
                    '#80010101'
            }
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            height: 45

        }
    }

}

