import QtQuick 2.0

Item { // Gets parent size
    property bool thickBars: false // Thick or thin bars
    property bool longBars: thickBars // Long bars
    property bool showValue: false // Show values as numbers
    property bool showLastValue: true // Can be used to hide last value (for 360 degree gauges)
    property bool absValue: false // Don't show negative numbers with -
    property bool limitAngle: false // Limit value2angle between min & max values
    property real valueMin: 0 // Minimum value to show
    property real valueMax: 100 // Maximum value to show
    property real barValue: 10 // Value of one bar
    property int barsAngleMin: -90 // Angle of minimum value, 0=west
    property int barsAngle: 270 // Total angle of values
    property real valueMultiplier: 1 // Multiplier of value labels
    property real fontSizeMultiplier: 1

    readonly property int barCount: valueRange / barValue + 1
    readonly property real valueRange: valueMax - valueMin
    readonly property real barAngle: barsAngle / (barCount - 1)

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
                    width: longBars ? 50 : 25
                    height: thickBars ? 15 : 5
                    antialiasing: true
                }
                Text {
                    x: 70
                    visible: showValue && (showLastValue || index < barCount - 1)
                    color: "white"
                    font.pixelSize: 100 * fontSizeMultiplier
                    rotation: -barItem.rotation
                    anchors.verticalCenter: parent.verticalCenter
                    text: absValue ? Math.abs(value) : value
                    property int value: Math.round((index * barValue + valueMin) / valueMultiplier)
                }
            }
        }
    }
    function value2Angle(value) {
        if(limitAngle) {
            value = limitValue(value, valueMin, valueMax)
        }
        return barsAngleMin - 90 + ((value - valueMin) / valueRange) * barsAngle
    }
}
