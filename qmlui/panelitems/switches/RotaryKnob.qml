import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel

PanelItems.PanelItem {
    propertiesDialog: propertiesDialog
    clip: false

    property real rotationAngle: (parseFloat(rotaryRef.value) || 0)  * settings.rotationScale

    Rectangle {
        id: knob
        width: parent.width
        color: "grey"
        height: width
        radius: width / 2
        border.color: "darkgrey"
        border.width: width * 0.05
        Repeater {
            model: 10
            Rectangle {
                anchors.centerIn: knob
                width: knob.width / 10
                height: width
                radius: width/2
                color: "black"
                opacity: 0.2
                transform: [
                    Translate {
                        y: -knob.width*0.49 + height / 2
                        x: -width/2
                    },
                    Rotation {
                        angle: rotationAngle + 36 * index
                        origin.x: width / 2
                        origin.y: width / 2
                    }
                ]
            }
        }
    }
    MouseArea {
        id: knobMouseArea
        enabled: !editMode
        anchors.fill: knob
        onWheel: {
            var rotationChange = settings.changeValue * (wheel.angleDelta.y / 120)
            rotaryRef.value = parseFloat(rotaryRef.value) + rotationChange
        }
    }
    DataRef {
        id: rotaryRef
        name: settings.dataref
    }
    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        helpText: 'Knob you can rotate using mouse. Changes dataref up or down by defined value.';
        propertyItems: [
            Text { text: "Change amount" },
            TextField { text: settings.changeValue; onTextChanged: settings.changeValue = parseFloat(text) || 1 },
            Text { text: "Scale between dial angle and dataref value" },
            TextField { text: settings.rotationScale; onTextChanged: settings.rotationScale = parseFloat(text) || 1 },
            Text { text: "Dataref" },
            TextField { text: settings.dataref; onTextChanged: settings.dataref = text }
        ]
    }

    PanelItems.PanelItemSettings {
        id: settings
        property string dataref: ""
        property real changeValue: 1
        property real rotationScale: 1
    }
}
