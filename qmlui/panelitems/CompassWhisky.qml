import QtQuick 2.0
import org.vranki.extplane 1.0

PanelItem {
    id: compass
    property var labels: ['N', '', '', '3', '', '', '6', '', '', 'E', '', '', '12', '', '', '15', '', '', 'S', '', '', '21', '', '', '24', '', '', 'W', '', '', '30', '', '', '33', '', '']
    property int tapeWidth: 500
    property int tapeHeight: 30
    Component {
        id: tape
        Item {
            id: compassTape
            width: tapeWidth
            height: tapeHeight
            Repeater {
                model: 36
                Item {
                    width: 1
                    height: tapeHeight
                    x: tapeWidth - tapeWidth * (index/36)
                    Rectangle {
                        color: "white"
                        width: 2
                        height: tapeHeight * 0.2
                        y: tapeHeight - height
                    }
                    Text {
                        text: labels[index]
                        color: "white"
                        font.pixelSize: tapeHeight * 0.7
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
    }
    Rectangle {
        x: parent.width / 2
        scale: parent.height / 30
        Loader {
            id: primaryTape
            sourceComponent: tape
            x: - (360 - hdgRef.value) * (tapeWidth/360)
        }
        Loader {
            sourceComponent: tape
            x: primaryTape.x + ((hdgRef.value > 180) ? -tapeWidth : tapeWidth)
        }
    }
    Rectangle {
        color: "white"
        width: 3
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
    }
    DataRef {
        id: hdgRef
        name: "sim/cockpit/misc/compass_indicated"
        accuracy: 0.5
    }
}
