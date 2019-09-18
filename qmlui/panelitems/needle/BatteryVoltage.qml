import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems
import "../settingsui" as SettingsUi

ElectricGauge {
    propertiesDialog: propertiesDialog
    value: voltageRef.values[settings.batteryNumber] / settings.maxVoltage
    label: "DC Volts"
    labels: ['0', '6', '12', '18', '24']

    DataRef {
        id: voltageRef
        name: "sim/cockpit2/electrical/battery_voltage_indicated_volts"
        accuracy: 0.05
    }

    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        propertyItems: [
            Text { text: "Battery number" },
            SettingsUi.IntField { text: settings.batteryNumber; minValue: 0; maxValue: 7; onValueChanged: settings.batteryNumber = value },
            Text { text: "Max voltage" },
            SettingsUi.IntField { text: settings.maxVoltage; minValue: 1; onValueChanged: settings.maxVoltage = value }
        ]
    }

    PanelItems.PanelItemSettings {
        id: settings

        property int batteryNumber: 0
        property int maxVoltage: 24

        onMaxVoltageChanged: {
            var newLabels = []
            for(var i=0;i<lines;i++) {
                newLabels.push(Math.floor((i/(lines-1)) * maxVoltage))
            }
            labels = newLabels
        }
    }
}
