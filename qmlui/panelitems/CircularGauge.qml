import QtQuick 2.0

PanelItem {
    property double gaugeValue: 0

    width: Math.min(parent.width, parent.height)
    CircularGaugeBars {
        id: valueBars
        thickBars: true
        showValue: true
        barValue: 20
    }
    CircularGaugeBars {
        barValue: 5
    }
    Needle {
        transformOrigin: Item.Right
        y: parent.height / 2
        rotation: valueBars.value2Angle(gaugeValue)
    }
}
