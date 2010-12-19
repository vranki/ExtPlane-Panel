#include "altimeter.h"

Altimeter::Altimeter(QObject *parent, ExtPlaneConnection *conn) :
    PanelItem(parent), _client(this, typeName(), conn)
{
    _value = 0;
    _thickBars = 50;
    _thinBars = 10;
    _range1 = 500;
    _range2 = 5000;
    _numbers = 50;
    _numbersScale = 0.01;
    units = DISTANCE_M;
    _client.subscribeDataRef("sim/flightmodel/misc/h_ind", 3);
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(altChanged(QString,double)));
}

void Altimeter::setNumbers(float div) {
    _numbers = div;
}

void Altimeter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    static const QPoint needle[3] = {
        QPoint(4, 8),
        QPoint(-4, 8),
        QPoint(0, -95)
    };
    static const QPoint needle2[3] = {
        QPoint(8, 8),
        QPoint(-8, 8),
        QPoint(0, -70)
    };
    QColor needleColor(255, 255, 255);
    QColor needleColor2(200, 200, 200);

    int side = qMin(width(), height());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();
    painter->scale(side / 200.0, side / 200.0);
    painter->save();
    painter->translate(100, 100);
    painter->setPen(Qt::NoPen);

    painter->setBrush(needleColor2);
    painter->save();
    painter->rotate(value2Angle2(_value));
    painter->drawConvexPolygon(needle2, 3);
    painter->restore();
    painter->setBrush(needleColor);
    painter->save();
    painter->rotate(value2Angle1(_value));
    painter->drawConvexPolygon(needle, 3);
    painter->restore();

    painter->setPen(Qt::white);
    if(!_label.isEmpty()) {
        int textwidth = painter->fontMetrics().width(_label);
        painter->drawText(-textwidth/2,-70, textwidth, 200, Qt::AlignCenter, _label);
    }
    int textwidth = painter->fontMetrics().width("Altitude");
    painter->drawText(-textwidth/2,-130, textwidth, 200, Qt::AlignCenter, "Altitude");

    for (float i = 0 ; i <= _range1; i+=_thickBars) {
        painter->save();
        painter->rotate(value2Angle1(i));
        painter->drawRect(-1, -100, 2, 14);
        painter->restore();
    }
    for (float i = 0 ; i <= _range2; i+=_thinBars) {
        painter->save();
        painter->rotate(value2Angle1(i));
        painter->drawRect(-0.3, -100, 0.6, 8);
        painter->restore();
    }
    painter->setPen(QColor(200,200,200));

    if(_numbers != 0) {
        for (float i = 0 ; i < _range1; i+=_numbers) {
            painter->save();
            painter->rotate(value2Angle1(i));
            painter->save();
            QString lineNumber = QString::number(i*_numbersScale);
            painter->translate(0,-70);
            painter->rotate(-value2Angle1(i));
            int width = painter->fontMetrics().width(lineNumber);
            int height =painter->fontMetrics().height();
            painter->drawText(-width/2,-height/2,width,height, Qt::AlignCenter,  lineNumber);
            painter->restore();
            painter->restore();
        }
    }

    painter->restore();

    painter->restore();
    PanelItem::paint(painter, option, widget);
}


void Altimeter::setLabel(QString text) {
    _label = text;
}

void Altimeter::setRanges(float r1, float r2) {
  _range1 = r1;
  _range2 = r2;

  update();
}

void Altimeter::setUnit(DistanceUnit unit) {
    units = unit;
    setLabel(Units::unitName(units));
    update();
}

void Altimeter::altChanged(QString name, double alt) {
    double altConverted = Units::convertDistance(DISTANCE_FT, units, alt);
    if(altConverted == _value) return;
    _value = altConverted;
    update();
}

void Altimeter::setBars(float thick, float thin) {
    _thinBars = thin;
    _thickBars = thick;
}

float Altimeter::value2Angle1(float value) {
    return (value / _range1) * 360.0;
}
float Altimeter::value2Angle2(float value) {
    return (value / _range2) * 360.0;
}

QString Altimeter::typeName() {
    return typeNameStatic();
}

QString Altimeter::typeNameStatic() {
    return "indicator/altitude/basic";
}


void Altimeter::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);
    settings.setValue("unit", Units::unitName(units));
}

void Altimeter::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    DistanceUnit unit = Units::distanceUnitForName(unitname);
    setUnit(unit);
}


void Altimeter::createSettings(QGridLayout *layout) {
    QLabel *unitsLabel = new QLabel("Unit", layout->parentWidget());
    layout->addWidget(unitsLabel, layout->rowCount(), 0);
    DistanceUnitComboBox *unitsCombo = new DistanceUnitComboBox(layout->parentWidget(), units);
    connect(unitsCombo, SIGNAL(unitSelected(DistanceUnit)), this, SLOT(setUnit(DistanceUnit)));
    layout->addWidget(unitsCombo);
}
