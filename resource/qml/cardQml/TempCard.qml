/*******************************************************************************
* File Name     :   TempCard.qml
* Description   :   温度卡片
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
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 60
        Text {
            anchors.top: parent.top
            anchors.topMargin: -20
            id: tempValueId
            //字体
            font.family: "KMedium"
            font.pixelSize: 100
            font.bold: false
            color: "#ffffff"

            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 77
        }

        Column{
            anchors.top: parent.top
            anchors.left: tempValueId.right
            anchors.leftMargin: 30
            anchors.topMargin: isUsLan ? 12 :3

            //参数名称
            Tool.ParamName{
                text:qsTr("Temperature")
            }

            //参数单位
            Tool.ParamUnit{
                id:unit
                text:get_unit()
            }

        }

        Tool.History{
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 30
            onSignal_onClicked: {
                MainViewProperty.show_historyPage(1)
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
        onSignal_tempChanged:
        {
            tempValueId.text = MainViewProperty.slot_getTempValue();
        }
        onSignal_tempUnitChanged:
        {
            unit.text = get_unit()
        }
    }

    function get_unit()
    {
        if(MainViewProperty.get_tempUnit() == 0)
        {
            return  "℃";
        }
        else
        {
            return "℉"
        }

    }

}
