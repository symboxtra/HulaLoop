import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1

Canvas {
    id: canvas

    contextType: "2d"

    // Determine the spacing between each point
    property int numPoints: 16
    property int sp: Math.round(parent.width / numPoints)

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
        property variant items: []
    }

    Path {
        id: dline
        startY: parent.height / 2
        startX: 0
        PathCurve { x: sp * 1; y: parent.height / 2 - (itemarr.items[5]*100)  }
        PathCurve { x: sp * 2; y: parent.height / 2 - (itemarr.items[7]*100)  }
        PathCurve { x: sp * 3; y: parent.height / 2 - (itemarr.items[9]*100)  }
        PathCurve { x: sp * 4; y: parent.height / 2 - (itemarr.items[11]*100) }
        PathCurve { x: sp * 5; y: parent.height / 2 - (itemarr.items[13]*100) }
        PathCurve { x: sp * 6; y: parent.height / 2 - (itemarr.items[15]*100) }
        PathCurve { x: sp * 7; y: parent.height / 2 - (itemarr.items[17]*100) }
        PathCurve { x: sp * 8; y: parent.height / 2 - (itemarr.items[19]*100) }
        PathCurve { x: sp * 9; y: parent.height / 2 - (itemarr.items[21]*100) }

        PathCurve { x: sp * 10; y: parent.height / 2 - (itemarr.items[23]*100) }
        PathCurve { x: sp * 11; y: parent.height / 2 - (itemarr.items[25]*100) }
        PathCurve { x: sp * 12; y: parent.height / 2 - (itemarr.items[27]*100) }
        PathCurve { x: sp * 13; y: parent.height / 2 - (itemarr.items[29]*100) }
        PathCurve { x: sp * 14; y: parent.height / 2 - (itemarr.items[31]*100) }
        PathCurve { x: sp * 15; y: parent.height / 2 - (itemarr.items[33]*100) }
        PathCurve { x: sp * 16; y: parent.height / 2 - (itemarr.items[35]*100) }
    }

    onPaint: {
        context.strokeStyle = Qt.rgba(255, 255, 255);
        context.path = dline;
        context.stroke();
    }
}
