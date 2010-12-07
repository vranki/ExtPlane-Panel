#ifndef UNITS_H
#define UNITS_H

#include <QString>

enum VelocityUnit {
    VELOCITY_UNKNOWN = 0,
    VELOCITY_MS,
    VELOCITY_KMH,
    VELOCITY_KTS
};

class Units {
public:
    Units();
    static double convertSpeed(VelocityUnit from, VelocityUnit to, double value);
    static QString unitName(VelocityUnit unit);
    static VelocityUnit velocityUnitForName(QString name);
};

#endif // UNITS_H
