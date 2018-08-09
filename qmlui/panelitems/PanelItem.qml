import QtQuick 2.0

Item {
    id: panelItem
    objectName: "panelItem"
    property var connection: extplaneClient.extplaneConnection

    Rectangle {
        id: highlightRect
        color: "transparent"
        border.color: "yellow"
        anchors.fill: parent
        visible: editMode
    }

    MouseArea { // Dragging
        width: highlightRect.width
        height: highlightRect.height
        enabled: editMode && !dragArea.busy
        onPressed: {
            dragArea.dragItem = panelItem
            dragArea.xOffset = mouseX
            dragArea.yOffset = mouseY
            mouse.accepted = false
        }
    }
    Rectangle {
        id: resizeHandle
        color: "transparent"
        border.color: "yellow"
        x: parent.width - width
        y: parent.height - height
        width: 20
        height: 20
        visible: editMode
        MouseArea {
            anchors.fill: parent
            enabled: editMode && !dragArea.busy
            onPressed: {
                dragArea.resizeItem = panelItem
                dragArea.xOffset = mouseX
                dragArea.yOffset = mouseY
                mouse.accepted = false
            }
        }
    }
}
