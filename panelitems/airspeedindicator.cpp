#include "airspeedindicator.h"
#include <QLabel>
#include "../needles/gabalancedneedle.h"
#include "../widgets/velocityunitcombobox.h"
#include "../widgets/numberinputlineedit.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(AirspeedIndicator,"indicator/airspeed/round");

AirspeedIndicator::AirspeedIndicator(ExtPlanePanel *panel, ExtPlaneConnection *conn) : NeedleInstrument(panel),
    _client(this, typeName(), conn), interpolator(0, 5) {
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/cockpit2/gauges/indicators/airspeed_kts_pilot", 0.3);
    // _client.subscribeDataRef("simulated", 1);
    connect(&_client, SIGNAL(refChanged(QString,double)), &interpolator, SLOT(valueChanged(QString,double)));
    connect(&interpolator, SIGNAL(interpolatedValueChanged(QString,double)), this, SLOT(speedChanged(QString,double)));
    setBars(20, 10);
    setNumbers(50);
    setUnit(VELOCITY_KTS);
    setMaxValue(300);
    setVso(35);
    setVs(50);
    setVfe(110);
    setVno(150);
    setVne(200);
    yellowTriangle = vs;
    setNeedle(new GABalancedNeedle(this));
}

void AirspeedIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    setupPainter(painter);

    QPen pen = QPen(Qt::red);
    int side = qMin(width(), height());
//    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();
    painter->scale(side / 200.0, side / 200.0);
    painter->translate(100, 100);

    // VNE line
    painter->setBrush(Qt::red);
    painter->setPen(pen);
    painter->save();
    double vneUnits = Units::convertSpeed(VELOCITY_MS, units, vne);
    painter->rotate(value2Angle(vneUnits));
    painter->drawRect(-3, -100, 6, 20);
    painter->restore();

    // Arcs
    paintArc(painter, Qt::green, vs, vno);
    paintArc(painter, Qt::yellow, vno, vne);

    painter->save();
    painter->scale(0.95, 0.95);
    paintArc(painter, Qt::white, vso, vfe);
    painter->restore();

    // Yellow triangle
    painter->setBrush(Qt::yellow);
    painter->setPen(Qt::yellow);
    painter->save();
    double vsUnits = Units::convertSpeed(VELOCITY_MS, units, vs);
    painter->rotate(value2Angle(vsUnits));
    QPolygon p;
    p << QPoint(-5, -83) << QPoint(5, -83) << QPoint(0,-90);
    painter->drawPolygon(p);
    painter->restore();


    painter->restore();
    NeedleInstrument::paint(painter, option, widget);
}

void AirspeedIndicator::paintArc(QPainter *painter,QColor color, double start, double end) {
    painter->save();
    QPen pen(color);
    pen.setWidth(4);
    painter->setPen(pen);
    painter->setBrush(color);

    double startInUnits = Units::convertSpeed(VELOCITY_MS, units, start);
    double endInUnits = Units::convertSpeed(VELOCITY_MS, units, end);
    double startAngle = -(value2Angle(startInUnits) - 90)*16.0;
    double endAngle = -(value2Angle(endInUnits) - 90)*16.0;
    painter->drawArc(-98,-98,196, 196, startAngle, endAngle - startAngle);
    painter->restore();
}

void AirspeedIndicator::speedChanged(QString name, double speed) {
    Q_UNUSED(name);
    setValue(Units::convertSpeed(VELOCITY_KTS, units, speed));
}

void AirspeedIndicator::setUnit(VelocityUnit unit) {
    units = unit;
    setLabel(Units::unitName(units));
}

void AirspeedIndicator::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("unit", Units::unitName(units));
    settings.setValue("maxvalue", maxValue);
    settings.setValue("vso", vso);
    settings.setValue("vs", vs);
    settings.setValue("vfe", vfe);
    settings.setValue("vno", vno);
    settings.setValue("vne", vne);
}

void AirspeedIndicator::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    VelocityUnit unit = Units::velocityUnitForName(unitname);
    setUnit(unit);
    setMaxValue(settings.value("maxvalue", 300).toDouble());
    setVso(settings.value("vso", 35).toDouble());
    setVs(settings.value("vs", 50).toDouble());
    setVfe(settings.value("vfe", 110).toDouble());
    setVno(settings.value("vno", 150).toDouble());
    setVno(settings.value("vne", 200).toDouble());
}

void AirspeedIndicator::setMaxValue(float mv) {
    maxValue = mv;
    setScale(180, 0, 180+330, maxValue);
}

void AirspeedIndicator::setVso(float mv) {
    vso = mv;
    if (vso > maxValue) {
      vso = maxValue;
    }
    setScale(180, 0, 180+330, maxValue);
}

void AirspeedIndicator::setVs(float mv) {
    vs = mv;
    if (vs > maxValue) {
      vs = maxValue;
    }
    if (vs < vso) {
      vs = vso;
    }
    setScale(180, 0, 180+330, maxValue);
}

void AirspeedIndicator::setVfe(float mv) {
    vfe = mv;
    if (vfe > maxValue) {
      vfe = maxValue;
    }
    if (vfe < vs) {
      vfe = vs;
    }
    setScale(180, 0, 180+330, maxValue);
}

void AirspeedIndicator::setVno(float mv) {
    vno = mv;
    if (vno > maxValue) {
      vno = maxValue;
    }
    if (vno < vfe) {
      vno = vfe;
    }
    setScale(180, 0, 180+330, maxValue);
}

void AirspeedIndicator::setVne(float mv) {
    vne = mv;
    if (vne > maxValue) {
      vne = maxValue;
    }
    if (vne < vno) {
      vne = vno;
    }
    setScale(180, 0, 180+330, maxValue);
}

void AirspeedIndicator::createSettings(QGridLayout *layout) {
    NeedleInstrument::createSettings(layout);
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
    QLabel *vsoLabel = new QLabel("White arc start (Vso)", layout->parentWidget());
    layout->addWidget(vsoLabel);
    NumberInputLineEdit *vsoEdit = new NumberInputLineEdit(layout->parentWidget());
    vsoEdit->setText(QString::number(vso));
    layout->addWidget(vsoEdit);
    connect(vsoEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setVso(float)));
    QLabel *vsLabel = new QLabel("Green arc start (Vs)", layout->parentWidget());
    layout->addWidget(vsLabel);
    NumberInputLineEdit *vsEdit = new NumberInputLineEdit(layout->parentWidget());
    vsEdit->setText(QString::number(vs));
    layout->addWidget(vsEdit);
    connect(vsEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setVs(float)));
    QLabel *vfeLabel = new QLabel("White arc end (Vfe)", layout->parentWidget());
    layout->addWidget(vfeLabel);
    NumberInputLineEdit *vfeEdit = new NumberInputLineEdit(layout->parentWidget());
    vfeEdit->setText(QString::number(vfe));
    layout->addWidget(vfeEdit);
    connect(vfeEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setVfe(float)));
    QLabel *vnoLabel = new QLabel("Green arc end (Vno)", layout->parentWidget());
    layout->addWidget(vnoLabel);
    NumberInputLineEdit *vnoEdit = new NumberInputLineEdit(layout->parentWidget());
    vnoEdit->setText(QString::number(vno));
    layout->addWidget(vnoEdit);
    connect(vnoEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setVno(float)));
    QLabel *vneLabel = new QLabel("Maximum speed (Vne)", layout->parentWidget());
    layout->addWidget(vneLabel);
    NumberInputLineEdit *vneEdit = new NumberInputLineEdit(layout->parentWidget());
    vneEdit->setText(QString::number(vne));
    layout->addWidget(vneEdit);
    connect(vneEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setVne(float)));
}

void AirspeedIndicator::tickTime(double dt, int total) {
    interpolator.tickTime(dt, total);
}

void AirspeedIndicator::setInterpolationEnabled(bool ie) {
    interpolator.setEnabled(ie);
}
