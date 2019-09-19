import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.0

Button {
    id: fileSelector
    onClicked: fileDialog.visible=true

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: fileSelector.text = fileDialog.fileUrls[0]
    }
}
