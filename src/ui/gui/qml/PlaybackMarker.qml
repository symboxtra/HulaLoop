import QtQuick 2.0

Rectangle {
    id: marker
    color: "red"

    property double position: 0
    property int maxTime: 1
    property int leftMargin: 70
    property int rightMargin: 70
    property int span: (parent.width - leftMargin - rightMargin)

    function reset() {
        position = 0
    }

    x: leftMargin + span * (position / maxTime)

    anchors.bottomMargin: 60
    anchors.bottom: parent.bottom

    width: 2
    height: parent.height - 80

    z: 1000
}
