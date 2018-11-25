import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.0
import QtQuick.Controls 2.3
import QtQuick.Window 2.0
import Qt.labs.platform 1.0
import QtGraphicalEffects 1.0
import "../fonts/Icon.js" as MDFont

Rectangle {

    id: buttonPanel

    width: parent.width
    height: 115
    color: window.barColor

    Timer {
        id: countDownTimer
        objectName: "countDownTimer"

        interval: 1000
        running: false
        repeat: true
        onTriggered: {
            if (timeFuncs.time === 0) {
                window.textDisplayed = "Elapsed: 0"
                countDownTimer.stop()
                qmlbridge.record()
                recordingTimer.start()
            } else {
                window.textDisplayed = "Countdown: " + timeFuncs.time
                timeFuncs.time--
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
            if (timeFuncs.time >= timeFuncs.time2 - 1) {
                window.textDisplayed = "Elapsed: " + (timeFuncs.time + 1)
                qmlbridge.stop()
                recordingTimer.stop()
            } else {
                window.textDisplayed = "Elapsed: " + (++timeFuncs.time)
            }
        }
    }

    Item {
        id: timeFuncs
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

    RowLayout {
        id: transportLayout
        width: buttonPanel.width

        Item {
            Layout.preferredWidth: 10
        }

        RowLayout {
            id: rowLayout

            spacing: Math.round(buttonPanel.width * 0.0075)

            RoundButton {
                id: recordBtn
                objectName: "recordBtn"

                display: AbstractButton.TextOnly
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                background: Rectangle {
                    color: (recordBtn.pressed || !recordBtn.enabled) ?  "grey" : "darkgrey"
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

                    let success = qmlbridge.record()

                    if(success && (qmlbridge.getTransportState() === "Recording"))
                    {
                        // Update stop button
                        stopBtn.enabled = true;

                        // Update play/pause button
                        playpauseBtn.enabled = true;
                        playpauseBtn.contentItem.text = MDFont.Icon.pause;
                        playpauseBtn.contentItem.color = "white";

                        // Update self
                        enabled = false;
                    }
                }
            }

            RoundButton {
                id: stopBtn
                objectName: "stopBtn"
                enabled: false
                property bool isStopped: false

                display: AbstractButton.TextOnly
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                background: Rectangle {
                    color: (stopBtn.pressed || !stopBtn.enabled) ?  "grey" : "darkgrey"
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
                    let success = qmlbridge.stop()
                    console.log("Test: " + success)

                    if(success && (qmlbridge.getTransportState() === "Stopped"))
                    {
                        enabled = false;

                        recordBtn.enabled = false;
                        isStopped = true;

                        playpauseBtn.enabled = true;
                        playpauseBtn.contentItem.text = MDFont.Icon.play;
                        playpauseBtn.contentItem.color = "green";

                        exportBtn.enabled = true;
                    }
                }
            }

            RoundButton {
                id: playpauseBtn
                objectName: "playpauseBtn"
                enabled: false

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                display: AbstractButton.TextOnly

                contentItem: Text {
                    objectName: "play_icon"
                    font.family: "Material Design Icons"
                    font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                    text: MDFont.Icon.pause
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: (playpauseBtn.pressed || !playpauseBtn.enabled) ?  "grey" : "darkgrey"
                    radius: playpauseBtn.width / 2
                }

                onClicked: {

                    let success;

                    if(contentItem.text == MDFont.Icon.pause)
                    {
                        success = qmlbridge.pause();

                        if(success && (qmlbridge.getTransportState() === "Paused"))
                        {
                            contentItem.text = MDFont.Icon.play;
                            contentItem.color = "green";

                            if(!stopBtn.isStopped)
                            {
                                stopBtn.enabled = true;
                                recordBtn.enabled = true;
                            }
                        }
                    }
                    else
                    {
                        success = qmlbridge.play();

                        if(success && (qmlbridge.getTransportState() === "Playing"))
                        {
                            contentItem.text = MDFont.Icon.pause;
                            contentItem.color = "white";

                            stopBtn.enabled = false;
                            recordBtn.enabled = false;
                        }
                    }

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
                    color: (timerBtn.pressed || !timerBtn.enabled) ?  "grey" : "darkgrey"
                    radius: timerBtn.width / 2
                }

                onClicked: timerPopup.open()
            }

            RoundButton {
                id: exportBtn
                objectName: "exportBtn"
                enabled: false

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                display: AbstractButton.TextOnly

                contentItem: Text {
                    font.family: "Material Design Icons"
                    font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                    text: MDFont.Icon.export
                    color: "white"
                    //transform: Rotation {angle: 270}

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: (exportBtn.pressed || !exportBtn.enabled) ?  "grey" : "darkgrey"
                    radius: exportBtn.width / 2
                }

                onClicked:saveDialog.open()
            }

            RoundButton {
                id: checkUpdateBtn
                objectName: "checkUpdateBtn"

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                display: AbstractButton.TextOnly

                contentItem: Text {
                    font.family: "Material Design Icons"
                    font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                    text: MDFont.Icon.download
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    opacity: enabled ? 1 : 0.15
                    color: timerBtn.pressed ? "grey" : "darkgrey"
                    radius: timerBtn.width / 2
                }

                onClicked: qmlbridge.launchUpdateProcess()
            }

            DropShadow {
                visible: (recordBtn.enabled && !recordBtn.pressed) ? true : false
                color: "#606060"
                anchors.fill: recordBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: recordBtn
            }

            InnerShadow {
                visible: recordBtn.pressed ? true : false
                color: "#606060"
                anchors.fill: recordBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: recordBtn
            }

            DropShadow {
                visible: (stopBtn.enabled && !stopBtn.pressed) ? true : false
                color: "#606060"
                anchors.fill: stopBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: stopBtn
            }

            InnerShadow {
                visible: stopBtn.pressed ? true : false
                color: "#606060"
                anchors.fill: stopBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: stopBtn
            }

            DropShadow {
                visible: (playpauseBtn.enabled && !playpauseBtn.pressed) ? true : false
                color: "#606060"
                anchors.fill: playpauseBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: playpauseBtn
            }

            InnerShadow {
                visible: playpauseBtn.pressed ? true : false
                color: "#606060"
                anchors.fill: playpauseBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: playpauseBtn
            }

            DropShadow {
                visible: (timerBtn.enabled && !timerBtn.pressed) ? true : false
                color: "#606060"
                anchors.fill: timerBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: timerBtn
            }

            InnerShadow {
                visible: timerBtn.pressed ? true : false
                color: "#606060"
                anchors.fill: timerBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: timerBtn
            }

            DropShadow {
                visible: (exportBtn.enabled && !exportBtn.pressed) ? true : false
                color: "#606060"
                anchors.fill: exportBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: exportBtn
            }

            InnerShadow {
                visible: exportBtn.pressed ? true : false
                color: "#606060"
                anchors.fill: exportBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: exportBtn
            }
        }
        FileDialog {
            id: saveDialog
            objectName: "saveDialog"
            fileMode: FileDialog.SaveFile
            nameFilters: saveDialog.nameFilters
            folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
            onAccepted: {
                qmlbridge.saveFile(saveDialog.currentFile);
//                console.log(saveDialog.fileUrl.toString());
            }
        }

        GridLayout {
            Layout.alignment: Qt.AlignRight
            rows: 2
            columns: 2
            Label {
                id: inputDeviceLabel

                color: "black"
                text: "Input Device:"
            }
            ComboBox {
                id: iDeviceInfoLabel
                Layout.preferredWidth: Math.max(Math.round(window.width * 0.2),
                                                320)
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
                onActivated: {
                    console.log("Audio device has been changed to: " + iDeviceInfoLabel.currentText);
                    qmlbridge.setActiveInputDevice(iDeviceInfoLabel.currentText);
                }

                onPressedChanged: {

                    let selectedInd = 0;
                    if(currentIndex != -1)
                        selectedInd = currentIndex;

                    model.clear();
                    var idevices = qmlbridge.getInputDevices().split(',')
                        var i
                        for (i = 0; i < idevices.length; i++) {
                            model.append({
                                       "text": idevices[i]
                                   })
                        }

                    // Keep the previously selected device active
                    currentIndex = selectedInd
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
                Layout.preferredWidth: Math.max(Math.round(window.width * 0.2),
                                                320)
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
                onActivated: {
                    console.log("Audio device has been changed to: " + oDeviceInfoLabel.currentText);
                    qmlbridge.setActiveOutputDevice(oDeviceInfoLabel.currentText);
                }

                onPressedChanged: {

                    let selectedInd = 0;
                    if(currentIndex != -1)
                        selectedInd = currentIndex;

                    model.clear();
                    var odevices = qmlbridge.getOutputDevices().split(',')
                        var i
                        for (i = 0; i < odevices.length; i++) {
                            model.append({
                                       "text": odevices[i]
                                   })
                        }

                    // Keep the previously selected device active
                    currentIndex = selectedInd
                }
                currentIndex: 0
            }
        }

        Item {
            Layout.preferredWidth: 10
        }
    }

    Popup {
        id: timerPopup
        objectName: "timerPopup"

        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)

        width: 350
        height: 170
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        ColumnLayout {
            spacing: 10

            GridLayout {
                id: gridLayout
                Layout.alignment: Qt.AlignTop
                rows: 3
                columns: 2

                Label {
                    font.family: "Roboto"
//                    font.pixelSize: 20
                    text: "Delay Recording (hh:mm:ss)"
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                TextField {
                    id: delayInput
                    width: 100
                    objectName: "delayInput"
                    text: "00:00:00"
                    inputMask: "00:00:00"
                    color: "white"
                }

                Label {
                    font.family: "Roboto"
//                    font.pixelSize: 20
                    text: "Recording Time (hh:mm:ss)"
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                TextField {
                    id: recordTimeInput
                    width: 100
                    objectName: "recordTimeInput"
                    text: "00:00:00"
                    inputMask: "00:00:00"
                    color: "white"
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignCenter
                spacing: 60
                width: gridLayout.width
                anchors.leftMargin: 300
                Button {
                    Layout.alignment: Qt.AlignLeft
                    id: cancelBtn
                    onClicked: {
                        timerPopup.close()
                    }
                    Layout.preferredWidth: 75
                    contentItem: Text {
                        font.family: "Roboto"
//                        font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
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

                    Layout.preferredWidth: 75
                    contentItem: Text {
                        font.family: "Roboto"
//                        font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                        text: "OK"
                        color: "white"

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        onClosed: {
            console.log("popup clsoed");
        }
    }
}
