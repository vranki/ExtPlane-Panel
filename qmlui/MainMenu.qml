import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Dialog {
    title: "Main menu"
    modality: Qt.NonModal
    standardButtons: StandardButton.Close

    ColumnLayout {
        id: buttonColumn
        width: parent.width

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
        Label {
            text: "Press ESC to close or tab to open"
        }
    }
}
