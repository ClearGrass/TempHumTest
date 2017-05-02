import QtQuick 1.1
import MainView 1.0
import "../toolQml" as ToolQml

Rectangle {

    color: "#ffffff"
    width: 480; height: 854

    WarningPage{
        anchors.centerIn: parent
        visible: false
    }


    PmBlock{
        id:pm25
        anchors.left: parent.left
        anchors.top: parent.top
    }

    Column{
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        BaseBlock{
            id:tVocBlock
            dataName: qsTr("tVOC")
            dataDescribe:""
            dataColor: "#585858"
            dataValue:"0"
            dataUnit:get_tvocUnit()
            sensorstarting:true

        }
        Rectangle{
            height: 2
            width: 480
            color: "#cecece"
        }

        BaseBlock{
            id:co2eBlock
            dataName: qsTr("CO2 Equivalen")
            dataDescribe:""
            dataColor: "#585858"
            dataValue:"0"
            dataUnit:"ppm"
            sensorstarting:true

        }
        Rectangle{
            height: 2
            width: 480
            color: "#cecece"
        }
        BaseBlock{
            id:tempBlock
            dataName: qsTr("Temperature")
            dataDescribe:""
            dataColor: "#1190e2"
            dataUnit:get_TempUnit()
        }
        Rectangle{
            height: 2
            width: 480
            color: "#cecece"
        }
        BaseBlock{
            id:humBlock
            dataName: qsTr("Relative Humidity")
            dataColor: "#1190e2"
            dataUnit:"%"
        }
    }


    //获取温度单位
    function get_TempUnit()
    {
        if(MainViewProperty.get_tempUnit() == 0)
        {
            return "℃";
        }
        else
        {
            return "℉";
        }
    }


    function get_tvocUnit()
    {
        var unit = MainViewProperty.slot_getTvocUnit()
        if(unit =="mg/m3" )
        {
            return  "mg/m³"
        }
        else
        {
            return unit
        }
    }


    //信号槽连接
    Connections {

        target:MainViewProperty


        onSignal_tempChanged:
        {
            tempBlock.dataValue = MainViewProperty.slot_getTempValue()
        }
        onSignal_humChanged:
        {
            humBlock.dataValue  = MainViewProperty.slot_getHumValue()
            set_humDescribe()
        }

        onSignal_tempUnitChanged:
        {
            tempBlock.dataUnit = get_TempUnit()
        }
        onSignal_tvocUnitChange:
        {
            co2eBlock.dataUnit = get_tvocUnit()
        }
        onSignal_tVOCValueChanged:
        {
            tVocBlock.sensorstarting = false
            set_tvocValue()
        }
        onSignal_CO2evaluechanged:
        {
            co2eBlock.sensorstarting = false
            set_co2Value()
        }
    }

    function set_humDescribe()
    {
        var value = Number(humBlock.dataValue)
        if(value <= 19)
        {
            humBlock.dataDescribe = qsTr("Very dry")
        }
        else if(value > 19 && value <= 44)
        {
            humBlock.dataDescribe = qsTr("Dry")
        }
        else if(value > 44 && value <= 60)
        {
            humBlock.dataDescribe = qsTr("Optimum")
        }
        else if(value > 60 && value <= 70)
        {
            humBlock.dataDescribe = qsTr("Moist")
        }
        else if(value > 70 && value <= 80)
        {
            humBlock.dataDescribe = qsTr("Wet")
        }
        else
        {
            humBlock.dataDescribe = qsTr("Very wet")
        }
    }

    function set_tvocValue()
    {
        var value = Math.round(Number(MainViewProperty.slot_getTvocValue()) * 1000) /1000
        var fontColor = "";
        var desc = ""
        if(value <= 0.2)
        {
            desc = qsTr("Excellent")
            fontColor = "#1190e2" ;
        }
        else if(value > 0.2 && value <= 0.3)
        {
            desc = qsTr("Good")
            fontColor = "#1190e2" ;

        }
        else if(value > 0.3 && value <= 0.6)
        {
            desc = qsTr("Fair")
            fontColor = "#1190e2" ;

        }
        else if(value > 0.6 && value <= 3)
        {
            fontColor = "#ff4c41"
            desc = qsTr("Lightly polluted")
        }
        else
        {
            desc = qsTr("Severely polluted")
            fontColor = "#a30529"
        }
        tVocBlock.dataColor = fontColor
        tVocBlock.dataDescribe = desc
        tVocBlock.dataValue = value
    }

    function set_co2Value()
    {
        var value = Number(MainViewProperty.slot_getCO2eValue())
        var fontColor = "";
        var desc = ""
        if(value <= 1000)
        {
            fontColor = "#1190e2" ;
            desc = qsTr("Normal")
        }
        else if(value > 1000 && value <= 2000)
        {
            fontColor = "#ff821b" ;
            desc = qsTr("High")
        }
        else if(value > 2000 && value <= 3000)
        {
            fontColor = "#ff4c41" ;
            desc = qsTr("Very High")
        }
        else
        {
            fontColor = "#a30529" ;
            desc = qsTr("co2Excessive")
        }
        co2eBlock.dataValue = value.toString()
        co2eBlock.dataColor = fontColor
        co2eBlock.dataDescribe = desc;
    }

}
