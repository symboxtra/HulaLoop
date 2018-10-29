import QtQuick 2.10
import QtQuick.Window 2.10

import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

import hulaloop.qmlbridge 1.0

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
            transportState.text = qmlbridge.getTransportState()
        }
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
}