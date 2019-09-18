import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Dialog {
    title: "Main menu"
    modality: Qt.NonModal
    standardButtons: StandardButton.Close

    GridLayout {
        id: buttonColumn
        width: parent.width
        columns: 2
        Button {
            Layout.fillWidth: true
            text: "Add item (a)"
            onClicked: addItemDialog.visible = true
        }
        Button {
            Layout.fillWidth: true
            text: "Toggle fullscreen (f)"
            onClicked: window.toggleFullscreen()
        }
        Button {
            Layout.fillWidth: true
            text: "Save panels (s)"
            onClicked: panelItemArea.savePanel()
        }
        Button {
            Layout.fillWidth: true
            text: "Clear panel"
            onClicked: panelItemArea.clearPanel()
        }
        Button {
            Layout.fillWidth: true
            text: "Warning log"
            onClicked: warningLogWindow.open()
        }
        Item { width: 1; height: 1  }
        /*
        Button {
            Layout.fillWidth: true
            text: "Export panel file"
        }

        Button {
            Layout.fillWidth: true
            text: "Import panel file"
        }*/
        Label {
            text: "Panel number"
        }
        SpinBox {
            from: 0
            to: 42 // Should be enough for everybody
            value: panelItemArea.panelId
            onValueChanged: panelItemArea.loadPanel(value)
        }
        CheckBox {
            text: "Simulated connection (needs restart)"
            checkState: window.simulatedConnection ? Qt.Checked : Qt.Unchecked
            onClicked: window.simulatedConnection = !window.simulatedConnection
        }
        TextField {
            placeholderText: "ExtPlane IP address"
            onTextChanged: applicationSettings.extplaneHost = text
            text: applicationSettings.extplaneHost
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            enabled: !simulatedConnection
        }
        CheckBox {
            text: "Snap to grid"
            checkState: panelItemArea.snapToGrid ? Qt.Checked : Qt.Unchecked
            onClicked: panelItemArea.snapToGrid = !panelItemArea.snapToGrid
        }
        Button {
            Layout.fillWidth: true
            text: "Quit"
            onClicked: Qt.quit()
        }
        Label {
            text: "Press ESC to close or tab to open"
        }
    }
}
