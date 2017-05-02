import QtQuick 1.1
import MainView 1.0
Component {

    id:valueDelegate
    Rectangle {
        color:Qt.rgba(1,1,1,0)
        width:121;height:238;
        anchors.bottom: parent.bottom
        Image {
//            source:"qrc:/resource/images/mainPage/num/"+MainViewProperty.pmValue+".png";
        }
    }
}

