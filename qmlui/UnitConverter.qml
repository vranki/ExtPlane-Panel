import QtQuick 2.0

Item {
    property real valueIn: 0
    readonly property real valueOut: valueIn * scaleFactor
    property int inUnit: 0
    property int outUnit: 0
    readonly property real scaleFactor: unitTable[inUnit] * (1/unitTable[outUnit])
    readonly property string inUnitName: unitNames[inUnit]
    readonly property string outUnitName: unitNames[outUnit]

    /*
      // Enums supprted in Qt >= 5.10 - enable when it time comes.
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
        PressureBar,
        TurnRateRPM,
        TurnRateRadiansPerSecond
    }
    */
    readonly property int uNoUnit: 0
    readonly property int uVelocityKnots: 1
    readonly property int uVelocityMS: 2
    readonly property int uVelocityKMH: 3
    readonly property int uVelocityFPM: 4
    readonly property int uDistanceMeters: 5
    readonly property int uDistanceFeet: 6
    readonly property int uPressurehPa: 7
    readonly property int uPressureInHG: 8
    readonly property int uPressureBar: 9
    readonly property int uTurnRateRPM: 10
    readonly property int uTurnRateRadiansPerSecond: 11

    // Conversion table from unit to SI standard unit
    readonly property var unitTable: [1, 0.514444, 1, 0.277778, 0.00508, 1, 0.3048, 1,  33.86, 1000, 1, 9.549296586]
    // Unit names
    readonly property var unitNames: ["No unit", "Knots", "m/s", "km/h", "fpm", "m", "ft", "hPa", "InHg", "Bar", "RPM", "Rad/s"]
}
