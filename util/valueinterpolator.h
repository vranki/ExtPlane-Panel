#ifndef VALUEINTERPOLATOR_H
#define VALUEINTERPOLATOR_H

#include <QObject>

/**
 * A helper class that can be used to interpolate double values
 * to make transitions look smoother.
 *
 * Parameter speed controls how fast the value moves towards latest
 * known value. @todo: explain properly.
 *
 * Note: Interpolator is off by default. Enable it only if user
 * has enabled it in settings.
 */
class ValueInterpolator : public QObject
{
    Q_OBJECT
public:
    explicit ValueInterpolator(QObject *parent, double spd);
    
signals:
    void interpolatedValueChanged(QString name, double speed);

public slots:
    void valueChanged(QString name, double val);
    void tickTime(double dt, int total);
    void setEnabled(bool ie);
    void setSpeed(double newSpeed);
private:
    double currentValue, targetValue;
    double speed;
    QString refName;
    bool enabled;
};

#endif // VALUEINTERPOLATOR_H
