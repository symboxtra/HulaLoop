import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1

Canvas {
    id: canvas

    contextType: "2d"
    renderStrategy: Canvas.Threaded

    // Track the number of bins we have
    property int binCount: 0

    // Determine the spacing between each point and the interval on which to select bins
    property int numPoints: 16
    property int startPos: Math.round(parent.height / 1.17)
    property int sp: Math.round(parent.width / numPoints)
    property int intv: Math.floor(binCount / numPoints)

    function clear() {
        let ctx = getContext('2d');
        ctx.reset();
        canvas.requestPaint();
    }

    function readValues(anArray) {
        itemarr.items = anArray
    }

    Item{
        id: itemarr
        property variant items: [0]
    }

    Path {
        id: dline
        startY: startPos
        startX: 0
        PathCurve { x: sp * 1; y: startPos - (itemarr.items[intv * 1]*100) }
        PathCurve { x: sp * 2; y: startPos - (itemarr.items[intv * 2]*100) }
        PathCurve { x: sp * 3; y: startPos - (itemarr.items[intv * 3]*100) }
        PathCurve { x: sp * 4; y: startPos - (itemarr.items[intv * 4]*100) }
        PathCurve { x: sp * 5; y: startPos - (itemarr.items[intv * 5]*100) }
        PathCurve { x: sp * 6; y: startPos - (itemarr.items[intv * 6]*100) }
        PathCurve { x: sp * 7; y: startPos - (itemarr.items[intv * 7]*100) }
        PathCurve { x: sp * 8; y: startPos - (itemarr.items[intv * 8]*100) }
        PathCurve { x: sp * 9; y: startPos - (itemarr.items[intv * 9]*100) }

        PathCurve { x: sp * 10; y: startPos - (itemarr.items[intv * 10]*100) }
        PathCurve { x: sp * 11; y: startPos - (itemarr.items[intv * 11]*100) }
        PathCurve { x: sp * 12; y: startPos - (itemarr.items[intv * 12]*100) }
        PathCurve { x: sp * 13; y: startPos - (itemarr.items[intv * 13]*100) }
        PathCurve { x: sp * 14; y: startPos - (itemarr.items[intv * 14]*100) }
        PathCurve { x: sp * 15; y: startPos - (itemarr.items[intv * 15]*100) }
        PathCurve { x: sp * 16; y: startPos - (itemarr.items[intv * 16]*100) }
    }

    onPaint: {
        context.strokeStyle = Qt.rgba(255, 255, 255);
        context.path = dline;
        context.stroke();
    }
}
