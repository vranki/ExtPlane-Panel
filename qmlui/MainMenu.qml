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
            text: "Save panel (s)"
            onClicked: panelItemArea.savePanel()
        }
        Button {
            Layout.fillWidth: true
            text: "Quit"
            onClicked: Qt.quit()
        }
        CheckBox {
            text: "Simulated connection"
            checkState: window.simulatedConnection ? Qt.Checked : Qt.Unchecked
            onClicked: window.simulatedConnection = !window.simulatedConnection
        }
        TextField {
            placeholderText: "ExtPlane IP address"
            onTextChanged: {
                extplaneClient.extplaneConnection.hostName = text.length > 0 ? text : "127.0.0.1"
            }
            inputMethodHints: Qt.ImhFormattedNumbersOnly
        }
        Label {
            text: "Press ESC to close or tab to open"
        }
    }
}
