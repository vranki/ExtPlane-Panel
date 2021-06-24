import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel
import "../settingsui" as SettingsUi

PanelItems.PanelItem {
    id: label
    property var colors: ['black','white','red', 'yellow', 'green', 'deepskyblue','orange']
    property real rounder: Math.pow(10,settings.decimals)

    DataRef {
        id: labelRef
        name: settings.dataref
        accuracy: settings.accuracy
    }

    Text {
        text: settings.labelText + (settings.round ? (Math.round(labelRef.value * rounder)/rounder) : labelRef.value)
        color: colors[settings.annunciatorColor]
        font.pixelSize: parent.height * 0.75
        font.family: b612.name
        anchors.centerIn: parent
    }

    propertiesDialog.helpText: 'Simple label - can show static text and a text dataref value'
    propertiesDialog.propertyItems: [
        Text { text: "Label text" },
        TextField { text: settings.labelText; onTextChanged: settings.labelText = text },
        Text { text: "Dataref" },
        TextField { text: settings.dataref; onTextChanged: settings.dataref = text },
        Text { text: "Color" },
        SettingsUi.ColorSelector {
            colors: label.colors
            value: settings.annunciatorColor
            onValueChanged: settings.annunciatorColor = value},
        Text { text: "Accuracy" },
        TextField { text: settings.accuracy; onTextChanged: settings.accuracy = parseFloat(text) || 0 },
        Text { text: "Round" },
        CheckBox { checked: settings.round ; onCheckedChanged: settings.round = checked },
        Text { text: "Decimals" },
        SettingsUi.IntField { text: settings.decimals; onValueChanged: settings.decimals = value }
    ]

    PanelItems.PanelItemSettings {
        id: settings
        property string labelText: "LABEL"
        property string dataref: ""
        property int annunciatorColor: 0
        property real accuracy: 0.5
        property bool round: true
        property int decimals: 0
    }
}
