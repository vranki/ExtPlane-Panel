import QtQuick 2.0
import "panelitems"

MouseArea {
    id: dragArea
    property bool editMode: false


    // Internal:
    property var dragItem: null
    property var resizeItem: null
    property int xOffset
    property int yOffset
    property bool busy: resizeItem || dragItem
    anchors.fill: parent
    enabled: dragItem || resizeItem
    onPositionChanged: {
        if(dragItem) {
            dragItem.x = mouseX - xOffset
            dragItem.y = mouseY - yOffset
        } else if(resizeItem) {
            resizeItem.width = mouseX - resizeItem.x + xOffset
            resizeItem.height = mouseY - resizeItem.y + yOffset
        }
    }
    onReleased: resizeItem = dragItem = null

    AttitudeIndicator {
        x: 100
        y: 100
        width: 100
        height: 100
    }
    CompassBasic {
        x: 300
        y: 100
        width: 300
        height: 100
    }
    AirspeedIndicator {
        x: 200
        y: 300
        width: 300
        height: 300
    }
}
