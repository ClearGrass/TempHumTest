import QtQuick 1.0

Rectangle {
    id: vk
    property string rotateLeft: "\u2939"
    property string rotateRight: "\u2935"
    property string leftArrow: "\u2190"
    property string upperArrow: "\u2191"
    property string division : "\u00f7"
    property string multiplication : "\u00d7"
    property string squareRoot : "\u221a"
    property string plusminus : "\u00b1"
    property string shiftSource: "qrc:/resource/images/optionPage/keyboard/shiftIconNormal.png"
    property string deleteSource: "qrc:/resource/images/optionPage/keyboard/deleteIconNormal.png"
    property string shiftSourceUp: "qrc:/resource/images/optionPage/keyboard/shiftIconHoverBlack.png"
    signal buttonPressed(string op)
    signal symbolChanege()
    signal letterChange()
    signal sig_complete()
    signal signal_calcel()
    property int keyType: 1
    property int letterTyoe: 1
    property bool isWifiVK: false

    anchors {
        left: parent.left
        right: parent.right
    }

    property Item surface: parent

    y: (surface.height - vkc.height) - 4


    Column {
        id: vkc;

        anchors {
            horizontalCenter: parent.horizontalCenter
            margins: 8
        }

        spacing: 8
        Row {
            spacing: 8
            anchors.horizontalCenter: parent.horizontalCenter
            Button { lettertext: "1"; op: "1"; shiftText: ""; Component.onCompleted: clicked.connect(buttonPressed)
                id:l1
            }
            Button { lettertext: "2"; op: "2"; shiftText: "~"; Component.onCompleted: clicked.connect(buttonPressed)
                id:l2
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            l2.use_letter();
                        }
                        else
                        {
                            l2.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            l2.set_lower()
                        else
                            l2.set_upper()
                    }
                }
            }
            Button { lettertext: "3"; op: "3"; shiftText: "@"; Component.onCompleted: clicked.connect(buttonPressed)
                id:l3
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            l3.use_letter();
                        }
                        else
                        {
                            l3.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            l3.set_lower()
                        else
                            l3.set_upper()
                    }
                }
            }
            Button { lettertext: "4"; op: "4"; shiftText: "#"; Component.onCompleted: clicked.connect(buttonPressed)
                id:l4
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            l4.use_letter();
                        }
                        else
                        {
                            l4.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            l4.set_lower()
                        else
                            l4.set_upper()
                    }
                }
            }
            Button { lettertext: "5"; op: "5"; shiftText: "$"; Component.onCompleted: clicked.connect(buttonPressed)
                id:l5
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            l5.use_letter();
                        }
                        else
                        {
                            l5.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            l5.set_lower()
                        else
                            l5.set_upper()
                    }
                }
            }
            Button { lettertext: "6"; op: "6"; shiftText: "%"; Component.onCompleted: clicked.connect(buttonPressed)
                id:l6
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            l6.use_letter();
                        }
                        else
                        {
                            l6.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            l6.set_lower()
                        else
                            l6.set_upper()
                    }
                }
            }
            Button { lettertext: "7"; op: "7"; shiftText: "^"; Component.onCompleted: clicked.connect(buttonPressed)
                id:l7
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            l7.use_letter();
                        }
                        else
                        {
                            l7.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            l7.set_lower()
                        else
                            l7.set_upper()
                    }
                }
            }
            Button { lettertext: "8"; op: "8"; shiftText: "&"; Component.onCompleted: clicked.connect(buttonPressed)
                id:l8
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            l8.use_letter();
                        }
                        else
                        {
                            l8.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            l8.set_lower()
                        else
                            l8.set_upper()
                    }
                }
            }
            Button { lettertext: "9"; op: "9"; shiftText: "_"; Component.onCompleted: clicked.connect(buttonPressed)
                id:l9
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            l9.use_letter();
                        }
                        else
                        {
                            l9.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            l9.set_lower()
                        else
                            l9.set_upper()
                    }
                }
            }
            Button { lettertext: "0"; op: "0"; shiftText: ""; Component.onCompleted: clicked.connect(buttonPressed)
                id:l0            }
        }

        Row {
            spacing: 8
            anchors.horizontalCenter: parent.horizontalCenter
            Button { lettertext: "q"; op: "q"; shiftText: ""; Component.onCompleted: clicked.connect(buttonPressed)
                id:q
                Connections{
                    target: vk
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            q.set_lower()
                        else
                            q.set_upper()
                    }
                }
            }
            Button { lettertext: "w"; op: "w"; shiftText: "`"; Component.onCompleted: clicked.connect(buttonPressed)
                id:w
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            w.use_letter();
                        }
                        else
                        {
                            w.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            w.set_lower()
                        else
                            w.set_upper()
                    }
                }
            }
            Button { lettertext: "e"; op: "e"; shiftText: "|"; Component.onCompleted: clicked.connect(buttonPressed)
                id:e
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            e.use_letter();
                        }
                        else
                        {
                            e.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            e.set_lower()
                        else
                            e.set_upper()
                    }
                }
            }
            Button { lettertext: "r"; op: "r"; shiftText: "\\"; Component.onCompleted: clicked.connect(buttonPressed)
                id:r
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            r.use_letter();
                        }
                        else
                        {
                            r.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            r.set_lower()
                        else
                            r.set_upper()
                    }
                }
            }
            Button { lettertext: "t"; op: "t"; shiftText: "/"; Component.onCompleted: clicked.connect(buttonPressed)
                id:t
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            t.use_letter();
                        }
                        else
                        {
                            t.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            t.set_lower()
                        else
                            t.set_upper()
                    }
                }
            }
            Button { lettertext: "y"; op: "y"; shiftText: "*"; Component.onCompleted: clicked.connect(buttonPressed)
                id:y
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            y.use_letter();
                        }
                        else
                        {
                            y.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            y.set_lower()
                        else
                            y.set_upper()
                    }
                }
            }
            Button { lettertext: "u"; op: "u"; shiftText: "+"; Component.onCompleted: clicked.connect(buttonPressed)
                id:u
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            u.use_letter();
                        }
                        else
                        {
                            u.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            u.set_lower()
                        else
                            u.set_upper()
                    }
                }
            }
            Button { lettertext: "i"; op: "i"; shiftText: "-"; Component.onCompleted: clicked.connect(buttonPressed)
                id:i
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            i.use_letter();
                        }
                        else
                        {
                            i.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            i.set_lower()
                        else
                            i.set_upper()
                    }
                }
            }
            Button { lettertext: "o"; op: "o"; shiftText: "="; Component.onCompleted: clicked.connect(buttonPressed)
                id:o
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            o.use_letter();
                        }
                        else
                        {
                            o.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            o.set_lower()
                        else
                            o.set_upper()
                    }
                }
            }
            Button { lettertext: "p"; op: "p"; shiftText: "0"; Component.onCompleted: clicked.connect(buttonPressed)
                id:p
                Connections{
                    target: vk
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            p.set_lower()
                        else
                            p.set_upper()
                    }
                }

            }
        }
        Row {
            spacing: 8
            anchors.horizontalCenter: parent.horizontalCenter
            Button { lettertext: "a"; op: "a"; shiftText: "("; Component.onCompleted: clicked.connect(buttonPressed)
                id:a
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            a.use_letter();
                        }
                        else
                        {
                            a.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            a.set_lower()
                        else
                            a.set_upper()
                    }
                }
            }
            Button { lettertext: "s"; op: "s"; shiftText: ")"; Component.onCompleted: clicked.connect(buttonPressed)
                id:s
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            s.use_letter();
                        }
                        else
                        {
                            s.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            s.set_lower()
                        else
                            s.set_upper()
                    }
                }
            }
            Button { lettertext: "d"; op: "d"; shiftText: "["; Component.onCompleted: clicked.connect(buttonPressed)
                id:d
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            d.use_letter();
                        }
                        else
                        {
                            d.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            d.set_lower()
                        else
                            d.set_upper()
                    }
                }
            }
            Button { lettertext: "f"; op: "f"; shiftText: "]"; Component.onCompleted: clicked.connect(buttonPressed)
                id:f
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            f.use_letter();
                        }
                        else
                        {
                            f.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            f.set_lower()
                        else
                            f.set_upper()
                    }
                }
            }
            Button { lettertext: "g"; op: "g"; shiftText: "{"; Component.onCompleted: clicked.connect(buttonPressed)
                id:g
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            g.use_letter();
                        }
                        else
                        {
                            g.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            g.set_lower()
                        else
                            g.set_upper()
                    }
                }
            }
            Button { lettertext: "h"; op: "h"; shiftText: "}"; Component.onCompleted: clicked.connect(buttonPressed)
                id:h
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            h.use_letter();
                        }
                        else
                        {
                            h.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            h.set_lower()
                        else
                            h.set_upper()
                    }
                }
            }
            Button { lettertext: "j"; op: "j"; shiftText: "<";  Component.onCompleted: clicked.connect(buttonPressed)
                id:j
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            j.use_letter();
                        }
                        else
                        {
                            j.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            j.set_lower()
                        else
                            j.set_upper()
                    }
                }
            }
            Button { lettertext: "k"; op: "k"; shiftText: ">";  Component.onCompleted: clicked.connect(buttonPressed)
                id:k
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            k.use_letter();
                        }
                        else
                        {
                            k.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            k.set_lower()
                        else
                            k.set_upper()
                    }
                }
            }
            Button { lettertext: "l"; op: "l"; shiftText: "\'";  Component.onCompleted: clicked.connect(buttonPressed)
                id:l
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            l.use_letter();
                        }
                        else
                        {
                            l.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            l.set_lower()
                        else
                            l.set_upper()
                    }
                }
            }
        }
        Row {
            spacing: 8
            anchors.horizontalCenter: parent.horizontalCenter
            ButtonPlus{
                id:shift
                source:shiftSource;

                onClicked: {

                    //小写
                    if(letterTyoe == 1)
                    {
                        letterTyoe = 0;
                        shift.source  = shiftSourceUp
                        shift.color = "#ffffff"

                    }
                    else
                    {
                        letterTyoe = 1;
                        shift.source = shiftSource
                        shift.color = "#00000000"

                    }
                    vk.letterChange();
                }

            }
            Button { lettertext: "z"; op: "z"; shiftText: "\""; Component.onCompleted: clicked.connect(buttonPressed)
                id:z
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            z.use_letter();
                        }
                        else
                        {
                            z.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            z.set_lower()
                        else
                            z.set_upper()
                    }
                }
            }
            Button { lettertext: "x"; op: "x"; shiftText: ":";  Component.onCompleted: clicked.connect(buttonPressed)
                id:x
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            x.use_letter();
                        }
                        else
                        {
                            x.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            x.set_lower()
                        else
                            x.set_upper()
                    }
                }
            }
            Button { lettertext: "c"; op: "c"; shiftText: ";";  Component.onCompleted: clicked.connect(buttonPressed)
                id:c
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            c.use_letter();
                        }
                        else
                        {
                            c.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            c.set_lower()
                        else
                            c.set_upper()
                    }
                }
            }
            Button { lettertext: "v"; op: "v"; shiftText: "?"; Component.onCompleted: clicked.connect(buttonPressed)
                id:v
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            v.use_letter();
                        }
                        else
                        {
                            v.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            v.set_lower()
                        else
                            v.set_upper()
                    }
                }
            }
            Button { id:b;lettertext: "b"; op: "b"; shiftText: "!"; Component.onCompleted: clicked.connect(buttonPressed)
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            b.use_letter();
                        }
                        else
                        {
                            b.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            b.set_lower()
                        else
                            b.set_upper()
                    }
                }
            }
            Button { lettertext: "n"; op: "n"; shiftText: ",";  Component.onCompleted: clicked.connect(buttonPressed)
                id:n
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            n.use_letter();
                        }
                        else
                        {
                            n.use_sysmbol();
                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            n.set_lower()
                        else
                            n.set_upper()
                    }
                }
            }
            Button
            {
                id:m
                lettertext: "m"; op: "m"; shiftText: "."; shiftOp: "+" ; Component.onCompleted: clicked.connect(buttonPressed)
                Connections{
                    target: vk
                    onSymbolChanege:
                    {
                        if(keyType == 1)
                        {
                            m.use_letter();

                        }
                        else
                        {
                            m.use_sysmbol();

                        }
                    }
                    onLetterChange:
                    {
                        if(1 == letterTyoe)
                            m.set_lower()
                        else
                            m.set_upper()
                    }
                }
            }
            ButtonPlus{source:deleteSource;op: leftArrow; Component.onCompleted: clicked.connect(buttonPressed) }

        }
        Row {
            spacing: 8
            anchors.horizontalCenter: parent.horizontalCenter

            Button2s{
                id:cancelButton
                text:qsTr("cancel")
                onClicked: {
                    signal_calcel()
                }
            }

            Button2s{
                id:symbol
                text: "#+="
                onClicked:
                {
                    keyType = (keyType == 1) ?  0 :1;
                    l1.visible = keyType
                    l0.visible  = keyType
                    q.visible = keyType
                    p.visible = keyType
                    shift.visible = keyType
                    vk.symbolChanege()
                    if(1 == keyType)
                    {
                        symbol.text = "#+="
                    }
                    else
                    {
                        symbol.text = "abc"
                        q.set_lower()
                        p.set_lower()
                        shift.color = "#00000000"
                        letterTyoe = 1;
                        shift.source = shiftSource
                    }
                }
            }

            Button2sPlus{
                id: space
                text: qsTr("Space")
                onClicked: buttonPressed(" ")

            }

            Button2s{
                id:ok
                text: get_okStr()
                onClicked:
                {
                    console.log("ok  clicked-------")
                    sig_complete();
                }
            }
        }

    }
    function set_okEnable()
    {
        ok.set_enable()
    }
    function set_okDisable()
    {
        ok.set_disable()
    }

    function get_okStr()
    {
        if(isWifiVK)
        {
            return   qsTr("ConnectWifi")
        }
        else
        {
            return  qsTr("SearchLocaltion")
        }

    }

    function set_okText(text)
    {
        ok.text = text
    }

    function set_spaceText(text)
    {
        ok.text = text
    }

}
