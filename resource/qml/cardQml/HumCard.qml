/*******************************************************************************
* File Name     :   humCard.qml
* Description   :   湿度卡片
* Author        :   Jason
* Date          :   2016-9-28
*******************************************************************************/
import QtQuick 1.1
import MainView 1.0
import "../toolQml" as Tool

Item{
    height: 240
    width: 854
    property bool isUsLan: MainViewProperty.slot_isUsLang()

    Item{

        height: 73
        width: 854

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 20
        id:valueInfoItem

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 77

            id: humValueId
            //字体
            font.family: "KMedium"
            font.pixelSize: 100
            font.bold: false
            color: "#ffffff"
        }
        Column{
            anchors.top: parent.top
            anchors.left: humValueId.right
            anchors.leftMargin: 30
            anchors.topMargin: isUsLan ? 2 : -8

            //参数名称
            Tool.ParamName{
                text:qsTr("Relative Humidity")
            }

            //参数单位
            Tool.ParamUnit{
                id:unit
                text:"%"
            }

        }
    }

    Text {
        anchors.top: valueInfoItem.bottom
        anchors.topMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 77

        id: describeText
        text: ""
        //字体
        font.family: "MI LANTING"
        font.pixelSize: 30
        font.bold: false
        color: "#ffffff"
        wrapMode: Text.Wrap
        width: 650
    }


    Tool.History{
        anchors.bottom: describeText.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 30
        onSignal_onClicked: {
            MainViewProperty.show_historyPage(2)
        }
    }

    Connections {
        target:MainViewProperty

        //修改主值
        onSignal_humChanged:
        {
            humValueId.text = MainViewProperty.slot_getHumValue();
            set_humDescribe()
        }
    }

    function set_humDescribe()
    {
        var value = Number(humValueId.text)
        if(value <= 19)
        {
              describeText.text = qsTr("Very dry, turning on humidifiers is recommended.")
        }
        else if(value > 19 && value <= 44)
        {
            describeText.text = qsTr("Dry, turning on humidifiers is recommended.")
        }
        else if(value > 44 && value <= 60)
        {
            describeText.text = qsTr("Optimum")
        }
        else if(value > 60 && value <= 70)
        {
            describeText.text = qsTr("Moist")
        }
        else if(value > 70 && value <= 80)
        {
            describeText.text = qsTr("Wet, turning on dehumidifiers is recommended.")
        }
        else
        {
            describeText.text = qsTr("Very wet, turning on dehumidifiers is recommended.")
        }
    }

}
