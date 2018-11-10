import QtQuick 2.10
import QtQuick.Window 2.10

import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

import hulaloop.qmlbridge 1.0
import hulaloop.systrayicon 1.0

ApplicationWindow {

    id: window
    title: qsTr("HulaLoop")

    visible: true
    width: 1024
    height: 576

    Material.theme: Material.Grey
    Material.accent: Material.Orange

    QMLBridge {
        id: qmlbridge

        onStateChanged: {

            if(qmlbridge.getTransportState() === "Recording")
            {
                systrayicon.showMessage("HulaLoop Information", "HulaLoop has started recording audio!")
                systrayicon.setRecordIcon()
            }
            else
                systrayicon.setDefaultIcon()

            systrayicon.setToolTip(qsTr("HulaLoop - " + qmlbridge.getTransportState()))
            transportState.text = qmlbridge.getTransportState()
        }
    }

    SystemTrayIcon {
        id: systrayicon

        Component.onCompleted: systrayicon.init()
    }

    ButtonPanel {
        id: btnPanel
    }

    Visualizer {
        id: visualize
    }

    Label {
        id: transportState
        objectName: "transportState"

        anchors.top: btnPanel.bottom
        text: qmlbridge.getTransportState()
    }

    onClosing: {
        // This gets called when the user presses the exit btn
        var hasUnsavedFiles = qmlbridge.checkIfUnsavedFiles();
        if(hasUnsavedFiles)
        {
            // user has unsaved files, prompt to ask if they are sure they want to exit
            close.accepted = false;
        }
        else
        {
            // user has no unsaved files, just exit

        }
    }
}
