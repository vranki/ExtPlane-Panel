import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems

PanelItems.PanelItem {
    propertiesDialog: propertiesDialog

    DataRef {
        id: apuEgtRef
        //name: "sim/cockpit2/electrical/APU_EGT_c"
        name: "x737/ovh/APUGENPanel/APU_EGT"
        accuracy: 1
    }

    CircularGauge {
        id: gauge
        gaugeValue: parseFloat(apuEgtRef.value)
        barValue: 100
        thinBarValue: 25
        barsAngleMin: 200
        valueMax: settings.maxValue
        topLabel: "EGT"
        bottomLabel: "°C x 100"
        valueMultiplier: 100
    }

    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        propertyItems: [
            Text { text: "Max value" },
            TextField { text: settings.maxValue; inputMethodHints: Qt.ImhDigitsOnly; onTextChanged: settings.maxValue = text }
        ]
    }

    PanelItems.PanelItemSettings {
        id: settings
        property int maxValue: 1000
    }
}
