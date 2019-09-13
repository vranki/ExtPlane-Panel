import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems

PanelItems.PanelItem {
    DataRef {
        id: fuelTempRef
        name: "x737/ovh/fuelPanel/fuelTemp"
        accuracy: 1
    }

    CircularGauge {
        id: gauge
        anchors.fill: parent
        gaugeValue: parseFloat(fuelTempRef.value)
        barValue: 20
        thinBarValue: 5
        barsAngleMin: -45
        valueMin: -60
        valueMax: 60
        topLabel: "FUEL TEMP"
        bottomLabel: "°C"
    }
}
