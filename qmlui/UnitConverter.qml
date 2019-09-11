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
        VelocityKMH
    }
    // Conversion table from unit to SI standard unit
    readonly property var unitTable: [1, 0.514444, 1, 0.277778]
    // Unit names
    readonly property var unitNames: ["No unit", "Knots", "m/s", "km/h"]

    function convertUnit(value) {
        return value * scaleFactor
    }
}
