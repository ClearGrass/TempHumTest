import QtQuick 1.1

Item{
    property string sourceIma: "qrc:/resource/images/optionPage/label/label_selected.png"
    property alias model: listView.model
    property alias curindex: listView.currentIndex
    property string fontFamily: "MI LANTING"

    Component {
        id: baseDelegate
        Item {
            id: wrapper
            width: 854; height: 94

            Text {
                id:baseName
                text:  listView.model.title(index)
                //字体
                font.family: fontFamily
                font.pixelSize: 26
                font.bold: false
                color: "#ffffff"

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 40
            }


            Image {
                id: ima
                visible: false
                source:sourceIma
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }

            HorizonLine{
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter
                width: 774
            }

            states: State {
                name: "Current"
                when: wrapper.ListView.isCurrentItem

                PropertyChanges { target: baseName; color: "#33b4ff" }
                PropertyChanges { target: ima; visible: true }

            }

            MouseArea{
                anchors.fill: wrapper
                onClicked: {
                    listView.currentIndex = index
                }
            }

        }
    }
    ListView {
        id: listView

        width: 854; height: 380
        delegate: baseDelegate
        focus: true
        clip: true
        highlightFollowsCurrentItem: true

    }

}
