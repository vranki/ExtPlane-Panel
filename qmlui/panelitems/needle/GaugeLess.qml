import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems

PanelItems.PanelItem {
    DataRef {
        id: gaugeLessRef
        name: settings.dataref
        accuracy: settings.accuracy
    }

    CircularGauge {
        id: gauge
        anchors.fill: parent
        gaugeValue: parseFloat(gaugeLessRef.value) || 0
        barValue: 20
        thinBarValue: 5
        barsAngle: settings.barsAngle
        barsAngleMin: settings.barsAngleMin
        valueMin: settings.valueMin
        valueMax: settings.valueMax
        topLabel: ""
        bottomLabel: ""
        showGauge: parent.editMode
    }

    propertiesDialog.propertyItems: [
        Text { text: "Dataref" },
        TextField { text: settings.dataref; onTextChanged: settings.dataref = text },
        Text { text: "Lower angle" },
        TextField { text: settings.barsAngleMin; onTextChanged: settings.barsAngleMin = parseFloat(text) || 0 },
        Text { text: "Gauge angle" },
        TextField { text: settings.barsAngle; onTextChanged: settings.barsAngle = parseFloat(text) || 0 },
        Text { text: "Min. Value" },
        TextField { text: settings.valueMin; onTextChanged: settings.valueMin = parseFloat(text) || 0 },
        Text { text: "Max. Value" },
        TextField { text: settings.valueMax; onTextChanged: settings.valueMax = parseFloat(text) || 0 },
        Text { text: "Accuracy" },
        TextField { text: settings.accuracy; onTextChanged: settings.accuracy = parseFloat(text) || 0 }
    ]

    PanelItems.PanelItemSettings {
        id: settings
        property string dataref: ""
        property real barsAngle: 90
        property real barsAngleMin: -30
        property real valueMin: 0
        property real valueMax: 100
        property real accuracy: 0.5
    }
}
