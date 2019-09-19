import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems
import "../settingsui" as SettingsUi

PanelItems.PanelItem {
    DataRef {
        id: fuelRef
        name: "sim/cockpit2/engine/indicators/fuel_pressure_psi"
        accuracy: 0.1
    }

    CircularGauge {
        id: gauge
        anchors.fill: parent
        gaugeValue: parseFloat(fuelRef.value) || 0
        barValue: 5
        thinBarValue: 1
        barsAngleMin: -45
        barsAngle: 270
        valueMax: settings.maxValue
        topLabel: "FUEL PRESSURE"
        bottomLabel: "PSI"
    }


    propertiesDialog.propertyItems: [
        Text { text: "Maximum value (psi)" },
        SettingsUi.IntField { text: settings.maxValue; minValue: 1; onValueChanged: settings.maxValue = value }
    ]


    PanelItems.PanelItemSettings {
        id: settings
        property int maxValue: 100
    }
}
