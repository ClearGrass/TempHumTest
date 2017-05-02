import QtQuick 1.0

import "../content"
Rectangle {
    property int rootWidth: 347
    property bool english:false
    signal siangl_curIndexChange()
    function set_curMonth()
    {
        var d = new Date();
        spinner.currentIndex = d.getMonth()
    }

    function get_curValue()
    {
        return (spinner.currentIndex+1)
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
                text:
                {
                    get_monthFromIndex(index)
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
            onCurrentIndexChanged: {
                siangl_curIndexChange()
            }


        }


    function add_zero(temp)
    {
        if(temp<10) return "0"+temp;
        else return temp;
    }
    function get_monthFromIndex(index)
    {
        var str;
        if(english == false)
        {
            str = index +1 +"月";
        }
        else
        {
            switch(index)
            {
            case 0:
                str = "Jan";
                break;
            case 1:
                str = "Feb";
                break;
            case 2:
                str = "Mar";
                break;
            case 3:
                str = "Apr";
                break;
            case 4:
                str = "May";
                break;
            case 5:
                str = "Jun";
                break;
            case 6:
                str = "Jul";
                break;
            case 7:
                str = "Aug";
                break;
            case 8:
                str = "Sep";
                break;
            case 9:
                str = "Oct";
                break;
            case 10:
                str = "Nov";
                break;
            case 11:
                str = "Dec";
                break;
            }
        }
        return str;
    }

}



