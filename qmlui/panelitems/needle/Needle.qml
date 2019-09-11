import QtQuick 2.0

Image {
    property int needleType: 0
    source: "needle-" + needleType + ".svg"
    width: parent.width
    height: parent.height
    transformOrigin: Item.Center
    fillMode: Image.PreserveAspectFit
    scale: 0.85
}
