/*
 *  engine_rpm.cpp
 *  extplane-panel
 *
 *  Created by bobgates on 2011/07/23.
 *  Copyright 2011 DaffeySoft. All rights reserved.
 *
 */

#include "engine_rpm.h"

EngineRPM::EngineRPM(QObject *parent, ExtPlaneConnection *conn) : NeedleInstrument(parent), _client(this, typeName(), conn) {
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/cockpit2/engine/indicators/engine_speed_rpm", 15.0);//, 10.0);
    connect(&_client, SIGNAL(refChanged(QString,QString)), this, SLOT(rpmChanged(QString,QString)));
    setBars(500, 100);
    setNumbers(500);
    setNumberScale(0.01);
    setUnit(VELOCITY_FPM);
    setMaxValue(3000);
    setValue(2900);
    isTotalEnergy = false;
    setNumberFontsize(20);
    
    Arc * arc = new Arc(this, this);
    arc->_use = true;
    arc->_minValue = 1900.;
    arc->_maxValue= 2700.;
    arc->_radius_inner=0.95;
    arc->_radius_outer=1.0;
    arc->_color = QColor(0,200,0);
    addArc(arc);
    
    Arc * arc1 = new Arc(this, this);
    arc1->_minValue = 2700.;
    arc1->_maxValue= 2800.;
    arc1->_radius_inner=0.90;
    arc1->_radius_outer=1.0;
    arc1->_color = QColor(200,0,0);
    addArc(arc1);
    
    
}

void EngineRPM::rpmChanged(QString name, QString valueString) {
    
    QStringList cmd = valueString.split(" ", QString::SkipEmptyParts);
    QString rpmStr = cmd.value(3);
    rpmStr.chop(1);
    
    qDebug() << Q_FUNC_INFO << "rpmStr " << rpmStr;
    
    
    setValue(rpmStr.toDouble());
}

void EngineRPM::setUnit(VelocityUnit unit) {
    setLabel("RPM");
}

void EngineRPM::storeSettings(QSettings &settings) {
    NeedleInstrument::storeSettings(settings);
    
    settings.setValue("unit", Units::unitName(units));
    settings.setValue("maxvalue", QString::number(maxValue));
    settings.setValue("scalevalue", QString::number(numberScale));
    settings.setValue("totalenergy", isTotalEnergy);
}

void EngineRPM::loadSettings(QSettings &settings) {
    NeedleInstrument::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    VelocityUnit unit = Units::velocityUnitForName(unitname);
    setUnit(unit);
    setMaxValue(settings.value("maxvalue", 300).toFloat());
    setNumberScale(settings.value("scalevalue", 1.).toFloat());
}

QString EngineRPM::typeName() {
    return typeNameStatic();
}

QString EngineRPM::typeNameStatic() {
    return "indicator/EngineRPM/round";
}

void EngineRPM::setMaxValue(float mv) {
    maxValue = mv;
    setScale(240, 0, 360+120, maxValue);
}

void EngineRPM::setNumberScale(float ns) {
    numberScale = ns;
    setNumberMult(ns);
}

void EngineRPM::createSettings(QGridLayout *layout) {
   
    QLabel *unitsLabel = new QLabel("Unit", layout->parentWidget());
    layout->addWidget(unitsLabel, layout->rowCount(), 0);
    VelocityUnitComboBox *unitsCombo = new VelocityUnitComboBox(layout->parentWidget(), units);
    connect(unitsCombo, SIGNAL(unitSelected(VelocityUnit)), this, SLOT(setUnit(VelocityUnit)));
    layout->addWidget(unitsCombo);

    NeedleInstrument::CreateNumberEntry(layout, "Maximum value", 
                                          maxValue, 
                                          this,
                                          SLOT(setMaxValue(float)));
    
    /*
    QLabel *maxLabel = new QLabel("Maximum value", layout->parentWidget());
    layout->addWidget(maxLabel);
    NumberInputLineEdit *maxValueEdit = new NumberInputLineEdit(layout->parentWidget());
    maxValueEdit->setText(QString::number(maxValue));
    layout->addWidget(maxValueEdit);
    connect(maxValueEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMaxValue(float)));
    */
    QLabel *scaleLabel = new QLabel("Scale numbers", layout->parentWidget());
    layout->addWidget(scaleLabel);
    NumberInputLineEdit *scaleValueEdit = new NumberInputLineEdit(layout->parentWidget());
    scaleValueEdit->setText(QString::number(numberScale));
    layout->addWidget(scaleValueEdit);
    connect(scaleValueEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setNumberScale(float)));

    NeedleInstrument::createSettings(layout);
}

