import QtQuick 2.0
import QtQuick.Shapes 1.0

Shape {
    id: shape
    property real startAngle: 0 // Angle, relative from top
    property real endAngle: 90 // End
    property real arcAngle: endAngle - startAngle
    property real radius: centerX - arcWidth / 2
    property real arcWidth: 5
    property color arcColor: "white"
    readonly property real centerX: width/2
    readonly property real centerY: centerX
    antialiasing: true
    rotation: startAngle

    ShapePath {
        strokeWidth: arcWidth
        strokeColor: arcColor
        fillColor: "transparent"
        startX: centerX; startY: centerY - radius
        PathArc {
            x: shape.centerX + Math.sin(arcAngle * Math.PI / 180) * radiusX
            y: shape.centerY - Math.cos(arcAngle * Math.PI / 180) * radiusY
            radiusX: shape.radius
            radiusY: shape.radius
            useLargeArc: arcAngle > 180
        }
    }
}
