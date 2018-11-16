import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.2

Rectangle {

    id: bottomRect
    width: parent.width
    height: 20
    color: "white"
    anchors.bottom: parent.bottom

    RowLayout{
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
