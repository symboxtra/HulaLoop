import QtQuick 2.0
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.2

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
		}

		RoundButton {
			id: stopBtn
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
		}

		RoundButton {
			id: playBtn
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
		}

		RoundButton {
			id: pauseBtn
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
		}
	}
}
