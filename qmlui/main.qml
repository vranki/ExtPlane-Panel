import QtQuick 2.6
import QtQuick.Window 2.2
import Qt.labs.settings 1.0
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("ExtPlane Panel")
    color: "black"
    property int panelId: 0
    property bool simulatedConnection: true
    Text {
        color: extplaneClient.extplaneConnection.connected ? "white" : "red"
        text: extplaneClient.connectionMessage + (extplaneClient.simulated ? "" : " | " + extplaneClient.extplaneConnection.networkError)
    }

    Item {
        focus: !mainMenu.visible
        Keys.onSpacePressed: panelItemArea.editMode = !panelItemArea.editMode
        Keys.onEscapePressed: Qt.quit()
        Keys.onTabPressed: mainMenu.visible = !mainMenu.visible
        Keys.onPressed: {
            if (event.text === 'f') window.toggleFullscreen()
            if (event.text === 'a') addItemDialog.visible = !addItemDialog.visible
            if (event.text === 's') panelItemArea.savePanel()
        }
        Keys.onDeletePressed: panelItemArea.deleteSelectedItem()
    }
    PanelItemArea {
        id: panelItemArea
    }
    MainMenu {
        id: mainMenu
    }
    AddItemDialog {
        id: addItemDialog
    }
    Settings {
        category: "mainwindow"
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }
    Settings {
        category: "panel/" + panelId
        property int itemCount: 0
    }
    Settings {
        id: applicationSettings
        category: "application"
        property alias simulatedConnection: window.simulatedConnection
        property alias panelId: panelItemArea.panelId
    }
    function toggleFullscreen() {
        if(window.visibility & Window.FullScreen) {
            window.showNormal()
        } else {
            window.showFullScreen()
        }
    }
    Component.onCompleted: {
        extplaneClient.simulated = simulatedConnection
    }
    onSimulatedConnectionChanged: extplaneClient.simulated = simulatedConnection

    // Utility functions (move to own file?)

    // Limit value between min & max
    function limitValue(value, min, max) {
        return Math.max(Math.min(value, max), min)
    }
}
