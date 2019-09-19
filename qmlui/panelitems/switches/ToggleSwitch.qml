import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel

PanelItems.PanelItem {
    clip: false
    property color frontColor: "white"
    property color backColor: "black"
    property bool isOn: switchRef.value == "1"

    Image {
        id: switchImage
        source: (settings.invert ? !isOn : isOn) ? "switch-up.svg" : "switch-down.svg"
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }
    Text {
        text: settings.labelText
        color: frontColor
        font.pixelSize: parent.height * 0.2
        anchors.horizontalCenter: switchImage.horizontalCenter
        anchors.bottom: switchImage.top
    }
    Text {
        text: settings.upText
        color: frontColor
        font.pixelSize: parent.height * 0.15
        anchors.left: switchImage.horizontalCenter
        anchors.leftMargin: switchImage.paintedWidth / 2
        anchors.bottom: switchImage.verticalCenter
        anchors.bottomMargin: parent.height * 0.2
    }
    Text {
        text: settings.downText
        color: frontColor
        font.pixelSize: parent.height * 0.15
        anchors.left: switchImage.horizontalCenter
        anchors.leftMargin: switchImage.paintedWidth / 2
        anchors.top: switchImage.verticalCenter
        anchors.topMargin: parent.height * 0.2
    }

    MouseArea {
        id: buttonMouseArea
        enabled: !editMode
        anchors.fill: switchImage
        onClicked: switchRef.setValue(isOn ? "0" : "1")
    }
    DataRef {
        id: switchRef
        name: settings.dataref
    }
    propertiesDialog.helpText: 'A generic toggle switch, can set a dataref 1 or 0. \
Check invert if you want to have 1 down and 0 up';
    propertiesDialog.propertyItems: [
        Text { text: "Label text" },
        TextField { text: settings.labelText; onTextChanged: settings.labelText = text },
        Text { text: "Up text" },
        TextField { text: settings.upText; onTextChanged: settings.upText = text },
        Text { text: "Down text" },
        TextField { text: settings.downText; onTextChanged: settings.downText = text },
        Text { text: "Dataref" },
        TextField { text: settings.dataref; onTextChanged: settings.dataref = text },
        Text { text: "Invert" },
        CheckBox { checked: settings.invert ; onCheckedChanged: settings.invert = checked }
    ]


    PanelItems.PanelItemSettings {
        id: settings
        property string labelText: ""
        property string upText: ""
        property string downText: ""
        property string dataref: ""
        property bool invert: false
    }
}
