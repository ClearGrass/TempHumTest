import QtQuick 1.1
import "../baseQml" as BaseQml
import Setting 1.0

Rectangle{
    property string sourceIcon: "qrc:resource/images/optionPage/icon/icon_pmFrequency.png"
    property int settingType: SettingProperty.slot_getPmDetetingSettingType()

    height: 480
    width: 854
    color:"#292a2c"

    BaseQml.FrameTop
    {
        id:frameTop
        iconSource: sourceIcon
        topTitle: settingType == 0  ? qsTr("Power Adaptor") :  qsTr("Battery")
    }

    //返回按钮
    BaseQml.BackButton{
        id:backButton
    }



    BaseQml.BaseListView{
        id:powerAdapter
        model:pmModel
        curindex:get_initIndex()
        anchors{
            bottom:frameTop.bottom
            left:parent.left
        }

    }

    ListModel {
        id:pmModel
        ListElement {

        }
        ListElement {

        }
        ListElement {
        }

        function title( index) {
            if ( title[ "text" ] === undefined) {
                title.text = [
                            qsTr( "5 mins" ) ,
                            qsTr( "15 mins" ) ,
                            qsTr( "30 mins" )
                        ]
            }
            return title.text [ index]
        }

    }

    function get_initIndex()
    {
        var  value = SettingProperty.slot_getPmDetectingFrequency(settingType)
       if(value == "5")
       {
           return 0;
       }
       else if(value == "15")
       {
           return 1;
       }
       else
       {
           return 2;
       }

    }

    function get_curIndex()
    {
        var  curindex = powerAdapter.curindex;

        switch(curindex)
        {
        case 0:
            return "5";
        case 1:
            return "15";
        case 2:
            return "30";

        }
    }

    MouseArea{
        anchors.fill: backButton
        onClicked: {
            SettingProperty.slot_setPmDetetingFrequency(settingType,get_curIndex())
            SettingProperty.slot_setQMl(19)
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
        onSignal_back:
        {
            SettingProperty.slot_setQMl(19)
        }
    }
}
