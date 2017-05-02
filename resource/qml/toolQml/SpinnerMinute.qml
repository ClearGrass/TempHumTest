import QtQuick 1.0
import "../content"
Rectangle {
    property bool english:false
    property int rootWidth: 347
    function set_curValue(value)
    {
        spinner.currentIndex = value
    }
    function get_curValue()
    {
        return add_zero(spinner.currentIndex);
    }


    function set_curMini()
    {
        var d = new Date()
        spinner.currentIndex = d.getMinutes()
    }


    clip: true
    width: rootWidth; height: 194
    color: "#00FFFFFF"

        Spinner {
            anchors.centerIn: parent
            id: spinner
            width: rootWidth; height: 194
            focus: true
            model: 60
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
                text: add_zero(index) ;
                color:
                {
                    if(index == spinner.currentIndex)
                        "#22aaff"
                    else
                        '#80010101'
                }
                height: 45
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }

    function add_zero(temp)
    {
        if(temp<10) return "0"+temp;
        else return temp;
    }
}

