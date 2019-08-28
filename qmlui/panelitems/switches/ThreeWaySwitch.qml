import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel

PanelItems.PanelItem {
    propertiesDialog: propertiesDialog
    clip: false
    property color frontColor: "white"
    property color backColor: "black"
    property int position: parseInt(switchRef.value) || 0

    Image {
        property var positionImages: ["switch-up.svg", "switch-center.svg", "switch-down.svg"]
        property int positionImageIndex: Math.max(Math.min(position, 2), 0)
        id: switchImage
        source: positionImages[positionImageIndex]
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        MouseArea {
            id: topMouseArea
            enabled: !editMode
            width: parent.width
            height: parent.height/3
            onPressed: switchRef.setValue("0")
            onReleased: if(settings.upReturns) switchRef.setValue("1")
        }
        MouseArea {
            id: centerMouseArea
            enabled: !editMode
            width: parent.width
            height: parent.height/3
            anchors.centerIn: parent
            onPressed: switchRef.setValue("1")
        }
        MouseArea {
            id: bottomMouseArea
            enabled: !editMode
            width: parent.width
            height: parent.height/3
            anchors.bottom: parent.bottom
            onPressed: switchRef.setValue("2")
            onReleased: if(settings.downReturns) switchRef.setValue("1")
        }
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
        text: settings.centerText
        color: frontColor
        font.pixelSize: parent.height * 0.15
        anchors.left: switchImage.horizontalCenter
        anchors.leftMargin: switchImage.paintedWidth / 2
        anchors.verticalCenter: switchImage.verticalCenter
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
    DataRef {
        id: switchRef
        name: settings.dataref
    }
    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        helpText: 'A generic 3-way switch, can set a dataref 0, 1 or 2. \
You can set up/down positions to return to center when released.'
        propertyItems: [
            Text { text: "Label text" },
            TextField { text: settings.labelText; onTextChanged: settings.labelText = text },
            Text { text: "Up text" },
            TextField { text: settings.upText; onTextChanged: settings.upText = text },
            Text { text: "Center text" },
            TextField { text: settings.centerText; onTextChanged: settings.centerText = text },
            Text { text: "Down text" },
            TextField { text: settings.downText; onTextChanged: settings.downText = text },
            Text { text: "Dataref" },
            TextField { text: settings.dataref; onTextChanged: settings.dataref = text },
            Text { text: "Up returns" },
            CheckBox { checked: settings.upReturns ; onCheckedChanged: settings.upReturns = checked },
            Text { text: "Down returns" },
            CheckBox { checked: settings.downReturns ; onCheckedChanged: settings.downReturns = checked }
        ]
    }

    PanelItems.PanelItemSettings {
        id: settings
        property string labelText: ""
        property string upText: ""
        property string centerText: ""
        property string downText: ""
        property string dataref: ""
        property bool upReturns: false
        property bool downReturns: false
    }
}
