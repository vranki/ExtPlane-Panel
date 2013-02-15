#ifndef AIRSPEEDINDICATOR_H
#define AIRSPEEDINDICATOR_H

#include "needleinstrument.h"

/**
 * GA airspeed indicator
 *
 * Something like this http://www.transair.co.uk/products/images/large/winter_ASI_mph.jpg
 */
class AirspeedIndicator : public NeedleInstrument {
    Q_OBJECT
public:
    Q_INVOKABLE AirspeedIndicator(QObject *parent, ExtPlaneConnection *conn);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void createSettings(QGridLayout *layout);
public slots:
    void speedChanged(QString name, double speed);
    void setUnit(VelocityUnit un);
    void setMaxValue(float mv);
    virtual void tickTime(double dt, int total);
private:
    void paintArc(QPainter *painter,QColor color, double start, double end);
    ExtPlaneClient _client;
    VelocityUnit units;
    double maxValue;
    // Always in m/s
    double vne, yaStart, yaEnd, gaStart, gaEnd, waStart, waEnd;
    double yellowTriangle;
    ValueInterpolator interpolator;
};

#endif // AIRSPEEDINDICATOR_H
