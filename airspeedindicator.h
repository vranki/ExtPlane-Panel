#ifndef AIRSPEEDINDICATOR_H
#define AIRSPEEDINDICATOR_H

#include "needleinstrument.h"
#include "units.h"

class AirspeedIndicator : public NeedleInstrument {
    Q_OBJECT
public:
    AirspeedIndicator(QObject *parent, ExtPlaneConnection *conn);
    void setUnit(VelocityUnit unit);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual QString typeName();
    static QString typeNameStatic();
public slots:
    void speedChanged(QString name, double speed);
private:
    ExtPlaneClient _client;
    VelocityUnit units;
};

#endif // AIRSPEEDINDICATOR_H
