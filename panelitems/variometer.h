#ifndef VARIOMETER_H
#define VARIOMETER_H

#include <QLabel>
#include <QCheckBox>
#include "needleinstrument.h"
#include "../units.h"
#include "widgets/velocityunitcombobox.h"
#include "widgets/numberinputlineedit.h"

class Variometer : public NeedleInstrument
{
    Q_OBJECT
public:
    explicit Variometer(QObject *parent, ExtPlaneConnection *conn);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual QString typeName();
    static QString typeNameStatic();
    virtual void createSettings(QGridLayout *layout);
signals:

public slots:
    void velocityChanged(QString name, double speed);
    void setUnit(VelocityUnit un);
    void setMaxValue(float mv);
    void setNumberScale(float ns);
    void setIsTotalEnergy(bool te);
private:
    ExtPlaneClient _client;
    VelocityUnit units;
    float maxValue;
    float numberScale;
    bool isTotalEnergy;
};

#endif // VARIOMETER_H
