
import QtQuick 1.0
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle {
    id:wifipage
    width: 854; height: 480

    property string unlock1: "qrc:/resource/images/optionPage/wifiIcon/unlock1.png"
    property string unlock2: "qrc:/resource/images/optionPage/wifiIcon/unlock2.png"
    property string unlock3: "qrc:/resource/images/optionPage/wifiIcon/unlock3.png"
    property string unlock4: "qrc:/resource/images/optionPage/wifiIcon/unlock4.png"

    property string lock1: "qrc:/resource/images/optionPage/wifiIcon/lock1.png"
    property string lock2: "qrc:/resource/images/optionPage/wifiIcon/lock2.png"
    property string lock3: "qrc:/resource/images/optionPage/wifiIcon/lock3.png"
    property string lock4: "qrc:/resource/images/optionPage/wifiIcon/lock4.png"


    property string fontFamily: "MI LANTING"

    property string enterblue: "qrc:/resource/images/optionPage/button/button_enter_blue.png"

    property string curssid: ""
    property int  curIndex: 0
    property bool threadIsBusy: false
    color:"#292a2c"
    z:11
    property int entry: SettingProperty.slot_getEntry()

    property bool modelMouseareaVisible: true
    Text {
        z:20
        id:removeAll
        text: qsTr("清空已存Wi-Fi")
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 20
        anchors.rightMargin: 100

        font.family: fontFamily
        font.pixelSize: 26
        font.bold: false
        color:"#ffffff"
        MouseArea{
            height: 76
            width: 100
            anchors.centerIn:  removeAll
            onClicked: {
                SettingProperty.slot_forgetAllWifi()
            }
            onPressed: {
                removeAll.color = "#80ffffff"
            }
            onReleased:{
                removeAll.color = "#ffffff"

            }
        }

    }

    Text {
        z:20
        id: ignoreWifi
        color: "#ffffff"
        font.family: fontFamily
        font.pixelSize: 30
        visible: false
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 20
        anchors.rightMargin: 60
        MouseArea{
            height: 76
            width: 100
            anchors.centerIn: ignoreWifi
            id:mouseAreaignoreWifi
            visible: false
            onClicked: {
                SettingProperty.slot_wizardFinish();
            }
        }
        Connections{
            target: SettingProperty
            onSignal_changeEntry:
            {
                entry = SettingProperty.slot_getEntry()
                if(3 == entry)
                {
                    mouseAreaignoreWifi.visible  = true
                    ignoreWifi.visible  = true
                    var lan = SettingProperty.slot_getWifiWizardLanguage()

                    if(lan == "US.qm")
                    {
                        ignoreWifi.text = "Skip"

                    }
                    else if(lan == "ZH_SC.qm")
                    {
                        ignoreWifi.text = "跳过"

                    }
                    else
                    {
                        ignoreWifi.text = "跳過"

                    }
                }
            }
        }
    }

    KeyboardPage{
        id:vk
        onBacksig:{
            backMouseArea.enabled = true
            listView.enabled = true;
            backButton.enabled  = true
            vk.page_hide()
            SettingProperty.slot_wifiListAllowRefresh(true);
        }
        onComplete:{
            backMouseArea.enabled = true
            listView.enabled = true;
            backButton.enabled  = true
            vk.page_hide()

            //链接wifi
            SettingProperty.slot_wifiConnect(vk.wifissic.toString(),vk.password.toString())
            SettingProperty.slot_wifiListAllowRefresh(false);
            SettingProperty.slot_setWifiStatus(curIndex,3)

            threadIsBusy = true

        }
    }


    WifiConnInfoPage{
        id:wificonn
        anchors.centerIn: parent
        onComplete:{
            backMouseArea.enabled = true
            listView.enabled = true;
            backButton.enabled  = true
        }
        onSignal_reEnterPWD: {
            vk.wifissic =curssid

            modelMouseareaVisible  = false
            backMouseArea.enabled = false
            listView.enabled = false;
            backButton.enabled  = false
            vk.page_show();
            SettingProperty.slot_wifiListAllowRefresh(false);

        }
    }

    BaseQml.FrameTop
    {
        id:frameTop
        topTitle: "Wi-Fi"
    }

    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }

    Component {
        id: petDelegate
        Item {
            id: wrapper
            width: 854; height: 94


            //是否保存密码标志位
            Rectangle{
                height: 10
                width: 10
                radius: 5
                color: "red"
                visible: key
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
            }

            Row{
                anchors.left: parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter
                spacing: 34
                Image {
                    id: wifiIcon
                    source:  get_signSource(signalNum, lock)
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: signalstrength
                    text: strength
                    anchors.verticalCenter: parent.verticalCenter
                    //字体
                    font.family: fontFamily
                    font.pixelSize: 26
                    font.bold: false
                    color:"#ffffff"
                }
                Text {
                    id:wifiId
                    text:  ssid
                    anchors.verticalCenter: parent.verticalCenter

                    //字体
                    font.family: fontFamily
                    font.pixelSize: 26
                    font.bold: false
                    color:"#ffffff"
                }


            }

            BaseQml.Waitting{
                id:wait
                visible: get_waittinVis(status)
                run:get_waittinVis(status)
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 40
            }
            Row{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 0

                Text {
                    id: wifiStatu
                    text: get_statuesDes(status)
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: -3
                    //字体
                    font.family: fontFamily
                    font.pixelSize: 24
                    font.bold: false
                    color: status == 2  ? "#ea4632" :"#80ffffff"
                }

                Image {
                    source: enterblue
                    visible: (wifiStatu.text == "") ? false :true
                }

            }


            BaseQml.HorizonLine{
                width: 774
                anchors.horizontalCenter:parent.horizontalCenter
                anchors.bottom: parent.bottom
            }

            MouseArea{
                id:modelMouseArea
                anchors.fill: wrapper
                visible: modelMouseareaVisible
                onClicked: {
                    wrapper.ListView.view.currentIndex = index

                    if(threadIsBusy == false)
                    {
                        curssid =  wifiModel[index].ssid;
                        wificonn.wifissic = curssid
                        wificonn.ip = wifiModel[index].ip
                        var stat = wifiModel[index].status

                        if(wifiStatu.text == "")
                        {
                            var keyType = SettingProperty.slot_get_keyType(curssid);

                            //无密码
                            if(keyType == 2)
                            {
                                curIndex = index;
                                vk.wifissic =curssid

                                modelMouseareaVisible  = false
                                backMouseArea.enabled = false
                                listView.enabled = false;
                                backButton.enabled  = false
                                vk.page_show();
                                SettingProperty.slot_wifiListAllowRefresh(false);


                            }
                            else if(keyType == 1)
                            {
                                //公有Wi-Fi
                                SettingProperty.slot_connect_publicWiFi(curssid)
                                SettingProperty.slot_setWifiStatus(index,3)
                                threadIsBusy = true
                                SettingProperty.slot_wifiListAllowRefresh(false);


                            }
                            else
                            {
                                SettingProperty.slot_connect_withNetID(curssid);
                                SettingProperty.slot_setWifiStatus(index,3)
                                threadIsBusy = true
                                SettingProperty.slot_wifiListAllowRefresh(false);


                            }
                        }
                        else
                        {
                            backMouseArea.enabled = false
                            listView.enabled = false;
                            backButton.enabled  = false
                            if(stat == 0)
                                wificonn.show(1)
                            else if(stat == 2)
                                wificonn.show(stat)


                        }
                    }

                } // end onClicked
            }  //end MouseArea

        }
    }

    Connections{
        target: SettingProperty
        onSignal_showWifiList:
        {
            modelMouseareaVisible  = true
            backMouseArea.enabled = true
            listView.enabled = true;
            backButton.enabled  = true
        }
        onSignal_connRes:
        {
            threadIsBusy = false
            var value =SettingProperty.slot_getWifiConnRes()
            SettingProperty.slot_wifiListAllowRefresh(true);
            if(3 == entry)
            {
                ignoreWifi.visible = true
            }

            if(0xF00B == value)
            {
                listView.currentIndex = 0

                //出厂设置
                if(3 == entry)
                {
                    var lan = SettingProperty.slot_getWifiWizardLanguage()
                    if(lan == "US.qm")
                    {
                        ignoreWifi.text = "Done"

                    }
                    else
                    {
                        ignoreWifi.text = "完成"

                    }
                    SettingProperty.slot_setMainWifiPageIsHide();

                    SettingProperty.slot_wizardFinish();

                }
            }

        }

    }
    Connections{
        target: vk
        onBacksig:{
            backMouseArea.visible = true
            modelMouseareaVisible  = true

        }
        onComplete:{
            backMouseArea.visible = true
            modelMouseareaVisible  = false

        }
    }

    ListView {
        id: listView

        width: 854; height: 380
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 100

        model: wifiModel
        delegate: petDelegate
        focus: true
        clip: true
        highlightFollowsCurrentItem: true
    }

    MouseArea{
        id:backMouseArea
        anchors.fill: backButton
        onClicked:
        {
            if(3 != entry)
            {
                SettingProperty.slot_wifiListAllowRefresh(true);
                SettingProperty.slot_setQMl(0)
                if(entry == 1)
                {
                    SettingProperty.on_page_defalut_back()
                }
            }
            else if(3 ==entry )  //开机向导
            {
                SettingProperty.slot_jumpLanWizard();
            }

        }
        onPressed: {
            backButton.opacity = 0.5
        }
        onReleased: {
            backButton.opacity = 1.0
        }
    }


    function get_signSource(siganlNum, isLock){
        var sig = lock1
        switch(siganlNum)
        {
        case 1:
            if(isLock)
                sig = lock1;
            else
                sig = unlock1
            break;
        case 2:
            if(isLock)
                sig = lock2;
            else
                sig = unlock2
            break;
        case 3:
            if(isLock)
                sig = lock3;
            else
                sig = unlock3
            break;
        case 4:
            if(isLock)
                sig = lock4;
            else
                sig = unlock4
            break;
        default:
            break;
        }

        return sig;
    }
    function get_statuesDes(value)
    {
        switch(value)
        {
        case 0:
            return qsTr("Connected")
        case 1:
            //未链接
            return ""
        case 2:
            return qsTr("No internet connection")

        default:
            return ""
        }

    }


    function get_waittinVis(value)
    {
        if(3 == value)
        {
            //开机向导
            if(entry == 3)
            {
                ignoreWifi.visible = false
            }
            return true
        }
        else
        {
            return false
        }
    }

    function get_selewifiIcon(sigNum)
    {
        switch(sigNum)
        {
        case 1:
            return wifiSelcSig1;
        case 2:
            return wifiSelcSig2;
        case 3:
            return wifiSelcSig3;
        case 4:
            return wifiSelcSig4;
        }
    }
    function show_ignore()
    {
        if(3 == SettingProperty.slot_getEntry())
        {
            mouseAreaignoreWifi.visible  = true
            ignoreWifi.visible  = true
        }
        else
        {
            mouseAreaignoreWifi.visible  = false
            ignoreWifi.visible  = false
        }
    }

    Connections{
        target: SettingProperty
        onSignal_back:
        {
            if(vk.visible  == false && wificonn.visible == false)
            {
                if(3 != entry)
                {
                    SettingProperty.slot_wifiListAllowRefresh(true);
                    SettingProperty.slot_setQMl(0)
                    if(entry == 1)
                    {
                        SettingProperty.on_page_defalut_back()
                    }
                }
                else if(3 ==entry )  //开机向导
                {
                    SettingProperty.slot_jumpLanWizard();
                }
            }
            else
            {

                if(vk.visible  == true)
                {
                    backMouseArea.enabled = true
                    listView.visible = true;
                    backButton.enabled  = true
                    vk.page_hide()
                    SettingProperty.slot_wifiListAllowRefresh(true);
                }
                else if(wificonn.visible = true)
                {
                    wificonn.hide()
                }
            }
        }
    }



}
