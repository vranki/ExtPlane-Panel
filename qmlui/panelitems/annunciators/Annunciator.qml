import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel

PanelItems.PanelItem {
    propertiesDialog: propertiesDialog
    property var colors: ['red', 'yellow', 'green', 'blue']
    property color textColor: active ? "black" : "#494949"
    property color lightColor: colors[settings.annunciatorColor]
    property bool active: annunciatorRef.value === "1"

    readonly property bool twoLines: settings.bottomText.length

    DataRef {
        id: annunciatorRef
        name: settings.dataref
    }
    Rectangle {
        anchors.fill: parent
        radius: 2
        color: active ? colors[settings.annunciatorColor] : "black"
        border.color: "#494949"
        border.width: 2
    }
    Rectangle {
        anchors.fill: parent
        radius: 2
        color: colors[settings.annunciatorColor]
        opacity: 0.02
    }
    Text { // One line version
        text: settings.topText.length ? settings.topText : "ANN TEXT"
        color: textColor
        font.pixelSize: parent.height * 0.3
        anchors.centerIn: parent
        visible: !twoLines
    }
    Text { // Two line version
        text: settings.topText.length ? settings.topText : "ANN TEXT"
        color: textColor
        font.pixelSize: parent.height * 0.3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height * 0.1
        visible: twoLines
    }
    Text { // Two line version, bottom
        text: settings.bottomText
        color: textColor
        font.pixelSize: parent.height * 0.3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.1
        visible: twoLines
    }
    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        helpText: 'Generic annunciator light with text. Set bottom text for 2 rows.'
        propertyItems: [
            Text { text: "Text (top)" },
            TextField { text: settings.topText; onTextChanged: settings.topText = text },
            Text { text: "Text (bottom)" },
            TextField { text: settings.bottomText; onTextChanged: settings.bottomText = text },
            Text { text: "Color" },
            SpinBox {
                value: settings.annunciatorColor;
                onValueChanged: settings.annunciatorColor = limitValue(value, 0, colors.length - 1)
                background: Rectangle { color: lightColor; opacity: 0.5; anchors.fill: parent }
            },
            Text { text: "Dataref" },
            TextField { text: settings.dataref; onTextChanged: settings.dataref = text }
        ]
    }

    PanelItems.PanelItemSettings {
        id: settings
        property string topText: ""
        property string bottomText: ""
        property int annunciatorColor: 0
        property string dataref: ""
    }
}
