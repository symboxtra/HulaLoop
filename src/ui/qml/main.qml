import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

import hulaloop.qmlbridge 1.0

ApplicationWindow {

	id: window
	title: qsTr("HulaLoop")

	visible: true
	width: 640
	height: 360

    Material.theme: Material.Grey
	Material.accent: Material.Orange

	QMLBridge {
		id: qmlbridge
		onStateChanged: {
			transportState.text = qmlbridge.getTransportState()
		}
	}

	ButtonPanel {
		id: btnPanel
	}

    Visualizer {
        id: visualize
    }

	Label {
		id: transportState
		objectName: "transportState"

		anchors.top: btnPanel.bottom
		text: "Initial State"
	}
}