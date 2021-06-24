import QtQuick 2.0
import ".." as PanelItems

PanelItems.PanelItem {
    property real gaugeValue: 0
    property real barValue: 10
    property real thinBarValue: 0
    property real barsAngle: 270
    property real barsAngleMin: 0
    property real valueMin: 0
    property real valueMax: 100
    property string topLabel: ""
    property string bottomLabel: ""
    property real valueMultiplier: 1
    property int needleType: 0
    property bool showGauge: true
    readonly property real barAngle: valueBars.barAngle

    // Keep rectangular
    readonly property int itemSize: Math.min(parent.width, parent.height)
    width: itemSize
    height: itemSize

    CircularGaugeBars {
        id: valueBars
        thickBars: true
        showValue: true
        barValue: parent.barValue
        barsAngle: parent.barsAngle
        barsAngleMin: parent.barsAngleMin
        valueMin: parent.valueMin
        valueMax: parent.valueMax
        valueMultiplier: parent.valueMultiplier
        showGauge: parent.showGauge
        Needle {
            needleType: parent.parent.needleType
            rotation: valueBars.value2Angle(gaugeValue)
        }
    }
    CircularGaugeBars {
        barValue: thinBarValue
        visible: thinBarValue
        barsAngle: parent.barsAngle
        barsAngleMin: parent.barsAngleMin
        valueMin: parent.valueMin
        valueMax: parent.valueMax
        showGauge: parent.showGauge
        z: -10
    }
    Text {
        text: topLabel
        color: "white"
        anchors.horizontalCenter: valueBars.horizontalCenter
        anchors.top: valueBars.top
        anchors.topMargin: valueBars.height / 3
        z: -5
        font.pixelSize: parent.height * 0.05
        font.family: b612.name
    }
    Text {
        text: bottomLabel
        color: "white"
        anchors.horizontalCenter: valueBars.horizontalCenter
        anchors.bottom: valueBars.bottom
        anchors.bottomMargin: valueBars.height / 3
        z: -5
        font.pixelSize: parent.height * 0.05
        font.family: b612.name
    }
    function value2Angle(value) { return valueBars.value2Angle(value) }
}
