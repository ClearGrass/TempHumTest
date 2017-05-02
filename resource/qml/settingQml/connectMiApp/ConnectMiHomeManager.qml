import QtQuick 1.0
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle {
    width: 854
    height: 480
    color:"#292a2c"
    property int entry:SettingProperty.slot_getEntry()
    property bool bindStat: SettingProperty.slot_getAppBindStat()

    //横向线条
    BaseQml.FrameTop
    {
        id:frameTop
        topTitle:  qsTr("Connect to Mi Home")
    }

    //step1
    TipsPage{
        id:tipPage
        visible: SettingProperty.slot_getAppBindStat() ? false :true
        onSignal_next: {
            frameTop.topTitle = qsTr("Download Mi Home App")
            tipPage.visible = false
            downloadApp.visible = true

        }
    }

    //step2
    DownloadApp{
        id:downloadApp
        visible: false
        onSignal_next: {
            frameTop.topTitle = qsTr("Connect to Mi Home")
            downloadApp.visible = false
            connApp.visible = true
            SettingProperty.slot_refresh_appQrcode()
        }
    }

    //step3
    ConnMiApp{
        id:connApp
        visible: false
    }

    //step4
    ConnectSuccess{
        id:connResult
        visible:  SettingProperty.slot_getAppBindStat()
    }

    BaseQml.WifiDisconnect{
        id:wifiDisconnet
        visible: false
    }

    BaseQml.InternetAbnormal{
        id:internetAbnormal
        visible: false
    }


    //返回按钮
    BaseQml.BackButton{
        id:backButton
        z:11
    }

    MouseArea{
        anchors.fill: backButton
        onClicked:
        {
            back_step()
        }
        onPressed: {
            backButton.opacity = 0.5
        }
        onReleased: {
            backButton.opacity = 1.0
        }
    }

    onBindStatChanged: {
        if(bindStat)
        {
            tipPage.visible = false
            downloadApp.visible = false
            connApp.visible = false
            connResult.visible = true
        }
        else
        {
            tipPage.visible = true
            downloadApp.visible = false
            connApp.visible = false
            connResult.visible = false
        }
    }

    Connections{
        target: SettingProperty
        onSignal_changeEntry:
        {
            entry = SettingProperty.slot_getEntry()
        }

        onSignal_back:
        {
            back_step()
        }
        onSignal_updateAppBindStatus:
        {
            bindStat = SettingProperty.slot_getAppBindStat()
        }
        onSignal_net_abnormal:
        {
            connApp.visible = false
            internetAbnormal.visible  = true
        }
        onSignal_net_disconnect:
        {
            connApp.visible = false
            wifiDisconnet.visible = true
        }
    }

    function back_step()
    {
        SettingProperty.slot_removeQrcode()
        if(connApp.visible)
        {
            connApp.visible =false
            downloadApp.visible  = true
        }
        else if(downloadApp.visible)
        {
            downloadApp.visible = false
            tipPage.visible  = true
        }
        else
        {
            SettingProperty.slot_setQMl(0)
            if(entry == 1)
            {
                SettingProperty.on_page_defalut_back()
            }
        }
    }

}
