import QtQuick 2.0

Item {
    property real valueIn: 0
    property real valueOut: convertUnit(valueIn)
    property int inUnit: 0
    property int outUnit: 0
    readonly property real scaleFactor: unitTable[inUnit] * (1/unitTable[outUnit])
    readonly property string inUnitName: unitNames[inUnit]
    readonly property string outUnitName: unitNames[outUnit]

    enum Unit {
        NoUnit,
        VelocityKnots,
        VelocityMS,
        VelocityKMH,
        VelocityFPM,
        DistanceMeters,
        DistanceFeet,
        PressurehPa,
        PressureInHG,
        PressureBar
    }
    // Conversion table from unit to SI standard unit
    readonly property var unitTable: [1, 0.514444, 1, 0.277778, 0.00508, 1, 0.3048, 1,  33.86, 1000]
    // Unit names
    readonly property var unitNames: ["No unit", "Knots", "m/s", "km/h", "fpm", "m", "ft", "hPa", "InHg", "Bar"]

    function convertUnit(value) {
        return value * scaleFactor
    }
}
