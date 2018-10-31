import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
Canvas {
    id:canvas
    width: 400; height: 200
    contextType: "2d"
    z:bluerect.z+5
    function readValues(anArray) {
        /*for (var i=0; i<anArray.length; i++){
            //console.log("Array item:", anArray[i])
           var items=anArray;
        }*/
        itemarr.items=anArray


    }

    /*Item{
        id:drawfc
        function drawline(){
            for(var i=0;i<25;i++){

            }
        }
    }*/
    Item{
        id: itemarr
        property variant items: []
    }

    Path {

            id: dline
            startX: 0; startY: 100

            PathCurve { x: 75; y: itemarr.items[0]}
            PathCurve { x: 200; y: itemarr.items[1] }
            PathCurve { x: 325; y: itemarr.items[2]}
            PathCurve { x: 400; y: itemarr.items[3] }

        }

        onPaint: {
            context.strokeStyle = Qt.rgba(.4,.6,.8);
            context.path =dline;
            //canvas.clear();
            context.stroke();
        }

}

/*
Canvas{
    id:canvas
    z:bluerect.z+1
    anchors.fill: parent
    property int lastX: 0
    property int lastY: 0

    function clear(){
        var ctx=getContext('2d');
        ctx.reset();
        canvas.requestPaint();
    }

    MouseArea{
        id:area
        anchors.fill: parent

        onClicked: {
            canvas.lastX=mouseX;
            canvas.lastY=mouseY;
        }
        onPositionChanged: {
            canvas.requestPaint();
        }
    }

    onPaint: {
        var ctx = getContext('2d');
        ctx.lineWidth = 5;
        ctx.beginPath();
        ctx.moveTo(lastX,lastY);
        lastX=area.mouseX;
        lastY=area.mouseY;
        ctx.lineTo(lastX,lastY);
        ctx.stroke();
    }
}
*/
