import QtQuick 2.10
import QtQuick.Window 2.10

import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

// import Qt.labs.platform 1.0

import hulaloop.qmlbridge 1.0

ApplicationWindow {

    id: window
    title: qsTr("HulaLoop")

    visible: true
    width: 1024
    height: 576

    Material.theme: Material.Grey
    Material.accent: Material.Orange
    property string textDisplayed: "Elapsed time: 0"

    // SystemTrayIcon {
    //     id: systray
    //     visible: true

    //     iconSource: "qrc:/res/hulaloop-logo.svg"
    //     tooltip: qsTr("HulaLoop")

    //     menu: Menu {
    //         MenuItem {
    //             text: qsTr("Quit HulaLoop")
    //             onTriggered: Qt.quit()
    //         }
    //     }
    // }

    QMLBridge {
        id: qmlbridge
        onStateChanged: {
            // if(qmlbridge.getTransportState() === "Recording")
            //     systray.showMessage("HulaLoop Information", "HulaLoop has started recording audio!")
            // systray.tooltip = qsTr("HulaLoop - " + qmlbridge.getTransportState())
            transportState.text = qmlbridge.getTransportState()
        }
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
