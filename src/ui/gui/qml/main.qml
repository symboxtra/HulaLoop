import QtQuick 2.10
import QtQuick.Window 2.10

import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.1
import hulaloop.qmlbridge 1.0
import hulaloop.systrayicon 1.0

ApplicationWindow {
    id: window

    title: "HulaLoop"
    minimumHeight: 500
    minimumWidth: 800

    visible: true
    width: 1024
    height: 576

    Material.theme: Material.Grey
    Material.accent: Material.Orange

    property string textDisplayed: "Elapsed: 0"
    property string currentState: "Unknown"
    property string barColor: "#888888"

    property string visType: "Line"

    property int lastVisBarCount: 0
    property int trimFront: 3
    property int trimBack: 3

    QMLBridge {
        id: qmlbridge

        onStateChanged: {
            currentState = qmlbridge.getTransportState()

            if(qmlbridge.getTransportState() === "Recording")
            {
                systrayicon.showMessage(qsTr("HulaLoop Information"), qsTr("HulaLoop has started recording audio!"))
                systrayicon.setRecordIcon()
            }
            else
            {
                systrayicon.setDefaultIcon()
            }

            systrayicon.setToolTip(qsTr("HulaLoop - " + qmlbridge.getTransportState()))
        }

        onVisData: {
            if (visType === "Bar")
            {
                lv.visible = false
                cv.visible = false
                // Update the number of bars
                lastVisBarCount = dataIn.length - trimBack - trimFront

                for (var i = trimFront; i < dataIn.length - trimBack; i++) {
                    rectgen.itemAt(i - trimFront).height = Math.round(Math.min(dataIn[i], 1) * visualize.height)
                }
            }
            else if (visType === "Circle")
            {
                lv.visible = false
                cv.visible = true
                //circle visualizer instead of bars
                cv.readValues(dataIn)
                cv.clear()

                cv.binCount = dataIn.length - trimBack - trimFront
            }
            else if (visType === "Line")
            {
                lv.visible = true
                cv.visible = false
                lv.readValues(dataIn)
                lv.clear()

                // Update after values have been assigned
                lv.binCount = dataIn.length - trimBack - trimFront
            }
        }
    }

    SystemTrayIcon {
        id: systrayicon

        Component.onCompleted: systrayicon.init()
    }

    ButtonPanel {
        id: btnPanel
    }

    BottomRectangle {
        id: bottomRectangle
    }

    Rectangle {
        id: visualize
        width: parent.width
        height: parent.height - btnPanel.height - bottomRectangle.height
        color: "#080808"

        anchors.left: parent.left
        anchors.bottom: bottomRectangle.top

        LineVisualizer {
            id: lv

            width: parent.width
            height: parent.height
            z: 100

            anchors.centerIn: parent
        }

        CircleVisualizer {
            id: cv

            width: parent.width
            height: parent.height
            z: 100

            anchors.centerIn: parent
        }

        Row {
            // Shift over to accomodate for the weird gap
            x: visualize.width / lastVisBarCount * -2
            y: parent.height - 50
            width: parent.width
            height: parent.height

            Repeater {
                id: rectgen
                model: lastVisBarCount

                Rectangle {
                    id: testrec
                    color: "#005B9A"
                    width: Math.ceil(visualize.width / lastVisBarCount)

                    border.width: 1
                    border.color: "#003366"

                    transform: Rotation {
                        origin.x: 25
                        origin.y: 25
                        angle: 180
                    }
                }
            }
        }
    }
}
