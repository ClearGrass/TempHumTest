import QtQuick 1.0
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle {
    width: 854; height: 480

    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_wifi.png"
    property int entry:SettingProperty.slot_getEntry()

    color:"#292a2c"
    z:11

    BaseQml.FrameTop
    {
        id:frameTop
        iconSource: sourceIcon
        topTitle: "Wi-Fi"
    }

    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }

    BaseQml.Waitting{
        id:waitAni
        anchors.centerIn: parent
        run:true
    }

    MouseArea{
        anchors.fill: backButton
        onClicked:
        {
            if(3 !=entry)
            {
                waitAni.stop_wait();
                SettingProperty.slot_setQMl(0)
                if(entry == 1)
                {
                    SettingProperty.on_page_defalut_back()
                }
            }
            else //开机向导
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
    Connections{
        target: SettingProperty
        onSignal_changeEntry:
        {
            entry = SettingProperty.slot_getEntry()
        }

    }

}
