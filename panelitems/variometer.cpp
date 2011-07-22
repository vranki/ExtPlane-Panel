#include "variometer.h"

Variometer::Variometer(QObject *parent, ExtPlaneConnection *conn) : NeedleInstrument(parent), _client(this, typeName(), conn) {
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/flightmodel/position/vh_ind_fpm", 2.0);
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(velocityChanged(QString,double)));
    setBars(500, 100);
    setNumbers(500);
    setNumberScale(0.01);
    setUnit(VELOCITY_FPM);
    setMaxValue(2000);
    isTotalEnergy = false;
    setIsTotalEnergy(false);
}

void Variometer::velocityChanged(QString name, double speed) {
    if(isTotalEnergy) {
        setValue(Units::convertSpeed(VELOCITY_FPM, units, speed));
    } else {
        setValue(Units::convertSpeed(VELOCITY_FPM, units, speed));
    }
}

void Variometer::setUnit(VelocityUnit unit) {
    units = unit;
    setLabel(Units::unitName(units));
}

void Variometer::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("unit", Units::unitName(units));
    settings.setValue("maxvalue", QString::number(maxValue));
    settings.setValue("scalevalue", QString::number(numberScale));
    settings.setValue("totalenergy", isTotalEnergy);
}

void Variometer::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    VelocityUnit unit = Units::velocityUnitForName(unitname);
    setUnit(unit);
    setMaxValue(settings.value("maxvalue", 300).toFloat());
    setNumberScale(settings.value("scalevalue", 1.).toFloat());
    setIsTotalEnergy(settings.value("totalenergy", false).toBool());
}

QString Variometer::typeName() {
    return typeNameStatic();
}

QString Variometer::typeNameStatic() {
    return "indicator/variometer/round";
}

void Variometer::setMaxValue(float mv) {
    maxValue = mv;
    setScale(180-70, -maxValue, 360+70, maxValue);
}

void Variometer::setNumberScale(float ns) {
    numberScale = ns;
    setNumberMult(ns);
}

void Variometer::createSettings(QGridLayout *layout) {
    QLabel *unitsLabel = new QLabel("Unit", layout->parentWidget());
    layout->addWidget(unitsLabel, layout->rowCount(), 0);
    VelocityUnitComboBox *unitsCombo = new VelocityUnitComboBox(layout->parentWidget(), units);
    connect(unitsCombo, SIGNAL(unitSelected(VelocityUnit)), this, SLOT(setUnit(VelocityUnit)));
    layout->addWidget(unitsCombo);

    QLabel *maxLabel = new QLabel("Maximum value", layout->parentWidget());
    layout->addWidget(maxLabel);
    NumberInputLineEdit *maxValueEdit = new NumberInputLineEdit(layout->parentWidget());
    maxValueEdit->setText(QString::number(maxValue));
    layout->addWidget(maxValueEdit);
    connect(maxValueEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMaxValue(float)));

    QLabel *scaleLabel = new QLabel("Scale numbers", layout->parentWidget());
    layout->addWidget(scaleLabel);
    NumberInputLineEdit *scaleValueEdit = new NumberInputLineEdit(layout->parentWidget());
    scaleValueEdit->setText(QString::number(numberScale));
    layout->addWidget(scaleValueEdit);
    connect(scaleValueEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setNumberScale(float)));
    
    
    QCheckBox *totalCheckbox = new QCheckBox("Total energy", layout->parentWidget());
    totalCheckbox->setChecked(isTotalEnergy);
    layout->addWidget(totalCheckbox);
    connect(totalCheckbox, SIGNAL(clicked(bool)), this, SLOT(setIsTotalEnergy(bool)));
}

void Variometer::setIsTotalEnergy(bool te) {
    if(te == isTotalEnergy) return;
    isTotalEnergy = te;
    if(isTotalEnergy) {
        _client.unsubscribeDataRef("sim/flightmodel/position/vh_ind_fpm");
        _client.subscribeDataRef("sim/cockpit2/gauges/indicators/total_energy_fpm", 0.5);
    } else {
        _client.unsubscribeDataRef("sim/cockpit2/gauges/indicators/total_energy_fpm");
        _client.subscribeDataRef("sim/flightmodel/position/vh_ind_fpm", 10.);
    }
}
