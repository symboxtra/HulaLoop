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
            text: "Timer:"
        }
        Timer {
            id: countDownTimer
            objectName: "countDownTimer"

            interval: 1000
            running: false
            repeat: true
            onTriggered: {
                if (textCountdown.time == 0) {
                    textCountdown.text = 0
                    countDownTimer.stop()
                    qmlbridge.record()
                    recordingTimer.start()
                } else {
                    textCountdown.text = textCountdown.time
                    textCountdown.time--
                }
            }
        }
        Timer {
            id: recordingTimer
            objectName: "recordingTimer"

            interval: 1000
            running: false
            repeat: true
            onTriggered: {
                // Since the timer starts at 0, go to endTime - 1
                if (textCountdown.time >= textCountdown.time2 - 1) {
                    textCountdown.text = textCountdown.time + 1
                    qmlbridge.stop()
                    recordingTimer.stop()
                } else {
                    textCountdown.text = textCountdown.time + 1
                    textCountdown.time++
                }
            }
        }
        Text {
            id: textCountdown
            objectName: "textCountdown"

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

        GridLayout {
            anchors.leftMargin: buttonPanel.width * 0.9
            rows: 2
            columns: 2
            Label {
                id: inputDeviceLabel

                color: "black"
                text: "Input Device:"
            }
            ComboBox {
                id: iDeviceInfoLabel
                Layout.preferredWidth: Math.round(window.width * 0.2)
                model: ListModel {
                    id: iDeviceItems
                    Component.onCompleted: {
                        var idevices = qmlbridge.getInputDevices().split(',')
                        var i
                        for (i = 0; i < idevices.length; i++) {
                            append({
                                       "text": idevices[i]
                                   })
                        }
                    }
                }
                currentIndex: 0
            }
            Label {
                id: outputDeviceLabel

                color: "black"
                text: "Output Device:"
            }
            ComboBox {
                id: oDeviceInfoLabel
                Layout.preferredWidth: Math.round(window.width * 0.2)
                model: ListModel {
                    id: oDeviceItems
                    Component.onCompleted: {
                        var odevices = qmlbridge.getOutputDevices().split(',')
                        var i
                        for (i = 0; i < odevices.length; i++) {
                            append({
                                       "text": odevices[i]
                                   })
                        }
                    }
                }
                currentIndex: 0
            }
        }
    }

    Popup {
        id: timerPopup
        objectName: "timerPopup"

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

                TextInput {
                    id: delayInput
                    objectName: "delayInput"

                    text: "00:00:00"
                    inputMask: "00:00:00"
                    color: "white"
                }

                /*ComboBox {
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
                }*/
                Label {
                    font.family: "Roboto"
                    font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                    text: "Recording Time (hh:mm:ss)"
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                TextInput {
                    id: recordTimeInput
                    objectName: "recordTimeInput"

                    text: "00:00:00"
                    inputMask: "00:00:00"
                    color: "white"
                }
                /*ComboBox {
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
                }*/
            }

            RowLayout {
                Layout.alignment: Qt.AlignCenter
                spacing: Math.round(buttonPanel.width * 0.05)
                width: gridLayout.width / 2

                Button {
                    Layout.alignment: Qt.AlignLeft
                    id: cancelBtn
                    onClicked: {
                        timerPopup.close()
                    }
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
                    onClicked: {
                        countDownTimer.start()
                        timerPopup.close()
                    }

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
