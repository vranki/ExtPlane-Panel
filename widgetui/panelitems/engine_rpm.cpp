#include "engine_rpm.h"

#include <QLabel>
#include <QCheckBox>

#include "../widgets/velocityunitcombobox.h"
#include "../widgets/numberinputlineedit.h"
#include "../util/console.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(EngineRPM,"indicator/enginerpm/round")

EngineRPM::EngineRPM(ExtPlanePanel *panel, ExtPlaneConnection *conn) : NeedleInstrument(panel), _client(this, typeName(), conn) {
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/cockpit2/engine/indicators/engine_speed_rpm", 15.0);//, 10.0);
    connect(&_client, SIGNAL(refChanged(QString,QStringList)), this, SLOT(rpmChanged(QString,QStringList)));
    setBars(500, 100);
    setNumbers(500);
    setNumberScale(0.01);
    setUnit(VELOCITY_FPM);
    setMaxValue(3000);
    setValue(2900);
    isTotalEnergy = false;
    _setMaxValueAutomatically = false;
    _engineNumber = 1;
    
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

void EngineRPM::rpmChanged(QString name, QStringList values) {
    Q_UNUSED(name);
    //TODO: seems this is arbitrarily taking the first engine; we should add a setting for which value we want
    QString rpmStr = _engineNumber > values.count() ? "0" : values.at(_engineNumber-1);
    double rpm = rpmStr.toDouble();
    if(_setMaxValueAutomatically && rpm > _maxValue) {
        setMaxValue(rpm);
    }
    setValue(rpm);
}

void EngineRPM::setUnit(VelocityUnit unit) {
    Q_UNUSED(unit);
    setLabel("RPM");
}

void EngineRPM::storeSettings(QSettings &settings) {
    NeedleInstrument::storeSettings(settings);
    
    settings.setValue("unit", Units::unitName(units));
    settings.setValue("maxvalue", QString::number(maxValue));
    settings.setValue("scalevalue", QString::number(_numberScale));
    settings.setValue("totalenergy", isTotalEnergy);
    settings.setValue("enginenumber", _engineNumber);
}

void EngineRPM::loadSettings(QSettings &settings) {
    NeedleInstrument::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    VelocityUnit unit = Units::velocityUnitForName(unitname);
    setUnit(unit);
    setMaxValue(settings.value("maxvalue", 300).toDouble());
    setNumberScale(settings.value("scalevalue", 1.).toDouble());
    setEngineNumber(settings.value("enginenumber", 1).toInt());
}

void EngineRPM::setMaxValue(float mv) {
    maxValue = mv;
    setScale(240, 0, 360+120, maxValue);
}

void EngineRPM::setNumberScale(float ns) {
    _numberScale = ns;
}

void EngineRPM::createSettings(QGridLayout *layout) {
   
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
    scaleValueEdit->setText(QString::number(_numberScale));
    layout->addWidget(scaleValueEdit);
    connect(scaleValueEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setNumberScale(float)));

    createNumberInputSetting(layout,"Engine Number",_engineNumber,SLOT(setEngineNumber(float)));

    NeedleInstrument::createSettings(layout);
}

