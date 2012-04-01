#include "valueinterpolator.h"

ValueInterpolator::ValueInterpolator(QObject *parent, double spd) :
    QObject(parent), speed(spd), currentValue(0), targetValue(0)
{
}

void ValueInterpolator::valueChanged(QString name, double val) {
    if(refName.isNull()) {
        refName = name;
        currentValue = val;
        emit interpolatedValueChanged(refName, currentValue);
    }
    targetValue = val;
}

void ValueInterpolator::tickTime(double dt, int total) {
    double diff = qAbs(currentValue - targetValue);
    if(currentValue < targetValue)
        currentValue += diff * speed * dt;

    if(currentValue > targetValue)
        currentValue -= diff * speed * dt;

    emit interpolatedValueChanged(refName, currentValue);
}
