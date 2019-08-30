import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems

PanelItems.PanelItem {
    propertiesDialog: propertiesDialog

    DataRef {
        id: airspeedRef
        name: "sim/cockpit2/gauges/indicators/airspeed_kts_pilot"
    }

    DataRef {
        id: vneRef
        name: "sim/aircraft/view/acf_Vne"
    }

    CircularGaugeBars {
        id: valueBars

        thickBars: true
        showValue: true
        barValue: settings.thickBarValue
        valueMax: vneRef.value || 100
        CircularGaugeBars {
            barValue: settings.thinBarValue
            valueMax: valueBars.valueMax
        }
        Needle {
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
