import QtQuick 1.1

Item {
    property string releaseSource: "qrc:/resource/images/test/buttonBorder.png"
    property string pressSource: "qrc:/resource/images/test/buttonPress.png"
    property string logoSource: ""
    property string maintext: ""
    property string subText: ""
    signal click()

    width: 281
    height: 120
    Image {
        id: back
        source: releaseSource
        anchors.fill: parent
    }

   MouseArea{
       anchors.fill: parent
       onPressed: {
           back.source = pressSource
       }
       onReleased: {
           back.source = releaseSource
       }
       onClicked: {
           click()
       }
   }

   Image {
       id: logo
       source: logoSource
       anchors.verticalCenter: parent.verticalCenter
       anchors.left: parent.left
       anchors.leftMargin: 10
   }

   Text {
       id: main
       text: maintext

       color: "#ffffff"
       //字体
       font.family: "Helvetica"
       font.pixelSize: 24

       anchors.left:logo.right
       anchors.leftMargin: 10
       anchors.top: parent.top
       anchors.topMargin: 30
   }

   Text {
       id: sub
       text: subText.length > 17 ? subText.substring(0,14)+"..." : subText

       color: "#808080"
       //字体
       font.family: "Helvetica"
       font.pixelSize: 24

       anchors.left: logo.right
       anchors.leftMargin: 10
       anchors.bottom: parent.bottom
       anchors.bottomMargin: 25
   }



}
