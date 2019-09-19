import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.0
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel
import "../settingsui" as SettingsUi

PanelItems.PanelItem {
    clip: false
    property var statusNames: ["Image not set", "Ready", "Loading..", "Error loading\n" + image.source ]

    Image {
        id: image
        anchors.fill: parent
        source: settings.imageFile
        fillMode: settings.stretch ? Image.Stretch : Image.PreserveAspectFit
    }
    Text {
        color: "red"
        text: statusNames[image.status]
        visible: image.status != Image.Ready
        anchors.centerIn: image
    }

    propertiesDialog.helpText: 'Can show an image from file'
    propertiesDialog.propertyItems: [
        Text { text: "Image file name" },
        SettingsUi.FileSelector { text: settings.imageFile ; onTextChanged: settings.imageFile = text},
        Text { text: "Stretch image to fit item" },
        CheckBox { checked: settings.stretch ; onCheckedChanged: settings.stretch = checked }
    ]


    PanelItems.PanelItemSettings {
        id: settings
        property string imageFile: ""
        property bool stretch: false
    }
}
