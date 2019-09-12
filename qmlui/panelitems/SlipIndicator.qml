import QtQuick 2.0
import org.vranki.extplane 1.0

PanelItem {
    property real slipRange: 10
    DataRef {
        id: slipRef
        name: "sim/cockpit2/gauges/indicators/slip_deg"
        accuracy: 0.1
    }
    Rectangle {
        anchors.fill: parent
        radius: height / 4
        gradient: Gradient {
            GradientStop {position: 0.0; color: "gray"}
            GradientStop {position: 0.5; color: "white"}
            GradientStop {position: 1.0; color: "gray"}
        }
    }
    Rectangle {
        height: parent.height
        width: height
        radius: width / 2
        color: "black"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: limitValue(-slipRef.value / slipRange, -1, 1) * (parent.width - width) / 2
    }
    Item {
        height: parent.height
        width: height
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            width: height * 0.1
            height: parent.height
            color: "black"
            opacity: 0.3
        }
        Rectangle {
            width: height * 0.1
            height: parent.height
            color: "black"
            opacity: 0.3
            anchors.right: parent.right
        }
    }
}
