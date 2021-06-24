import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel
import "../settingsui" as SettingsUi

PanelItems.PanelItem {
    clip: false
    property bool isOn: settings.dataref === "" ? switchRef.values[settings.index] === "1" : switchRef.value === "1"

    Image {
        id: switchImage
        source: settings.red ? isOn ? "rcap-open.svg" : "rcap_closed.svg" :
                               isOn ? "bcap-open.svg" : "bcap-closed.svg"
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        rotation: settings.rotation
    }

    MouseArea {
        id: buttonMouseArea
        acceptedButtons: Qt.RightButton
        propagateComposedEvents: true
        enabled: !editMode
        anchors.fill: switchImage
        onClicked: extplaneClient.extplaneConnection.commandOnce(settings.command)
    }
    DataRef {
        id: switchRef
        name: settings.dataref== "" ? "laminar/B738/button_switch/cover_position" : settings.dataref
    }
    propertiesDialog.helpText: 'A B738 generic cap switch, controlled by an array or a concrete dataref ';
    propertiesDialog.propertyItems: [
        Text { text: "Index" },
        SettingsUi.IntField { text: settings.index; onValueChanged: settings.index = limitValue(value, 0, 10) },
        Text { text: "Dataref for non generic caps" },
        TextField { text: settings.dataref; onTextChanged: settings.dataref = text },
        Text { text: "Toggle Cap Command" },
        TextField { text: settings.command; onTextChanged: settings.command = text },
        Text { text: "Angle" },
        TextField { text: settings.rotation; onTextChanged: settings.rotation = parseFloat(text) | 0 },
        Text { text: "Red Cap" },
        CheckBox { checked: settings.red ; onCheckedChanged: settings.red = checked }
    ]

    PanelItems.PanelItemSettings {
        id: settings
        property string command: "laminar/B738/button_switch_cover00"
        property string dataref: ""
        property int index: 0
        property real rotation: 0
        property bool red: false
    }
}
