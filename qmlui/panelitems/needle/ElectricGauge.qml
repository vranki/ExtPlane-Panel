import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems
import "../settingsui" as SettingsUi

// Generic electrically actuated rectangular needle gauge.
PanelItems.PanelItem {
    property string label: ""
    property real value: 0 // From 0 to 1
    property var labels: ['0', '1/4', '1/2', '3/4', '1/1'] // 5 labels for lines
    readonly property int lines: 5

    Rectangle {
        id: meter
        color: "black"
        border.color: "gray"
        width: parent.width
        height: width * 0.4
        clip: true
        Text {
            text: label
            color: "white"
            font.pixelSize: parent.height * 0.15
            font.family: b612.name
            y: parent.height * 0.50
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Item {
            id: lineRect
            readonly property int lineSpacing: width / lines
            width: parent.width * 0.8
            height: parent.height * 0.2
            anchors.horizontalCenter: parent.horizontalCenter

            Repeater {
                model: lines
                Rectangle {
                    color: "white"
                    height: lineRect.height
                    width: height * 0.2
                    anchors.top: lineRect.top
                    anchors.horizontalCenter: lineRect.horizontalCenter
                    anchors.horizontalCenterOffset: (index - Math.floor(lines / 2)) * lineRect.lineSpacing
                    rotation: (index - Math.floor(lines / 2)) * 10
                    Text {
                        text: labels[index]
                        color: "white"
                        x: -width / 2
                        y: parent.height
                        font.pixelSize: meter.height * 0.15
                        font.family: b612.name
                        rotation: -parent.rotation
                    }
                }
            }
        }
        Needle {
            width: parent.width * 1.2
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.bottom
            anchors.verticalCenterOffset: parent.height * 0.2
            rotation: -37 + limitValue(value, 0, 1) * 73 // Magic values to match lines
        }
    }
}
