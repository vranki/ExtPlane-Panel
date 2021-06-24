import QtQuick 2.11
import Qt.labs.settings 1.0
import org.vranki.extplane 1.0
import "panelitems"

MouseArea {
    id: dragArea
    property bool editMode: false
    property int panelId: 0
    property bool snapToGrid: true
    property int gridSize: 5

    // Internal:
    property QtObject dragItem: null
    property QtObject resizeItem: null
    property QtObject selectedItem: null
    property int xOffset
    property int yOffset
    property bool busy: resizeItem || dragItem
    property string datastore: ""
    property var panelsModel: [] // Contains all panels

    anchors.fill: parent
    enabled: dragItem || resizeItem

    onPositionChanged: {
        if(dragItem) {
            dragItem.x = snapValue(mouseX - xOffset)
            dragItem.y = snapValue(mouseY - yOffset)
        } else if(resizeItem) {
            resizeItem.width = snapValue(mouseX - resizeItem.x + xOffset)
            resizeItem.height = snapValue(mouseY - resizeItem.y + yOffset)
        }
    }

    onDragItemChanged: if(dragItem) selectItem(dragItem)
    onResizeItemChanged: if(resizeItem) selectItem(resizeItem)
    onReleased: resizeItem = dragItem = null
    onEditModeChanged: if(!editMode) selectItem(null)
    onDoubleClicked: if(dragItem) dragItem.showProperties()

    Connections {
        target: addItemDialog
        onAddItem: addItem(itemName, undefined, width/2, height/2, 200, 200)
    }

    function snapValue(value) {
        return snapToGrid ? Math.floor(value / gridSize) * gridSize : value
    }

    // x,y,width & height can be -1 for loading
    function addItem(itemName, itemId, x, y, width, height) {
        if(!itemName) {
            console.error("addItem: trying to add item with null name!")
            return
        }
        if(itemId < 0) {
            console.error("addItem: trying to add item with negative itemId!")
            return
        }
        // console.log(itemName, itemId, x, y, width, height)

        var component = Qt.createComponent("qrc:///panelitems/" + itemName + ".qml");
        if (component.status === Component.Error) {
            console.log("Component failed to load:", component.errorString() )
            return
        }
        if(!itemId) { // No itemId, assign new
            itemId = settings.largestId
            settings.largestId = settings.largestId + 1 // Just increase this
        }
        var newItem = component.createObject(dragArea, {
                                                 "itemName": itemName,
                                                 "itemId": itemId,
                                                 "panelId": panelId,
                                                 "editMode": Qt.binding(function() { return editMode })
                                             });

        if (newItem === null) {
            console.log("Error creating panel item!", itemName, itemId, panelId);
            return
        }

        // Defined so set
        if(x > 0 && y > 0 && width > 0 && height > 0) {
            newItem.x = x
            newItem.y = y
            newItem.width = width
            newItem.height = height
        }

        // Make sure no zero-size items can be created
        newItem.width = Math.max(10, newItem.width)
        newItem.height = Math.max(10, newItem.height)

        panelItemModel.append( {"itemName": itemName, "item": newItem} )
        selectItem(newItem)
        // console.log("Added", itemName, newItem.itemId, newItem.x, newItem.y, newItem.width, newItem.height)

        return newItem
    }

    function selectItem(newItem) {
        for(var i=0;i<panelItemModel.count;i++) {
            panelItemModel.get(i).item.isSelectedItem = false
        }
        if(newItem) newItem.isSelectedItem = true
        selectedItem = newItem
    }

    function deleteSelectedItem() {
        if(selectedItem) {
            var deletedIndex = undefined
            for(var i=0;i<panelItemModel.count;i++) {
                if(panelItemModel.get(i).item === selectedItem) deletedIndex = i
            }
            panelItemModel.remove(deletedIndex)
            selectedItem.destroy()
            selectItem(null)
        }
    }

    Connections {
        target: window
        onSaveAll: {
            var panelModel = []

            for(var i=0;i<panelItemModel.count;i++) {
                var itemObject = panelItemModel.get(i)
                panelModel.push({
                                    "itemName": itemObject.itemName,
                                    "itemId": itemObject.item.itemId
                                })
            }
            panelsModel[panelId] = { "panelId": panelId, "panelItems": panelModel }
            datastore = JSON.stringify(panelsModel)
            console.log("Saved:", datastore)
        }
    }

    function loadPanel(id) {
        if(id === panelId && panelItemModel.count) return; // Panel already displayed
        panelId = id || 0
        if(!datastore.length) return; // Data store not initialized yet, delay loading
        clearPanel()

        if(!panelsModel[panelId]) {
            panelsModel[panelId] = { "panelId": panelId, "panelItems": [] } // New panel
        }

        var panelModel = panelsModel[panelId].panelItems
        for(var i=0;i<panelModel.length;i++) {
            var item = panelModel[i]
            addItem(item.itemName, item.itemId, -1, -1, -1, -1)
            item.x = Math.max(0, item.x || 0)
            item.y = Math.max(0, item.y || 0)
            item.width = Math.max(5, item.width || 0)
            item.height = Math.max(5, item.height || 0)
        }
        if (!panelModel.length) mainMenu.visible = true
    }

    // Deletes all items from this panel
    function clearPanel() {
        while(panelItemModel.count) {
            var item = panelItemModel.get(panelItemModel.count - 1).item
            item.destroy()
            panelItemModel.remove(panelItemModel.count - 1)
        }
    }

    function duplicateSelectedItem() { // TODO: WIP
        if(selectedItem) {
            console.log("Duplicating ", selectedItem.itemName, selectedItem.x + snapValue, selectedItem.y + snapValue, selectedItem.width, selectedItem.height)
            var oldItem = selectedItem
            var newItem = addItem(selectedItem.itemName, undefined, selectedItem.x + gridSize, selectedItem.y + gridSize, selectedItem.width, selectedItem.height)
            for(var i=0;i<oldItem.children.length;i++) {
                // console.log("child:", oldItem.children[i])
            }
            if(newItem.copySettings) newItem.copySettings(oldItem)
            // extplaneUtilities.copySettings(oldItem, newItem)
        }
    }
    ListModel { id: panelItemModel }

    ListView {
        width: parent.width/2
        height: parent.height
        model: panelItemModel
        delegate: Text { text: item.itemId + ": " + itemName; color: item.isSelectedItem ?  "white" : "gray"}
        interactive: false
        visible: editMode
    }

    Settings {
        id: settings
        category: "panelitems"
        fileName: "panel.ini"
        property alias datastore: dragArea.datastore
        property int largestId: 0
        onDatastoreChanged: {
            if(!panelsModel.length) { // Load only once on startup
                panelsModel = JSON.parse(datastore)
                console.log(datastore)
                // panelsModel = [] // To reset all
                loadPanel(panelId)
            }
        }
    }
}
