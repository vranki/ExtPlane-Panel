import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

Window {
    flags: Qt.Dialog
    width: propertyContainer.width + 50
    height: propertyContainer.height + 50
    property alias propertyItems: propertyContainer.children
    Flickable {
        anchors.fill: parent
        GridLayout {
            id: propertyContainer
            anchors.fill: parent
            columns: 2
            columnSpacing: 30
            rowSpacing: 30
        }
    }
}
