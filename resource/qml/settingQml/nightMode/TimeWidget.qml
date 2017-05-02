import QtQuick 1.1
import "../baseQml" as BaseQml
import "../../toolQml" as ToolQml
import Setting 1.0

Item{
    id:rootItem
    property alias text: title.text
    height: 854
    width: 480
    property bool isHour24: SettingProperty.slot_get24Hours()
    property bool isUSlan: SettingProperty.slot_isUsLang();
    z:20
    property bool  isStartTime: false

    property string ampTime: ""
    property string interTime: ""
    signal signal_ok()
    BaseQml.Messagebox{
        id:messageBox
        anchors.centerIn: parent
        leftText: qsTr("Yes")
        rightText: qsTr("Cancel")
        z:23
        Text {
            id:title
            text: "开始"
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
            ToolQml.SpinnerAmp{
                id:amp
                visible: !isHour24
                anchors.verticalCenter: parent.verticalCenter
            }
            ToolQml.SpinnerHour{
                id:hour
                visible: isHour24
                english: isUSlan

            }
            ToolQml.SpinnerHour12{
                id:hour12
                visible: !isHour24
                english: isUSlan
            }
            ToolQml.SpinnerMinute{
                id:minute
                english: isUSlan
                rootWidth: isHour24 ? 347 : 232

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

        onSignal_leftButtonOnClicked : {
            if(isHour24)
            {
                SettingProperty.slot_setNightIntrTime(hour.get_curValue() +":" + minute.get_curValue(), isStartTime)
            }
            else
            {
                SettingProperty.slot_setNightIntrTime(hour12.get_curValue()  + ":" + minute.get_curValue() + " " + amp.get_curValue() , isStartTime)
            }

            rootItem.visible = false
            signal_ok();
        }

    }
    function initFunction()
    {
        interTime = SettingProperty.slot_getNightInterTime(isStartTime ? 1 : 2);
        if(isHour24)
        {
            hour.set_curValue(Number(interTime.substring(0,interTime.indexOf(":"))))
        }
        else
        {
            ampTime = SettingProperty.slot_getAmp(isStartTime ? 1 : 2);
            hour12.set_curValue(Number(interTime.substring(0,interTime.indexOf(":"))))
        }

        minute.set_curValue(Number(interTime.substring(interTime.lastIndexOf(":")+1)))

    }

    MouseArea{
        z:12
        anchors.fill: parent
        onClicked: {

        }
    }
}
