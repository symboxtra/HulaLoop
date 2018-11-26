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
            startY: 2*visualize.height/2
            startX: visualize.width/4
            /*PathCurve { x: 0; y: 200-(itemarr.items[5]*100)}
            PathCurve { x: 80; y: 200-(itemarr.items[7]*100) }
            PathCurve { x: 160; y: 200-(itemarr.items[9]*100)}
            PathCurve { x: 240; y: 200-(itemarr.items[11]*100) }
            PathCurve { x: 320; y: 200-(itemarr.items[13]*100)}
            PathCurve { x: 400; y: 200-(itemarr.items[15]*100) }
            PathCurve { x: 480; y: 200-(itemarr.items[17]*100)}
            PathCurve { x: 560; y: 200-(itemarr.items[19]*100) }

            PathCurve { x: 640; y: 200-(itemarr.items[21]*100)}
            PathCurve { x: 720; y: 200-(itemarr.items[23]*100) }
            PathCurve { x: 800; y: 200-(itemarr.items[25]*100)}
            PathCurve { x: 880; y: 200-(itemarr.items[27]*100) }
            PathCurve { x: 960; y: 200-(itemarr.items[29]*100)}
             PathCurve { x: 1040; y: 200-(itemarr.items[31]*100) }
            PathCurve { x: 1120; y: 200-(itemarr.items[33]*100)}
            PathCurve { x: 1200; y: 200-(itemarr.items[35]*100) }*/
            //PathCurve { x: visualize.width/4; y: 200}
           // PathCurve { x: visualize.width/4-(itemarr.items[5]*100); y: 2*visualize.height/8-(itemarr.items[5]*100) }
            PathCurve { x: visualize.width/2; y: 2*visualize.height/4-(itemarr.items[9]*100) }
           // PathCurve { x: visualize.width/4-(itemarr.items[13]*100); y: 2*visualize.height/8-(itemarr.items[13]*100) }
            PathCurve { x: 3*visualize.width/4 +(itemarr.items[17]*100); y: 2*visualize.height/2}
            //PathCurve { x: visualize.width/4-(itemarr.items[23]*100); y: 2*visualize.height/8-(itemarr.items[23]*100) }
            PathCurve { x: visualize.width/2; y: 2*3*visualize.height/4 +(itemarr.items[27]*100)}
            //PathCurve { x: visualize.width/4-(itemarr.items[31]*100); y: 2*visualize.height/8-(itemarr.items[31]*100) }
            PathCurve { x: visualize.width/4-(itemarr.items[35]*100); y: 2*visualize.height/2}



    }
        onPaint: {
            context.strokeStyle = Qt.rgba(0,0,0);
            context.path =dline;
            context.stroke();

        }

}

