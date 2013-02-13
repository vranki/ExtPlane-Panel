#ifndef VARIOMETER_H
#define VARIOMETER_H

#include "needleinstrument.h"

class Variometer : public NeedleInstrument
{
    Q_OBJECT
public:
    Q_INVOKABLE Variometer(QObject *parent, ExtPlaneConnection *conn);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual QString typeName();
    virtual void createSettings(QGridLayout *layout);
public slots:
    void velocityChanged(QString name, double speed);
    void setUnit(VelocityUnit un);
    void setMaxValue(float mv);
    void setIsTotalEnergy(bool te);
    virtual void tickTime(double dt, int total);
private:
    ExtPlaneClient _client;
    VelocityUnit units;
    float maxValue;
    float numberScale;
    bool isTotalEnergy;
    ValueInterpolator interpolator;
};

#endif // VARIOMETER_H
