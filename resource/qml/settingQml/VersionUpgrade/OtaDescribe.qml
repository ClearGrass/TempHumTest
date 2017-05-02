import QtQuick 1.0
import Setting 1.0
import "../baseQml" as BaseQml


Item{
    id:otaDescribe
    z:22
    visible: false
    signal close()
    signal sure()
    property alias text: des.text
    BaseQml.Messagebox{
        leftText: qsTr("DescUpdate")
        rightText: qsTr("Cancel")
        Text {
            id:title
            color:"#292a2c"
            font.family: "MI LANTING"
            font.pixelSize: 30
            text: qsTr("Update:")
            anchors.top: parent.top
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Item {
            height: 205
            width: 500
            anchors.top: title.bottom
            anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                id: des
                anchors.fill: parent
                wrapMode: Text.Wrap
                color:"#292a2c"
                font.family: "MI LANTING"
                font.pixelSize: 24
            }
        }

        onSignal_rightButtonOnClicked:  {
            otaDescribe.visible  = false
            close()
        }
        onSignal_leftButtonOnClicked: {
            otaDescribe.visible  = false
            sure()
        }
    }


}
