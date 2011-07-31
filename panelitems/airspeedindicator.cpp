#include "airspeedindicator.h"

AirspeedIndicator::AirspeedIndicator(QObject *parent, ExtPlaneConnection *conn) : NeedleInstrument(parent),
_client(this, typeName(), conn) {
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/cockpit2/gauges/indicators/airspeed_kts_pilot", 0.1);
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(speedChanged(QString,double)));
    setBars(10, 5);
    setNumbers(20);
    setUnit(VELOCITY_KTS);
    //    setMaxValue(300);
    Arc * arc = new Arc(this, this);
    arc->_use = true;                     // White arc
    arc->_minValue = 40.;
    arc->_maxValue= 85.;
    arc->_radius_inner=0.95;
    arc->_radius_outer=1.0;
    arc->_color = QColor(255,255,255);
    addArc(arc);

    Arc * arc1 = new Arc(this, this);
    arc1->_use = true;                    
    arc1->_minValue = 50.;                 // Green arc
    arc1->_maxValue=126.;
    arc1->_radius_inner=0.90;
    arc1->_radius_outer=0.95;
    arc1->_color = QColor(0,200,0);
    addArc(arc1);
    
    Arc * arc2 = new Arc(this, this);
    arc2->_use = true;                     
    arc2->_minValue = 126.;                 // Yellow arc
    arc2->_maxValue=162.;
    arc2->_radius_inner=0.90;
    arc2->_radius_outer=0.95;
    arc2->_color = QColor(230,230,0);
    addArc(arc2);

    Arc * arc3 = new Arc(this, this);
    arc3->_use = true;                     
    arc3->_minValue = 157.;                 // Red arc
    arc3->_maxValue=162.;
    arc3->_radius_inner=0.95;
    arc3->_radius_outer=1.0;
    arc3->_color = QColor(230,0,0);
    addArc(arc3);
    
}

void AirspeedIndicator::speedChanged(QString name, double speed) {
    setValue(Units::convertSpeed(VELOCITY_KTS, units, speed));
}

void AirspeedIndicator::setUnit(VelocityUnit unit) {
    units = unit;
    setLabel(Units::unitName(units));
}

void AirspeedIndicator::storeSettings(QSettings &settings) {
    NeedleInstrument::storeSettings(settings);

    settings.setValue("unit", Units::unitName(units));
    
    int nArcs = numberOfArcs();
    settings.setValue("numberOfArcs", QString::number(nArcs));
    
    if (nArcs){
        Arc * arc;
        for(int i=0;i<nArcs;i++){
            arc = getArc(i);
            settings.beginGroup("arc-" + QString::number(i));
            settings.setValue("use", QString::number(arc->_use));
            settings.setValue("minValue", QString::number(arc->_minValue));
            settings.setValue("maxValue", QString::number(arc->_maxValue));
            settings.setValue("innerRadius", QString::number(arc->_radius_inner));
            settings.setValue("outerRadius", QString::number(arc->_radius_outer));
            settings.setValue("color", arc->_color.name());
            settings.endGroup();
        }
    }
}

void AirspeedIndicator::loadSettings(QSettings &settings) {
    NeedleInstrument::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    VelocityUnit unit = Units::velocityUnitForName(unitname);
    setUnit(unit);
    
    qDebug() << Q_FUNC_INFO << "About to clear arcs "; 
    clearArcs();
    
    int nArcs = settings.value("numberOfArcs", 4).toInt();
    
    for (int i=0;i<nArcs;i++){
        Arc * arc = new Arc(this, this);
        settings.beginGroup("arc-" + QString::number(i));
        arc->_use = (settings.value("use", 0).toInt()==1);
        arc->_minValue = settings.value("minValue", 40).toFloat();
        arc->_maxValue = settings.value("maxValue", 100).toFloat();
        arc->_radius_inner = settings.value("innerRadius", 0.7).toFloat();
        arc->setOuter(settings.value("outerRadius", 0.8).toFloat());
        arc->_color.setNamedColor(settings.value("color", "#ffffff").toString());
        settings.endGroup();
        addArc(arc);
    }
    
    //    setMaxValue(settings.value("maxvalue", 300).toFloat());
}

QString AirspeedIndicator::typeName() {
    return typeNameStatic();
}

QString AirspeedIndicator::typeNameStatic() {
    return "indicator/airspeed/round";
}
//void AirspeedIndicator::setMaxValue(float mv) {
    //    maxValue = mv;
    // setScale(25, NeedleInstrument::_zeroValue, 0+330, maxValue);
    //}

void AirspeedIndicator::createSettings(QGridLayout *layout) {
    QLabel *unitsLabel = new QLabel("Unit", layout->parentWidget());
    layout->addWidget(unitsLabel, layout->rowCount(), 0);
    VelocityUnitComboBox *unitsCombo = new VelocityUnitComboBox(layout->parentWidget(), units);
    connect(unitsCombo, SIGNAL(unitSelected(VelocityUnit)), this, SLOT(setUnit(VelocityUnit)));
    layout->addWidget(unitsCombo);

    NeedleInstrument::createSettings(layout);

}
