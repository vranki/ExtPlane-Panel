import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel

PanelItems.PanelItem {
    clip: false
    property color frontColor: "white"
    property color backColor: "black"
    property int position: switchRef.value == settings.upValue ? 0 : switchRef.value == settings.centerValue ? 1 : 2

    Image {
        property var positionImages: ["switch-up.svg", "switch-center.svg", "switch-down.svg"]
        property int positionImageIndex: limitValue(position, 0, positionImages.length - 1)
        id: switchImage
        source: positionImages[positionImageIndex]
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        rotation: parseInt(settings.rotation)
        MouseArea {
            id: topMouseArea
            enabled: !editMode
            width: parent.width
            height: parent.height/3
            onPressed: settings.upCommand == "" ? switchRef.setValue(settings.upValue) : extplaneClient.extplaneConnection.commandBegin(settings.upCommand)
            onReleased: if (settings.upCommand == "")
                        {
                            if(settings.upReturns) switchRef.setValue(settings.centerValue)
                        }
                        else
                            extplaneClient.extplaneConnection.commandEnd(settings.upCommand)
        }
        MouseArea {
            id: centerMouseArea
            enabled: !editMode
            width: parent.width
            height: parent.height/3
            anchors.centerIn: parent
            onPressed: settings.downCommand == "" ? switchRef.setValue(settings.centerValue) : position == 0 ?
                          extplaneClient.extplaneConnection.commandOnce(settings.downCommand) : extplaneClient.extplaneConnection.commandOnce(settings.upCommand)
        }
        MouseArea {
            id: bottomMouseArea
            enabled: !editMode
            width: parent.width
            height: parent.height/3
            anchors.bottom: parent.bottom
            onPressed: settings.downCommand == "" ? switchRef.setValue(settings.downValue) : extplaneClient.extplaneConnection.commandBegin(settings.downCommand)
            onReleased: if (settings.downCommand == "")
                        {
                            if(settings.downReturns) switchRef.setValue(settings.centerValue)
                        }
                        else
                            extplaneClient.extplaneConnection.commandEnd(settings.downCommand)
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
    propertiesDialog.helpText: 'A generic 3-way switch, can set a dataref arbitrary values (defaults are 0, 1 or 2). \
You can set up/down positions to return to center when released.'
    propertiesDialog.propertyItems: [
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
        Text { text: "Move Up Command" },
        TextField { text: settings.upCommand; onTextChanged: settings.upCommand = text },
        Text { text: "Move Down Command" },
        TextField { text: settings.downCommand; onTextChanged: settings.downCommand = text },
        Text { text: "Up returns" },
        CheckBox { checked: settings.upReturns ; onCheckedChanged: settings.upReturns = checked },
        Text { text: "Down returns" },
        CheckBox { checked: settings.downReturns ; onCheckedChanged: settings.downReturns = checked },
        Text { text: "Angle" },
        TextField { text: settings.rotation; onTextChanged: settings.rotation = text },
        Text { text: "Up value" },
        TextField { text: settings.upValue ; onTextChanged: settings.upValue = text },
        Text { text: "Center value" },
        TextField { text: settings.centerValue ; onTextChanged: settings.centerValue = text },
        Text { text: "Down value" },
        TextField { text: settings.downValue ; onTextChanged: settings.downValue = text }
    ]

    PanelItems.PanelItemSettings {
        id: settings
        property string labelText: ""
        property string upText: ""
        property string centerText: ""
        property string downText: ""
        property string dataref: ""
        property bool upReturns: false
        property bool downReturns: false
         property string rotation: "0"
        property string upValue: "0"
        property string centerValue: "1"
        property string downValue: "2"
        property string upCommand: ""
        property string downCommand: ""
    }
}
