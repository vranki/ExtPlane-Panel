import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems

PanelItems.PanelItem {
    propertiesDialog: propertiesDialog
    clip: false

    Panel.UnitConverter {
        id: unitConverter
        inUnit: Panel.UnitConverter.Unit.VelocityFPM
        outUnit: settings.isMs ? Panel.UnitConverter.Unit.VelocityMS : Panel.UnitConverter.Unit.VelocityFPM
    }

    DataRef {
        id: variometerRef
        name: settings.isTotalEnergy ? "" : "sim/flightmodel/position/vh_ind_fpm"
        accuracy: 10
        scaleFactor: unitConverter.scaleFactor
    }

    DataRef {
        id: totalEnergyRef
        accuracy: 10 // Todo: there's laminar/ask21/vvi_ms_compensated
        name: settings.isTotalEnergy ? "sim/cockpit2/gauges/indicators/total_energy_fpm" : ""
        scaleFactor: unitConverter.scaleFactor
    }

    CircularGaugeBars {
        id: valueBars

        thickBars: true
        showValue: true
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
            rotation: valueBars.value2Angle(settings.isTotalEnergy ? totalEnergyRef.value : variometerRef.value)
            movementDuration: 16
        }
        Image {
            source: "overlay-0.svg"
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
        }
    }
    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        propertyItems: [
            Text { text: "In m/s, instead of knots" },
            CheckBox { checked: settings.isMs ; onCheckedChanged: settings.isMs = checked },
            Text { text: "Total energy variometer" },
            CheckBox { checked: settings.isTotalEnergy ; onCheckedChanged: settings.isTotalEnergy = checked }
        ]
    }

    PanelItems.PanelItemSettings {
        id: settings
        property bool isMs: false
        property bool isTotalEnergy: false
    }
}
