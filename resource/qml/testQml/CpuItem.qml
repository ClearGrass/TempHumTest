import QtQuick 1.1

Item {
    height: 140
    width: 854
    property string buttonPressed: "qrc:/resource/images/test/smallbuttonPress.png"
    property string buttonRelease: "qrc:/resource/images/test/smallbuttonRelease.png"
    property bool isCpu120M: true
    property bool isCpu100: false
    property string strFre: "0"
    Row{
        anchors.centerIn: parent
        spacing: 3
        BorderImage {
            id: name
            source: "qrc:/resource/images/test/borderImage.png"
            width: 422; height: 140
            border.left: 0; border.top: 0
            border.right: 0; border.bottom: 0

            Text {
                id: cpuFrequ
                text:"CPU frequency: 1.008G"

                color: "#ffffff"
                //字体
                font.family: "Helvetica"
                font.pixelSize: 24

                anchors.left:parent.left
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 2
            }

            Image {
                id: cpu120m
                source: isCpu120M ? buttonPressed: buttonRelease
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.bottomMargin: 20
                anchors.leftMargin: 20
                Text {
                    text:"120 M"

                    color: "#ffffff"
                    //字体
                    font.family: "Helvetica"
                    font.pixelSize: 24

                    anchors.centerIn: parent
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        MainViewProperty.slot_set120M()
                        isCpu120M = true
                    }
                }

            }

            Image {
                id: cpu1G
                source: isCpu120M ? buttonRelease :buttonPressed
                anchors.bottom: parent.bottom
                anchors.right:  parent.right
                anchors.bottomMargin: 20
                anchors.rightMargin: 20
                Text {
                    text:"1.008 G"

                    color: "#ffffff"
                    //字体
                    font.family: "Helvetica"
                    font.pixelSize: 24

                    anchors.centerIn: parent
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        MainViewProperty.slot_set1G()
                        isCpu120M = false
                    }
                }
            }


        }
        BorderImage {
            source: "qrc:/resource/images/test/borderImage.png"
            width: 422; height: 140
            border.left: 0; border.top: 0
            border.right: 0; border.bottom: 0

            Text {
                id: cpuused
                text:"CPU usage: " + strFre

                color: "#ffffff"
                //字体
                font.family: "Helvetica"
                font.pixelSize: 24

                anchors.left:parent.left
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 2
            }

            Image {
                id: cpuuese0
                source: isCpu100 ? buttonRelease : buttonPressed
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.bottomMargin: 20
                anchors.leftMargin: 20

                Text {
                    text:"0"

                    color: "#ffffff"
                    //字体
                    font.family: "Helvetica"
                    font.pixelSize: 24

                    anchors.centerIn: parent
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        MainViewProperty.slot_setCpuUsage0()
                        isCpu100 = false
                    }
                }
            }

            Image {
                id: cpuused100
                source: isCpu100 ? buttonPressed : buttonRelease
                anchors.bottom: parent.bottom
                anchors.right:  parent.right
                anchors.bottomMargin: 20
                anchors.rightMargin: 20

                Text {
                    text:"100"

                    color: "#ffffff"
                    //字体
                    font.family: "Helvetica"
                    font.pixelSize: 24

                    anchors.centerIn: parent
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        MainViewProperty.slot_setCpuUseage100()
                        isCpu100 = true
                    }
                }
            }
        }
    }

    Connections{
        target: MainViewProperty
        onSignal_updateUsage:
        {
            strFre = MainViewProperty.slot_getUsage()

            cpuused.text = "CPU usage: " + strFre
        }
        onSignal_updateFre:
        {
            var fre = MainViewProperty.slot_getFre()
            cpuFrequ.text ="CPU frequency: " +  fre + "G"
            if(fre == 0.12)
            {
                isCpu120M = true
            }
            else
            {
                isCpu120M = false
            }

        }
        onSignal_cpuNoLoad:
        {
            isCpu100 = false
        }
        onSignal_cpuLoad:
        {
            isCpu100 = true
        }
    }
}
