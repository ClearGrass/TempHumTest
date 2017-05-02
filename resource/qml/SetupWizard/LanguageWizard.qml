import QtQuick 1.0
import "../settingQml/baseQml" as BaseQml
import SetupWizard 1.0
Rectangle {
    property string sourceIcon: "qrc:/resource/images/optionPage/logo/logo_language.png"
    property string sourceIma: "qrc:/resource/images/optionPage/label/label_selected.png"

    property int curIndex: 0
    //页面宽度和高度
    property int itemHeight: 480
    property int itemWidth:  854
    property string whiteColor: "#ffffff"
    property string fontFamily: "MI LANTING"

    height: itemHeight
    width:  itemWidth
    color:"#292a2c"
    z:11
    //横向线条
    BaseQml.FrameTop
    {
        id:frameTop
        topTitle: ""
        iconShow: true
        Image {
            source: sourceIcon
            anchors.centerIn: parent
        }
    }


    Component {
        id: langDelegate
        Item {
            id: wrapper
            width: 854; height: 94
            Text {
                id: id
                text: languageid
                visible: false
            }
            Text {
                id:langename
                text:  languagename
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
            BaseQml.HorizonLine{
                id:line2
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: 40
                width: 774
            }

//            states: State {
//                name: "Current"
//                when: wrapper.ListView.isCurrentItem

//                PropertyChanges { target: langename; color: "#33b4ff" }

//                PropertyChanges { target: ima; visible: true }


//            }

            MouseArea{
                anchors.fill: wrapper
                onClicked: {
                    wrapper.ListView.view.currentIndex = index
                    curIndex = index
                    SetupWizardProperty.slot_setLanguage(curIndex)
                }

            }

        }
    }



    ListView {
        id: listView

        width: 854; height: 380
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 100

        model: laguageModel
        delegate: langDelegate
        focus: true
        clip: true
        highlightFollowsCurrentItem: true
    }

    ListModel {
        id:laguageModel
        ListElement {
            languagename: "中文（简体）"
            languageid: "ZH_SC"

        }
        ListElement {
            languagename: "中文（繁體）"
            languageid: "ZH_TC"

        }
        ListElement {
            languagename: "English"
            languageid: "US"
        }
    }
}

