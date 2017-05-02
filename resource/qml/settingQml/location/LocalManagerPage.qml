import QtQuick 1.0
import "../baseQml" as BaseQml
import Setting 1.0
Rectangle{
    id:manager
    color: "#000000"
    height: 480
    width: 854
    property string seleCity : ""

    property string searchStr: ""

    Text {
        id: ip
        text: ""
        //字体
        font.family: "MI LANTING"
        font.pixelSize: 24

        color: "#ffffff"

        anchors.left: parent.left
        anchors.leftMargin: 70
        anchors.top: parent.top
        anchors.topMargin: 20
        z:30
    }
    LocationPage{
        id:localpage
        pageVisible:true
        visible: true
        onBack: {
            manager.visible  = false
        }
        onShowVk: {
            show_vk();
        }

    }

    KeyboardPage{
        id:vk
        pageVisible:false
        visible: false

        onComplete: {
            show_searchPage();

        }
        onBacksig: {
            show_localPage()
        }

    }

    SearchResult{
        id:searchRes
        pageVisible:false
        visible: false

        onComplete: {
            show_localPage()
            if(cityNum >0)
            {
                seleCity = searchRes.selectCity
                SettingProperty.set_cityManual(seleCity)
                //                localpage.start_manualLocalAnimation()
                localpage.manualLocalCity = seleCity;
                localpage.set_autoLocal(false)

            }
            else
            {
                seleCity = ""
                localpage.set_autoLocal(true)
            }

        }

        onBack: {
            show_vk()
        }
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

    function show_vk(){

        //隐藏定位页面
        localpage.pageVisible = false
        localpage.visible = false

        //隐藏搜索结果页面
        searchRes.visible = false
        searchRes.pageVisible = false

        //显示键盘
        vk.pageVisible = true
        vk.visible = true
        vk.clear()
    }

    function show_searchPage()
    {
        //隐藏定位页面
        localpage.pageVisible = false
        localpage.visible = false

        //显示搜索结果页面
        searchRes.visible = true
        searchRes.pageVisible = true

        //隐藏键盘
        vk.pageVisible = false
        vk.visible = false
        var tmp = vk.content
        if(tmp.length > 12)
        {
            tmp = tmp.substring(0, 12) + "..."
        }
        searchRes.text = tmp

        SettingProperty.slot_searchCity(vk.content)
        //
        searchRes.start_animation()

    }

    function show_localPage()
    {
        //显示定位页面
        localpage.pageVisible = true
        localpage.visible = true
        localpage.stop_animation()

        //隐藏搜索结果页面
        searchRes.visible = false
        searchRes.pageVisible = false
        searchRes.stop_animation()

        //隐藏键盘
        vk.pageVisible = false
        vk.visible = false
    }

    Connections{
        target: SettingProperty
        onSignal_showCityList:
        {
            searchRes.stop_animation()
            searchRes.show(SettingProperty.slot_getSearchCityNum())
        }
        onSignal_localnet_abnormal:{
            localpage.manualLocalCity = ""
            show_localPage()
            internetAbnormal.visible = true
        }
        onSignal_loaclnet_disconnect:
        {
            localpage.manualLocalCity = ""
            show_localPage()
            wifiDisconnect.visible = true
        }

        onSignal_autoLocalRes:
        {
            var city = SettingProperty.slot_getAutoLocalRes()
            if(city != "")
            {
                localpage.netLocalCity = city
            }
            else
            {
                localpage.netLocalCity = qsTr("Unknown Location")

            }
            localpage.stop_animation()
        }
        onSignal_initLocalpage:{

            //手动定位
            if(SettingProperty.slot_getLocalMode() == 0)
            {
                localpage.set_autoLocal(false)
                localpage.manualLocalCity = SettingProperty.slot_get_locationMannual()
                if(localpage.manualLocalCity == "")
                {
                }
                else
                {
                }
            }
            else
            {

                localpage.set_autoLocal(true )
                localpage.manualLocalCity = "";
            }
            var city = SettingProperty.slot_get_locationAuto()
            if(city != "")
            {
                localpage.netLocalCity = city
            }
            else
            {
                localpage.netLocalCity = qsTr("Unknown Location")
            }
        }
        onSignal_localNetIpChange:
        {
            ip.text = "ip:" + SettingProperty.slot_getLocalIp()
        }
        onSignal_back:
        {
            if(searchRes.visible)
            {
                show_vk()
                return ;
            }
            else if(vk.visible)
            {
                show_localPage()
            }
            else
            {
                SettingProperty.slot_setQMl(0)
            }

        }
    }

}
