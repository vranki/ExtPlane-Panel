#ifndef UNITS_H
#define UNITS_H

#include <QString>

enum VelocityUnit {
    VELOCITY_UNKNOWN = 0,
    VELOCITY_MS,
    VELOCITY_KMH,
    VELOCITY_KTS,
    VELOCITY_FPM
};

enum DistanceUnit {
    DISTANCE_UNKNOWN = 0,
    DISTANCE_M,
    DISTANCE_FT
};

class Units {
public:
    Units();
    static double convertSpeed(VelocityUnit from, VelocityUnit to, double value);
    static double convertDistance(DistanceUnit from, DistanceUnit to, double value);
    static QString unitName(VelocityUnit unit);
    static QString unitName(DistanceUnit unit);
    static VelocityUnit velocityUnitForName(QString name);
    static DistanceUnit distanceUnitForName(QString name);
};

#endif // UNITS_H
