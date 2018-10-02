import QtQuick 2.9
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

import "../fonts/Icon.js" as MDFont

Rectangle {

    id: buttonPanel

    width: parent.width
    height: parent.height * 0.15
    color: "lightgrey"

    RowLayout {

        id: rowLayout
        anchors.rightMargin: buttonPanel.width * 0.7
        anchors.fill: parent

        RoundButton {
            id: recordBtn
            objectName: "recordBtn"

            display: AbstractButton.TextOnly
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            background: Rectangle {
                opacity: enabled ? 1 : 0.15
                color: recordBtn.pressed ? "grey" : "darkgrey"
                radius: recordBtn.width / 2
            }

            contentItem: Text {
                font.family: "Material Design Icons"
                font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                text: MDFont.Icon.record
                color: "red"

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                // Add JavaScript for delay timer
                qmlbridge.record()
            }
        }

        RoundButton {
            id: stopBtn
            objectName: "stopBtn"

            display: AbstractButton.TextOnly
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            background: Rectangle {
                opacity: enabled ? 1 : 0.15
                color: stopBtn.pressed ? "grey" : "darkgrey"
                radius: stopBtn.width / 2
            }

            contentItem: Text {
                font.family: "Material Design Icons"
                font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                text: MDFont.Icon.stop
                color: "black"

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                // Add JavaScript for recording time
                qmlbridge.stop()
            }
        }

        RoundButton {
            id: playBtn
            objectName: "playBtn"

            display: AbstractButton.TextOnly
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            contentItem: Text {
                font.family: "Material Design Icons"
                font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                text: MDFont.Icon.play
                color: "green"

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                opacity: enabled ? 1 : 0.15
                color: playBtn.pressed ? "grey" : "darkgrey"
                radius: playBtn.width / 2
            }

            onClicked: {
                qmlbridge.play()
            }
        }

        RoundButton {
            id: pauseBtn
            objectName: "pauseBtn"

            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            display: AbstractButton.TextOnly

            contentItem: Text {
                font.family: "Material Design Icons"
                font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                text: MDFont.Icon.pause
                color: "white"

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                opacity: enabled ? 1 : 0.15
                color: pauseBtn.pressed ? "grey" : "darkgrey"
                radius: pauseBtn.width / 2
            }

            onClicked: {
                qmlbridge.pause()
            }
        }

        RoundButton {
            id: timerBtn
            objectName: "timerBtn"

            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            display: AbstractButton.TextOnly

            contentItem: Text {
                font.family: "Material Design Icons"
                font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                text: MDFont.Icon.timer
                color: "white"

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                opacity: enabled ? 1 : 0.15
                color: timerBtn.pressed ? "grey" : "darkgrey"
                radius: timerBtn.width / 2
            }

            onClicked: timerPopup.open()
        }

        Label {
            id: timerLabel

            color: "black"
            text: "HH:MM:SS"
        }
    }

    Popup {
        id: timerPopup

        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)

        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        ColumnLayout {
            spacing: Math.round(buttonPanel.height * 0.15)

            GridLayout {
                id: gridLayout
                Layout.alignment: Qt.AlignTop
                rows: 3
                columns: 2

                Label {
                    font.family: "Roboto"
                    font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                    text: "Delay Recording (hh:mm:ss)"
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: delayDropdown

                    editable: true
                    model: ListModel {
                        id: model
                        ListElement {
                            text: "1"
                        }
                        ListElement {
                            text: "5"
                        }
                        ListElement {
                            text: "10"
                        }
                    }
                }

                Label {
                    font.family: "Roboto"
                    font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                    text: "Recording Time (hh:mm:ss)"
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    editable: true
                    model: ListModel {
                        id: model1
                        ListElement {
                            text: "1"
                        }
                        ListElement {
                            text: "5"
                        }
                        ListElement {
                            text: "10"
                        }
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignCenter
                spacing: Math.round(buttonPanel.width * 0.05)
                width: gridLayout.width / 2

                Button {
                    Layout.alignment: Qt.AlignLeft
                    id: cancelBtn
                    Layout.preferredWidth: Math.round(buttonPanel.width * 0.15)
                    contentItem: Text {
                        font.family: "Roboto"
                        font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                        text: "CANCEL"
                        color: "white"

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button {
                    Layout.alignment: Qt.AlignRight
                    id: okBtn
                    Layout.preferredWidth: Math.round(buttonPanel.width * 0.15)
                    contentItem: Text {
                        font.family: "Roboto"
                        font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                        text: "OK"
                        color: "white"

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}
