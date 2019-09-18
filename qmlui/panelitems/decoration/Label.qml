import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel

PanelItems.PanelItem {
    propertiesDialog: propertiesDialog

    DataRef {
        id: labelRef
        name: settings.dataref
    }

    Text {
        text: settings.labelText + labelRef.value
        color: "white"
        font.pixelSize: parent.height * 0.75
        font.family: b612.name
        anchors.centerIn: parent
    }

    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        helpText: 'Simple label - can show static text and a text dataref value'
        propertyItems: [
            Text { text: "Label text" },
            TextField { text: settings.labelText; onTextChanged: settings.labelText = text },
            Text { text: "Dataref" },
            TextField { text: settings.dataref; onTextChanged: settings.dataref = text }
        ]
    }

    PanelItems.PanelItemSettings {
        id: settings
        property string labelText: ""
        property string dataref: ""
    }
}
