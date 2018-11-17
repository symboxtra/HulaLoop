import QtQuick 2.0
import QtQuick.Layouts 1.3

import QtQuick.Controls 2.2


import "../fonts/Icon.js" as MDFont

Rectangle {

    id: visualize
    width: window.width
    height: parent.height - btnPanel.height - bottomRectangle.height
    color: "black"
    anchors.top: btnPanel.bottom

    Row {
        y: parent.height-50
        x: -40
        Item{
            id: gennums
            function generate(element){
                for(var i =0;i<Math.round(visualize.width / 10);i++){
                    element.itemAt(i).height=rand.randomNumber()*visualize.height
                  }
             }
        }

        Item{
            id: rand
            function randomNumber() {
                    return Math.random() ;
            }
        }
        Timer {
            interval: 300; running: true; repeat: true
            triggeredOnStart: true
            onTriggered: gennums.generate(rectgen)
        }
        Repeater{
            id:rectgen
            model: Math.round(visualize.width / 10)
            Rectangle { id: testrec; color: "#005B9A"; width: 10; border.width: 1; border.color: "#003366"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        }

    }
}
