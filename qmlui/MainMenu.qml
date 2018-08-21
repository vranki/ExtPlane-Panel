import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Rectangle {
    color: "gray"
    opacity: 0.8
    width: buttonColumn.width + 20
    height: buttonColumn.height + 20
    anchors.centerIn: parent
    ColumnLayout {
        id: buttonColumn
        anchors.centerIn: parent
        Button {
            Layout.fillWidth: true
            text: "Add item"
            onClicked: addItemDialog.visible = true
        }
        Button {
            Layout.fillWidth: true
            text: "Toggle fullscreen (f)"
            onClicked: window.toggleFullscreen()
        }
        Button {
            Layout.fillWidth: true
            text: "Quit (esc)"
            onClicked: Qt.quit()
        }
        Label {
            text: "Press Tab to close"
        }
    }
}
