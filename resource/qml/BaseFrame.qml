import QtQuick 1.1
import MainView 1.0
import HistoryPage 1.0
import "toolQml" as ToolQml


Item {
    objectName: "base"
    property int currentIndex: 0
    property int animationDuration: 750
    property int bottomlayoutSpacing: 37
    property int spacing1: 37
    property int spacing2: 146
    property int pmminorWidth: 181
    property bool  haveNewVersion: false
    width:parent.width
    height: parent.height
    x:0;y:0;z:10;
    anchors.centerIn: parent



    //WiFi和电源
    ToolQml.WifiAndBattery{
        id: wifiAndPower
    }



}
