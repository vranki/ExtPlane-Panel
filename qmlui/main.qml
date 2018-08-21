import QtQuick 2.6
import QtQuick.Window 2.2
import Qt.labs.settings 1.0
import org.vranki.extplane 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("ExtPlane Panel")
    color: "black"
    property int panelId: 0

    Text {
        color: extplaneClient.extplaneConnection.connected ? "white" : "red"
        text: extplaneClient.connectionMessage + " " + (extplaneClient.extplaneConnection.connected ? "Connected" : extplaneClient.extplaneConnection.networkError)
    }

    Item {
        focus: true
        Keys.onSpacePressed: panelItemArea.editMode = !panelItemArea.editMode
        Keys.onEscapePressed: Qt.quit()
        Keys.onTabPressed: mainMenu.visible = !mainMenu.visible
        Keys.onPressed: {
            if (event.text === 'f') window.toggleFullscreen()
        }
    }

    PanelItemArea {
        id: panelItemArea
    }
    MainMenu {
        id: mainMenu
        visible: true
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
    function toggleFullscreen() {
        if(window.visibility & Window.FullScreen) {
            window.showNormal()
        } else {
            window.showFullScreen()
        }
    }
}
