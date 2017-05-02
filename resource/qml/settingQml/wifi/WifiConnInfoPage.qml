import QtQuick 1.0
import "../baseQml" as BaseQml
import Setting 1.0

Item {
    id:wifiConnInfo
    width: 854; height: 480
    visible: false
    property string closeIcon: "qrc:/resource/images/optionPage/button/button_close.png"
    property string waitingSource: "qrc:/resource/images/optionPage/tools/loading_black.png"
    property string wifissic: ""
    property string ip: ""

    //已联网 boxtype == 1 ； 无法联网 boxtype == 2； 密码错误 type == 3； 连接超时 boxtype == 4
    property int boxType: 1
    property bool isForget: false
    z:11
    signal complete();
    signal signal_reEnterPWD()
    BaseQml.Messagebox{
        visible: parent.visible
        id:messageBox
        anchors.centerIn: parent
        leftText:{
            if(boxType == 1)
            {
                qsTr("Forget This Wi-Fi")
            }
            else if(boxType == 3)
            {
                qsTr("Re-enter password")
            }
            else
            {
                qsTr("Re-connect")
            }
        }

        rightText:qsTr("Close")

        Column{
            id:textcolumun
            z:24
            spacing: 5
            anchors.top: parent.top
            anchors.topMargin: 100
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                id: ssid
                text: wifissic
                font.family: "MI LANTING"
                font.pixelSize: 30
                font.bold: false
                color:"#cc000000"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: {
                    if(boxType == 1)
                    {
                        qsTr("Connected to internet")
                    }
                    else if(boxType == 2)
                    {
                        qsTr("No internet connection")
                    }
                    else if(boxType == 3)
                    {
                        qsTr("Password incorrect")
                    }
                    else{
                        qsTr("Connection timeout")
                    }
                }
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "MI LANTING"
                font.pixelSize: 30
                font.bold: false
                color:"#cc000000"
                opacity: 0.7
            }

        }
        BaseQml.Waitting{
            sourceLoading: waitingSource
            anchors.top: textcolumun.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            id:waitAni
            run:false
            visible: false
        }

        onSignal_leftButtonOnClicked: {
            if(boxType == 1) //忘记密码
            {
                isForget = true
                waitAni.visible = true
                waitAni.startAnimation()
                SettingProperty.slot_forgetWifi(wifissic)
            }
            else if(boxType ==3) //重新输入
            {
                signal_reEnterPWD()
                wifiConnInfo.visible = false

            }
            else  //重新连接
            {

            }
        }

        onSignal_rightButtonOnClicked: {
            hide()
        }

    }



    Connections{
        target: SettingProperty
        onSignal_showWifiList:
        {
            if(isForget)
            {
                isForget = false
                waitAni.visible = false
                waitAni.stopAnimation()
                hide();
            }
        }
        onSignal_connRes:
        {
            var value =SettingProperty.slot_getWifiConnRes()
            if(0xF00C == value)
            {
                show(3);

            }
            else if(0xF00D == value){
                show(4);
            }
        }
    }


    function hide()
    {
        wifiConnInfo.visible = false
        complete();
    }

    function show(value)
    {
        boxType = value
        wifiConnInfo.visible = true
    }


    MouseArea{
        anchors.fill: parent
        onClicked: {

        }
    }



}
