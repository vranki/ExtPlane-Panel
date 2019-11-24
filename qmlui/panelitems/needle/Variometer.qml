import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems

PanelItems.PanelItem {
    clip: false

    Panel.UnitConverter {
        id: unitConverter
        inUnit: uVelocityFPM
        outUnit: settings.isMs ? uVelocityMS : uVelocityFPM
    }
    Panel.UnitConverter {
        id: laminarUnitConverter
        inUnit: uVelocityMS
        outUnit: settings.isMs ? uVelocityMS : uVelocityFPM
    }

    DataRef {
        id: variometerRef
        name: settings.isTotalEnergy ? "" : "sim/flightmodel/position/vh_ind_fpm"
        accuracy: 10
        scaleFactor: unitConverter.scaleFactor
    }

    DataRef { // Laminar ASK-21 dataref, in m/s
        id: laminarVariometerRef
        name: settings.isLaminarAsk && settings.isTotalEnergy ? "laminar/ask21/vvi_ms_compensated" : ""
        accuracy: 0.01
        scaleFactor: laminarUnitConverter.scaleFactor
    }

    DataRef {
        id: totalEnergyRef
        accuracy: 10
        name: (settings.isTotalEnergy && !settings.isLaminarAsk) ? "sim/cockpit2/gauges/indicators/total_energy_fpm" : ""
        scaleFactor: unitConverter.scaleFactor
    }

    CircularGaugeBars {
        id: valueBars

        thickBars: true
        showValue: true
        absValue: true
        fontSizeMultiplier: 1.3
        barValue: settings.isMs ? 1 : 200
        valueMax: settings.isMs ? 5 : 1000
        valueMin: -valueMax
        barsAngleMin: -135
        limitAngle: true

        Text {
            text: unitConverter.outUnitName
            color: "white"
            font.pixelSize: parent.height * 0.05
            font.family: b612.name
            y: parent.height * 0.70
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Needle {
            rotation: valueBars.value2Angle(settings.isTotalEnergy ? ( settings.isLaminarAsk ? laminarVariometerRef.value : totalEnergyRef.value) : variometerRef.value)
            movementDuration: 32
        }
        Image {
            source: "overlay-0.svg"
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
        }
    }
    propertiesDialog.propertyItems: [
        Text { text: "In m/s, instead of knots" },
        CheckBox { checked: settings.isMs ; onCheckedChanged: settings.isMs = checked },
        Text { text: "Total energy variometer" },
        CheckBox { checked: settings.isTotalEnergy ; onCheckedChanged: settings.isTotalEnergy = checked },
        Text { text: "Use Laminar ASK-21 custom dataref" },
        CheckBox {
            checked: settings.isLaminarAsk
            onCheckedChanged: {
                settings.isLaminarAsk = checked
                if(checked) settings.isTotalEnergy = true
            }
        }
    ]

    PanelItems.PanelItemSettings {
        id: settings
        property bool isMs: false
        property bool isTotalEnergy: false
        property bool isLaminarAsk: false
    }
}
