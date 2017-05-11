import QtQuick 1.0

Rectangle {
    id: powerSwitch
    z:0
    width: 854; height: 480
    color: "#292a2c"

    property string stringLock: "qrc:/resource/images/poweron/lock.png"
    property string stringUnlock: "qrc:/resource/images/poweron/unlock.png"
    property string powerBackSource: "qrc:/resource/images/poweron/powerBack.png"
    property string stringLockBack: "qrc:/resource/images/poweron/lockBack.png"
    property bool on: false


    Image {
        z:1
        id: powerBackground
        source: powerBackSource
        anchors.centerIn: parent

        Image {
            id: name
            anchors.right: parent.right
            anchors.top: parent.top
            source: stringUnlock
        }

        Image {
            id: slidingBlock
            source: stringLockBack
            Image {
                id: lockImage
                source: stringLock
                anchors.centerIn: parent
            }
            x:0;y:0
            opacity: 1
            MouseArea {
                anchors.fill: parent
                drag.target: slidingBlock; drag.axis: Drag.XAxis; drag.minimumX: 0; drag.maximumX: 286
                onReleased: releaseSwitch()
            }
            onXChanged: {
                if(slidingBlock.x > 282)
                {
                    lockImage.source = stringUnlock
                }
                else
                {
                    lockImage.source = stringLock
                }
            }
        }
    }


    function releaseSwitch() {
        if(slidingBlock.x < 150)
        {
            slidingBlock.x = 0
        }
        else
        {
            slidingBlock.x = 286
            PagePowerOn.slot_power_on();
        }
    }


    MouseArea{
        anchors.fill: parent
        onClicked: {
//            PagePowerOn.slot_power_off();
        }
    }
}


