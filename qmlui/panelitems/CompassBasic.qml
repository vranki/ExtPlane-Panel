import QtQuick 2.0
import org.vranki.extplane 1.0

PanelItem {
    Text {
        text: Math.round(hdgRef.value)
        color: "white"
        font.pixelSize: parent.height * 0.75
        anchors.centerIn: parent

        DataRef {
            id: hdgRef
            name: "sim/cockpit/misc/compass_indicated"
        }
    }
    Rectangle {
        anchors.fill: parent
        radius: 5
        border.color: "white"
        color: "transparent"
    }
}
