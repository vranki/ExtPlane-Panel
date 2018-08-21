import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Rectangle {
    color: "gray"
    opacity: 0.8
    width: buttonColumn.width + 20
    height: buttonColumn.height + 20
    anchors.centerIn: parent
    visible: false
    property string previewItem: ""
    ColumnLayout {
        id: buttonColumn
        anchors.centerIn: parent
        ListView {
            width: 500; height: 200

            model: panelItemModel
            delegate: Button {
                text: name + ": " + itemName
                checkable: true
                checked: itemName === previewItem
                onClicked: previewItem = itemName
                Layout.fillWidth: true
            }
        }
        Rectangle {
            color: "black"
            width: 500
            height: 400
            Loader {
                source: previewItem ? "qrc:///panelitems/" + previewItem + ".qml" : ""
                width: parent.width * 0.8
                height: parent.height * 0.8
                anchors.centerIn: parent
            }
        }
        Button {
            Layout.fillWidth: true
            text: "Close"
            onClicked: parent.parent.visible = false
        }
    }
    ListModel {
        id: panelItemModel
        ListElement {
            name: "Compass"
            itemName: "CompassBasic"
        }
        ListElement {
            name: "Airspeed indicator"
            itemName: "AirspeedIndicator"
        }
    }
}
