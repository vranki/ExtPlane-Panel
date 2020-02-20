import QtQuick 2.0
import org.vranki.extplane 1.0
import "needle" as Needle

PanelItem {
    Item {
        width: height
        height: parent.height
        property bool twoMin: false // 2 min = power planes, 1 min = gliders

        DataRef {
            id: turnRef
            name: "sim/flightmodel/misc/turnrate_noroll" // "sim/cockpit2/gauges/indicators/turn_rate_heading_deg_pilot"
            accuracy: 0.05
        }

        Text {
            text: "L"
            color: "white"
            font.pixelSize: parent.height * 0.15
            y: parent.height * 0.2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -parent.width / 4
        }
        Text {
            text: "R"
            color: "white"
            y: parent.height * 0.2
            font.pixelSize: parent.height * 0.15
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: parent.width / 4
        }
        Image {
            source: "turnslip-mark.svg"
            width: height
            height: parent.height * 0.05
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: parent.width * 0.2
            rotation: 20
            y: parent.height * 0.12
        }
        Image {
            source: "turnslip-mark.svg"
            width: height
            height: parent.height * 0.05
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -parent.width * 0.2
            rotation: -20
            y: parent.height * 0.12
        }
        Image {
            source: "turnslip-mark.svg"
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width * 0.1
            height: width
            y: parent.height * 0.07
            rotation: 180
        }
        SlipIndicator {
            width: parent.width * 0.5
            height: width * 0.2
            anchors.horizontalCenter: parent.horizontalCenter
            y: parent.height * 0.6
        }
        Needle.Needle {
            needleType: 3
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
            rotation: limitValue(parseFloat(twoMin ? turnRef.value : turnRef.value / 2), -50, 50)
        }
    }
}
