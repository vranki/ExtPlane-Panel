import QtQuick 2.0

Item { // Gets parent size
    property bool thickBars: false // Thick or thin bars
    property bool showValue: false // Show values as numbers
    property double valueMin: 0 // Minimum value to show
    property double valueMax: 100 // Maximum value to show
    property double barValue: 10 // Value of one bar
    property int barsAngleMin: -90 // Angle of minimum value, 0=west
    property int barsAngle: 270 // Total angle of values
    property double valueMultiplier: 1 // Multiplier of value labels

    readonly property int barCount: valueRange / barValue + 1
    readonly property double valueRange: valueMax - valueMin
    readonly property double barAngle: barsAngle / (barCount - 1)

    width: Math.min(parent.width, parent.height)
    height: width

    Item { // Scales content
        id: barsItem
        width: 1000
        height: width
        scale: parent.width / 1000
        transformOrigin: Item.TopLeft
        Repeater {
            id: thickBarsRepeater
            model: barCount
            Item {
                id: barItem
                width: barsItem.width
                height: 15
                anchors.centerIn: barsItem
                rotation: index * barAngle + barsAngleMin // TODO: valueMin vaikuttaa
                transformOrigin: Item.Center
                Rectangle {
                    color: "white"
                    width: thickBars ? 50 : 25
                    height: thickBars ? 15 : 5
                    antialiasing: true
                }
                Text {
                    x: 70
                    visible: showValue
                    color: "white"
                    font.pixelSize: 50
                    rotation: -barItem.rotation
                    anchors.verticalCenter: parent.verticalCenter
                    text: Math.round((index * barValue + valueMin) / valueMultiplier)
                }
            }
        }
    }
    function value2Angle(value) {
        return barsAngleMin - 90 + ((value - valueMin) / valueRange) * barsAngle
        // return barsAngleMin + ((value - valueMin) / valueRange) * barsAngle
    }
}
