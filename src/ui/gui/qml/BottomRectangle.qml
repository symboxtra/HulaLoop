import QtQuick 2.10

import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Rectangle {
    id: bottomRect

    width: parent.width
    height: 20
    color: window.barColor
    anchors.bottom: parent.bottom

    RowLayout {
        anchors.right: parent.right
        anchors.rightMargin: 15

        Label {
            id: transportState
            objectName: "transportState"

            color: "black"
            text: window.currentState
        }

        Label {
            id: textCountdown
            objectName: "textCountdown"

            color: "black"
            text:  ("  |  " + window.textDisplayed)
        }
    }
}