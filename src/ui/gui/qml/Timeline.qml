import QtQuick 2.0
import QtCharts 2.0

ChartView {

    property int maxTime: 20
    property double nextSample: 0

    function reset() {
        samples.clear()
        maxTime = 20
        nextSample = 0
    }

    anchors.margins: 0

    // Strip annoying margins
    x: -50
    y: -10
    width: parent.width + 60
    height: parent.height + 20

    animationOptions: ChartView.NoAnimation
    backgroundColor: "#080808"
    backgroundRoundness: 0
    dropShadowEnabled: false

    legend.visible: false

    ValueAxis  {
        id: time
        min: 0
        max: maxTime
        gridVisible: false
        lineVisible: false
    }

    ValueAxis  {
        id: amplitude
        min: -1
        max: 1
        gridVisible: false
        lineVisible: false
        labelsVisible: false
    }

    property var samples: LineSeries {
        name: "samples"
        axisX: time
        axisY: amplitude
    }
}