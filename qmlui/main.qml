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
        datarefProvider: extplaneConnection
    }
    ExtPlaneConnection {
        id: extplaneConnection
        hostName: "localhost"
        Component.onCompleted: startConnection()
    }
    Text {
        color: extplaneConnection.connected ? "white" : "red"
        text: (extplaneConnection.connected ? "Connected" : extplaneConnection.networkError)
    }
    Text {
        anchors.centerIn: parent
        text: hdgRef.value

        ClientDataRef {
            id: hdgRef
            name: "sim/cockpit/misc/compass_indicated"
            client: extplaneClient
        }
    }
}
