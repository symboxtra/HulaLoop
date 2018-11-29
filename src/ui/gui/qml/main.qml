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

    property int lastVisBarCount: 0
    property int trimFront: 3
    property int trimBack: 3

    property bool anim: false

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

            if (qmlbridge.getTransportState() === qsTr("Recording", "state"))
            {
                let channels = 2
                let sampleRate = 44100
                let interval = Math.min(rawData.length, 441); // Grab every 441st sample
                let refsPerSec = channels * sampleRate / interval
                let scale = 5 // Scale up

                let sum = 0;
                for (var i = 0; i < rawData.length; i++)
                {
                    if (i % interval == 0)
                    {
                        timeline.samples.append(timeline.nextSample, sum / interval * scale);
                        sum = 0;

                        // Move forward
                        // The multiplied value is arbitrarily adjusted
                        // to line up with the time markers. IDK man
                        timeline.nextSample += 1 / refsPerSec * 5.73

                        // Scale the plot
                        if (timeline.nextSample >= timeline.maxTime)
                        {
                            timeline.maxTime += 5
                        }
                    }
                    else
                    {
                        sum += rawData[i];
                    }
                }
            }

            // TODO: Update when we have the ability to switch visualizers
            if (true)
            {
                // Update the number of bars
                /*lastVisBarCount = dataIn.length - trimBack - trimFront

                for (var i = trimFront; i < dataIn.length - trimBack; i++) {
                    rectgen.itemAt(i - trimFront).height = Math.round(Math.min(dataIn[i], 1) * visualize.height)
                }*/

                //circle visualizer instead of bars
                canvas2.readValues(dataIn)
                canvas2.clear()
            }
            else
            {
                canvas.readValues(dataIn)
                canvas.clear()

                // Update after values have been assigned
                canvas.binCount = dataIn.length - trimBack - trimFront
            }
        }

        onDiscarded: {
            timeline.reset()
        }

    }

    SystemTrayIcon {
        id: systrayicon

        Component.onCompleted: systrayicon.init()
    }

    Rectangle {
        id: visualize
        width: parent.width
        height: parent.height - btnPanel.height - bottomRectangle.height
        color: "#080808"
        state: window.anim ? "hide" : "default"

        anchors.left: parent.left
        anchors.bottom: bottomRectangle.top

        states: [
            State {
                name: "hide"
                AnchorChanges {
                    target: visualize
                    anchors.bottom: btnPanel.bottom
                }
            },
            State {
                name: "default"
                AnchorChanges {
                    target: visualize
                    anchors.bottom: bottomRectangle.top
                }
            }

        ]

        transitions: [
            Transition {
                AnchorAnimation {
                    duration: 250
                }
            }
        ]

        DynamicLine {
            id: canvas

            width: parent.width
            height: parent.height
            z: 100

            anchors.centerIn: parent
        }

        CircleVis {
            id: canvas2

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

    Rectangle {
        id: editor
        width: parent.width
        height: parent.height - btnPanel.height - bottomRectangle.height
        color: "#080808"

        anchors.margins: 0

        Timeline {
            id: timeline
        }

        anchors.left: parent.left
        anchors.top: bottomRectangle.top
        state: window.anim ? "show" : "default"
        states: [
            State {
                name: "default"
                AnchorChanges {
                    target: editor
                    anchors.top: bottomRectangle.top
                }
            },
            State {
                name: "show"
                AnchorChanges {
                    target: editor
                    anchors.top: btnPanel.bottom
                }
            }

        ]

        transitions: [
            Transition {
                AnchorAnimation {
                    duration: 250
                }
            }
        ]

    }

    ButtonPanel {
        id: btnPanel
    }

    BottomRectangle {
        id: bottomRectangle
    }


}
