import QtQuick 1.0

Rectangle{
    height: 36
    width: 36
    property string sourceLoading: "qrc:/resource/images/optionPage/tools/loading.png"
    property bool run: true
    color: "#00000000"
    Image {
        id: loadingImage
        source: sourceLoading
        smooth: true
        anchors.centerIn: parent

        NumberAnimation on rotation {
            id:animationLarge ; from: 0; to: 360; running: run; loops: Animation.Infinite; duration: 1000
        }

    }


    function startAnimation()
    {
        animationLarge.start()
    }
    function stopAnimation()
    {
        animationLarge.stop()
    }

    function isStart()
    {
        if(animationLarge.running == true )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    function stop_wait()
    {
        animationLarge.stop()
    }



}
