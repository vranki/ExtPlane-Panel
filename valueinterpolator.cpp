#include "valueinterpolator.h"

ValueInterpolator::ValueInterpolator(QObject *parent, double spd) :
    QObject(parent), speed(spd), currentValue(0), targetValue(0), enabled(false) {
}

void ValueInterpolator::valueChanged(QString name, double val) {
    if(refName.isNull()) { // First update
        refName = name;
        currentValue = val;
        targetValue = val;
        emit interpolatedValueChanged(refName, currentValue);
    }

    if(enabled) {
        targetValue = val;
    } else {
        currentValue = val;
        emit interpolatedValueChanged(refName, currentValue);
    }
}

void ValueInterpolator::tickTime(double dt, int total) {
    if(!enabled) return;

    double lastValue = currentValue;
    double diff = qAbs(currentValue - targetValue);
    if(currentValue < targetValue)
        currentValue += diff * speed * dt;

    if(currentValue > targetValue)
        currentValue -= diff * speed * dt;

    if(lastValue != currentValue)
        emit interpolatedValueChanged(refName, currentValue);
}

void ValueInterpolator::setEnabled(bool ie) {
    enabled = ie;
}

void ValueInterpolator::setSpeed(double newSpeed)
{
    speed = newSpeed;
}
