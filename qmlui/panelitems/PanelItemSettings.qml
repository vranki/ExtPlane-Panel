import QtQuick 2.0
import Qt.labs.settings 1.0

Settings {
    id: settings
    fileName: "panel.ini"
    category: "panelitem-" + panelId + "/" + itemId
}
