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

    Label {
        id: textCountdown
        objectName: "textCountdown"

        color: "black"
        font.pixelSize: Math.ceil(btnPanel.width * 0.02)
        text: window.textDisplayed
    }


    Label {
        id: transportState
        objectName: "transportState"
        font.pixelSize: Math.ceil(parent.width * 0.02)
        color: "black"
        text: window.currentState
        anchors.right: parent.right
    }
}
