import QtQuick 1.1
import MainView 1.0
/*******************************************************************************
* File Name     :   TimeAndDate.qml
* Description   :   时间日期界面:主要显示当前时间日期，pm2.5、温湿度、co2、tVOC等信息
* Author        :   Jason
*******************************************************************************/
Item
{
    property bool timeMode:  MainViewProperty.slot_getTimeMode()
    property bool isUSlan: MainViewProperty.slot_isUsLang();
    id:timeAndDate

    //动画持续时间
    property int aniDuration: 750

    //定时器间隔时间
    property int timerInterval: 1000

    property string dateLabel: ""
    property string weekLabel: ""


    width: parent.width
    height: parent.height

    //布局
    anchors.left: parent.left
    anchors.top:  parent.top

    Column{
        id:anchorColumn
        anchors.centerIn: parent
        Row{
            id:timeRow
            spacing: 20
            Text {
                id: amp
                font.pixelSize: 36
                font.family: "MI LANTING"
                color : "#ffffff"

            }
            Row
            {

                spacing: 10
                Text {
                    id:hourzText
                    //字体
                    font.family: "KMedium"
                    font.pixelSize: 200
                    font.bold: false
                    color: "#ffffff"
                    //                font.letterSpacing: -5
                }

                Column{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -20
                    spacing: 40
                    Rectangle{
                        height: 20
                        width: 20
                        radius: 10
                    }
                    Rectangle{
                        height: 20
                        width: 20
                        radius: 10
                    }
                }

                Text {
                    id:minuteText
                    //字体
                    font.family: "KMedium"
                    font.pixelSize: 200
                    font.bold: false
                    color: "#ffffff"
                    //                font.letterSpacing: -5
                }

            }

            Column{
                spacing: 10
                anchors.top: parent.top
                anchors.topMargin: -10

                Text {
                    id: dataAndWeek
                    text: dateLabel + weekLabel
                    font.pixelSize: 36
                    font.family: "MI LANTING"
                    color : "#ffffff"
                    horizontalAlignment: Text.AlignRight
                }


                Row{
                    id:outWeatherRow
                    visible: false
                    spacing: 11
                    Text {
                        id: weatherText
                        font.pixelSize: 36
                        font.family: "MI LANTING"
                        color : "#ffffff"
                    }
                    Image {
                        source: "qrc:/resource/images/mainPage/button/enter.png"
                        anchors.verticalCenter: parent.verticalCenter
                        MouseArea{
                            height: 94
                            width: 200
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            onClicked: {
                                MainViewProperty.show_historyPage(5)
                            }
                            onPressed: {
                                parent.opacity = 0.5
                            }
                            onReleased: {
                                parent.opacity = 1.0
                            }
                        }
                    }

                }
            }
        }


        Column
        {
            anchors.left: parent.left
            anchors.leftMargin: 20

            spacing: 10
            Text {
                id: describe
                font.pixelSize: 30
                font.family: "MI LANTING"
                color : "#ffffff"
                visible: false
                width: 704
                wrapMode: Text.Wrap

            }
            Text {
                id: outEnv
                font.pixelSize: 30
                font.family: "MI LANTING"
                color : "#ffffff"
                visible: false
                width: 704
                wrapMode: Text.Wrap
            }
        }
    }


    Connections {

        target:MainViewProperty

        onSignal_timeModeChange:
        {
            timeMode = MainViewProperty.slot_getTimeMode()
        }
        onSignal_update_weather:
        {
            set_weatherInfo();
            set_outDoorWeatherInfo()
        }
        onSignal_CO2evaluechanged:
        {
            set_curEnvInfo()
        }
    }

    //时间定时器 一秒执行一次
    Timer{
        interval: timerInterval;
        repeat: true;
        triggeredOnStart: true;
        running: true;

        onTriggered: {
            set_dataTime();
        }

    }

    //
    function set_dataTime()
    {
        var d = new Date();
        var week;
        switch (d.getDay()){
        case 1: week= qsTr("Mon"); break;
        case 2: week=qsTr("Tue"); break;
        case 3: week=qsTr("Wed"); break;
        case 4: week=qsTr("Thu"); break;
        case 5: week=qsTr("Fri"); break;
        case 6: week=qsTr("Sat"); break;
        default: week=qsTr("Sun");
        }
        var month ;
        if(isUSlan == true)
        {
            month = get_englishMonthStr(d.getMonth())
        }
        else
        {
            month =  d.getMonth()+1;
        }

        var days = d.getDate();
        var hours = d.getHours();
        var minutes = d.getMinutes();
        var seconds=d.getSeconds();

        //12小时制
        if(timeMode == false)
        {
            if(hours == 12)
            {
                amp.text = qsTr("PM")
            }
            else if (hours == 0)
            {
                hours = 12
                amp.text = qsTr("AM")
            }
            else if(hours > 12)
            {
                hours = hours -12
                amp.text = qsTr("PM")
            }
            else
            {
                amp.text = qsTr("AM")
            }


            amp.visible = true

        }
        else
        {
            //24小时制
            amp.visible = false
        }

        hourzText.text = hours
        minuteText.text = add_zero(minutes)

        //英文
        if(isUSlan == true)
        {
            weekLabel = days + " " + month;
            dateLabel = week + ",\n"
        }
        else
        {
            weekLabel = week;
            dateLabel = month + "月" + days +"日 ";
        }

    }
    function add_zero(temp)
    {
        if(temp<10) return "0"+temp;
        else return temp;
    }

    function get_englishMonthStr(month)
    {
        var str = "";
        switch(month)
        {
        case 0:
            str = "January";
            break;
        case 1:
            str = "February";
            break;
        case 2:
            str = "March";
            break;
        case 3:
            str = "April";
            break;
        case 4:
            str = "May";
            break;
        case 5:
            str = "June";
            break;
        case 6:
            str = "July";
            break;
        case 7:
            str = "August";
            break;
        case 8:
            str = "September";
            break;
        case 9:
            str = "October";
            break;
        case 10:
            str = "November";
            break;
        case 11:
            str = "December";
            break;
        }
        return str;
    }
    function set_opacity(value)
    {
        outdoor.set_iconOpacity(value)
        indoor.set_opacity(value)
    }

    function set_weatherInfo()
    {
        var des = MainViewProperty.slot_getOutDoorWeatherIcon()
        outWeatherRow.visible = true
        anchorColumn.visible = false
        anchorColumn.visible = true
        var strTmp = ""
        if(des == "CLEAR_DAY")
        {
            strTmp = qsTr("Sunny");
        }
        else if (des == "CLEAR_NIGHT")
        {
            strTmp = qsTr("Clear");

        }
        else if ((des == "PARTLY_CLOUDY_DAY")|| (des == "PARTLY_CLOUDY_NIGHT"))
        {
            strTmp = qsTr("Partly Cloudy");
        }
        else if(des == "CLOUDY")
        {
            strTmp = qsTr("Cloudy");
        }
        else if(des == "RAIN")
        {
            strTmp = qsTr("Rain");
        }
        else if(des == "SNOW")
        {
            strTmp = qsTr("Snow");
        }
        else if(des == "WIND")
        {
            strTmp = qsTr("Windy");
        }
        else if(des == "FOG")
        {
            strTmp = qsTr("Foggy");
        }
        else if(des == "HAZE")
        {
            strTmp = qsTr("Haze");
        }
        else if(des == "SLEET")
        {
            strTmp = qsTr("Sleet");
        }
        else
        {
            outWeatherRow.visible = false
        }
        weatherText.text = strTmp
    }

    function set_outDoorWeatherInfo()
    {
        var local =  MainViewProperty.slot_getLocaltion()
        var outPm25 = MainViewProperty.slot_getOutDoorPm25()
        var outTemp = MainViewProperty.slot_getOutDoorTemp();
        var tmpUnit = MainViewProperty.get_tempUnit() == 0 ? "℃" : "℉"
        var outdoorSr = qsTr(": Outdoor Temp ");
        var pmUnit = " μg/m³"
        var allValues = local + outdoorSr + outTemp  + tmpUnit + qsTr(" outdoor, PM2.5 is ") +outPm25 + pmUnit + qsTr(", ");
        if(Number(outPm25) <= 35)
        {
            outEnv.text  = allValues + qsTr("Air quality is excellent.")
            outEnv.visible =  true

        }
        else if(Number(outPm25) >= 36 && Number(outPm25) <= 75)
        {
            outEnv.text  =allValues + qsTr("Air quality is good.")
            outEnv.visible =  true

        }
        else if(Number(outPm25) >= 76 && Number(outPm25) <= 115)
        {
            outEnv.text  =allValues + qsTr("Lightly polluted.")
            outEnv.visible =  true

        }
        else if (Number(outPm25) >= 116 && Number(outPm25) <= 150)
        {
            outEnv.text  =allValues + qsTr("Moderately polluted.")
            outEnv.visible =  true

        }
        else if(Number(outPm25) >= 151 && Number(outPm25) <= 250)
        {
            outEnv.text  =allValues + qsTr("Heavily polluted.")
            outEnv.visible =  true

        }
        else if(Number(outPm25) >= 251 && Number(outPm25) != 9999)
        {
            outEnv.text  =allValues + qsTr("Severely polluted.")
            outEnv.visible =  true

        }
        else{
            outEnv.visible = false
        }
    }

    function  set_curEnvInfo()
    {
        var curPm25Value = MainViewProperty.pmValue;
        var tvocUnit = Math.round(Number(MainViewProperty.slot_getTvocValue()) * 1000) / 1000;
        var tvocValue = MainViewProperty.slot_getTvocValue()
        var co2eValue = MainViewProperty.slot_getCO2eValue()
//        console.log("<<<<<<<<<<<<<<<<<<<<<<<<<Current environment>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
//        console.log("pm = " ,curPm25Value)
//        console.log("co2eValue = " ,co2eValue)
//        console.log("tvocValue = " ,tvocValue)

//        console.log("<<<<<<<<<<<<<<<<<<<<<<<<<Current environment>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")

        var pmUnit = " μg/m³"
        var curEnvDesStr = qsTr("Current environment: ");
        var strTmp = ""
        if(curPm25Value <= 75 && tvocValue <= 0.6 && co2eValue <= 1000)
        {
            curEnvDesStr = curEnvDesStr + qsTr("all readings are good.")
        }
        else if(curPm25Value > 75 && tvocValue <= 0.6 && co2eValue <= 1000)
        {
            curEnvDesStr = curEnvDesStr + qsTr("PM2.5 is ") + curPm25Value + pmUnit
            if(curPm25Value <= 115)
            {
                curEnvDesStr  = curEnvDesStr + qsTr(", ") + qsTr("lightly polluted.")
            }
            else if(curPm25Value > 115 && curPm25Value < 150)
            {
                curEnvDesStr  = curEnvDesStr + qsTr(", ") + qsTr("moderately polluted.")
            }
            else if(curPm25Value > 150 && curPm25Value < 250)
            {
                curEnvDesStr  = curEnvDesStr + qsTr(", ") + qsTr("heavily polluted.")
            }
            else
            {
                curEnvDesStr  = curEnvDesStr + qsTr(", ") + qsTr("severely polluted.")
            }
        }
        else if(tvocValue > 0.6 && curPm25Value <= 75 &&  co2eValue <=1000)
        {
            if(tvocValue <= 3)
            {
                curEnvDesStr = curEnvDesStr + qsTr("tVOC is ") + tvocValue + " " +tvocUnit + qsTr(", ") + qsTr("lightly polluted.")
            }
            else
            {
                curEnvDesStr = curEnvDesStr + qsTr("tVOC is ") + tvocValue + " " +tvocUnit + qsTr(", ") + qsTr("severely polluted.")
            }
        }
        else if(curPm25Value <= 75 && tvocValue <= 0.6 && co2eValue > 1000)
        {
            if(co2eValue <= 2000)
            {
                curEnvDesStr = curEnvDesStr + qsTr("CO2e is ") + co2eValue + " ppm, " + qsTr("high.")
            }
            else if(co2eValue > 2000 && co2eValue <= 3000)
            {
                curEnvDesStr = curEnvDesStr + qsTr("CO2e is ") + co2eValue + " ppm, " + qsTr("very high.")
            }
            else
            {
                curEnvDesStr = curEnvDesStr + qsTr("CO2e is ") + co2eValue + " ppm, " + qsTr("excessively high.")
            }
        }
        else if(curPm25Value > 75 && tvocValue > 0.6 && co2eValue <= 1000)
        {
            curEnvDesStr = curEnvDesStr + qsTr("PM2.5 and tVOC are too high.")
        }
        else if(curPm25Value > 75 && tvocValue <= 0.6 && co2eValue > 1000)
        {
            curEnvDesStr = curEnvDesStr + qsTr("PM2.5 and CO2e are too high.")
        }
        else if(curPm25Value <= 75 && tvocValue > 0.6 && co2eValue > 1000)
        {
            curEnvDesStr = curEnvDesStr + qsTr("tVOC and CO2e are too high.")
        }
        else
        {
            curEnvDesStr = curEnvDesStr + qsTr("PM2.5 and tVOC, CO2, are too high.")
        }
        describe.visible = true
        anchorColumn.visible = false
        anchorColumn.visible = true
        describe.text = curEnvDesStr

    }


}
