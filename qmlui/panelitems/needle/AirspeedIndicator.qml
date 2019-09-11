import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems

PanelItems.PanelItem {
    propertiesDialog: propertiesDialog
    clip: false
    property real arcWidth: width * 0.03 // For main color arches

    DataRef {
        id: airspeedRef
        name: "sim/cockpit2/gauges/indicators/airspeed_kts_pilot"
    }

    DataRef {
        id: vneRef
        name: "sim/aircraft/view/acf_Vne"
    }
    DataRef {
        id: vsRef
        name: "sim/aircraft/view/acf_Vs"
    }
    DataRef {
        id: vnoRef
        name: "sim/aircraft/view/acf_Vno"
    }
    DataRef {
        id: vsoRef
        name: "sim/aircraft/view/acf_Vso"
    }
    DataRef {
        id: vfeRef
        name: "sim/aircraft/view/acf_Vfe"
    }

    CircularGaugeBars {
        id: valueBars

        thickBars: true
        showValue: true
        barValue: settings.thickBarValue
        valueMax: vneRef.value || 100

        GaugeArc { // Vne bar
            anchors.fill: parent
            arcColor: "red"
            z: -10
            startAngle: valueBars.value2Angle(vneRef.value)
            arcAngle: 0.3
            arcWidth: width * 0.06
        }
        GaugeArc { // Yellow arc
            anchors.fill: parent
            arcColor: "yellow"
            z: -20
            startAngle: valueBars.value2Angle(vnoRef.value)
            arcAngle: valueBars.value2Angle(vneRef.value) - startAngle
            arcWidth: arcWidth
        }
        GaugeArc { // Vno arc
            anchors.fill: parent
            arcColor: "green"
            z: -20
            startAngle: valueBars.value2Angle(vsRef.value)
            arcAngle: valueBars.value2Angle(vnoRef.value) -startAngle
            arcWidth: arcWidth
        }
        GaugeArc { // White flap arc
            anchors.fill: parent
            radius: width / 2 - 2*arcWidth
            arcColor: "white"
            z: -30
            startAngle: valueBars.value2Angle(vsoRef.value)
            arcAngle: valueBars.value2Angle(vfeRef.value) - startAngle
            arcWidth: arcWidth
        }
        CircularGaugeBars {
            barValue: settings.thinBarValue
            valueMax: valueBars.valueMax
        }
        Needle {
            needleType: 1
            rotation: valueBars.value2Angle(airspeedRef.value)
        }
    }
    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        propertyItems: [
            Text { text: "Thick bars every" },
            TextField { text: settings.thickBarValue; inputMethodHints: Qt.ImhDigitsOnly; onTextChanged: settings.thickBarValue = text },

            Text { text: "Thin bars every" },
            TextField { text: settings.thinBarValue; inputMethodHints: Qt.ImhDigitsOnly; onTextChanged: settings.thinBarValue = text }
        ]
    }

    PanelItems.PanelItemSettings {
        id: settings
        property int thickBarValue: 20
        property int thinBarValue: 5
    }
}
