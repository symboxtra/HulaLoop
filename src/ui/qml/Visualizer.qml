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
    Row {
        y: parent.height-50
        x: -40
        Repeater{
            model: 30
            Rectangle { color: "grey"; width: 10; height: 5*index; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        }
        Repeater{
            model: 35
            Rectangle { color: "grey"; width: 10; height: 150-2*index; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180}
        }
        }

/*
        Rectangle { color: "grey"; width: 10; height: 100; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 150; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 200; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 200; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 150; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 100; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 100; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 150; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 200; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 200; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 150; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 100; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 100; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 150; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
        Rectangle { color: "grey"; width: 10; height: 200; border.width: 1; border.color: "black"; transform: Rotation { origin.x: 25; origin.y: 25; angle: 180} }
*/
    }


}
