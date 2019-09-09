import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12
import org.vranki.extplane 1.0
import ".." as PanelItems
import "../.." as Panel

PanelItems.PanelItem {
    property int rowCount: 16
    property int columnCount: 24
    property int rowHeight: height / (rowCount)
    property int charWidth: width / (columnCount)

    GridLayout {
        anchors.fill: parent
        Layout.margins: 0
        columns: columnCount
        rowSpacing: 0
        columnSpacing: 0
        Repeater {
            id: refRepeater
            model: rowCount * columnCount

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 0

            Rectangle {
                property int row: index / columnCount
                property int column: index - (row * columnCount)

                color: charText.reverseVideo ? charText.mainColor : "black"
                Layout.preferredHeight: rowHeight
                Layout.preferredWidth: charWidth
                Layout.margins: 0

                Text {
                    id: charText
                    property string character: rowRepeater.itemAt(row).rowRef.value[column] || ""
                    property int charStyle: rowRepeater.itemAt(row).formatRef.values[column] ? rowRepeater.itemAt(row).formatRef.values[column].charCodeAt(0) : 0
                    property bool smallText: !(charStyle & (1 << 7))
                    property bool reverseVideo: (charStyle & (1 << 6))
                    property bool underline: (charStyle & (1 << 4))
                    property bool blink: (charStyle & (1 << 5))
                    property int colorBits: charStyle & 7
                    property var colors: ['black', 'cyan', 'red', 'yellow', 'green', 'magenta', 'amber', 'white']
                    property color mainColor: colors[colorBits]
                    text: character
                    font.pixelSize: rowHeight
                    height: rowHeight
                    anchors.centerIn: parent
                    visible: blink ? blinkTimer.blinkOn : true

                    transform: Scale {
                        yScale: charText.smallText ? 0.5 : 1
                        origin.x: charText.width / 2
                        origin.y: charText.height / 2
                    }
                    font.family: "Monospace"
                    color: charText.reverseVideo ? "black" : mainColor

                    Rectangle { // Underline bar
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: parent.height * 0.05
                        color: charText.color
                        visible: charText.underline && charText.character
                    }
                    Timer {
                        id: blinkTimer
                        property bool blinkOn: true
                        running: charText.blink
                        interval: 500
                        repeat: true
                        onTriggered: blinkOn = !blinkOn
                    }
                }
            }
        }
    }
    Repeater {
        id: rowRepeater
        model: rowCount
        Item {
            property alias rowRef: rowRef
            property alias formatRef: formatRef
            DataRef {
                id: rowRef
                name: "sim/cockpit2/radios/indicators/fms_cdu1_text_line" + index
            }
            DataRef {
                id: formatRef
                name: "sim/cockpit2/radios/indicators/fms_cdu1_style_line" + index
                dataFormat: "binary"
            }
        }
    }
}
