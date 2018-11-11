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
        updateHostUrl: "https://api.github.com/repos/jmcker/HulaLoop/releases/latest"

        onBytesDownloaded: progressBar.value += (numBytesDownloaded / downloadSize)
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

                if(updater.checkForUpdate())
                {
                    progressBar.indeterminate = false
                    status.text = qsTr("Downloading updates...")

                    if(updater.downloadUpdate())
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