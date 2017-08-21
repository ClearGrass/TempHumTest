import QtQuick 1.1
import MainView 1.0
import "valueQml" as ValueQml
import "cardQml" as CardQml
import "verticalScreen" as Vertical
import "toolQml" as ToolQml

Rectangle {
    property int  childWidth: 854
    property int  childHeight: 480
    property string backgroundColor: "#00000000"
    property bool allowPulldown: false


    z:20
    id:root
    visible: true
    width:childWidth;height:childHeight
    color: "#000000"


    //竖屏
    Vertical.VerticalScreen{
        id:vertica
        visible: false
    }

    VisualItemModel {
        id: changePageModel

        Rectangle {
            color: backgroundColor
            width: childWidth; height: childHeight
            ValueQml.TimeAndDate {
                id:timeAndDate
                anchors.centerIn:parent;
            }

        }

        Rectangle {
            color: backgroundColor
            width: childWidth; height: childHeight

            CardQml.PmCard{
                id:pm
                onSensorPmBackIndexChanged: {
                    control.pmIndex =  pm.sensorPmBackIndex
                }
                onOutPmBackIndexChanged: {
                    control.outPmIndex = pm.outPmBackIndex
                }
            }

        }


        Rectangle {
            width: childWidth; height: childHeight
            color: backgroundColor
            CardQml.TvocCard{
                id:tvoc
                anchors.top: parent.top
                anchors.left: parent.left
                onSensortVOCBackIndexChanged: {
                    control.tVOCIndex = tvoc.sensortVOCBackIndex
                }
            }
            CardQml.Co2Card{
                id:co2e
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                onSensorCO2BackIndexChanged: {
                    control.co2Index = co2e.sensorCO2BackIndex
                }
            }
        }

        Rectangle {
            color: backgroundColor
            width: childWidth; height: childHeight

            CardQml.TempCard{
                id:temp
                anchors.top: parent.top
                anchors.left: parent.left
            }
            CardQml.HumCard{
                id:hum
                anchors.left: parent.left
                anchors.bottom: parent.bottom
            }
        }

        Rectangle {
            color: backgroundColor
            width: childWidth; height: childHeight
            ValueQml.AppAndWifi {
                id:appAndWifi
            }

        }

    }

    VisualItemModel {
        id: changePageModel1

        Rectangle {
            color: backgroundColor
            width: childWidth; height: childHeight

            CardQml.PmCard{
                onSensorPmBackIndexChanged: {
                    control.pmIndex =  pm.sensorPmBackIndex
                }
                onOutPmBackIndexChanged: {
                    control.outPmIndex = pm.outPmBackIndex
                }
            }

        }


        Rectangle {
            width: childWidth; height: childHeight
            color: backgroundColor
            CardQml.TvocCard{
                anchors.top: parent.top
                anchors.left: parent.left
                onSensortVOCBackIndexChanged: {
                    control.tVOCIndex = tvoc.sensortVOCBackIndex
                }
            }
            CardQml.Co2Card{
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                onSensorCO2BackIndexChanged: {
                    control.co2Index = co2e.sensorCO2BackIndex
                }
            }
        }

        Rectangle {
            color: backgroundColor
            width: childWidth; height: childHeight

            CardQml.TempCard{
                anchors.top: parent.top
                anchors.left: parent.left
            }
            CardQml.HumCard{
                anchors.left: parent.left
                anchors.bottom: parent.bottom
            }
        }

        Rectangle {
            color: backgroundColor
            width: childWidth; height: childHeight
            ValueQml.AppAndWifi {
            }

        }

    }

    PageControl {
        id:control
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        listModel: MainViewProperty.slot_timeIsInit() ?  changePageModel :changePageModel1
        //        visible: false
        onSignal_mouseMove: {
            //            co2e.set_opacity(1.0)
            //            pm.set_opacity(1.0)
            //            tvoc.set_opacity(1.0)
            //            temp.set_opacity(1.0)
            //            hum.set_opacity(1.0)
            //            appAndWifi.set_opacity(1.0)
            //            timeAndDate.set_opacity(1.0)
        }

    }

    Connections {
        target:MainViewProperty

        //横屏
        onSignal_horizonScreen:{

            control.visible  = true
            vertica.visible = false

            root.width = childWidth;
            root.height = childHeight

        }

        //竖屏
        onSignal_vertialScreen:{
            control.visible = false
            vertica.visible = true

            root.width = childHeight;
            root.height =  childWidth
        }
        onSignal_marginTouch:
        {
            rootMouseArea.enabled = true
        }
        onSignal_no_marginTouch:
        {
            rootMouseArea.enabled = false
        }
        onSignal_timeInit_success:
        {
            control.listModel = changePageModel
        }

    }

    MouseArea{
        id:rootMouseArea
        enabled: false
        anchors.fill: parent
        onClicked: {

        }
    }

}
