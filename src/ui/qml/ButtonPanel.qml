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
            interval: 1000
            running: false
            repeat: true
            onTriggered: {
                if(textCountdown.time <= 0){
                    countDownTimer.stop();
                    qmlbridge.record();
                    recordingTimer.start();
                }
                else{
                    textCountdown.text = textCountdown.time;
                    textCountdown.time--;
                }
            }
        }
        Timer {
            id: recordingTimer
            interval: 1000
            running: false
            repeat: true
            onTriggered: {
                if(textCountdown.time >= textCountdown.time2){
                    qmlbridge.stop();
                    recordingTimer.stop();
                }
                textCountdown.text = textCountdown.time;
                textCountdown.time++;
            }
        }
        Text {
            id: textCountdown
            text: "0"
            property int time: getTime()
            property int time2: getTime2()
            function getTime(){
                let d = delayInput.text;
                //console.log(d);
                let h = parseInt(d.substring(0,2));
                let m = parseInt(d.substring(3,5));
                let s = parseInt(d.substring(6,8));
                let timeRem = h * 60 * 60 + m * 60 + s;
                return timeRem;
            }
            function getTime2(){
                let d = recordTimeInput.text;
                //console.log(d);
                let h = parseInt(d.substring(0,2));
                let m = parseInt(d.substring(3,5));
                let s = parseInt(d.substring(6,8));
                let timeRem = h * 60 * 60 + m * 60 + s;
                return timeRem;
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
                        let idevices = qmlbridge.getInputDevices().split(',');
                        let i;
                        for(i = 0; i < idevices.length; i++){
                            append({
                                       text: idevices[i]
                                   })
                        }
                    }
                }
                onActivated: {
                    console.log("Audio device has been changed to: " + iDeviceInfoLabel.currentText);
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
                        let odevices = qmlbridge.getOutputDevices().split(',');
                        let i;
                        for(i = 0; i < odevices.length; i++){
                            append({
                                       text: odevices[i]
                                   })
                        }
                    }
                }
                onActivated: {
                    console.log("Audio device has been changed to: " + oDeviceInfoLabel.currentText);
                }
                currentIndex: 0
            }

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

                TextInput {
                    id: delayInput
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
                        timerPopup.close();
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
                        countDownTimer.start();
                        timerPopup.close();
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
