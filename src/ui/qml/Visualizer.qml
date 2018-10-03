import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtCharts 2.2
import QtQuick.Controls 2.2


import "../fonts/Icon.js" as MDFont

Rectangle {

    id: visualize
    width: parent.width
    height: parent.height * 0.6
    color: "lightsteelblue"
    y: 98

    /*var request = new XMLHttpRequest()
    var lines=""
    request.open: ('GET', 'test.txt')
    request.onreadystatechange:function(event) {
        if (request.readyState == XMLHttpRequest.DONE) {
            lines = request.responseText.split('\n')  // Assuming you've defined
            printf(lines)
        }
    }*/

    Row {
        y: parent.height-50
        x: -40
       // Repeater{
           // id: refresh
            //model:5
        Item{
            id: rand
            function randomNumber() {
                    return Math.random()*300 ;
                }
        }
            //property int heightvar:2
            Timer {
                  interval: 1000; running: true; repeat: true
                  triggeredOnStart: true
                  onTriggered: testrec.height = rand.randomNumber()
              }


       // Repeater{

          //  model: 70
            Rectangle { id: testrec; color: "grey"; width: 10; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
       // }
        /*Repeater{
            model: 35
            Rectangle { color: "grey"; width: 10; height: 170-5*index/(refresh.heightvar); border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180}
        }
        }*/

       // }
    }


}
