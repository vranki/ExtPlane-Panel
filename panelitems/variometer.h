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
class Variometer : public NeedleInstrument
{
    Q_OBJECT
public:
    Q_INVOKABLE Variometer(QObject *parent, ExtPlaneConnection *conn);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual QString typeName();
    virtual void createSettings(QGridLayout *layout);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
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
