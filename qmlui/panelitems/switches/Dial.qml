import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel
import "../settingsui" as SettingsUi

PanelItems.PanelItem {
    clip: false

    property int positionCount: settings.positionCount
    property var positionName: [ settings.name0, settings.name1, settings.name2, settings.name3, settings.name4, settings.name5, settings.name6, settings.name7 ]
    property var positionValue: [ settings.value0, settings.value1, settings.value2, settings.value3, settings.value4, settings.value5, settings.value6, settings.value7 ]
    property real positionAngle: settings.positionAngle
    property real positionAngleZero: settings.positionAngleZero
    property int position: value2Position(parseFloat(dialRef.value))
    readonly property real knobRadius: width * 0.3
    property int lineLength: height * 0.1

    DataRef {
        id: dialRef
        name: settings.dataRef
    }
    Repeater {
        model: settings.showLabels ? positionCount : 0
        Rectangle {
            id: positionRect
            property real lineRotation: positionAngleZero + index * positionAngle
            property real offsetSide: lineRotation ? (lineRotation < 0 ? -1 : 1) : 0
            height: lineLength
            width: height * 0.2
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            Text {
                id: textLabel
                text: positionName[index] || index + 1
                color: "white"

                transform: [
                    Translate { x: positionRect.offsetSide * textLabel.width/2 - textLabel.width / 2 },
                    Rotation {
                        angle: -lineRotation
                        origin.x: width / 2
                        origin.y: height / 2
                    },
                    Translate { y: - height }
                ]
            }
            transform: [
                Translate { y: -knobRadius / 2 - height / 2 },
                Rotation {
                    angle: lineRotation
                    origin.x: width / 2
                    origin.y: height / 2
                }
            ]
        }
    }

    Image {
        source: settings.style == 0 ? "dialknob.svg" : settings.style == 1 ? "knob1.svg" : "knob2.svg"
        anchors.centerIn: parent
        width: knobRadius
        height: knobRadius
        rotation: positionAngleZero + position * positionAngle
        MouseArea {
            id: wholeArea
            anchors.fill: parent
            enabled: !editMode & !settings.enableMouseAreas
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onPressed: {
                if(settings.leftCommand == "" & settings.leftCommand == "")
                {
                    if(pressedButtons & Qt.LeftButton) position = Math.max(position - 1, 0)
                    if(pressedButtons & Qt.RightButton) position = Math.min(position + 1, positionCount - 1)

                    if(positionValue[position] !== undefined)
                        dialRef.value = positionValue[position]
                }
                else
                {
                    if(pressedButtons & Qt.LeftButton) extplaneClient.extplaneConnection.commandOnce(settings.leftCommand)
                    if(pressedButtons & Qt.RightButton) extplaneClient.extplaneConnection.commandOnce(settings.rightCommand)
                }
            }
        }
        MouseArea {
            id: leftMouseArea
            enabled: !editMode & settings.enableMouseAreas
            width: parent.width/2
            height: parent.height
            anchors.left: parent.left
            onPressed: {
                if(settings.leftCommand == "" & settings.leftCommand == "")
                {
                    position = Math.max(position - 1, 0)
                    if(positionValue[position] !== undefined)
                        dialRef.value = positionValue[position]
                }
                else
                {
                    extplaneClient.extplaneConnection.commandOnce(settings.leftCommand)
                }
            }
        }
        MouseArea {
            id: reightMouseArea
            enabled: !editMode & settings.enableMouseAreas
            width: parent.width/2
            height: parent.height
            anchors.right: parent.right
            onPressed: {
                if(settings.leftCommand == "" & settings.leftCommand == "")
                {
                    position = Math.min(position + 1, positionCount - 1)
                    if(positionValue[position] !== undefined)
                        dialRef.value = positionValue[position]
                }
                else
                {
                    extplaneClient.extplaneConnection.commandOnce(settings.rightCommand)
                }
            }
        }
    }
    propertiesDialog.helpText: 'A generic dial, can set a dataref to different integer values.'

    propertiesDialog.propertyItems: [
        Text { text: "Dataref" },
        TextField { text: settings.dataRef; onTextChanged: settings.dataRef = text },
        Text { text: "Move Right Command" },
        TextField { text: settings.rightCommand; onTextChanged: settings.rightCommand = text },
        Text { text: "Move Left Command" },
        TextField { text: settings.leftCommand; onTextChanged: settings.leftCommand = text },
        Text { text: "Number of positions" },
        SettingsUi.IntField { text: settings.positionCount; onValueChanged: settings.positionCount = limitValue(value, 2, 8) },
        Text { text: "Label 1" },
        TextField { text: settings.name0; onTextChanged: settings.name0 = text },
        Text { text: "Label 2" },
        TextField { text: settings.name1; onTextChanged: settings.name1 = text },
        Text { text: "Label 3" },
        TextField { text: settings.name2; onTextChanged: settings.name2 = text },
        Text { text: "Label 4" },
        TextField { text: settings.name3; onTextChanged: settings.name3 = text },
        Text { text: "Label 5" },
        TextField { text: settings.name4; onTextChanged: settings.name4 = text },
        Text { text: "Label 6" },
        TextField { text: settings.name5; onTextChanged: settings.name5 = text },
        Text { text: "Label 7" },
        TextField { text: settings.name6; onTextChanged: settings.name6 = text },
        Text { text: "Label 8" },
        TextField { text: settings.name7; onTextChanged: settings.name7 = text },
        Text { text: "Value 1" },
        TextField  { text: settings.value0; onTextChanged: settings.value0 = parseFloat(text) | 0 },
        Text { text: "Value 2" },
        TextField  { text: settings.value1; onTextChanged: settings.value1 = parseFloat(text) | 0 },
        Text { text: "Value 3" },
        TextField  { text: settings.value2; onTextChanged: settings.value2 = parseFloat(text) | 0 },
        Text { text: "Value 4" },
        SettingsUi.IntField { text: settings.value3; onValueChanged: settings.value3 = value },
        Text { text: "Value 5" },
        SettingsUi.IntField { text: settings.value4; onValueChanged: settings.value4 = value },
        Text { text: "Value 6" },
        SettingsUi.IntField { text: settings.value5; onValueChanged: settings.value5 = value },
        Text { text: "Value 7" },
        SettingsUi.IntField { text: settings.value6; onValueChanged: settings.value6 = value },
        Text { text: "Value 8" },
        SettingsUi.IntField { text: settings.value7; onValueChanged: settings.value7 = value },
        Text { text: "Knob rotation" },
        SettingsUi.IntField { text: settings.positionAngle; onValueChanged: settings.positionAngle = value },
        Text { text: "Initial Angle" },
        TextField {text: settings.positionAngleZero; onTextChanged: settings.positionAngleZero = parseFloat(text) || 0 },
        Text { text: "Show Labels" },
        CheckBox { checked: settings.showLabels ; onCheckedChanged: settings.showLabels = checked },
        Text { text: "Style 0 1 2" },
        SettingsUi.IntField { text: settings.style; onValueChanged: settings.style = value },
        Text { text: "Enable mouse areas to use left mouse button" },
        CheckBox { checked: settings.enableMouseAreas ; onCheckedChanged: settings.enableMouseAreas = checked }
    ]

    PanelItems.PanelItemSettings {
        id: settings
        property int positionCount: 3
        property string dataRef: ""
        property string leftCommand: ""
        property string rightCommand: ""
        property string name0: "OFF"
        property string name1: "AUTO"
        property string name2: "ON"
        property string name3: ""
        property string name4: ""
        property string name5: ""
        property string name6: ""
        property string name7: ""
        property real value0: 0
        property real value1: 1
        property real value2: 2
        property int value3: 3
        property int value4: 4
        property int value5: 5
        property int value6: 6
        property int value7: 7
        property real positionAngle: 30
        property real positionAngleZero: -30
        property bool showLabels: true
        property int style: 0
        property bool enableMouseAreas: false
    }
    function value2Position(value)
    {
        for (var i = 0; i < positionValue.length; i++)
            if (positionValue[i] === value) return i

        return 0
    }
}
