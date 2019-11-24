import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems

PanelItems.PanelItem {
    property int maxValue: (redlineRef.value  * 1.3) || 4500

    DataRef {
        id: rpmRef
        name: "sim/cockpit2/engine/indicators/engine_speed_rpm"
        accuracy: 10
    }
    DataRef {
        id: redlineRef
        name: "sim/aircraft/engine/acf_RSC_redline_eng"
        scaleFactor: radianConverter.scaleFactor
    }
    Panel.UnitConverter {
        id: radianConverter
        inUnit: uTurnRateRadiansPerSecond
        outUnit: uTurnRateRPM
    }

    CircularGauge {
        id: gauge
        anchors.fill: parent
        gaugeValue: parseFloat(rpmRef.value)
        barValue: maxValue > 5000 ? 1000 : 500
        thinBarValue: barValue / 10
        barsAngleMin: -45
        valueMin: 0
        valueMax: maxValue
        valueMultiplier: maxValue > 5000 ? 1000 : 100
        topLabel: "RPM"
        bottomLabel: "x " + valueMultiplier

        GaugeArc { // Max rpm bar
            anchors.fill: parent
            arcColor: "red"
            z: -10
            startAngle: gauge.value2Angle(redlineRef.value)
            arcAngle: 0.3
            arcWidth: width * 0.06
        }
    }
}
