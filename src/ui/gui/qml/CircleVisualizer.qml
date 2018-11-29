import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1

Rectangle {
    id: circleRect

    visible: false
    color: "transparent"

    // Track the number of bins we have
    property int binCount: 0

    // Determine the spacing between each point and the interval on which to select bins
    property int numPoints: 16
    property int intv: Math.floor(binCount / numPoints)
    property variant items: [0]

    function clear() {
        let ctx = canvasCircle.getContext('2d');
        ctx.reset();
        canvasCircle.requestPaint();
    }

    function readValues(array) {
        items = array
    }

    Canvas {
        id: canvasCircle

        width: parent.width
        height: parent.height

        contextType: "2d"
        renderStrategy: Canvas.Threaded

        Path {
            id: circle

            startY: (visualize.height / 2) + (100 * Math.sin(Math.PI / 2))
            startX: (visualize.width / 2) + (100 * Math.cos(Math.PI / 2))
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 1] * 20) * Math.cos(5 * Math.PI / 8); y: (visualize.height / 2) + (100 + items[intv * 1] * 20) * Math.sin(5 * Math.PI / 8) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 2] * 20) * Math.cos(3 * Math.PI / 4); y: (visualize.height / 2) + (100 + items[intv * 2] * 20) * Math.sin(3 * Math.PI / 4) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 3] * 20) * Math.cos(7 * Math.PI / 8); y: (visualize.height / 2) + (100 + items[intv * 3] * 20) * Math.sin(7 * Math.PI / 8) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 4] * 20) * Math.cos(Math.PI); y: (visualize.height / 2) + (100 + items[intv * 4] * 20) * Math.sin(Math.PI) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 5] * 20) * Math.cos(9 * Math.PI / 8); y: (visualize.height / 2) + (100 + items[intv * 5] * 20) * Math.sin(9 * Math.PI / 8) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 6] * 20) * Math.cos(5 * Math.PI / 4); y: (visualize.height / 2) + (100 + items[intv * 6] * 20) * Math.sin(5 * Math.PI / 4) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 7] * 20) * Math.cos(11 * Math.PI / 8); y: (visualize.height / 2) + (100 + items[intv * 7] * 20) * Math.sin(11 * Math.PI / 8) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 8] * 20) * Math.cos(3 * Math.PI / 2); y: (visualize.height / 2) + (100 + items[intv * 8] * 20) * Math.sin(3 * Math.PI / 2) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 9] * 20) * Math.cos(13 * Math.PI / 8); y: (visualize.height / 2) + (100 + items[intv * 9] * 20) * Math.sin(13 * Math.PI / 8) }

            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 10] * 20) * Math.cos(7 * Math.PI / 4); y: (visualize.height / 2) + (100 + items[intv * 10] * 20) * Math.sin(7 * Math.PI / 4) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 11] * 20) * Math.cos(15 * Math.PI / 8); y: (visualize.height / 2) + (100 + items[intv * 11] * 20) * Math.sin(15 * Math.PI / 8) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 12] * 20) * Math.cos(2 * Math.PI); y: (visualize.height / 2) + (100 + items[intv * 12] * 20) * Math.sin(2 * Math.PI) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 13] * 20) * Math.cos(Math.PI / 8); y: (visualize.height / 2) + (100 + items[intv * 13] * 20) * Math.sin(Math.PI / 8) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 14] * 20) * Math.cos(Math.PI / 4); y: (visualize.height / 2) + (100 + items[intv * 14] * 20) * Math.sin(Math.PI / 4) }
            PathCurve { x: (visualize.width / 2) + (100 + items[intv * 15] * 20) * Math.cos(3 * Math.PI / 8); y: (visualize.height / 2) + (100 + items[intv * 15] * 20) * Math.sin(3 * Math.PI / 8) }
            PathCurve { x: (visualize.width / 2) + (100 * Math.cos(Math.PI / 2)); y: (visualize.height / 2) + (100 * Math.sin(Math.PI / 2)) }
        }

        onPaint: {
            context.strokeStyle = Qt.rgba(255, 255, 255);
            context.path = circle;
            context.stroke();
        }
    }
}