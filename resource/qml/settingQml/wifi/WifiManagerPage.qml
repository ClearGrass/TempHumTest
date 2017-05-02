import QtQuick 1.0
import "../baseQml" as BaseQml
import Setting 1.0
Rectangle{
    color: "#000000"
    height: 480
    width: 854
    property bool wifiListIsEmpty: SettingProperty.slot_wifiListIsEmpty()



    WifiPage{
        id:wifiPage
        visible:!wifiListIsEmpty
    }

    WaittingPage{
        id:waitpage
        visible: wifiListIsEmpty
        entry: SettingProperty.slot_getEntry()
    }

    Connections{
        target: SettingProperty
        onSignal_showWifiList:
        {
            waitpage.visible = false
            wifiPage.visible = true;
            wifiPage.show_ignore()

        }
        onSignal_changeEntry:
        {
            waitpage.entry = SettingProperty.slot_getEntry()
        }

    }
}
