/*******************************************************************************
* File Name     :   TvocCard.qml
* Description   :   tVoc卡片
* Author        :   Jason
* Date          :   2016-9-28
*******************************************************************************/
import QtQuick 1.1
import MainView 1.0
import "../toolQml" as Tool

Item{
    height: 240
    width: 854
    property int sensortVOCBackIndex: 1
    property bool isUsLan: MainViewProperty.slot_isUsLang()

    Row{
        id:descRow
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 77
        Text {
            id: describeText
            //字体
            font.family: "MI LANTING"
            font.pixelSize: 30
            font.bold: false
            color: "#ffffff"
            wrapMode: Text.Wrap
            width: 650

            visible: tVocValueId.visible
        }
        Tool.SensorStarting{
            fontColor: "#ffffff"
            fontsize: 30
            visible: !tVocValueId.visible
        }
    }


    Item{
        height: 73
        width: 854

        anchors.bottom: descRow.top
        anchors.left: parent.left
        anchors.bottomMargin: 16

        Row{
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 77
            id:valueRow
            //值
            Text {
                anchors.verticalCenter: parent.verticalCenter
                id: tVocValueId
                //字体
                font.family: "KMedium"
                font.pixelSize: 100
                font.bold: false
                color: "#ffffff"
                visible: false
            }
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 12
                id: tVocStartValue
                text: "0.000"
                //字体
                font.family: "KMedium"
                font.pixelSize: 100
                font.bold: false
                color: "#ffffff"
                opacity: 0.2
                visible: true
            }
        }

        //描述 & 单位
        Column{
            anchors.top: parent.top
            anchors.left: valueRow.right
            anchors.leftMargin: 20
            anchors.topMargin: isUsLan ? 2 : -7
            //参数名称
            Tool.ParamName{
                text: qsTr("tVOC")
            }

            //参数单位
            Tool.ParamUnit{
                id:unit
                text: MainViewProperty.slot_getTvocUnit()
            }

        }


        Tool.History{
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 30
            onSignal_onClicked: {
                MainViewProperty.show_historyPage(4)
            }
        }

    }


    Rectangle{
        height: 1
        width: 744
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 80
        color: "#ffffff"
        opacity: 0.3
    }

    Connections{
        target: MainViewProperty
        onSignal_tvocUnitChange:
        {
            unit.text =  MainViewProperty.slot_getTvocUnit()
        }
        onSignal_tVOCValueChanged:
        {
            set_tVOCValue()
        }
    }

    function set_tVOCValue()
    {
        var value = Math.round(Number(MainViewProperty.slot_getTvocValue()) * 1000) / 1000;
        var devise = "";
        var srtDescribe = "";
        if(value <= 0.2)
        {
            srtDescribe = qsTr("Excellent")
            sensortVOCBackIndex = 1
        }
        else if(value > 0.2 && value <= 0.3){
            srtDescribe = qsTr("Good")
            sensortVOCBackIndex = 1

        }
        else if(value > 0.3 && value <= 0.6){
            srtDescribe = qsTr("Fair")
            sensortVOCBackIndex = 1

        }
        else
        {
            if(value > 0.6 && value <= 3){
                srtDescribe = qsTr("Lightly polluted")
                sensortVOCBackIndex = 4
            }
            else
            {
                srtDescribe = qsTr("Severely polluted")
                sensortVOCBackIndex = 5
            }

            var pmValue = Number(MainViewProperty.pmValue)
            if(pmValue <= 75)
            {
                devise = qsTr(", Time to open the window.")
            }
            else if(pmValue > 75 && pmValue != 9999 )
            {
                devise = qsTr(", Time to turn on ventilation devices.")
            }

        }
        describeText.text = srtDescribe + devise

        tVocValueId.text = value.toString()
        tVocValueId.visible = true
        var len =value.toString().length
        switch(len)
        {
        case 1:
//            tVocValueId.text += "."
            tVocStartValue.text = ".000"
            tVocStartValue.visible = true
            tVocValueId.anchors.verticalCenterOffset = 12
            tVocStartValue.anchors.verticalCenterOffset = 0

            break;
        case 3:
            tVocStartValue.text = "00"
            tVocStartValue.visible = true
            tVocValueId.anchors.verticalCenterOffset = 0
            tVocStartValue.anchors.verticalCenterOffset = 12
            break;
        case 4:
            tVocStartValue.text = "0"
            tVocStartValue.visible = true
            tVocValueId.anchors.verticalCenterOffset = 0
            tVocStartValue.anchors.verticalCenterOffset = 12
            break;
        case 5:
            tVocStartValue.text = ""
            tVocStartValue.visible = false
            tVocValueId.anchors.verticalCenterOffset = 0
            tVocStartValue.anchors.verticalCenterOffset = 12
            break;
        default:
            tVocStartValue.text = ""
            tVocStartValue.visible = false
            tVocValueId.anchors.verticalCenterOffset = 0
            tVocStartValue.anchors.verticalCenterOffset = 12
            break;
        }

    }


}
