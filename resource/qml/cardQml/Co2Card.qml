/*******************************************************************************
* File Name     :   Co2Card.qml
* Description   :   co2卡片
* Author        :   Jason
* Date          :   2016-9-28
*******************************************************************************/
import QtQuick 1.1
import MainView 1.0
import "../toolQml" as Tool

Item{
    height: 240
    width: 854
    property int sensorCO2BackIndex: 1
    property bool isUsLan: MainViewProperty.slot_isUsLang()

    Item{

        height: 73
        width: 854

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 30
        id:valueInfoItem

        Row{
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 77
            id:valueRow
            Text {
                id: co2eStartValue
                text: "0000"
                //字体
                font.family: "KMedium"
                font.pixelSize: 100
                font.bold: false
                color: "#ffffff"
                opacity: 0.2
                visible: true
            }

            Text {
                id: co2eValueId
                //字体
                font.family: "KMedium"
                font.pixelSize: 100
                font.bold: false
                color: "#ffffff"
                visible: false
            }

        }

        Column{
            id:paraColumn
            anchors.top: parent.top
            anchors.left: valueRow.right
            anchors.leftMargin: 20
            anchors.topMargin: isUsLan ? -10 :-20

            //参数名称
            Tool.ParamName{
                text: qsTr("CO2 Equivalent")
            }

            //参数单位
            Tool.ParamUnit{
                id:unit
                text:"ppm"
            }

        }

    }


    Text {
        id: describeText
        //字体
        font.family: "MI LANTING"
        font.pixelSize: 30
        font.bold: false
        color: "#ffffff"
        wrapMode: Text.Wrap
        width: 650

        anchors.top: valueInfoItem.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 77
        visible: co2eValueId.visible

    }

    Tool.SensorStarting{
        fontColor: "#ffffff"
        fontsize: 30
        anchors.top: valueInfoItem.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 77
        visible: !co2eValueId.visible
    }


    Tool.History{
        anchors.bottom: describeText.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 30
        onSignal_onClicked: {
            MainViewProperty.show_historyPage(3)
        }
    }

    Connections {

        target:MainViewProperty
        onSignal_CO2evaluechanged:
        {
            set_co2Value()
        }

    }


    function set_co2Value()
    {
        var value = Math.round(Number(MainViewProperty.slot_getCO2eValue()));
        if(value == 9999)
        {
            co2eStartValue.text = "0000"
            co2eStartValue.visible = true
            co2eValueId.visible = false
            return ;
        }

        var devise = "";
        var srtDescribe = "";
        if(value <=1000)
        {
            sensorCO2BackIndex = 1
            srtDescribe = qsTr("Normal")
        }
        else
        {

            if(value > 1000 && value <= 2000){
                srtDescribe = qsTr("High")
                sensorCO2BackIndex = 3
            }
            else if(value > 2000 && value <= 3000){
                srtDescribe = qsTr("Very High")
                sensorCO2BackIndex = 4
            }
            else{
                srtDescribe = qsTr("Severely High")
                sensorCO2BackIndex = 5
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

        co2eValueId.text = value.toString()
        co2eValueId.visible = true
        var len =value.toString().length
        switch(len)
        {
        case 1:
            co2eStartValue.text = "000"
            co2eStartValue.visible = true
            break;
        case 2:
            co2eStartValue.text = "00"
            co2eStartValue.visible = true
            break;
        case 3:
            co2eStartValue.text = "0"
            co2eStartValue.visible = true
            break;
        case 4:
            co2eStartValue.text = ""
            co2eStartValue.visible = false
            break;
        default:
            co2eStartValue.text = ""
            co2eStartValue.visible = false
            break;
        }
    }
}
