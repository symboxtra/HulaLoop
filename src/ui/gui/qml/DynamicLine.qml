import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
Canvas {
    id:canvas
    width: window.width
    height: window.height
    contextType: "2d"
    z:bluerect.z+5
    function clear(){
        var ctx=getContext('2d');
        ctx.reset();
        canvas.requestPaint();
    }

    function readValues(anArray) {
        itemarr.items=anArray
    }

    Item{
        id: itemarr
        property variant items: []
    }

    Path {

            id: dline
            startY: 150+visualize.height/2 + 100*Math.sin(Math.PI/2)
            startX: visualize.width/2 + 100* Math.cos(Math.PI/2)
            PathCurve { x: visualize.width/2 + (100+itemarr.items[5]*50)* Math.cos(5*Math.PI/8); y:150+ visualize.height/2 + (100+itemarr.items[5]*50)*Math.sin(5*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[7]*50)* Math.cos(3*Math.PI/4); y:150+ visualize.height/2 + (100+itemarr.items[7]*50)*Math.sin(3*Math.PI/4) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[9]*50)* Math.cos(7*Math.PI/8); y:150+ visualize.height/2 + (100+itemarr.items[9]*50)*Math.sin(7*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[11]*50)* Math.cos(Math.PI); y: 150+visualize.height/2 + (100+itemarr.items[11]*50)*Math.sin(Math.PI) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[13]*50)* Math.cos(9*Math.PI/8); y:150+ visualize.height/2 + (100+itemarr.items[13]*50)*Math.sin(9*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[15]*50)* Math.cos(5*Math.PI/4); y:150+ visualize.height/2 + (100+itemarr.items[15]*50)*Math.sin(5*Math.PI/4) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[17]*50)* Math.cos(11*Math.PI/8); y:150+ visualize.height/2 + (100+itemarr.items[17]*50)*Math.sin(11*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[19]*50)* Math.cos(3*Math.PI/2); y:150+visualize.height/2 + (100+itemarr.items[19]*50)*Math.sin(3*Math.PI/2) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[21]*50)* Math.cos(13*Math.PI/8); y:150+ visualize.height/2 + (100+itemarr.items[21]*50)*Math.sin(13*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[23]*50)* Math.cos(7*Math.PI/4); y:150+ visualize.height/2 + (100+itemarr.items[23]*50)*Math.sin(7*Math.PI/4) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[25]*50)* Math.cos(15*Math.PI/8); y:150+ visualize.height/2 + (100+itemarr.items[25]*50)*Math.sin(15*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[27]*50)* Math.cos(2*Math.PI); y: 150+visualize.height/2 + (100+itemarr.items[27]*50)*Math.sin(2*Math.PI) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[29]*50)* Math.cos(Math.PI/8); y:150+ visualize.height/2 + (100+itemarr.items[29]*50)*Math.sin(Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[31]*50)* Math.cos(Math.PI/4); y:150+ visualize.height/2 + (100+itemarr.items[31]*50)*Math.sin(Math.PI/4) }
            PathCurve { x: visualize.width/2 + (100+itemarr.items[33]*50)* Math.cos(3*Math.PI/8); y:150+ visualize.height/2 + (100+itemarr.items[33]*50)*Math.sin(3*Math.PI/8) }
            PathCurve { x: visualize.width/2 + (100)* Math.cos(Math.PI/2); y:150+ visualize.height/2 + (100)*Math.sin(Math.PI/2) }

    }
        onPaint: {
            context.strokeStyle = Qt.rgba(0,0,0);
            context.path =dline;
            context.stroke();

        }

}

