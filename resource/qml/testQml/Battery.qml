import QtQuick 1.1

Item {
    height: 60
    width: 854
    property string backgroundSource: "qrc:/resource/images/test/power.png"
    property string chargingImageSource: "qrc:/resource/images/test/charging.png"
    property string timeImageSource: "qrc:/resource/images/test/time.png"
    property string timestring: "00:00:00"
    property string baselineValue: "0x000000"
    property string powerColor: "#ffffff"
    property int timeImageLeftmargin: 400
    property bool isCharging: true
    property string  currentelectricity : ""
    property int levelWidth: 20
    property int percent : 80
    //横线
    Line{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }

    Image {
        id: power
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        source: backgroundSource

        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 3
            anchors.leftMargin: 3
            id:level
            height: 10

            width:levelWidth
            color: powerColor
        }
    }

    Text {
        id: percenetText
        text: percent + "%"
        color: "#ffffff"

        //字体
        font.family: "DINCondensedC"
        font.pixelSize: 30

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: power.right
        anchors.leftMargin: 10
    }

    Image {
        id: percentImage
        source: chargingImageSource

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: percenetText.right
        anchors.leftMargin: 30
        visible: isCharging

    }

    Text
    {
        id: electricity
        text: currentelectricity
        color: "#ffffff"

        //字体
        font.family: "DINCondensedC"
        font.pixelSize: 30

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: percentImage.right
        anchors.leftMargin: 10

    }

    Image {
        id: timeImage
        source: timeImageSource

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: timeImageLeftmargin
    }

    Text
    {
        id: timeText
        text: timestring
        color: "#ffffff"

        //字体
        font.family: "DINCondensedC"
        font.pixelSize: 30

        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 3
        anchors.left: timeImage.right
        anchors.leftMargin: 10
    }

    Text
    {
        id: baselineText
        text: baselineValue
        color: "#ffffff"

        //字体
        font.family: "DINCondensedC"
        font.pixelSize: 30

        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 3
        anchors.right:  parent.right
        anchors.rightMargin: 5
    }




}
