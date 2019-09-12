import QtQuick 2.0
import QtQuick.Controls 2.2
import org.vranki.extplane 1.0
import QtQuick.Layouts 1.3

import "../.." as Panel
import ".." as PanelItems

PanelItems.PanelItem {
    propertiesDialog: propertiesDialog
    clip: false

    Panel.UnitConverter {
        id: altitudeConverter
        inUnit: Panel.UnitConverter.Unit.DistanceFeet
        outUnit: settings.isMs ? Panel.UnitConverter.Unit.DistanceMeters : Panel.UnitConverter.Unit.DistanceFeet
    }
    Panel.UnitConverter {
        id: pressureConverter
        inUnit: Panel.UnitConverter.Unit.PressureInHG
        outUnit: settings.isMs ? Panel.UnitConverter.Unit.PressurehPa : Panel.UnitConverter.Unit.PressureInHG
    }

    DataRef {
        id: altitudeRef
        name: "sim/flightmodel/misc/h_ind"
        accuracy: 10
        scaleFactor: altitudeConverter.scaleFactor
    }

    DataRef {
        id: altimeterSettingRef
        name: "sim/cockpit/misc/barometer_setting"
        scaleFactor: pressureConverter.scaleFactor
    }

    CircularGaugeBars {
        id: valueBars

        thickBars: true
        showValue: true
        barValue: settings.isMs ? 1000 : 1000
        valueMax: settings.isMs ? 10000 : 10000
        valueMultiplier: 1000
        barsAngleMin: 90
        barsAngle: 359
        showLastValue: false

        Text {
            text: altitudeConverter.outUnitName + " x " + valueBars.valueMultiplier
            color: "white"
            font.pixelSize: parent.height * 0.03
            font.family: b612.name
            y: parent.height * 0.70
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            text: parseFloat(altimeterSettingRef.value).toPrecision(6) + " " + pressureConverter.outUnitName
            color: "white"
            font.pixelSize: parent.height * 0.03
            font.family: b612.name
            x: parent.width * 0.60
            anchors.verticalCenter: parent.verticalCenter
        }
        Needle {
            needleType: 2
            rotation: valueBars.value2Angle(altitudeRef.value)
        }
        Needle {
            needleType: 1
            rotation: valueBars.value2Angle(altitudeRef.value * 10)
            Behavior on rotation { PropertyAnimation {
                    easing.type: Easing.Linear
                    duration: 500
                }
            }
        }
    }
    Panel.PanelItemPropertiesDialog {
        id: propertiesDialog
        propertyItems: [
            Text { text: "In meters instead of feet" },
            CheckBox { checked: settings.isMs ; onCheckedChanged: settings.isMs = checked }
        ]
    }

    PanelItems.PanelItemSettings {
        id: settings
        property bool isMs: false
    }
}
