#ifndef VARIOMETER_H
#define VARIOMETER_H

#include "needleinstrument.h"
/**
 * GA mechanical variometer (aka VVI)
 *
 * Something like this
 *
 * http://www.wingsandwheels.com/images/5stv_5stvm.jpg
 */
#define VARIOMETER_ACCURACY 0.05
#define VARIOMETER_ACCURACY_TE 20

class Variometer : public NeedleInstrument
{
    Q_OBJECT
public:
    Q_INVOKABLE Variometer(ExtPlanePanel *panel, ExtPlaneConnection *conn);
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
    virtual void setInterpolationEnabled(bool ie);
protected:
    virtual void paintTopPixmap();
private:
    ExtPlaneClient _client;
    VelocityUnit units;
    float maxValue;
    float numberScale;
    bool isTotalEnergy;
    ValueInterpolator interpolator;
};

#endif // VARIOMETER_H
