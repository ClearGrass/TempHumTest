import QtQuick 1.1

Item {
    property string logoSource:  "qrc:/resource/images/test/light.png"
    property string slideSource:  "qrc:/resource/images/test/slide_point.png"
    property string unitSource: "qrc:/resource/images/test/smallRHUnit.png"

    property int value: 0

    width: 854
    height: 80

    Image {
        id: logo
        source: logoSource

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 60
    }


    Rectangle{
        height: 4
        width: 582
        color: "#000000"
        radius: 2
        anchors.centerIn: parent

        Rectangle{
            id:sliderRect
            height: parent.height
            radius: parent.radius
            color: "#00e5ff"
            width: slideImage.x + 18
        }
        Image {
            id: slideImage
            source: slideSource
            x:value/100 * 582 - 18
            y:-18
            states: State {
                name: "pressed"; when: sliderMousarea.pressed == true
                PropertyChanges { target: slideImage; opacity:0.8 }
            }
            MouseArea{
                id:sliderMousarea
                height: 100
                width: 100
                anchors.centerIn: parent;
                drag.target: slideImage;
                drag.axis: Drag.XAxis;
                drag.minimumX: -18;
                drag.maximumX: 564;
                onMouseXChanged:   {
                    var tmp = Math.round((slideImage.x + 18)/582 *100);
                    if(tmp != value)
                    {
                        value = tmp
                    }
                }
            }
        }

    }

    Text {
        id: percentText
        text: value
        color: "#ffffff"

        //字体
        font.family: "DINCondensedC"
        font.pixelSize: 36

        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 3
        anchors.right: parent.right
        anchors.rightMargin: 73
    }

    Image {
        source: unitSource
        anchors.left: percentText.right
        anchors.verticalCenter: parent.verticalCenter

    }


}
