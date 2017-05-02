import QtQuick 1.1
import MainView 1.0
import "../toolQml" as ToolQml
Image {
    property alias rootSource: rootImage.source
    property string scanningSource: "qrc:/resource/images/verticalScreen/starting.png"
    property string refreshSource: "qrc:/resource/images/verticalScreen/refresh.png"
    property bool allowRefresh: MainViewProperty.slot_getVerticalScreenRefresh()
    id: rootImage
    source: "qrc:/resource/images/verticalScreen/pmBack_good_.png"
    clip: true
    z:0
    Image {
        id: refreshImage
        source: refreshSource
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 25
        visible: allowRefresh
        z:1
        Text {
            width: 240
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 150
            text: qsTr("Press the button to refresh")
            font.family: "MI LANTING"
            font.pixelSize: 30
            color: "#fefefe"
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignLeft
        }
    }


    //传感器异常提示
    Text {
        id:tipText
        text: "Unable to get readings,\n please try again later."
        //字体
        font.family: "MI LANTING"
        font.pixelSize: 24
        color: "#fefefe"
        anchors.centerIn: parent
        visible: false

    }

    Image {
        id: scaning
        source: scanningSource
        y:-scaning.height
        x:0
    }
    //    Image {
    //        id: scaning2
    //        rotation: 180
    //        source: scanningSource
    //        y:rootImage.height
    //        x:0
    //    }

    SequentialAnimation{
        id:apAnimation
        loops: Animation.Infinite
        running: false
        NumberAnimation { target: scaning; property: "y"; from: 0 - scaning.height; to:rootImage.height; duration: 1000; easing.type: Easing.Linear; }
        //        NumberAnimation { target: scaning2;property: "y"; from: rootImage.height; to:0 - scaning2.height; duration: 1000; easing.type: Easing.Linear; }
    }
    SmoothedAnimation{ target: scaning;  property: "y";  duration: 1000}
    //    SmoothedAnimation{ target: scaning2; property: "y"; duration: 1000}


    Row{
        id:nameAndDesRow
        anchors.top: parent.top
        anchors.topMargin: 103
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20
        visible: !refreshImage.visible
        Text {
            text: "PM2.5"

            //字体
            font.family: "MI LANTING"
            font.pixelSize: 28
            color: "#fefefe"
        }
        Text {
            text: "|"

            //字体
            font.family: "MI LANTING"
            font.pixelSize: 28
            color: "#fefefe"
            visible: desCribText.visible
        }

        //环境评价
        Text {
            id:desCribText

            //字体
            font.family: "MI LANTING"
            font.pixelSize: 28
            color: "#fefefe"
        }
    }

    Row{
        id:pmValueRow
        anchors.top: parent.top
        anchors.topMargin: 155
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id:polishing
            text: "000"

            //字体
            font.family: "KMedium"
            font.pixelSize: 160
            color: "#ffffff"
            opacity: 0.2
            visible: true
        }
        //值
        Text {
            id:pmValue
            visible: false
            text: ""

            //字体
            font.family: "KMedium"
            font.pixelSize: 160
            color: "#ffffff"
        }
    }
    Text{
        id:pmUnit
        visible: false
        font.family: "MI LANTING"
        font.pixelSize: 24
        color: "#80fefefe"
        text:"μg/m³"
        anchors.top: pmValueRow.bottom
        anchors.topMargin: -20
        anchors.right: pmValueRow.right

    }


    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        id:timeText

        //字体
        font.family: "MI LANTING"
        font.pixelSize: 26
        color: "#fefefe"
        visible: false
    }



    function start_animation()
    {
        polishing.text = "000";
        polishing.visible = true
        pmValue.visible = false
        timeText.text = qsTr("Detecting")
        desCribText.visible = false

        timeText.visible = true


        //        scaning2.visible = true
        scaning.visible = true
        apAnimation.start();
    }

    function stop_animation()
    {
        apAnimation.stop()
        //        scaning2.visible = false
        scaning.visible = false
        timeText.visible = false
        desCribText.visible = true
    }


    Connections {
        target:MainViewProperty
        onSignal_pmChanged:
        {
            if(!allowRefresh)
            {
                dealwith_color()
            }
        }
        onSignal_updatePmElapsed:
        {
            if(!allowRefresh)
            {
                timeText.text = MainViewProperty.slot_getpmLastUpdateTime()
                timeText.visible = true
            }
        }
        onSingal_reading:
        {
            if(allowRefresh)
            {
                allowRefresh = false
            }

            start_animation()
        }
        onSignal_startPmAnimation:
        {
            if(!allowRefresh)
                start_animation()
        }
        onSignal_pmDetected:
        {
            if(!allowRefresh)
            {
                stop_animation()
                timeText.visible = false
            }
        }

    }
    function dealwith_color()
    {
        var value = MainViewProperty.pmValue

        if(value < 76)
        {
            if(value < 36)
                desCribText.text = qsTr("pmExcellent");
            else
                desCribText.text = qsTr("pmGood");

            rootSource = "qrc:/resource/images/verticalScreen/pmBack_good_.png";
        }
        else if(value >= 76 && value <=115)
        {
            desCribText.text = qsTr("pmLight Pollution");
            rootSource = "qrc:/resource/images/verticalScreen/pmBackLightly_polluted.png";

        }
        else if(value >= 116 && value <=150)
        {
            desCribText.text = qsTr("pmModerate");
            rootSource = "qrc:/resource/images/verticalScreen/pmBack_Moderately_polluted.png";

        }
        else if(value >= 151 && value <=250)
        {
            desCribText.text = qsTr("pmHeavy");
            rootSource = "qrc:/resource/images/verticalScreen/pmBack_heavily_polluted.png";

        }
        else if(value > 251 && value != 9999)
        {
            desCribText.text = qsTr("pmSevere");
            rootSource = "qrc:/resource/images/verticalScreen/pmBack_severely_polluted.png";
        }
        else
        {
            desCribText.text = "";
            rootSource = "qrc:/resource/images/verticalScreen/pmBack_good_.png";
        }

        set_pmValue(value)
    }

    function set_pmValue(value)
    {
        if(Number(value) >0 && Number(value) <10)
        {
            set_pmData_normal(true)
            pmValue.text = value
            polishing.text = "00";
            polishing.visible = true
            pmValue.visible = true

        }
        else if(Number(value) >= 10 && Number(value) < 100)
        {
            set_pmData_normal(true)
            pmValue.text = value
            polishing.text = "0";
            polishing.visible = true
            pmValue.visible = true

        }
        else if(Number(value) >= 100 && Number(value) < 9998)
        {
            set_pmData_normal(true)
            pmValue.text = value
            polishing.visible = false
            pmValue.visible = true
        }
        else{
            set_pmData_normal(false)
        }

    }

    function set_pmData_normal(flag)
    {
        tipText.visible = !flag
        nameAndDesRow.visible = flag
        timeText.visible = false
        pmUnit.visible = flag
        pmValueRow.visible = flag
    }

}


