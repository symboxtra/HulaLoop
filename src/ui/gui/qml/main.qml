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
    property string currentState: qmlbridge.getTransportState()
    property string barColor: "#888888"
    property bool updateAndTimerBtnEnabled: true

    property int lastVisBarCount: 0
    property int trimFront: 3
    property int trimBack: 3

    QMLBridge {
        id: qmlbridge

        onStateChanged: {
            currentState = qmlbridge.getTransportState()

            // enable/disable update button
            if(currentState === "Ready"){
                updateAndTimerBtnEnabled = true;
            }
            else{
                updateAndTimerBtnEnabled = false;
            }

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

            // TODO: Update when we have the ability to switch visualizers
            if (false)
            {
                // Update the number of bars
                lastVisBarCount = dataIn.length - trimBack - trimFront

                for (var i = trimFront; i < dataIn.length - trimBack; i++) {
                    rectgen.itemAt(i - trimFront).height = Math.round(Math.min(dataIn[i], 1) * visualize.height)
                }
            }
            else
            {
                canvas.readValues(dataIn)
                canvas.clear()

                // Update after values have been assigned
                canvas.binCount = dataIn.length - trimBack - trimFront
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

        DynamicLine {
            id: canvas

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

    onClosing: {
        // This gets called when the user presses the exit btn
        var wannaClose = qmlbridge.wannaClose();
        if(wannaClose)
        {
            // user closed due to not saving files or they have no unsaved files
            close.accepted = true;
        }
        else
        {
            // user cancelled and wants to save the files
            close.accepted = false;
        }
    }
}
