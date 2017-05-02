/*******************************************************************************
* File Name     :   PmCard.qml
* Description   :   pm2.5卡片
* Author        :   Jason
* Date          :   2016-9-28
*******************************************************************************/
import QtQuick 1.1
import "../toolQml" as Tool
import MainView 1.0

Image
{
    property int outPmBackIndex: 1
    property int sensorPmBackIndex: 1
    property string strAdvise: qsTr("Time to turn on air-purification devices.")
    property bool isUsLang: MainViewProperty.slot_isUsLang()
    height: 480
    width: 854

    Column{
        anchors.left: parent.left
        anchors.leftMargin: 80
        anchors.verticalCenter: parent.verticalCenter

        Row
        {
            id:valueRowInfo
            spacing: 20
            Row{
                id:valueInfoRow
                Text {
                    id: polishing
                    text: "0"
                    //字体
                    font.family: "KMedium"
                    font.pixelSize: 200
                    font.bold: false
                    color: "#ffffff"
                    opacity: 0.2
                }
                Text {
                    id: pmValueId
                    //字体
                    font.family: "KMedium"
                    font.pixelSize: 200
                    font.bold: false
                    color: "#ffffff"
                    visible: false
                }
            }

            Column{
                spacing: 0
                anchors.top: parent.top
                anchors.topMargin: -10
                //参数名称
                Tool.ParamName{
                    id:dataName
                    text: "PM2.5"
                }

                //参数单位
                Tool.ParamUnit{
                    id:dataUnit
                    text:"μg/m³"
                }

                Rectangle{
                    width: 10
                    height: 5
                    color: "#00000000"
                }

                Text{
                    id:timeText
                    font.family: "MI LANTING"
                    font.pixelSize: 24
                    color: "#80fefefe"
                }
            }
        }


        Column
        {
            spacing: 12

            Text {
                id: describe
                font.pixelSize: 30
                font.family: "MI LANTING"
                color : "#ffffff"
                wrapMode: Text.Wrap
                width: 650
            }

            Text {
                id: outEnv
                font.pixelSize: 30
                font.family: "MI LANTING"
                color : "#ffffff"
                visible: false
                wrapMode: Text.Wrap
                width: 650

            }
        }
    }


    Tool.Refresh{
        id:refreshImage
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 30

        onSignal_onClicked: {
            MainViewProperty.refresh_pmData();

        }
    }


    Tool.History{
        id:historyImage
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 30
        onSignal_onClicked: {
            MainViewProperty.show_historyPage(0)
        }
    }

    Connections {

        target:MainViewProperty

        onSignal_pmChanged:
        {
            set_pmValue(MainViewProperty.pmValue)

        }
        onSignal_startPmAnimation:
        {
            set_pmValue(9999)
            timeText.text = qsTr("Detecting")
            timeText.visible = true
            describe.visible = false
            refreshImage.start_animation()
            if(outEnv.visible)
            {
                outEnv.visible = false
                outEnv.visible = true
            }
        }
        onSignal_update_weather:
        {
            set_outDoorWeatherInfo()
        }
        onSignal_pmDetected:
        {
            refreshImage.stop_animation()
            timeText.visible = false
            set_pmDescribe(MainViewProperty.pmValue);

        }
        onSignal_updatePmElapsed:
        {
            timeText.text = MainViewProperty.slot_getpmLastUpdateTime()
            timeText.visible = true
        }
    }



    function set_pmValue(value)
    {
        if(Number(value) >0 && Number(value) <10)
        {
            pmValueId.text  = value
            polishing.text = "00";
            polishing.visible = true
            pmValueId.visible = true

        }
        else if(Number(value) >= 10 && Number(value) < 100)
        {
            pmValueId.text = value
            polishing.text = "0";
            polishing.visible = true
            pmValueId.visible = true

        }
        else if(Number(value) >= 100 && Number(value) < 9999)
        {
            pmValueId.text = value
            polishing.visible = false
            pmValueId.visible = true
        }
        else{
            polishing.text = "000"
            polishing.visible = true
            pmValueId.visible = false

        }

    }

    function set_pmDescribe(value)
    {
        describe.visible = true
        if(Number(value) <= 75)
        {
            sensorPmBackIndex = 1
            describe.text = qsTr("PM2.5 in current environment is low.")
        }
        else if(Number(value) >= 76 && Number(value) <= 115)
        {
            sensorPmBackIndex = 2
            describe.text = qsTr("Current environment is PM2.5 polluted lightly.") + strAdvise
        }
        else if (Number(value) >= 116 && Number(value) <= 150)
        {
            sensorPmBackIndex = 3
            describe.text = qsTr("Current environment is PM2.5 polluted moderately.")+ strAdvise
        }
        else if(Number(value) >= 151 && Number(value) <= 250)
        {
            sensorPmBackIndex = 4
            describe.text = qsTr("Current environment is PM2.5 polluted heavily.")+ strAdvise
        }
        else if(Number(value) >= 251 && Number(value) != 9999)
        {
            sensorPmBackIndex = 5
            describe.text = qsTr("Current environment is PM2.5 polluted severely.")+ strAdvise
        }
        else{
            sensorPmBackIndex = 1
            describe.text = qsTr("Unable to get readings, please try again later.")
        }
    }

    function set_outDoorWeatherInfo()
    {
        var local =  MainViewProperty.slot_getLocaltion()
        var outPm25 = MainViewProperty.slot_getOutDoorPm25()
        var outdoorSr = qsTr("Outdoor PM2.5 ");
        var pmUnit = " μg/m³"
        if(Number(outPm25) <= 35)
        {
            outPmBackIndex = 1
            outEnv.text  = local + ": " + outdoorSr  + outPm25 + pmUnit  + qsTr(", air quality is excellent.")
            outEnv.visible =  true

        }
        else if(Number(outPm25) >= 36 && Number(outPm25) <= 75)
        {
            outPmBackIndex = 1
            outEnv.text  =  local + ": " +  outdoorSr  + outPm25 + pmUnit  + qsTr(", Air quality is good.")
            outEnv.visible =  true

        }
        else if(Number(outPm25) >= 76 && Number(outPm25) <= 115)
        {
            outPmBackIndex = 2
            outEnv.text  = local + ": " + outdoorSr +  outPm25 + pmUnit  + qsTr(", lightly polluted.")
            outEnv.visible =  true

        }
        else if (Number(outPm25) >= 116 && Number(outPm25) <= 150)
        {
            outPmBackIndex = 3
            outEnv.text  =  local + ": " +  outdoorSr +  outPm25 + pmUnit  + qsTr(", moderately polluted.")
            outEnv.visible =  true

        }
        else if(Number(outPm25) >= 151 && Number(outPm25) <= 250)
        {
            outPmBackIndex = 4
            outEnv.text  =  local + ": " +  outdoorSr +  outPm25 + pmUnit  + qsTr(", heavily polluted.")
            outEnv.visible =  true

        }
        else if(Number(outPm25) >= 251 && Number(outPm25) != 9999)
        {
            outPmBackIndex = 5
            outEnv.text  = local + ": " +  outdoorSr +  outPm25 + pmUnit  + qsTr(", severely polluted.")
            outEnv.visible =  true

        }
        else{
            outPmBackIndex = 1
            outEnv.visible = false
        }

    }

}
