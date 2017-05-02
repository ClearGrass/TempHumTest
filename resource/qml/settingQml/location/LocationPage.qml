import QtQuick 1.1
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle
{
    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_location.png"
    property string enterSource: "qrc:/resource/images/optionPage/button/buttonEnter.png"
    property string selectedSource: "qrc:/resource/images/optionPage/label/label_selected.png"
    property string fontFamily: "MI LANTING"

    id:localtionPage
    //页面宽度和高度
    property int itemHeight: 480
    property int itemWidth:  854
    property string whiteColor: "#ffffff"
    property string blueColor: "#33b4ff"
    property bool pageVisible: true
    property string netLocalCity: ""
    property string manualLocalCity: ""
    property string selectString: qsTr("Select Location")
    property string selectedString: qsTr("Selected  Location")
    signal back()
    signal showVk()
    height: itemHeight
    width:  itemWidth
    color:"#292a2c"
    z:11
    //横向线条
    BaseQml.FrameTop
    {
        id:frameTop
        iconSource: sourceIcon
        topTitle: qsTr("Location")
    }

    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }

    Column{
        anchors.left: parent.left
        anchors.top: frameTop.bottom
        //基于网络定位
        Item{
            height: 94
            width: 854

            Image {
                id: netSelectedImage
                source: selectedSource
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                visible: false
            }

            Text {
                id: localDes
                text: netLocalCity

                //字体
                font.family: fontFamily
                font.pixelSize: 26

                color: blueColor

                anchors.left: parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter

            }

            BaseQml.Waitting{
                id:autoWaitting
                run:false
                visible: false

                anchors.left: parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: localRes
                text: qsTr("Network location")

                //字体
                font.family: fontFamily
                font.pixelSize: 26

                color: "#80ffffff"

                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 40

            }

            //分割线
            BaseQml.HorizonLine{
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter
                width: 774
            }

            MouseArea{
                anchors.fill: parent
                visible: pageVisible
                onClicked: {
                    start_autoLocalAnimation()
                    netSelectedImage.visible = true
                    localDes.color = blueColor
                    selectText.text = selectString
                    manualSelectedImage.visible = false
                    SettingProperty.slot_setAutoLocal();
                }
            }
        }


        //手动定位城市

       Item{
           height: 94
           width: 854
           id:manualItem
           Image {
               id: manualSelectedImage
               source: selectedSource
               anchors.left: parent.left
               anchors.verticalCenter: parent.verticalCenter
           }


           Text {
               id: curPositoin
               text: manualLocalCity
               color: manualSelectedImage.visible ? blueColor : whiteColor

               //字体
               font.family: fontFamily
               font.pixelSize: 26

               anchors.left: parent.left
               anchors.leftMargin: 40
               anchors.verticalCenter: parent.verticalCenter
           }

           //分割线
           BaseQml.HorizonLine{
               anchors.bottom: parent.bottom
               anchors.bottomMargin: 0
               anchors.horizontalCenter: parent.horizontalCenter
               width: 774
           }

       }

       //
       Item{
           height: 94
           width: 854

           Text {
               id:selectText
               text: qsTr("Select Location")

               //字体
               font.family: fontFamily
               font.pixelSize: 26

               color: whiteColor

               anchors.verticalCenter: parent.verticalCenter
               anchors.left: parent.left
               anchors.leftMargin: 40

           }

           Image {
               id: enterImage
               source: enterSource
               anchors.verticalCenter: parent.verticalCenter
               anchors.right: parent.right
           }

           Text {
               id: name
               text: qsTr("Search")

               //字体
               font.family: fontFamily
               font.pixelSize: 26

               color: "#80ffffff"

               anchors.verticalCenter: parent.verticalCenter
               anchors.right: enterImage.left

           }

           //分割线
           BaseQml.HorizonLine{
               anchors.bottom: parent.bottom
               anchors.bottomMargin: 0
               anchors.horizontalCenter: parent.horizontalCenter
               width: 774
           }

           MouseArea{
               anchors.fill: parent
               visible: pageVisible
               onClicked: {
                   stop_animation()

                   showVk()
               }
           }
       }
    }


    MouseArea{
        anchors.fill: backButton
        onClicked:
        {
            SettingProperty.slot_setQMl(0)
            back()
        }
        onPressed: {
            backButton.opacity = 0.5
        }
        onReleased: {
            backButton.opacity = 1.0
        }
    }


    function set_autoLocal(flag)
    {

        if(flag)
        {
            manualItem.visible = false
            netSelectedImage.visible = true
            localDes.color = blueColor
            selectText.text = selectString
        }
        else
        {
            manualSelectedImage.visible = true
            manualItem.visible = true
            netSelectedImage.visible = false
            localDes.color = whiteColor
            selectText.text = selectedString
        }

    }

    function stop_animation()
    {
        autoWaitting.stopAnimation()
        autoWaitting.visible  = false
        localDes.visible = true

    }

    function start_autoLocalAnimation()
    {
        localDes.visible = false

        autoWaitting.visible  = true
        autoWaitting.startAnimation()

    }

    function start_manualLocalAnimation()
    {
        autoWaitting.stopAnimation()
        autoWaitting.visible  = false
    }

}
