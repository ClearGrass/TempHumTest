
import QtQuick 1.0

import "../content"
Rectangle {
    property int rootWidth: 232
    property bool english:false
    function get_curValue()
    {
        return add_zero(spinner.currentIndex + 1);
    }
    function set_curValue(value)
    {
        if(value >12)
            value = value -12
        spinner.currentIndex = value -1
    }

    function set_curhour()
    {
       var d = new Date()
       var hour = d.getHours()
       if(hour > 12)
           hour =hour -12;
       spinner.currentIndex = hour -1
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
            id: spinner
            width: rootWidth; height: 194
            focus: true
            model: 12
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
                text: add_zero(index+1) ;
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
