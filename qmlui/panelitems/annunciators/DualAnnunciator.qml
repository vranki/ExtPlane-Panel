import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel
import "../settingsui" as SettingsUi

PanelItems.PanelItem {
    id: annunciator
    property var colors: ['red', 'yellow', 'green', 'blue','orange']
    property color textColor: dimmed ? "lightgrey" : active ? "white" : "black"
    property color lightColor: settings.overrideColor=="" ? colors[settings.annunciatorColor] : settings.overrideColor
    property bool active: annunciatorRef.value !== "0"
    property bool dimmed: annunciatorRef.value == "0.5"
    property alias settings: settings
    readonly property bool twoLines: settings.bottomText.length

    DataRef {
        id: annunciatorRef
        name: settings.dataref
    }
    Rectangle {
        anchors.fill: parent
        radius: 2
        color: lightColor
        opacity: dimmed ? 0.2 : active ? 0.7 : 0
        border.color: "#494949"
        border.width: 2
    }
    Rectangle {
        anchors.fill: parent
        radius: 2
        color: lightColor
        opacity: 0.02
    }
    Text { // One line version
        text: settings.topText.length ? settings.topText : "ANN TEXT"
        color: textColor
        font.pixelSize: parent.height * 0.3
        font.family: b612.name
        anchors.centerIn: parent
        visible: !twoLines
    }
    Text { // Two line version
        text: settings.topText.length ? settings.topText : "ANN TEXT"
        color: textColor
        font.pixelSize: parent.height * 0.3
        font.family: b612.name
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height * 0.1
        visible: twoLines
    }
    Text { // Two line version, bottom
        text: settings.bottomText
        color: textColor
        font.pixelSize: parent.height * 0.3
        font.family: b612.name
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.1
        visible: twoLines
    }
    propertiesDialog.helpText: 'Dual state annunciator light with text. Set bottom text for 2 rows.'
    propertiesDialog.propertyItems: [
        Text { text: "Text (top)" },
        TextField { text: settings.topText; onTextChanged: settings.topText = text },
        Text { text: "Text (bottom)" },
        TextField { text: settings.bottomText; onTextChanged: settings.bottomText = text },
        Text { text: "Color" },
        SettingsUi.ColorSelector {
            colors: annunciator.colors
            value: settings.annunciatorColor
            onValueChanged: settings.annunciatorColor = value
        },
        Text { text: "Dataref" },
        TextField { text: settings.dataref; onTextChanged: settings.dataref = text },
        Text { text: "Override Color" },
        TextField { text: settings.overrideColor; onTextChanged: settings.overrideColor = text }
    ]

    PanelItems.PanelItemSettings {
        id: settings
        property string topText: ""
        property string bottomText: ""
        property int annunciatorColor: 0
        property string dataref: ""
        property string overrideColor: ""
    }

    function copySettings(other) {
        console.log(other.settings)
        settings.topText = other.settings.topText
        settings.bottomText = other.settings.bottomText
        settings.annunciatorColor = other.settings.annunciatorColor
        settings.dataref = other.settings.dataref
        settings.overrideColor = other.settings.overrideColor
    }    
}
