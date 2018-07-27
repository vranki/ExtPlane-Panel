import QtQuick 2.6
import QtQuick.Window 2.2
import org.vranki.extplane 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("ExtPlane Panel")
    color: "black"
    ExtPlaneClient {
        id: extplaneClient
        simulated: true
    }
    Text {
        color: extplaneClient.extplaneConnection.connected ? "white" : "red"
        text: extplaneClient.connectionMessage + " " + (extplaneClient.extplaneConnection.connected ? "Connected" : extplaneClient.extplaneConnection.networkError)
    }
    Text {
        text: "HDG: " + hdgRef.value
        color: "white"

        ClientDataRef {
            id: hdgRef
            name: "sim/cockpit/misc/compass_indicated"
            client: extplaneClient
        }
        MouseArea {
            anchors.fill: parent
            drag.target: parent
        }
    }
    PaintedPanelItem {
        anchors.centerIn: parent
        width: 300
        height: 100
        MouseArea {
            anchors.fill: parent
            drag.target: parent
        }
    }
}
