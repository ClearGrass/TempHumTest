import QtQuick 1.0
import "../content"
Rectangle {
    property int rootWidth: 347
    property bool english:false
    function set_curValue(value)
    {
        spinner.currentIndex = value
    }
    function get_curValue()
    {
        return add_zero(spinner.currentIndex);
    }
    function set_curhour()
    {
        var d = new Date()
        spinner.currentIndex = d.getHours()
    }
    Rectangle{
        height: 194
        width: 2
        color: "#80010101"
        opacity: 0.2

        anchors.right: parent.right
        anchors.top: parent.top
    }

    width: rootWidth; height: 194
    color: "#00FFFFFF"
    clip: true

    Spinner {
        anchors.centerIn: parent
        id: spinner
        width: rootWidth; height: 194
        focus: true
        model: 24
        itemHeight: 45
        delegate: Text {
            font.family:  "MI LANTING"
            font.pixelSize:
            {
                if(index == spinner.currentIndex)
                    36
                else
                    26
            }
            text: add_zero(index);
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
    function add_zero(temp)
    {
        //        if(temp<10) return "0"+temp;
        //        else return temp;
        return temp;
    }
}

