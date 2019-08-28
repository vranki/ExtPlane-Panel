import QtQuick 2.0
import Qt.labs.settings 1.0
import "panelitems"

MouseArea {
    id: dragArea
    property bool editMode: false
    property int panelId: 0

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
            dragItem.x = mouseX - xOffset
            dragItem.y = mouseY - yOffset
        } else if(resizeItem) {
            resizeItem.width = mouseX - resizeItem.x + xOffset
            resizeItem.height = mouseY - resizeItem.y + yOffset
        }
    }

    onDragItemChanged: if(dragItem) selectItem(dragItem)
    onResizeItemChanged: if(resizeItem) selectItem(resizeItem)
    onReleased: resizeItem = dragItem = null
    onEditModeChanged: if(!editMode) selectItem(null)
    onDoubleClicked: if(dragItem) dragItem.showProperties()

    Connections {
        target: addItemDialog
        onAddItem: addItem(itemName, width/2, height/2, 200, 200)
    }

    // x,y,width & height can be -1 for loading
    function addItem(itemName, x, y, width, height) {
        var component = Qt.createComponent("qrc:///panelitems/" + itemName + ".qml");
        if (component.status === Component.Error) {
            console.log("Component failed to load:", component.errorString() )
            return
        }

        var newItem = component.createObject(dragArea, {
                                                 "itemName": itemName,
                                                 "itemId": panelItemModel.count,
                                                 "panelId": panelId,
                                                 "editMode": Qt.binding(function() { return editMode })
                                             });

        if (newItem === null) {
            console.log("Error creating panel item!");
            return
        }

        // Defined so set
        if(x > 0 && y > 0 && width > 0 && height > 0) {
            newItem.x = x
            newItem.y = y
            newItem.width = width
            newItem.height = height
        }
        panelItemModel.append( {"itemName": itemName, "item": newItem} )
        selectItem(newItem)
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
            var deletedIndex = selectedItem.itemId
            panelItemModel.remove(deletedIndex)
            for(var i=deletedIndex;i<panelItemModel.count;i++) {
                console.log("Setting item", panelItemModel.get(i).item.itemId, i)
                panelItemModel.get(i).item.itemId = i
            }
            selectedItem.destroy()
            selectItem(null)
        }
    }

    function savePanel() {
        var panelModel = []

        for(var i=0;i<panelItemModel.count;i++) {
            var item = panelItemModel.get(i).item
            panelModel.push({
                                "itemName": item.itemName
                            })
        }
        panelsModel[panelId] = { "panelId": panelId, "panelItems": panelModel }
        datastore = JSON.stringify(panelsModel)
        console.log("JSON data:", datastore)
    }

    function loadPanel(id) {
        if(id === panelId && panelItemModel.count) return; // Panel already displayed
        panelId = id || 0
        if(!datastore.length) return; // Data store not initialized yet, delay loading
        clearPanel()
        console.log("Loading panel", panelId)
        if(!panelsModel[panelId]) {
            panelsModel[panelId] = { "panelId": panelId, "panelItems": [] } // New panel
            console.log("New panel")
        }

        console.log("Panel data ", JSON.stringify(panelsModel[panelId]))
        var panelModel = panelsModel[panelId].panelItems
        for(var i=0;i<panelModel.length;i++) {
            var item = panelModel[i]
            console.log("Loading", i, item.itemName)
            addItem(item.itemName, -1, -1, -1, -1)
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

    ListModel { id: panelItemModel }

    ListView {
        width: parent.width/2
        height: parent.height
        model: panelItemModel
        delegate: Text { text: index + ": " + itemName; color: item.isSelectedItem ?  "white" : "gray"}
        interactive: false
        visible: editMode
    }

    Settings {
        category: "panelitems"
        property alias datastore: dragArea.datastore
        onDatastoreChanged: {
            if(!panelsModel.length) { // Load only once on startup
                panelsModel = JSON.parse(datastore)
                // panelsModel = [] // To reset all
                loadPanel(panelId)
            }
        }
    }
}
