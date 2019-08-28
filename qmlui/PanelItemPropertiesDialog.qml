import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

Window {
    id: itemPropertiesWindow
    flags: Qt.Dialog
    width: 600
    height: 400
    property alias propertyItems: propertyContainer.children
    property alias helpText: helpContainer.text
    Flickable {
        anchors.fill: parent
        ColumnLayout {
            anchors.fill: parent
            TextArea {
                id: helpContainer
                Layout.fillWidth: true
                wrapMode: TextArea.WordWrap
                readOnly: true
                textFormat: TextEdit.AutoText
                onLinkActivated: Qt.openUrlExternally(link)
            }
            GridLayout {
                id: propertyContainer
                columns: 2
                columnSpacing: 30
                rowSpacing: 30
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Layout.topMargin: 10
                Layout.bottomMargin: 10
                Layout.preferredHeight: 1
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
