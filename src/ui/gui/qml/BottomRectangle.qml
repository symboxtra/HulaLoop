import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.2

Rectangle {

    id: bottomRect
    width: parent.width
    height: parent.height * 0.05
    color: "white"
    anchors.bottom: parent.bottom

    RowLayout {
        id: timeLabelLayout

        Label {
            id: timerLabel

            color: "black"
            font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
            text: "Timer:"
        }

        Text {
            id: textCountdown
            objectName: "textCountdown"

            color: "black"
            font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
            text: "0"

            property int time: getTime()
            property int time2: getTime2()
            function getTime() {
                var d = delayInput.text
                var h = parseInt(d.substring(0, 2))
                var m = parseInt(d.substring(3, 5))
                var s = parseInt(d.substring(6, 8))
                var timeRem = h * 60 * 60 + m * 60 + s
                return timeRem
            }
            function getTime2() {
                var d = recordTimeInput.text
                var h = parseInt(d.substring(0, 2))
                var m = parseInt(d.substring(3, 5))
                var s = parseInt(d.substring(6, 8))
                var timeRem = h * 60 * 60 + m * 60 + s
                return timeRem
            }
        }
    }

    Text {
        id: transportState
        objectName: "transportState"
        text: "State: " + qmlbridge.getTransportState()
        anchors.right: parent.right
        font.pixelSize: Math.ceil(parent.width * 0.02)
    }
}
