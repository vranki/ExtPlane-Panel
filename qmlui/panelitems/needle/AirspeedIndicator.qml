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

    Panel.UnitConverter {
        id: unitConverter
        inUnit: Panel.UnitConverter.Unit.VelocityKnots
        outUnit: settings.isKmh ? Panel.UnitConverter.Unit.VelocityKMH : Panel.UnitConverter.Unit.VelocityKnots
    }

    DataRef {
        id: airspeedRef
        name: "sim/cockpit2/gauges/indicators/airspeed_kts_pilot"
        accuracy: 0.1
        scaleFactor: unitConverter.scaleFactor
    }

    DataRef {
        id: vneRef
        name: "sim/aircraft/view/acf_Vne"
        scaleFactor: unitConverter.scaleFactor
    }
    DataRef {
        id: vsRef
        name: "sim/aircraft/view/acf_Vs"
        scaleFactor: unitConverter.scaleFactor
    }
    DataRef {
        id: vnoRef
        name: "sim/aircraft/view/acf_Vno"
        scaleFactor: unitConverter.scaleFactor
    }
    DataRef {
        id: vsoRef
        name: "sim/aircraft/view/acf_Vso"
        scaleFactor: unitConverter.scaleFactor
    }
    DataRef {
        id: vfeRef
        name: "sim/aircraft/view/acf_Vfe"
        scaleFactor: unitConverter.scaleFactor
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
            // At least ASK-21 has vfe > vne
            visible: parseInt(vfeRef.value) < parseInt(vneRef.value)
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
        Text {
            text: unitConverter.outUnitName
            color: "white"
            font.pixelSize: parent.height * 0.08
            font.family: b612.name
            y: parent.height * 0.60
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Needle {
            needleType: 1
            rotation: valueBars.value2Angle(airspeedRef.value)
            Behavior on rotation { PropertyAnimation {
                    easing.type: Easing.InOutQuad
                    duration: 250
                }
            }
        }
    }
    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        propertyItems: [
            Text { text: "In km/h, instead of knots" },
            CheckBox { checked: settings.isKmh ; onCheckedChanged: settings.isKmh = checked },

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
        property bool isKmh: false
    }
}
