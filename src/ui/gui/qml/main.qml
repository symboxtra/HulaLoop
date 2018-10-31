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
    Visualizer{
        id:visualize
    }
    Rectangle {
        width: window.width
        height: parent.height * 0.6
        color: "lightsteelblue"
        y: 98

        Row {
            y: parent.height-50
            x: -40
            Repeater{
                id:rectgen
                model: Math.round(visualize.width / 10)
                Rectangle { id: testrec; color: "grey"; width: 10; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
            }
        }
    }

    QMLBridge {
        id: qmlbridge
        onStateChanged: {
            transportState.text = qmlbridge.getTransportState()
        }
        onVisData: {
            for(var i =0;i<dataIn.length;i++){
                rectgen.itemAt(i).height=dataIn[i]*100
            }
        }
    }

    ButtonPanel {
        id: btnPanel
    }

    Label {
        id: transportState
        objectName: "transportState"

        anchors.top: btnPanel.bottom
        text: "Initial State"
    }
}