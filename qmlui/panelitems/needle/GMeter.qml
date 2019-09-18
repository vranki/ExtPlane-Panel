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
        id: gRef
        name: "sim/flightmodel/forces/g_nrml"
        accuracy: 0.05
    }

    CircularGauge {
        id: gauge
        anchors.fill: parent
        gaugeValue: parseFloat(gRef.value)
        barValue: 1
        thinBarValue: 0.5
        barsAngleMin: barAngle * valueMin
        valueMin: settings.limitNegative - 1
        valueMax: settings.limitPositive + 1
        topLabel: "ACCELERATION"
        bottomLabel: "G UNITS"
        needleType: 1

        GaugeArc { // Min limit
            anchors.fill: parent
            arcColor: "red"
            z: -10
            startAngle: gauge.value2Angle(settings.limitNegative)
            arcAngle: 0.3
            arcWidth: width * 0.06
        }
        GaugeArc { // Max limit
            anchors.fill: parent
            arcColor: "red"
            z: -10
            startAngle: gauge.value2Angle(settings.limitPositive)
            arcAngle: 0.3
            arcWidth: width * 0.06
        }
    }

    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        propertyItems: [
            Text { text: "Negative limit" },
            SettingsUi.IntField { text: settings.limitNegative; minValue: -20; maxValue: 0; onValueChanged: settings.limitNegative = value },
            Text { text: "Positive limit" },
            SettingsUi.IntField { text: settings.limitPositive; minValue: 1; maxValue: 20; onValueChanged: settings.limitPositive = value }
        ]
    }

    PanelItems.PanelItemSettings {
        id: settings
        property int limitNegative: -4
        property int limitPositive: 6
    }
}
