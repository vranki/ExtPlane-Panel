import QtQuick 2.0
import org.vranki.extplane 1.0
import "."

PanelItem {
    Text {
        text: "HDG: " + hdgRef.value
        color: "white"

        ClientDataRef {
            id: hdgRef
            name: "sim/cockpit/misc/compass_indicated"
            client: extplaneClient
        }
    }
}
