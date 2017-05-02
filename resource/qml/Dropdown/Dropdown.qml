import QtQuick 1.1

Image {
    width: 854
    height: 480
    Rectangle{
        color: "#000000"
        opacity: 0.95
        anchors.fill: parent
    }
    property string sourceDaymode: "qrc:/resource/images/mainPage/dropdown/daymode.png"
    property string sourceNightmode: "qrc:/resource/images/mainPage/dropdown/nightmode.png"
    property string sourceAutolight: "qrc:/resource/images/mainPage/dropdown/autoLight.png"
    property string sourceManuallight: "qrc:/resource/images/mainPage/dropdown/manualLight.png"
    property string sourceUnlock: "qrc:/resource/images/mainPage/dropdown/unlock.png"
    property string sourceLock: "qrc:/resource/images/mainPage/dropdown/lock.png"
    property string sourceUpward: "qrc:/resource/images/mainPage/dropdown/upward.png"
    property string sourcelight1: "qrc:/resource/images/mainPage/dropdown/label_light1.png"
    property string sourcelight2: "qrc:/resource/images/mainPage/dropdown/label_light2.png"
    property string sourcesliderRelease: "qrc:/resource/images/mainPage/dropdown/slider_release.png"
    property string sourcesliderPress: "qrc:/resource/images/mainPage/dropdown/slider_press.png"
    property string screenoffSource: "qrc:/resource/images/poweroff/screenoff.png"
    property string screenoffPressedSource: "qrc:/resource/images/poweroff/screenoffPressed.png"

    property string fontFamily: "MI LANTING"
    property string whiteColor: "#ffffff"
    property bool isNightMode: DropDown.slot_getNightMode()
    property bool isAuto: DropDown.slot_isAuto()
    Row{

        anchors.top: parent.top
        anchors.topMargin: 116
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 170
        Image {
            id: nightmodeImage
            source: isNightMode ? sourceNightmode :sourceDaymode

            Text {
                text: qsTr("Night Mode")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.bottom
                anchors.topMargin: 30

                //字体
                font.family: fontFamily
                font.pixelSize: 26

                //加粗
                font.bold: false
                color:whiteColor
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    isNightMode = !isNightMode
                    DropDown.slot_setNightMode(isNightMode)
                }
            }
        }

        Image {
            id: autoLightImage
            source: isAuto ? sourceAutolight :sourceManuallight

            Text {
                text: qsTr("Auto-adjust")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.bottom
                anchors.topMargin: 30

                //字体
                font.family: fontFamily
                font.pixelSize: 26

                //加粗
                font.bold: false
                color:whiteColor
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    isAuto = !isAuto
                    DropDown.slot_setAutoChangeLight(isAuto)
                }
            }
        }


        Image {
            id: lockImage
            source: screenoffSource

            Text {
                text: qsTr("Screen Off")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.bottom
                anchors.topMargin: 30

                //字体
                font.family: fontFamily
                font.pixelSize: 26

                //加粗
                font.bold: false
                color:whiteColor
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    DropDown.slot_screen_off()
                }
                onPressed: {
                    lockImage.source = screenoffPressedSource
                }
                onReleased: {
                    lockImage.source = screenoffSource
                }
            }
        }


    }

    Image {
        id: upwardImage
        source: sourceUpward
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
    }


    Item{
        height: 33
        width: 854
        id:progress
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 117
        anchors.left: parent.left
        anchors.leftMargin: 0

        Image {
            id: light1
            source: sourcelight1
            anchors.left: parent.left
            anchors.leftMargin: 101
            anchors.verticalCenter: parent.verticalCenter
        }


        Image {
            id: light2
            source: sourcelight2
            anchors.right: parent.right
            anchors.rightMargin: 100
            anchors.verticalCenter: parent.verticalCenter
        }

        Item{
            id:sliderBack
            height: 6
            width: 560
            Rectangle{
                anchors.fill: parent
                radius: 3
                color: "#6d6e70"

            }
            z:1
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: light1.right
            anchors.leftMargin: 20

            Rectangle{
                id:sliderRect
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                radius: 3
                color: "#b1c3c5"
                width: slider.x +25
            }

            Image
            {
                id:slider
                x:-25
                y:-20
                z:10
                source: sourcesliderRelease
                opacity: 1.0
            }
            MouseArea{
                enabled: !isAuto
                height: 94
                width: 94
                id:sliderMousarea
                anchors.centerIn: slider
                drag.target: slider;
                drag.axis: Drag.XAxis;
                drag.minimumX: -25;
                drag.maximumX: 535;
                onMouseXChanged:   {
                    if(isAuto == false)
                    {
                        var tmp = Math.round((slider.x+25)/560 *100);
                        if(tmp != lightValue)
                        {
                            lightValue = tmp
                            console.log(tmp)
                            DropDown.slot_setLightValue(tmp)
                        }
                    }
                }
                onPressed: {
                    slider.source = sourcesliderPress
                }
                onReleased: {
                    slider.source = sourcesliderRelease
                }

            }

        }

    }

    Connections{
        target: DropDown
        onSignal_autoChangeValue:
        {
            if(isAuto)
                slider.x = DropDown.slot_getLightValue() /100 *560 -25
        }
        onSignal_updateNightMode:
        {
            isNightMode = DropDown.slot_getNightMode()
        }
    }



}
