import QtQuick 1.0
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle {
    width: 854; height: 480

    property string sourceDeleteIcon: "qrc:resource/images/optionPage/button/buttonDelete.png"
    property string sourceLineBule: "qrc:resource/images/optionPage/other/lineBlue.png"
    property string fontFamily: "MI LANTING"

    property string text: ""
    property int cityNum: 0
    property string selectCity: ""
    property bool pageVisible: true

    signal back()
    signal complete()
    color:"#292a2c"
    z:11

    //横向线条
    BaseQml.FrameTop
    {
        id:frameTop
        topTitle: text
    }


    BaseQml.Waitting{
        id:waitting
        run:false
        visible: false
        anchors.centerIn: parent
    }

    Text {
        id: cityIsEmpty
        text: qsTr("No results")
        anchors.centerIn: parent
        visible: false
        font.family: fontFamily
        font.pixelSize: 26
        color: "#ffffff";
    }

    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }

    Component {
        id: cityDelegate
        Item {
            id: wrapper
            width: 854; height: 94
            Text {
                id:desc
                text:  describe
                //字体
                font.family: fontFamily
                font.pixelSize: 26
                font.bold: false
                color: "#ffffff"

                anchors.verticalCenter:  parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 40
            }
            Text {
                id:cityName
                text:  name
                visible: false
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 0
            }

            BaseQml.HorizonLine{
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter
                width: 774
            }


            MouseArea{
                anchors.fill: wrapper
                onClicked: {
                    listView.currentIndex = index
                    visible: pageVisible

                    if(cityNum > 0)
                    {
                        selectCity = cityName.text;
                    }
                    cityIsEmpty.visible  = false

                    complete()


                }
            }

        }
    }



    ListView {
        id: listView

        width: 854; height: 380
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 100

        model: cityModel
        delegate: cityDelegate
        focus: true
        clip: true
        highlightFollowsCurrentItem: true
        visible: false
    }

    MouseArea{
        anchors.fill: backButton
        visible: pageVisible

        onClicked:
        {
            back();
        }
        onPressed: {
            backButton.opacity = 0.5
        }
        onReleased: {
            backButton.opacity = 1.0
        }
    }

    function show(num)
    {
        cityNum = num;
        stop_animation()
        if(cityNum > 0)
        {
            listView.visible = true
            cityIsEmpty.visible = false
        }
        else
        {
            listView.visible = false
            cityIsEmpty.visible = true
        }
    }

    function start_animation()
    {
        listView.visible = false
        waitting.visible = true
        waitting.startAnimation()
    }

    function stop_animation()
    {
        listView.visible = true
        waitting.stopAnimation()
        waitting.visible =false
    }
}
