#ifndef AIRSPEEDINDICATOR_H
#define AIRSPEEDINDICATOR_H

#include "needleinstrument.h"
#include "units.h"
#include <QLabel>
#include "velocityunitcombobox.h"

class AirspeedIndicator : public NeedleInstrument {
    Q_OBJECT
public:
    AirspeedIndicator(QObject *parent, ExtPlaneConnection *conn);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual QString typeName();
    static QString typeNameStatic();
    virtual void createSettings(QGridLayout *layout);
public slots:
    void speedChanged(QString name, double speed);
    void setUnit(VelocityUnit un);
private:
    ExtPlaneClient _client;
    VelocityUnit units;
};

#endif // AIRSPEEDINDICATOR_H
