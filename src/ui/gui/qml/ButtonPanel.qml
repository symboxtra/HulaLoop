import QtQuick 2.10

import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.3

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
                window.textDisplayed = qsTr("Elapsed: %1").arg(0)
                countDownTimer.stop()

                recordBtn.onClicked()

                if(timeFuncs.time2 === 0) {
                    recordingTimer.inf = true
                }
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
        property bool inf: false
        onTriggered: {
            // Since the timer starts at 0, go to endTime - 1
            if (!recordingTimer.inf && timeFuncs.time >= timeFuncs.time2 - 1) {
                window.textDisplayed = qsTr("Elapsed: %1").arg(++timeFuncs.time)
                stopBtn.onClicked();
            } else {
                window.textDisplayed = qsTr("Elapsed: %1").arg(++timeFuncs.time)
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
                property string tttext:"Record Audio"

                hoverEnabled: true
                ToolTip.delay: 500
                ToolTip.timeout: 5000
                ToolTip.visible: hovered
                ToolTip.text: qsTr(recordBtn.tttext)

                background: Rectangle {
                    color: (recordBtn.pressed || !recordBtn.enabled) ?  "grey" : "darkgrey"
                    radius: recordBtn.width / 2
                }

                contentItem: Text {
                    font.family: "Material Design Icons"
                    font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                    text:  MDFont.Icon.record
                    color: "red"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {

                    if(stopBtn.isStopped)
                    {
                        discardPopup.open()
                        return
                    }

                    if(timeFuncs.time < 0)
                    {
                        countDownTimer.start()
                        return
                    }

                    if(timeFuncs.time2 == 0)
                    {
                        recordingTimer.inf = true
                    }

                    let success = qmlbridge.record()

                    recordingTimer.start()

                    if(success && (qmlbridge.getTransportState() === qsTr("Recording", "state")))
                    {
                        // Update stop button
                        stopBtn.enabled = true

                        // Update play/pause button
                        playpauseBtn.enabled = true;
                        playpauseBtn.contentItem.text = MDFont.Icon.pause;
                        playpauseBtn.contentItem.color = "white";
                        playpauseBtn.tttext = "Pause audio"

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

                hoverEnabled: true
                ToolTip.delay: 500
                ToolTip.timeout: 5000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Stop recording")

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
                    recordBtn.tttext = "Discard recording"
                    if(success && (qmlbridge.getTransportState() === qsTr("Stopped", "state")))
                    {
                        enabled = false;

                        recordBtn.enabled = true;
                        recordBtn.contentItem.text = MDFont.Icon.delete;
                        isStopped = true;

                        playpauseBtn.enabled = true;
                        playpauseBtn.contentItem.text = MDFont.Icon.play;
                        playpauseBtn.contentItem.color = "green";

                        exportBtn.enabled = true;

                        timeFuncs.time = 0
                        recordingTimer.stop()
                    }
                }

            }

            RoundButton {
                id: playpauseBtn
                objectName: "playpauseBtn"
                enabled: false
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                display: AbstractButton.TextOnly
                property bool isRecording: false // Remove if we do not need for checking when not recording
                property string tttext : ""

                hoverEnabled: true
                ToolTip.delay: 500
                ToolTip.timeout: 5000
                ToolTip.visible: hovered
                ToolTip.text: qsTr(playpauseBtn.tttext)

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

                    if(contentItem.text === MDFont.Icon.pause)
                    {
                        success = qmlbridge.pause();

                        if(success && (qmlbridge.getTransportState() === qsTr("Paused", "state")))
                        {
                            contentItem.text = MDFont.Icon.play;
                            contentItem.color = "green";
                            playpauseBtn.tttext = "Playback audio"

                            if(!stopBtn.isStopped)
                            {
                                stopBtn.enabled = true;
                                recordBtn.enabled = true;
                            }
                            recordingTimer.stop()
                        }
                    }
                    else
                    {
                        success = qmlbridge.play();

                        if(success && (qmlbridge.getTransportState() === qsTr("Playing", "state")))
                        {
                            contentItem.text = MDFont.Icon.pause;
                            contentItem.color = "white";

                            if(!stopBtn.isStopped)
                            {
                                stopBtn.enabled = false;
                                recordBtn.enabled = false;
                            }
                        }
                    }

                }
            }

            RoundButton {
                id: timerBtn
                objectName: "timerBtn"

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                display: AbstractButton.TextOnly
                enabled: updateAndTimerBtnEnabled

                hoverEnabled: true
                ToolTip.delay: 500
                ToolTip.timeout: 5000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Set recording delay and duration")

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

                hoverEnabled: true
                ToolTip.delay: 500
                ToolTip.timeout: 5000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Export audio")

                contentItem: Text {
                    font.family: "Material Design Icons"
                    font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                    text: MDFont.Icon.export
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: (exportBtn.pressed || !exportBtn.enabled) ?  "grey" : "darkgrey"
                    radius: exportBtn.width / 2
                }

                onClicked: saveDialog.open()
            }

            RoundButton {
                id: checkUpdateBtn
                objectName: "checkUpdateBtn"

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                display: AbstractButton.TextOnly
                enabled: updateAndTimerBtnEnabled

                hoverEnabled: true
                ToolTip.delay: 500
                ToolTip.timeout: 5000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Check for updates")

                contentItem: Text {
                    font.family: "Material Design Icons"
                    font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                    text: MDFont.Icon.download
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: (checkUpdateBtn.pressed || !checkUpdateBtn.enabled) ?  "grey" : "darkgrey"
                    radius: timerBtn.width / 2
                }

                onClicked: qmlbridge.launchUpdateProcess()
            }
            RoundButton {
                id: settingsBtn
                objectName: "settingsBtn"

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                display: AbstractButton.TextOnly

                contentItem: Text {
                    font.family: "Material Design Icons"
                    font.pixelSize: Math.ceil(buttonPanel.width * 0.02)
                    text: MDFont.Icon.settings
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    opacity: enabled ? 1 : 0.15
                    color: timerBtn.pressed ? "grey" : "darkgrey"
                    radius: timerBtn.width / 2
                }

                onClicked: settingsPopup.open()
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

            DropShadow {
                visible: (checkUpdateBtn.enabled && !checkUpdateBtn.pressed) ? true : false
                color: "#606060"
                anchors.fill: checkUpdateBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: checkUpdateBtn
            }

            InnerShadow {
                visible: checkUpdateBtn.pressed ? true : false
                color: "#606060"
                anchors.fill: checkUpdateBtn
                horizontalOffset: 2
                verticalOffset: 2
                samples: 3
                source: checkUpdateBtn
            }
        }

        FileDialog {
            id: saveDialog
            objectName: "saveDialog"
            fileMode: FileDialog.SaveFile
            nameFilters: ["WAVE Sound (*.wav)", "FLAC (*.flac)", "Core Audio Format (*.caf)", "Audio Interchange File Format (*.aiff)", "RAW Format (*.raw)", "All files (*)"]
            folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
            onAccepted: {
                qmlbridge.saveFile(saveDialog.currentFile);
            }
        }

        GridLayout {
            Layout.alignment: Qt.AlignRight
            rows: 2
            columns: 2
            Label {
                id: inputDeviceLabel

                color: "black"
                text: qsTr("Input Device:") + qmlbridge.emptyStr
            }
            ComboBox {
                id: iDeviceInfoLabel
                Layout.preferredWidth: Math.max(Math.round(window.width * 0.2), 320)

                hoverEnabled: true
                ToolTip.delay: 500
                ToolTip.timeout: 5000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Change input device")

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
                    if(currentIndex < 0)
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
                text: qsTr("Output Device:") + qmlbridge.emptyStr
            }
            ComboBox {
                id: oDeviceInfoLabel
                Layout.preferredWidth: Math.max(Math.round(window.width * 0.2), 320)

                hoverEnabled: true
                ToolTip.delay: 500
                ToolTip.timeout: 5000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Change output device")

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
        id: settingsPopup
        objectName: "settingsPopup"

        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)

        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        ColumnLayout {
            id: colLayout
            spacing: Math.round(buttonPanel.height * 0.15)

            GridLayout {
                id: gridLayout2
                Layout.alignment: Qt.AlignTop
                rows: 3
                columns: 2

                Label {
                    font.family: "Roboto"
                    text: qsTr("Display record devices") + qmlbridge.emptyStr
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Switch {
                    id: displayRecordDev
                    checked: qmlbridge.getShowRecordDevices()

                    onToggled: {
                        let val = true;
                        (position === 1.0) ? val = true : val = false;
                        qmlbridge.setShowRecordDevices(val);
                    }
                }

                Label {
                    font.family: "Roboto"
                    text: qsTr("Visualizer type") + qmlbridge.emptyStr
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: visSetting

                    model: [qsTr("Bar") + qmlbridge.emptyStr, qsTr("Circle") + qmlbridge.emptyStr, qsTr("Line") + qmlbridge.emptyStr]

                    onActivated: {
                        //add behavior for which setting it was just changed to
                        qmlbridge.visType = visSetting.currentText
                        qmlbridge.saveSettings()
                        qmlbridge.onVisData([], [0])
                    }

                    Component.onCompleted: {
                        for (var i = 0; i < visSetting.count; ++i)
                        {
                            if (visSetting.textAt(i) === qmlbridge.visType)
                            {
                                currentIndex = i;
                                break;
                            }
                            else
                                currentIndex = 0;
                        }
                    }
                }

                Label {
                    font.family: "Roboto"
                    text: qsTr("Language") + qmlbridge.emptyStr
                    color: "white"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: langSetting

                    // Layout.preferredWidth: Math.min(Math.round(window.width * 0.2), 100)
                    Layout.preferredWidth: visSetting.width + 20

                    textRole: "key"
                    model: ListModel {
                        id: langOpt

                        ListElement { key: "English - en"; value: "en" }
                        ListElement { key: "Español - es"; value: "es" }
                        ListElement { key: "Français - fr"; value: "fr" }
                        ListElement { key: "हिंदी - hi"; value: "hi" }
                        ListElement { key: "Polskie - pl"; value: "pl" }
                    }

                    onActivated: {
                        qmlbridge.loadLanguage(langOpt.get(langSetting.currentIndex).value)
                    }

                    Component.onCompleted: {
                        for (var i = 0; i < langOpt.count; ++i)
                        {
                            if (langOpt.get(i).value === qmlbridge.getSelectedLanguage())
                            {
                                currentIndex = i;
                                break;
                            }
                            else
                                currentIndex = 0;
                        }
                    }

                }

            }

        }

    }

    Popup {
        id: discardPopup
        objectName: "discardPopup"

        x: Math.round((window.width - width) / 2)
        y: Math.round((window.height - height) / 2)
        width: 255
        height: 100

        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        ColumnLayout {
            spacing: Math.round(window.height * 0.15)
            ColumnLayout {
                spacing: Math.round(discardPopup.height * 0.15)
                RowLayout{
                    Text {
                        id: textbot
                        color: "white"
                        text: qsTr("Are you sure you want to discard?") + qmlbridge.emptyStr
                    }
                }
                RowLayout {
                    Layout.alignment: Qt.AlignCenter
                    spacing: Math.round(buttonPanel.width * 0.05)
                    width: gridLayout.width / 2
                    Button {
                        text: "No"
                        onClicked: {
                            discardPopup.close()
                        }
                    }

                    Button {
                        text: "Yes"
                        onClicked: {
                            // Discard files
                            qmlbridge.discard()
                            recordBtn.tttext = "Record Audio"

                            // Start recording again
                            stopBtn.isStopped = false
                            recordBtn.contentItem.text = MDFont.Icon.record
                            discardPopup.close()

                            exportBtn.enabled = false;
                            timeFuncs.time = 0;
                            timeFuncs.time = 0;
                            delayInput.text = "00:00:00";
                            recordTimeInput.text = "00:00:00";
                            window.textDisplayed = qsTr("Elapsed: %1").arg(0);
                        }
                    }
                }
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
            spacing: 10

            GridLayout {
                id: gridLayout
                Layout.alignment: Qt.AlignTop
                rows: 3
                columns: 2

                Label {
                    font.family: "Roboto"
                    text: qsTr("Delay Recording (hh:mm:ss)") + qmlbridge.emptyStr

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
                    text: qsTr("Record Duration (hh:mm:ss)") + qmlbridge.emptyStr

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
                Button {
                    Layout.alignment: Qt.AlignLeft
                    id: cancelBtn
                    onClicked: {
                        timerPopup.close()
                    }
                    Layout.preferredWidth: 75
                    contentItem: Text {
                        font.family: "Roboto"

                        text: qsTr("CANCEL") + qmlbridge.emptyStr
                        color: "white"

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button {
                    Layout.alignment: Qt.AlignRight
                    id: okBtn
                    onClicked: {
                        timerPopup.close()
                    }

                    Layout.preferredWidth: 75
                    contentItem: Text {
                        font.family: "Roboto"
                        text: qsTr("OK") + qmlbridge.emptyStr

                        color: "white"

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}
