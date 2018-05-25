#include "units.h"

Units::Units()
{
}

double Units::convertSpeed(VelocityUnit from, VelocityUnit to, double value) {
    double ms = 0;
    if(from == VELOCITY_KTS)
        ms = value * 0.51;
    if(from == VELOCITY_KMH)
        ms = value * 0.278;
    if(from == VELOCITY_MS)
        ms = value;
    if(from == VELOCITY_FPM)
        ms = value * 0.00508;
    if(from == VELOCITY_KFPM)
        ms = value * 5.08;

    if(to == VELOCITY_KMH)
        return ms * 3.6;
    if(to == VELOCITY_KTS)
        return ms * 1.94;
    if(to == VELOCITY_FPM)
        return ms * 196.850393;
    if(to == VELOCITY_KFPM)
        return ms * 0.196850393;
    return ms;
}

double Units::convertDistance(DistanceUnit from, DistanceUnit to, double value) {
    double m = 0;
    if(from == DISTANCE_M)
        m = value;
    if(from == DISTANCE_FT)
        m = value * 0.3048;

    if(to == DISTANCE_FT)
        return m*3.2808399;

    return m;
}


double Units::convertPressure(PressureUnit from, PressureUnit to, double value) {
    double pa = 0;
    if(from == PRESSURE_PA)
        pa = value;
    if(from == PRESSURE_HPA)
        pa = value*100.0;
    if(from == PRESSURE_INCHES_HG)
        pa = value * 3386.389;

    if(to == PRESSURE_HPA)
        return pa/100.0;
    if(to == PRESSURE_INCHES_HG)
        return pa/3386.389;

    return pa;
}

QString Units::unitName(VelocityUnit unit) {
    if(unit == VELOCITY_KTS)
        return "kts";
    if(unit == VELOCITY_KMH)
        return "km/h";
    if(unit == VELOCITY_MS)
        return "m/s";
    if(unit == VELOCITY_FPM)
        return "ft/min";
    if(unit == VELOCITY_KFPM)
        return "ft/min x1000";
    return "?";
}

QString Units::unitName(DistanceUnit unit) {
    if(unit == DISTANCE_M)
        return "meters";
    if(unit == DISTANCE_FT)
        return "feet";
    return "?";
}

QString Units::unitName(PressureUnit unit) {
    if(unit == PRESSURE_PA)
        return "Pa";
    if(unit == PRESSURE_HPA)
        return "hPa";
    if(unit == PRESSURE_INCHES_HG)
        return "inHg";
    return "?";
}

VelocityUnit Units::velocityUnitForName(QString name) {
    if(name=="kts") return VELOCITY_KTS;
    if(name=="km/h") return VELOCITY_KMH;
    if(name=="m/s") return VELOCITY_MS;
    if(name=="ft/min") return VELOCITY_FPM;
    if(name=="ft/min x1000") return VELOCITY_KFPM;
    return VELOCITY_UNKNOWN;
}

DistanceUnit Units::distanceUnitForName(QString name) {
    if(name=="meters") return DISTANCE_M;
    if(name=="feet") return DISTANCE_FT;
    return DISTANCE_UNKNOWN;
}

PressureUnit Units::pressureUnitForName(QString name) {
    if(name=="Pa") return PRESSURE_PA;
    if(name=="hPa") return PRESSURE_HPA;
    if(name=="inHg") return PRESSURE_INCHES_HG;
    return PRESSURE_UNKNOWN;
}
