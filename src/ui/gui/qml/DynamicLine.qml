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
        for (var i=0; i<anArray.length; i++){
            //console.log("Array item:", anArray[i])
           //var items=anArray;
            //itemarr.items.at[i]=anArray[i]*100;
        }
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
            startY: 200
            startX: 0
            PathCurve { x: 0; y: 200-(itemarr.items[5]*100)}
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
            PathCurve { x: 1200; y: 200-(itemarr.items[35]*100) }

            /*PathCurve { x: 320; y: 200-(itemarr.items[16]*100)}
            PathCurve { x: 340; y: 200-(itemarr.items[17]*100) }
            PathCurve { x: 360; y: 200-(itemarr.items[18]*100)}
            PathCurve { x: 380; y: 200-(itemarr.items[19]*100) }

            PathCurve { x: 400; y: 200-(itemarr.items[20]*100)}
            PathCurve { x: 420; y: 200-(itemarr.items[21]*100) }
            PathCurve { x: 440; y: 200-(itemarr.items[22]*100)}
            PathCurve { x: 460; y: 200-(itemarr.items[23]*100) }*/


    }
    Path {

            id: dlinetwo
            startY:100
            startX:300

/*




            PathCurve { x: 480; y: 100-(itemarr.items[24]*100)}
            PathCurve { x: 500; y: 100-(itemarr.items[25]*100) }
            PathCurve { x: 520; y: 100-(itemarr.items[26]*100)}
            PathCurve { x: 540; y: 100-(itemarr.items[27]*100) }
            PathCurve { x: 560; y: 100-(itemarr.items[28]*100)}
            PathCurve { x: 580; y: 100-(itemarr.items[29]*100) }
            PathCurve { x: 600; y: 100-(itemarr.items[30]*100)}
            PathCurve { x: 620; y: 100-(itemarr.items[31]*100) }

            PathCurve { x: 640; y: 100-itemarr.items[32]}
            PathCurve { x: 660; y: 100-itemarr.items[33] }
            PathCurve { x: 680; y: 100-itemarr.items[34]}
            PathCurve { x: 700; y: 100-itemarr.items[35] }
            PathCurve { x: 720; y: 100-itemarr.items[36]}
            PathCurve { x: 740; y: 100-itemarr.items[37] }
            PathCurve { x: 760; y: 100-itemarr.items[38]}
           PathCurve { x: 780; y: 100-itemarr.items[39] }

            PathCurve { x: 800; y: 100-itemarr.items[40]}
            PathCurve { x: 820; y: 100-itemarr.items[41] }
            PathCurve { x: 840; y: 100-itemarr.items[42]}
            PathCurve { x: 860; y: 100-itemarr.items[43] }
            PathCurve { x: 880; y: 100-itemarr.items[44]}
            PathCurve { x: 900; y: 100-itemarr.items[45] }
            PathCurve { x: 920; y: 100-itemarr.items[46]}
            PathCurve { x: 940; y: 100-itemarr.items[47] }

            PathCurve { x: 980; y: 100-itemarr.items[48]}
            PathCurve { x: 1000; y: 100-itemarr.items[49] }
            PathCurve { x: 1020; y: 100-itemarr.items[50]}
            PathCurve { x: 1040; y: 100-itemarr.items[51] }
            PathCurve { x: 1060; y: 100-itemarr.items[52]}
            PathCurve { x: 1080; y: 100-itemarr.items[53] }
            PathCurve { x: 1100; y: 100-itemarr.items[54]}
            PathCurve { x: 1120; y: 100-itemarr.items[55] }

            PathCurve { x: 1140; y: 100-itemarr.items[56]}
            PathCurve { x: 1160; y: 100-itemarr.items[55] }
            PathCurve { x: 1180; y: 100-itemarr.items[56]}
            PathCurve { x: 1200; y: 100-itemarr.items[57] }
            PathCurve { x: 1220; y: 100-itemarr.items[58]}
            PathCurve { x: 1240; y: 100-itemarr.items[60] }
            PathCurve { x: 1260; y: 100-itemarr.items[61]}
            PathCurve { x: 1280; y: 100-itemarr.items[62] }*/
        }

        onPaint: {
            context.strokeStyle = Qt.rgba(0,0,0);
            context.path =dline;

            //canvas.clear();
            context.stroke();
           /* context.path=dlinetwo;
            context.stroke();*/
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
