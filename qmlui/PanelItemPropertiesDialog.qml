import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

import "panelitems/settingsui" as SettingsUi

Window {
    id: itemPropertiesWindow
    flags: Qt.Dialog
    width: 800
    height: 600
    property alias propertyItems: propertyContainer.children
    property alias helpText: helpContainer.text
    property var panelItem: undefined

    Flickable {
        width: parent.width
        height: parent.height
        flickableDirection: Flickable.VerticalFlick

        contentWidth: columnLayout.width
        contentHeight: columnLayout.height

        ScrollBar.vertical: ScrollBar { }

        ColumnLayout {
            id: columnLayout
            Layout.bottomMargin: 20
            Layout.topMargin: 5
            Layout.fillWidth: true
            TextArea {
                id: helpContainer
                wrapMode: TextArea.WordWrap
                readOnly: true
                textFormat: TextEdit.AutoText
                onLinkActivated: Qt.openUrlExternally(link)
                text: helpText
            }
            // Common properties
            RowLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Label { text: "Z value (higher on top)" }
                SettingsUi.IntField {
                    value: 0
                    maxValue: 10
                    minValue: -10
                    onValueChanged: panelItem.z = value
                }
            }

            GridLayout {
                id: propertyContainer
                columns: 2
                columnSpacing: 20
                rowSpacing: 20
                Layout.fillWidth: true
                Layout.leftMargin: 10
            }

            Item {
                width: itemPropertiesWindow.width
                height: okButton.height
                Button {
                    id: okButton
                    text: "Ok"
                    onClicked: itemPropertiesWindow.close()
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                }
            }
        }
    }
}
