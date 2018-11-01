import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

import hulaloop.updater 1.0

ApplicationWindow {

    id: window

    flags: Qt.FramelessWindowHint
    width: 480
    height: 180
    visible: true

    Material.theme: Material.Dark
    Material.accent: Material.Orange

    Updater {
        id: updater
        updateHostUrl: "https://api.github.com/repos/bmatzelle/gow/releases/latest"

        onBytesDownloaded: progressBar.value += (updater.getNumBytesDownloaded() / updater.getDownloadSize())
    }

    Item {
        id: root
        width: parent.width
        anchors.centerIn: parent

        Label {
            id: title

            anchors.bottom: progressBar.top
            anchors.horizontalCenter: parent.horizontalCenter

            text: qsTr("Updating HulaLoop")
            font.pointSize: Math.round(0.025 * window.width)
        }

        ProgressBar {
            id: progressBar
            objectName: "progressBar"

            anchors.centerIn: root
            width: 0.8 * window.width
            height: 0.3 * window.height

            indeterminate: true
            antialiasing: true

            from: 0
            to: 1
            value: 0
            visible: true
	    }

        Label {
            id: status

            anchors.top: progressBar.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            text: qsTr("Checking for updates...")
            font.pointSize: Math.round(0.025 * window.width)

            Component.onCompleted: {
                updater.checkForUpdate()

                if(updater.foundUpdate())
                {
                    progressBar.indeterminate = false
                    status.text = qsTr("Downloading updates...")

                    updater.downloadUpdate()
                    if(updater.finishedDownload())
                        updater.startHulaLoopInstaller()
                }
                else
                {
                    status.text = qsTr("No updates found.")
                    updater.startHulaLoopApp()
                }

            }

        }

    }

}