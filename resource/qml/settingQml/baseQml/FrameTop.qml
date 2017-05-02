import QtQuick 1.1

Rectangle
{
    property string iconSource: ""
    property string topTitle: ""
    property bool iconShow: false

    height: 100
    width: 854
    color:"#232426"

    anchors.top: parent.top
    anchors.left: parent.left
    anchors.topMargin: 0
    anchors.leftMargin: 0

    Text {
        id: title
        text: topTitle

        //字体
        font.family: "MI LANTING"
        font.pixelSize: 26

        //加粗
        font.bold: true
        color:"#ffffff"

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 100

    }


    //分割线
    HorizonLine{
        id:line
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
    }

    function show_line()
    {
        line.visible = true
    }

    function hide_line()
    {
        line.visible = false
    }

}
