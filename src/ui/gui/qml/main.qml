import QtQuick 2.10
import QtQuick.Window 2.10

import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.1
import hulaloop.qmlbridge 1.0

ApplicationWindow {
    id: window
    title: qsTr("HulaLoop")
    minimumHeight: 500
    minimumWidth: 800
    visible: true
    width: 1024
    height: 576

    Material.theme: Material.Grey
    Material.accent: Material.Orange
    DynamicLine{
        id: canvas
    }

    Visualizer{
        id:visualize
    }

    Rectangle {
        id: bluerect
        width: window.width
        height: parent.height
        color: "lightsteelblue"
        y: 98

        Row {

            y: parent.height-150
            x: -40
            //Rectangle { id: testrec2; color: "orange"; width: 10;height:200; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }

            Repeater{
                id:rectgen
                model: Math.round(visualize.width / 10)
                Rectangle { id: testrec; color: "grey"; width: window.width/64; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
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
            canvas.readValues(dataIn)
            canvas.clear()
            //canvas.requestPaint()
        }

    }

    ButtonPanel {
        id: btnPanel
    }

    Label {
        id: transportState
        objectName: "transportState"

        anchors.top: btnPanel.bottom
        text: qmlbridge.getTransportState()
    }
}
