#include "airspeedindicator.h"
#include <QLabel>
#include "../needles/gabalancedneedle.h"
#include "widgets/velocityunitcombobox.h"
#include "widgets/numberinputlineedit.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(AirspeedIndicator,"indicator/airspeed/round");

AirspeedIndicator::AirspeedIndicator(QObject *parent, ExtPlaneConnection *conn) : NeedleInstrument(parent),
    _client(this, typeName(), conn), interpolator(0, 10) {
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/cockpit2/gauges/indicators/airspeed_kts_pilot", 1);
    // _client.subscribeDataRef("simulated", 1);
    connect(&_client, SIGNAL(refChanged(QString,double)), &interpolator, SLOT(valueChanged(QString,double)));
    connect(&interpolator, SIGNAL(interpolatedValueChanged(QString,double)), this, SLOT(speedChanged(QString,double)));
    setBars(20, 10);
    setNumbers(50);
    setUnit(VELOCITY_KTS);
    setMaxValue(300);
    vne = 55.55;
    yaStart = 27.7;
    yaEnd = vne;
    gaStart = 13.8;
    gaEnd = yaStart;
    waStart = 10.0;
    waEnd = yaStart;
    yellowTriangle = gaStart;
    setNeedle(new GABalancedNeedle(this));
}

void AirspeedIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QPen pen = QPen(Qt::red);
    int side = qMin(width(), height());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();
    painter->scale(side / 200.0, side / 200.0);
    painter->translate(100, 100);

    // VNE line
    painter->setBrush(Qt::red);
    painter->setPen(pen);
    painter->save();
    double inUnits = Units::convertSpeed(VELOCITY_MS, units, vne);
    painter->rotate(value2Angle(inUnits));
    painter->drawRect(-3, -100, 6, 20);
    painter->restore();

    // Arcs
    paintArc(painter, Qt::yellow, yaStart, yaEnd);
    paintArc(painter, Qt::green, gaStart, gaEnd);

    painter->save();
    painter->scale(0.95, 0.95);
    paintArc(painter, Qt::white, waStart, waEnd);
    painter->restore();

    // Yellow triangle
    painter->setBrush(Qt::yellow);
    painter->setPen(Qt::yellow);
    painter->save();
    inUnits = Units::convertSpeed(VELOCITY_MS, units, yellowTriangle);
    painter->rotate(value2Angle(inUnits));
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
}

void AirspeedIndicator::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    VelocityUnit unit = Units::velocityUnitForName(unitname);
    setUnit(unit);
    setMaxValue(settings.value("maxvalue", 300).toFloat());
}

void AirspeedIndicator::setMaxValue(float mv) {
    maxValue = mv;
    setScale(180, 0, 180+330, maxValue);
}

void AirspeedIndicator::createSettings(QGridLayout *layout) {
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
}

void AirspeedIndicator::tickTime(double dt, int total) {
    interpolator.tickTime(dt, total);
}
