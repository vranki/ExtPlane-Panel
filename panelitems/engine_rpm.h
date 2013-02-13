/*
 *  Created by bobgates on 2011/07/23.
 */

#ifndef ENGINERPM_H
#define ENGINERPM_H

#include "needleinstrument.h"
#include "../units.h"

class EngineRPM : public NeedleInstrument
{
    Q_OBJECT
public:
    Q_INVOKABLE EngineRPM(QObject *parent, ExtPlaneConnection *conn);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual QString typeName();
    virtual void createSettings(QGridLayout *layout);
signals:
    
    public slots:
    void rpmChanged(QString name, QString valueString);
    void setUnit(VelocityUnit un);
    void setMaxValue(float mv);
    void setNumberScale(float ns);

private:
    ExtPlaneClient _client;
    VelocityUnit units;
    float maxValue;
    //float numberScale;
    bool isTotalEnergy;
};

#endif // ENGINERPM_H

