import QtQuick 1.0
import "../baseQml" as BaseQml
import "../keyboard" as KeyBoard
import Setting 1.0

Rectangle {
    id:keyboard
    width: 854; height: 480
    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_wifi.png"
    property string sourceDeleteIcon: "qrc:resource/images/optionPage/button/buttonDelete.png"

    property string deleteArrow: "\u2190"
    property string wifissic:""

    property string password: ""
    signal backsig();
    signal complete();
    property string fontFamily: "MI LANTING"

    color:"#292a2c"
    z:11
    visible:  false


    Rectangle{

        Text {
            id: tipText
            text: qsTr("Password")
            font.family: fontFamily
            font.pixelSize: 26
            horizontalAlignment: Text.AlignHCenter

            color: "#80ffffff";
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 40
            visible: true
        }
        width: 406; height: 66
        anchors.top: parent.top
        anchors.topMargin: 50
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

            echoMode:TextInput.Password
            passwordCharacter: "*"


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
            anchors.bottomMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Image {
            id: deleteButton
            source: sourceDeleteIcon
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset:-20
            anchors.left:  parent.right
            anchors.leftMargin: 10
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
        isWifiVK: true
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
        var t = lineEdit.text

        if (op == deleteArrow) {
            t = t.toString().slice(0, -1)
            if (t.length == 0) {
                t = ""
            }
        } else {
            t = t + op.toString()
        }
        password = t
        lineEdit.text = t
        if(password.length < 8)
        {
            vk.set_okDisable()
        }
        else
        {
            vk.set_okEnable()
        }
    }

    function page_hide(){
        keyboard.visible = false
    }

    function page_show(){
        keyboard.visible = true
        clear()

    }

    function clear()
    {
        lineEdit.text = ""
        vk.set_okDisable()
    }

    Connections{
        target: SettingProperty
        onSignal_changeEntry:
        {
            entry = SettingProperty.slot_getEntry()
            if(3 == entry)
            {
                var lan = SettingProperty.slot_getWifiWizardLanguage()

                if(lan == "US.qm")
                {
                    vk.set_spaceText("space")
                    vk.set_okText("Connect")

                }
                else if(lan == "ZH_SC.qm")
                {
                    vk.set_spaceText("空格")
                    vk.set_okText("连接")

                }
                else
                {
                    vk.set_spaceText("空格")
                    vk.set_okText("連接")
                }
            }
        }
    }



}
