import QtQuick 1.1
import "../baseQml" as BaseQml
import "../../toolQml" as ToolQml
import Setting 1.0

Item{
    id:rootItem
    height: 480
    width: 854
    property bool isHour24: SettingProperty.slot_get24Hours()
    property bool isUSlan: SettingProperty.slot_isUsLang();
    property string strSetDatetime: ""
    z:10
    property bool  isStartTime: false

    property string ampTime: ""
    property string interTime: ""
    signal signal_ok()
    BaseQml.Messagebox{
        id:messageBox
        anchors.centerIn: parent
        leftText:qsTr("Yes")
        rightText:   qsTr("Cancel")
        Text {
            id:title
            text: qsTr("Date and Time")
            font.pixelSize: 26
            font.family: "MI LANTING"
            color : "#cc010101"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 37
            z:24
        }

        Row
        {
            z:40
            anchors.top: parent.top
            anchors.topMargin: 98
            anchors.horizontalCenter: parent.horizontalCenter
            ToolQml.SpinnerDay{
                id:dayEng
                english: isUSlan
                visible: isUSlan
                rootWidth:isHour24 ? 173 : 137
            }
            ToolQml.SpinnerMonth{
                id:month
                english: isUSlan
                onSiangl_curIndexChange: {
                    if(isUSlan)
                        dayEng.setModelDays(getCountDays(month.get_curValue()))
                    else
                        dayChi.setModelDays(getCountDays(month.get_curValue()))
                }
                rootWidth:isHour24 ? 174 : 142

            }
            ToolQml.SpinnerDay{
                id:dayChi
                english: isUSlan
                visible: !isUSlan
                rootWidth:isHour24 ? 173 : 137
            }
            ToolQml.SpinnerAmp{
                id:amp
                visible: !isHour24
                rootWidth:141
            }
            ToolQml.SpinnerHour{
                id:hour24
                english: isUSlan
                visible: isHour24
                rootWidth:174

            }
            ToolQml.SpinnerHour12{
                id:hour12
                english: isUSlan
                visible: !isHour24
                rootWidth: 137

            }

            ToolQml.SpinnerMinute{
                english: isUSlan
                id:minute
                rootWidth:isHour24 ? 174 : 137

            }
        }

        Rectangle{
            z:24
            height: 2
            width: parent.width
            color: "#80010101"
            opacity: 0.2
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 98
        }

        onSignal_rightButtonOnClicked: {
            rootItem.visible = false
        }

        onSignal_leftButtonOnClicked: {
            var h;
            if(SettingProperty.slot_get24Hours() == true)
            {
                h = hour24.get_curValue();
            }
            else
            {
                h = hour12.get_curValue()
                var mp = amp.get_curValue()
                if(h == 12)
                {
                    if(mp == "AM")
                        h = "00";
                    else
                        h = "12"
                }
                else
                {
                    if(mp == "PM")
                    {
                        h = h + 12;
                    }
                }


            }
            var d = new Date();
            var year = d.getFullYear();
            var day = (isUSlan == true ) ? dayEng.get_curValue() : dayChi.get_curValue();
            console.log("day = ", day)
            var dateTime = year + "-" + month.get_curValue() + "-" + day + " " + h + ":" + minute.get_curValue() + ":"+"00";
            console.log(dateTime)
            SettingProperty.slot_setDateTime(dateTime.toString())
            strSetDatetime = dateTime.toString()
            rootItem.visible = false
            signal_ok();
        }

    }
    function initFunction()
    {
        month.set_curMonth()
        dayChi.set_curDay()
        dayEng.set_curDay()
        minute.set_curMini()
        amp.set_curAmp()

        if(isHour24)
        {
            hour24.set_curhour()
        }
        else
        {
            hour12.set_curhour()
        }

    }

    function getCountDays(month)
    {

        var d = new Date();
        var year = d.getFullYear();
        var dat = year + "/" + month + "/" + "1 00:00:00";
        var curDate = new Date(dat);
        /* 获取当前月份 */
        var curMonth = curDate.getMonth();
        /*  生成实际的月份: 由于curMonth会比实际月份小1, 故需加1 */
        curDate.setMonth(curMonth + 1);
        /* 将日期设置为0, 这里为什么要这样设置, 我不知道原因, 这是从网上学来的 */
        curDate.setDate(0);
        /* 返回当月的天数 */

        return curDate.getDate()
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            //
        }
    }
}
