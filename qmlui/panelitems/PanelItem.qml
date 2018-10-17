import QtQuick 2.0
import Qt.labs.settings 1.0

Item {
    id: panelItem
    property int itemId: -1 // Index in item list
    property int panelId: -1 // Panel id to save in
    property string itemName: ""
    property bool editMode: false
    property bool isSelectedItem: false

    Rectangle {
        id: highlightRect
        color: "transparent"
        border.color: "yellow"
        anchors.fill: parent
        visible: editMode
        z: 100
        opacity: isSelectedItem ? 1 : 0.3
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
        z: 100
        visible: editMode
        opacity: isSelectedItem ? 1 : 0.3
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
    Settings {
        category: "panel-" + panelId + "-item-" + itemId
        property alias x: panelItem.x
        property alias y: panelItem.y
        property alias width: panelItem.width
        property alias height: panelItem.height
    }
}
