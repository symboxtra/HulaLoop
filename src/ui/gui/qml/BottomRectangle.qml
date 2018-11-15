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

    Text {
        id: textCountdown
        objectName: "textCountdown"

        color: "black"
        font.pixelSize: Math.ceil(btnPanel.width * 0.02)
        text: window.textDisplayed
    }


    Text {
        id: transportState
        objectName: "transportState"
        text: "State: " + qmlbridge.getTransportState()
        anchors.right: parent.right
        font.pixelSize: Math.ceil(parent.width * 0.02)
    }
}
