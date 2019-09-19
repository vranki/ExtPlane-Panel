import QtQuick 2.0
import org.vranki.extplane 1.0

PanelItem {
    Rectangle {
        width: parent.width
        height: parent.height
        radius: parent.width * 0.05
        border.color: "white"
        color: "black"
        Text {
            text: Math.round(hdgRef.value)
            color: "white"
            font.pixelSize: parent.height * 0.75
            anchors.centerIn: parent

            DataRef {
                id: hdgRef
                name: "sim/cockpit/misc/compass_indicated"
                accuracy: 1
            }
        }
    }
}
