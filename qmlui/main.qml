import QtQuick 2.6
import QtQuick.Window 2.2
import org.vranki.extplane 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("ExtPlane Panel")
    color: "black"
    ExtPlaneConnection {
        id: extplaneConnection
        Component.onCompleted: startConnection()
    }
    Text {
        color: "white"
        text: extplaneConnection.connected ? "Connected" : "Not connected"
    }
}
