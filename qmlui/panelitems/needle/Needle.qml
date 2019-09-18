import QtQuick 2.0

Image {
    property int needleType: 0
    property int movementDuration: 50
    source: "needle-" + needleType + ".svg"
    width: parent.width
    height: parent.height
    transformOrigin: Item.Center
    fillMode: Image.PreserveAspectFit
    scale: 0.85
    Behavior on rotation { PropertyAnimation {
            easing.type: Easing.InOutQuad
            duration: movementDuration
        }
    }
}
