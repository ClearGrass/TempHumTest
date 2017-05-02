import QtQuick 1.1
import "../settingQml/baseQml" as BaseQml

Rectangle{
    height: 480
    width:  854
    color:"#292a2c"
    anchors.centerIn: parent
    visible: true
    BaseQml.Waitting{
        id:waitAnimation
        anchors.centerIn: parent
        run:true
    }

}

