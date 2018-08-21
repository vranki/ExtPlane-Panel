import QtQuick 2.0
import org.vranki.extplane 1.0

PanelItem {
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
        barValue: (vneRef.value > 200) ? 50 : 20
        valueMax: vneRef.value || 100
        CircularGaugeBars {
            barValue: (vneRef.value > 200) ? 10 : 5
            valueMax: valueBars.valueMax
        }
        Needle {
            transformOrigin: Item.Right
            y: parent.height / 2
            rotation: valueBars.value2Angle(airspeedRef.value)
        }
    }
}
