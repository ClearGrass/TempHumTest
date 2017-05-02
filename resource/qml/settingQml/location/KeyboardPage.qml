import QtQuick 1.0
import "../baseQml" as BaseQml
import "../keyboard" as KeyBoard
import Setting 1.0

Rectangle {
    id:keyboard
    width: 854; height: 480
    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_location.png"
    property string sourceDeleteIcon: "qrc:resource/images/optionPage/button/buttonDelete.png"

    property string deleteArrow: "\u2190"
    property string content: ""
    signal backsig();
    signal complete();
    property bool pageVisible: true
    property string fontFamily: "MI LANTING"

    color:"#292a2c"
    z:11
    visible:  false


    Rectangle{
        Text {
            id: tipText
            text: qsTr("Please input the city name")
            font.family: fontFamily
            font.pixelSize: 26
            horizontalAlignment: Text.AlignHCenter

            color: "#80ffffff";
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30
            visible: true
        }
        width: 406; height: 66
        anchors.top: parent.top
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#00000000"
        TextInput{
            id: lineEdit
            width: 406; height: 66
            anchors.fill: parent
            font.family: fontFamily
            font.pixelSize: 26
            color: "#ffffff";
            smooth: true
            visible: true
            horizontalAlignment: Text.AlignHCenter

            onTextChanged: {
                if(text.length == "")
                {
                    deleteButton.visible = false
                    tipText.visible  = true
                }
                else
                {
                    deleteButton.visible = true
                    tipText.visible  = false
                }
            }
        }
        BaseQml.HorizonLine {
            id: horiLine
            height: 3
            width: 406
            color: "#bfbfbf"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Image {
            id: deleteButton
            source: sourceDeleteIcon
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset:-10
            anchors.left:  parent.right
            anchors.leftMargin: 12
            visible: false
            MouseArea{
                height: 100
                width: 100
                anchors.centerIn: parent
                onClicked: {
                    content = ""
                    lineEdit.text = "";
                }
            }
        }
    }

    KeyBoard.VirtualKeyboard{
        id: vk;

        onButtonPressed: {

            doOperation(op)
        }
        onSig_complete:
        {
            complete();
        }
        onSignal_calcel: {
            backsig();
        }

    }

    function doOperation(op) {
        var t = content
        var lineeidtText = "";

        if (op == deleteArrow) {
            t = t.toString().slice(0, -1)
            if (t.length == 0) {
                t = ""
            }
        } else {
            t = t + op.toString()
        }
        content = t

        lineEdit.text = t;
    }

    function page_hide(){
        backArea.visible = false
        keyboard.visible = false
    }

    function page_show(){
        backArea.visible  = true
        keyboard.visible = true
        clear()

    }

    function clear()
    {
        content = ""
        lineEdit.text = "";
    }

}
