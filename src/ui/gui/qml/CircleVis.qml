import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
Canvas {
    id:canvasCircle
    width: window.width
    height: window.height
    contextType: "2d"
    z:bluerect.z+5
    function clear(){
        var ctx=getContext('2d');
        ctx.reset();
        canvasCircle.requestPaint();
    }

    function readValues(anArray) {
        itemarr.items=anArray
    }

    Item{
        id: itemarr
        property variant items: [0]
    }

    Path {

            id: dline
            startY: visualize.height/2 + 100*Math.sin(Math.PI/2)
            startX: visualize.width/2 + 100* Math.cos(Math.PI/2)
            PathCurve { x: visualize.width/2 + (100+itemarr.items[5]*20)* Math.cos(5*Math.PI/8); y:visualize.height/2 + (100+itemarr.items[5]*20)*Math.sin(5*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[7]*20)* Math.cos(3*Math.PI/4); y:visualize.height/2 + (100+itemarr.items[7]*20)*Math.sin(3*Math.PI/4) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[9]*20)* Math.cos(7*Math.PI/8); y:visualize.height/2 + (100+itemarr.items[9]*20)*Math.sin(7*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[11]*20)* Math.cos(Math.PI); y:visualize.height/2 + (100+itemarr.items[11]*20)*Math.sin(Math.PI) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[13]*20)* Math.cos(9*Math.PI/8); y:visualize.height/2 + (100+itemarr.items[13]*20)*Math.sin(9*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[15]*20)* Math.cos(5*Math.PI/4); y:visualize.height/2 + (100+itemarr.items[15]*20)*Math.sin(5*Math.PI/4) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[17]*20)* Math.cos(11*Math.PI/8); y:visualize.height/2 + (100+itemarr.items[17]*20)*Math.sin(11*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[19]*20)* Math.cos(3*Math.PI/2); y:visualize.height/2 + (100+itemarr.items[19]*20)*Math.sin(3*Math.PI/2) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[21]*20)* Math.cos(13*Math.PI/8); y:visualize.height/2 + (100+itemarr.items[21]*20)*Math.sin(13*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[23]*20)* Math.cos(7*Math.PI/4); y:visualize.height/2 + (100+itemarr.items[23]*20)*Math.sin(7*Math.PI/4) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[25]*20)* Math.cos(15*Math.PI/8); y:visualize.height/2 + (100+itemarr.items[25]*20)*Math.sin(15*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[27]*20)* Math.cos(2*Math.PI); y:visualize.height/2 + (100+itemarr.items[27]*20)*Math.sin(2*Math.PI) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[29]*20)* Math.cos(Math.PI/8); y:visualize.height/2 + (100+itemarr.items[29]*20)*Math.sin(Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[31]*20)* Math.cos(Math.PI/4); y:visualize.height/2 + (100+itemarr.items[31]*20)*Math.sin(Math.PI/4) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[33]*20)* Math.cos(3*Math.PI/8); y:visualize.height/2 + (100+itemarr.items[33]*20)*Math.sin(3*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100)* Math.cos(Math.PI/2); y:visualize.height/2 + (100)*Math.sin(Math.PI/2) }

    }
        onPaint: {
            context.strokeStyle = Qt.rgba(255, 255, 255);
            context.path =dline;
            context.stroke();

        }

}