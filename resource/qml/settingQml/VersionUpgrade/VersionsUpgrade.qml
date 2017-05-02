/*******************************************************************************
* File Name     :   VersionUpgrade.qml
* Description   :   固件升级界面文件
* Author        :   虎正玺
* Date          :   2016-11-10
*******************************************************************************/
import QtQuick 1.1
import Setting 1.0
import "../baseQml" as BaseQml


Rectangle {

    //资源文件定义
    property string buttonBlue: "qrc:/resource/images/optionPage/button/buttonblue.png"
    property string buttonspace: "qrc:/resource/images/optionPage/button/buttonspace.png"
    property string enterSource: "qrc:/resource/images/optionPage/button/button_enter_blue.png"

    property string newVersionId: "1.0"

    property int downloadPercent: 0

    //颜色定义
    property string blackColor: "#292a2c"
    property string blueColor: "#33bbff"
    property string whiteColor: "#ffffff"
    property string transColor: "#00000000"
    property string redColor: "#f44336"
    property string fontFamily: "MI LANTING"

    Rectangle{
        height: 54
        width: 160
        z:20
        radius: 4
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 20
        anchors.leftMargin: 40

        Text {
            anchors.centerIn: parent
            text: "检查测试版本"
            color:blackColor
            font.family: fontFamily
            font.pixelSize: 26
            visible: true

        }
        id:checkDebug

        MouseArea{
            anchors.fill: checkDebug
            onClicked: {
                waiting.visible = true
                waiting.startAnimation()
                SettingProperty.slot_start_checkUpdate(1);

            }

        }
    }

    width: 854
    height: 480
    z:1
    id: upgrade

    color: blackColor

    BaseQml.FrameTop
    {
        id:frameTop
        topTitle: qsTr("Software Version")
    }


    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }


    //网络未连接
    BaseQml.WifiDisconnect{
        id:wifiDisconnect
        visible: false
    }

    //无法联网
    BaseQml.InternetAbnormal{
        id:internetAbnormal
        visible: false
    }

    Text {
        id: curVersion
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 271
        text: qsTr("Current software version:")+SettingProperty.slot_getCurVersion(true)+"/"+SettingProperty.slot_getCurSystem(true)
        color:whiteColor
        font.family: fontFamily
        font.pixelSize: 26
    }

    BaseQml.Waitting{
        run:false
        id:waiting
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: curVersion.bottom
        anchors.topMargin: 60
    }

    Text {
        id: newVersion
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 234
        text: qsTr("Update available:")+newVersionId
        color:whiteColor
        font.family: fontFamily
        font.pixelSize: 26
        visible: false

    }
    Text {
        id: noVersion
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 234
        text: qsTr("Have not found updates")
        color:whiteColor
        font.family: fontFamily
        font.pixelSize: 26
        visible: false

    }

    OtaDescribe{
        id:otadescRec
        anchors.centerIn: parent

        onClose: {
            mousAreaUpdate.visible = true
        }
        onSure: {
            mousAreaUpdate.visible = true
            SettingProperty.slot_start_download()
            updating()
        }
    }


    Text {
        id:versionDes
        anchors.horizontalCenter:  parent.horizontalCenter
        anchors.horizontalCenterOffset: -15
        anchors.bottom: update.top
        anchors.bottomMargin: 79

        text: qsTr("Update")
        color:whiteColor
        font.family: fontFamily
        font.pixelSize: 26
        MouseArea{
            height:40
            width: 160
            id:mouseAreaVersionDes
            anchors.centerIn:  versionDes
            onClicked: {
                otadescRec.visible = true
                mousAreaUpdate.visible = false
            }
            onPressed: {
                enterImage.opacity = 0.5
                versionDes.opacity = 0.5
            }
            onReleased: {
                enterImage.opacity = 1.0
                versionDes.opacity = 1.0
            }
        }

        Image {
            id: enterImage
            source: enterSource
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.right
            visible: parent.visible
        }
    }




    Image{
        id: check
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter

        source: buttonspace
        Text {

            //布局
            anchors.centerIn: parent
            text: qsTr("Check for Updates")
            color:"#b3ffffff"
            font.family: fontFamily
            font.pixelSize: 26
        }

    }

    Image{
        id: update
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        source:buttonBlue

        Text {
            id: updatetest

            //布局
            anchors.centerIn: parent
            text: qsTr("Update Now")
            color:whiteColor
            font.family: fontFamily
            font.pixelSize: 26
        }

    }


    //下载
    Text {
        id: downloading
        text: qsTr("Downloading")+downloadPercent +"%"
        color:blueColor
        font.family: fontFamily
        font.pixelSize: 26
        visible: false

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 143
    }

    //下载进度条
    Rectangle{
        id:prograss
        color: whiteColor
        width: 482
        height: 8
        radius: 4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: downloading.top
        anchors.bottomMargin: 30
        visible: false

        Rectangle{
            id:prograssBar
            height: 8
            radius: 4
            width: prograss.width * (downloadPercent /100.0)
            color: blueColor
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 0
            anchors.leftMargin: 0
        }
    }


    Image{
        id: retry
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        source:buttonBlue

        Text {

            //布局
            anchors.centerIn: parent
            text: qsTr("Retry")
            color:blackColor
            font.family: fontFamily
            font.pixelSize: 26
        }

    }

    Text {
        id: downloadFail
        text: qsTr("Failed to download")
        color: redColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom:retry.top
        anchors.bottomMargin: 49
        font.family: fontFamily
        font.pixelSize: 26
        visible: false
    }

    Text {
        id: reboot
        text: qsTr("After complete the installation, the device will restart automatically.")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 105

        font.family: fontFamily
        font.pixelSize: 26
        color: blueColor

        visible: false
    }

    Text {
        id: settingUp
        text: qsTr("Installing")

        font.family: fontFamily
        font.pixelSize: 26
        color: whiteColor

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: reboot.top
        anchors.bottomMargin: 14
        visible: false
    }

    MouseArea{
        id:mousAreaCheck
        anchors.fill: check
        onClicked: {
            waiting.visible = true
            waiting.startAnimation()
            SettingProperty.slot_start_checkUpdate(0);
        }
        onPressed: {
            check.opacity = 0.5
        }
        onReleased: {
            check.opacity = 1.0
        }

    }
    MouseArea{
        id:mousAreaUpdate
        anchors.fill: update
        onClicked: {
            SettingProperty.slot_start_download()
            updating()
        }
        onPressed: {
            update.opacity = 0.5
        }
        onReleased: {
            update.opacity = 1.0
        }
    }

    MouseArea{
        id:mousAreaRetry
        anchors.fill: retry
        onClicked: {
            SettingProperty.slot_start_download()
            updating();
        }
        onPressed: {
            retry.opacity = 0.5
        }
        onReleased: {
            retry.opacity = 1.0
        }
    }

    MouseArea{
        id:mousAreaBack
        anchors.fill: backButton
        onClicked: {
            SettingProperty.slot_setQMl(0)
        }
        onPressed: {
            backButton.opacity = 0.5
        }
        onReleased: {
            backButton.opacity = 1.0
        }

    }

    Connections{
        target: SettingProperty

        //更新进度条
        onSignal_updateProgress:
        {
            downloadPercent = SettingProperty.slot_getProgress();
            if(prograss.visible == false)
            {
                updating()
            }
        }

        //更新版本信息
        onSignal_update_firmware:
        {
            newVersionId = SettingProperty.slot_getCurVersion(false);

            exise_version(true)
        }

        //无版本信息
        onSignal_no_firmware:
        {
            exise_version(false)
        }

        //下载失败
        onSignal_update_download_fail:
        {
            retry_update();
        }

        onSignal_update_download_success:{
            download_success()
        }

        onSignal_initFirwarePage:{
            page_init()
        }
        onSignal_errorInfo:{
            var errorno =  SettingProperty.slot_getErrorCode()
            if(1 == errorno)
            {
                internetAbnormal.visible = true
            }
            else if(2 == errorno)
            {
                wifiDisconnect.visible = true
            }
            else
            {
            }


        }
        onSignal_back:
        {
            if(SettingProperty.slot_getCurPageIndex() == 16)
            {
                if(otadescRec.visible == false)
                {
                    SettingProperty.slot_setQMl(0)
                }
                else
                {
                    otadescRec.visible = false
                }
            }
        }

    }

    //页面初始化
    function page_init()
    {

        newVersionId = SettingProperty.slot_getCurVersion(false)
        if(newVersionId == "")
        {
            exise_version(false)
            noVersion.visible = false
            waiting.stopAnimation()
        }
        else
        {
            exise_version(true)
        }
        //        mousAreaBack.visible = true

    }

    //检查更新
    function check_update(){
        curVersion.visible = true
        newVersion.visible  = false
        noVersion.visible = false
        downloading.visible = false
        prograss.visible  = false
        downloadFail.visible  = false
        reboot.visible  = false
        settingUp.visible = false

        //
        mousAreaCheck.visible  = true
        mousAreaRetry.visible = false
        mousAreaUpdate.visible  = false
        check.visible = true
        update.visible = false
        retry.visible  = false
        versionDes.visible  = false
        mouseAreaVersionDes.visible  = false
    }

    //有版本
    function exise_version(flag)
    {


        curVersion.visible = true

        downloading.visible = false
        prograss.visible  = false
        downloadFail.visible  = false
        reboot.visible  = false
        settingUp.visible = false
        waiting.visible = false

        if(flag == true)
        {
            noVersion.visible = false
            newVersion.visible  = true
            newVersion.color = blueColor
            mousAreaUpdate.visible  = true
            mousAreaCheck.visible  = false
            check.visible = false
            update.visible = true
            versionDes.visible  = true
            otadescRec.text = SettingProperty.slot_getVersionDes()
            mouseAreaVersionDes.visible  = true

        }
        else
        {
            newVersion.visible  = false
            noVersion.visible = true
            noVersion.color  = blueColor
            mousAreaUpdate.visible  = false
            mousAreaCheck.visible  = true
            check.visible = true
            update.visible = false
            versionDes.visible  = false
            mouseAreaVersionDes.visible  = false

        }

        mousAreaRetry.visible = false

        retry.visible  = false
    }

    //正在更新
    function updating()
    {
        curVersion.visible = true
        newVersion.visible  = true
        noVersion.visible = false
        downloading.visible = true
        prograss.visible  = true
        downloadFail.visible  = false
        reboot.visible  = false
        settingUp.visible = false
        prograssBar.color = blueColor


        mousAreaCheck.visible  = false
        mousAreaRetry.visible = false
        mousAreaUpdate.visible  = false
        versionDes.visible  = false

        //下载过程中不允许返回
        //        mousAreaBack.visible = false
        check.visible = false
        update.visible = false
        retry.visible  = false
        waiting.visible = false
        waiting.stopAnimation()
        mouseAreaVersionDes.visible  = false

    }

    //重试
    function retry_update()
    {

        curVersion.visible = true
        newVersion.visible  = true
        noVersion.visible = false
        downloading.visible = false
        prograss.visible  = true
        downloadFail.visible  = true
        reboot.visible  = false
        settingUp.visible = false
        prograssBar.color = redColor
        versionDes.visible  = false
        mouseAreaVersionDes.visible  = false


        mousAreaCheck.visible  = false
        mousAreaRetry.visible = true
        mousAreaUpdate.visible  = false

        //        mousAreaBack.visible = true
        check.visible = false
        update.visible = false
        retry.visible  = true
        waiting.visible = false
        waiting.stopAnimation()

    }

    //下载成功
    function download_success()
    {
        curVersion.visible = true
        newVersion.visible  = true
        noVersion.visible = false
        downloading.visible = false
        prograss.visible  = true
        downloadFail.visible  = false
        reboot.visible  = true
        settingUp.visible = true
        prograssBar.color = blueColor
        versionDes.visible  = false
        mouseAreaVersionDes.visible  = false


        mousAreaCheck.visible  = false
        mousAreaRetry.visible = false
        mousAreaUpdate.visible  = false

        //        mousAreaBack.visible = true
        check.visible = false
        update.visible = false
        retry.visible  = false
        waiting.visible = false
        waiting.stopAnimation()

    }



}
