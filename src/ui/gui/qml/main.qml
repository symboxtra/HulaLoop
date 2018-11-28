import QtQuick 2.10
import QtQuick.Window 2.10

import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

import hulaloop.qmlbridge 1.0
import hulaloop.systrayicon 1.0

ApplicationWindow {

    id: window
    title: "HulaLoop"

    visible: true
    width: 1024
    height: 576

    Material.theme: Material.Grey
    Material.accent: Material.Orange
    property string textDisplayed: "Elapsed: 0"
    property string currentState: "Unknown"
    property string barColor: "#888888"

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

    BottomRectangle {
        id: bottomRectangle
    }

}
