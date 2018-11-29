import QtQuick 2.0
import QtQuick 2.0
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.2

Rectangle {

    id: bottomRect
    width: parent.width
    height: 20
    color: window.barColor
    anchors.bottom: parent.bottom

    Button {
        height: parent.height * 1.3
        anchors.left: parent.left
        anchors.leftMargin: 15
        y: -3
        width: switchText.width
        Text {
            id: switchText
            y: 5
            text: window.anim ? "  Visualizer  " : "  Editor  "
        }
        onClicked: {
            window.anim = !window.anim;
        }
    }

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
