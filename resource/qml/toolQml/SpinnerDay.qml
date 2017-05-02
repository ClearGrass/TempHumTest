import QtQuick 1.0

import "../content"
Rectangle {
    property bool english:false
    property int rootWidth: 347

    function set_curDay()
    {
        var d = new Date()
        spinner.currentIndex = d.getDate()-1
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


    clip: true
    width: rootWidth; height: 194
    color: "#00FFFFFF"
    Spinner {
        id: spinner
        width: rootWidth; height: 194
        focus: true
        model: get_curModelDays()
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
                get_dayFromIndex(index)
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

    function add_zero(temp)
    {
        //        if(temp<10) return "0"+temp;
        //        else return temp;
        return temp;
    }

    function get_dayFromIndex(index)
    {
        var day = "";

        if(english == true)
        {
            day = add_zero(index+1);
        }
        else
        {
            day = add_zero(index+1)+"日"
        }
        return day;
    }

    function setModelDays(days)
    {
        if(days != spinner.model)
            spinner.model = days

    }

    function get_curModelDays()
    {
        var curDate = new Date();
        /* 获取当前月份 */
        var curMonth = curDate.getMonth();
        /*  生成实际的月份: 由于curMonth会比实际月份小1, 故需加1 */
        curDate.setMonth(curMonth + 1);
        /* 将日期设置为0 */
        curDate.setDate(0);
        /* 返回当月的天数 */
        return  curDate.getDate()
    }

}



