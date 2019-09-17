import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems
import "../settingsui" as SettingsUi

PanelItems.PanelItem {
    propertiesDialog: propertiesDialog

    DataRef {
        id: fuelRef
        name: "sim/cockpit2/fuel/fuel_quantity"
        accuracy: 1
    }

    CircularGauge {
        id: gauge
        anchors.fill: parent
        gaugeValue: (parseFloat(fuelRef.values[settings.tankNumber]) / settings.tankCapacity) * 100.0 // Convert to %
        barValue: 20
        thinBarValue: 10
        barsAngleMin: 45
        barsAngle: 90
        valueMax: settings.tankCapacity
        topLabel: "FUEL QTY %"
    }


    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        propertyItems: [
            Text { text: "Tank number" },
            SettingsUi.IntField { text: settings.tankNumber; minValue: 0; maxValue: 8; onValueChanged: settings.tankNumber = value },
            Text { text: "Tank capacity (kg)" },
            SettingsUi.IntField { text: settings.tankCapacity; minValue: 1; onValueChanged: settings.tankCapacity = value }
        ]
    }

    PanelItems.PanelItemSettings {
        id: settings
        property int tankNumber: 0
        property int tankCapacity: 100
    }
}
