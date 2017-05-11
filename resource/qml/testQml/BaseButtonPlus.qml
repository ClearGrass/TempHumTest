import QtQuick 1.1

Item {
    property string releaseSource: "qrc:/resource/images/test/buttonBorder.png"
    property string pressSource: "qrc:/resource/images/test/buttonPress.png"
    property string logoSource: ""
    property string maintext: ""
    property bool run: false

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
       smooth: true

       NumberAnimation on rotation {
           id:animationLarge ; from: 0; to: 360; running: run; loops: Animation.Infinite; duration: 2000
       }
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
       anchors.verticalCenter: parent.verticalCenter
   }

   function start_ani()
   {
       animationLarge.start()
   }

   function stop_ani()
   {
       animationLarge.stop()

   }



}
