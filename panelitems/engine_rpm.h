/*
 *  engine_rpm.h
 *  extplane-panel
 *
 *  Created by bobgates on 2011/07/23.
 *  Copyright 2011 DaffeySoft. All rights reserved.
 *
 */

#ifndef ENGINERPM_H
#define ENGINERPM_H

#include <QLabel>
#include <QCheckBox>
#include "needleinstrument.h"
#include "../units.h"
#include "widgets/velocityunitcombobox.h"
#include "widgets/numberinputlineedit.h"

class EngineRPM : public NeedleInstrument
{
    Q_OBJECT
public:
    explicit EngineRPM(QObject *parent, ExtPlaneConnection *conn);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual QString typeName();
    static QString typeNameStatic();
    virtual void createSettings(QGridLayout *layout);
signals:
    
    public slots:
    void rpmChanged(QString name, QString valueString);
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

#endif // ENGINERPM_H

