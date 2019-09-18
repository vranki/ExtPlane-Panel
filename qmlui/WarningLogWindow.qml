import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Dialog {
    title: "Warning log"
    modality: Qt.NonModal
    standardButtons: StandardButton.Close
    width: 800
    height: 400

    onRejected: warningLog.clear()

    ListModel {
        id: warningLog
    }
    ListView {
        header: Label {
            text: "Warning log"
            font.pixelSize: 22
            font.bold: true
        }
        width: parent.width
        height: parent.height
        model: warningLog
        delegate: Label { text: timestamp + " - " + message }
    }
    Connections {
        target: extplaneClient
        onExtplaneWarning: {
            console.log("Extplane-Warning:", message)
            warningLog.append({ "message": message, "timestamp": new Date().toLocaleTimeString(Qt.locale("en_UK")) } )
            if(!panelItemArea.editMode) warningLogWindow.open()
        }
    }
}
