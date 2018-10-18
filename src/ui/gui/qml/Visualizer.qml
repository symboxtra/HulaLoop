import QtQuick 2.0
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.2


import "../fonts/Icon.js" as MDFont

Rectangle {

    id: visualize
    width: window.width
    height: parent.height * 0.6
    color: "lightsteelblue"
    y: 98

    Row {
        y: parent.height-50
        x: -40
        Item{
            id: gennums
            function generate(element){
                for(var i =0;i<70;i++){
                    element.itemAt(i).height=rand.randomNumber()
                  }
             }
        }

        Item{
            id: rand
            function randomNumber() {
                    return Math.random()*200 ;
            }
        }
        Timer {
            interval: 300; running: true; repeat: true
            triggeredOnStart: true
            onTriggered: gennums.generate(rectgen)
        }
        Repeater{
            id:rectgen
            model: 70
            Rectangle { id: testrec; color: "grey"; width: 10; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        }

    }
}
