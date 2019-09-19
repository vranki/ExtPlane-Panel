import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel
import "../settingsui" as SettingsUi

PanelItems.PanelItem {
    property color frontColor: buttonMouseArea.pressed ? "black" : "white"
    property color backColor: buttonMouseArea.pressed ? "white" : "black"

    Rectangle {
        anchors.fill: parent
        radius: 5
        border.color: frontColor
        color: backColor
    }
    Text {
        text: settings.buttonText.length ? settings.buttonText : "Button text"
        color: frontColor
        font.pixelSize: parent.height * 0.75
        font.family: b612.name
        anchors.centerIn: parent
    }

    MouseArea {
        id: buttonMouseArea
        enabled: !editMode
        anchors.fill: parent
        onPressed: settings.isButton ? extplaneClient.extplaneConnection.buttonPress(settings.buttonId) : extplaneClient.extplaneConnection.keyPress(settings.buttonId)
        onReleased: if(settings.isButton) extplaneClient.extplaneConnection.buttonRelease(settings.buttonId)
    }
    propertiesDialog.helpText: 'Can act as X-Plane key or button command. \
See <a href="http://www.xsquawkbox.net/xpsdk/mediawiki/Category:XPLMUtilities">this page</a> for list of \
available button and key ID\'s. Buttons support press and release, keys only press.'
    propertiesDialog.propertyItems: [
        Text { text: "Button (not key)" },
        CheckBox { checked: settings.isButton ; onCheckedChanged: settings.isButton = checked },

        Text { text: "Button text" },
        TextField { text: settings.buttonText; onTextChanged: settings.buttonText = text },

        Text { text: "Button/key id" },
        SettingsUi.IntField { value: settings.buttonId; onValueChanged: settings.buttonId = value }
    ]

    PanelItems.PanelItemSettings {
        id: settings
        property bool isButton: false
        property string buttonText: ""
        property int buttonId: 0
    }
}
