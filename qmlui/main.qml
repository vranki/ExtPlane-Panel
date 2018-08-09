import QtQuick 2.6
import QtQuick.Window 2.2
import org.vranki.extplane 1.0
import "panelitems"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("ExtPlane Panel")
    color: "black"

    property bool editMode: false

    MouseArea {
        id: dragArea
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
    }
    ExtPlaneClient {
        id: extplaneClient
        objectName: "extplaneClient"
        simulated: true
        Component.onCompleted: createClient()
    }
    Text {
        color: extplaneClient.extplaneConnection.connected ? "white" : "red"
        text: extplaneClient.connectionMessage + " " + (extplaneClient.extplaneConnection.connected ? "Connected" : extplaneClient.extplaneConnection.networkError)
    }

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
    Item {
        focus: true
        Keys.onSpacePressed: editMode = !editMode
    }
}
