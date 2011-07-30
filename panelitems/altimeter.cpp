#include "altimeter.h"

Altimeter::Altimeter(QObject *parent, ExtPlaneConnection *conn) :
        PanelItem(parent), _client(this, typeName(), conn)
{
    _value = 0;
    setThickBars(50);
    setThinBars(10);
    setRange1(500);
    setRange2(5000);
    setNumbers(50);
    setNumbersScale(0.01);
    _baroPressure = 1013.25;
    _bezel = QPixmap::fromImage(QImage(QString("../../images/bezel_square_.png")), Qt::AutoColor);

    units = DISTANCE_M;
    baroUnits = PRESSURE_HPA;
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));
    _client.subscribeDataRef("sim/flightmodel/misc/h_ind", 3);
    _client.subscribeDataRef("sim/cockpit2/gauges/actuators/barometer_setting_in_hg_pilot");
}

void Altimeter::setNumbers(float div) {
    _numbers = div;
    update();
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

    painter->setBrush(Qt::black);
    painter->drawChord(-100,-100,200,200,0,360*16);
    
    painter->save();
    painter->scale(0.90, 0.90); // Scale to draw within bezel
    
    painter->setPen(Qt::white);
    if(!_label.isEmpty()) {
        int textwidth = painter->fontMetrics().width(_label);
        painter->drawText(-textwidth/2,-70, textwidth, 200, Qt::AlignCenter, _label);
    }
    int textwidth = painter->fontMetrics().width("Altitude");
    painter->drawText(-textwidth/2,-130, textwidth, 200, Qt::AlignCenter, "Altitude");

    painter->setBrush(Qt::black);
    painter->drawRect(30,-10,60,20);
    QString pressureText = QString::number(_baroPressure);
    textwidth = painter->fontMetrics().width(pressureText);
    painter->drawText(30,-10, 60, 20, Qt::AlignRight | Qt::AlignVCenter, pressureText);

    painter->setBrush(Qt::white);
    if(_thickBars > 0) {
        for (float i = 0 ; i <= _range1; i+=_thickBars) {
            painter->save();
            painter->rotate(value2Angle1(i));
            painter->drawRect(-1, -100, 2, 14);
            painter->restore();
        }
    }
    if(_thinBars > 0) {
        for (float i = 0 ; i <= _range2; i+=_thinBars) {
            painter->save();
            painter->rotate(value2Angle1(i));
            painter->drawRect(-0.3, -100, 0.6, 8);
            painter->restore();
        }
    }
    painter->setPen(QColor(200,200,200));

    if(_numbers != 0) {
        for (float i = 0 ; i < _range1; i+=_numbers) {
            painter->save();
            painter->rotate(value2Angle1(i));
            painter->save();
            QString lineNumber = QString::number(i*_numbersScale);
            painter->translate(0,-73);
            painter->rotate(-value2Angle1(i));
            
            painter->setPen(QColor(200,200,200));
            painter->setFont(QFont(QString("Helvetica"), 16, QFont::Bold, false));
            
            int width = painter->fontMetrics().width(lineNumber);
            int height =painter->fontMetrics().height();
            painter->drawText(-width/2,-height/2,width,height, Qt::AlignCenter,  lineNumber);
            painter->restore();
            painter->restore();
        }
    }
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
    painter->setBrush(Qt::white);

    painter->restore();
    
    painter->drawPixmap(-100,-100,200,200,_bezel);
    
    painter->restore();

    painter->restore();

    PanelItem::paint(painter, option, widget);
}


void Altimeter::setLabel(QString text) {
    _label = text;
}

void Altimeter::setRange1(float r1) {
    _range1 = qMax(r1, 1.0f);
    update();
}

void Altimeter::setRange2(float r2) {
    _range2 = qMax(r2, 1.0f);
    update();
}

void Altimeter::setUnit(DistanceUnit unit) {
    units = unit;
    setLabel(Units::unitName(units));
    update();
}

void Altimeter::refChanged(QString name, double alt) {
    if(name=="sim/flightmodel/misc/h_ind") {
        double altConverted = Units::convertDistance(DISTANCE_FT, units, alt);
        if(altConverted == _value) return;
        _value = altConverted;
    } else if(name=="sim/cockpit2/gauges/actuators/barometer_setting_in_hg_pilot") {
        double pressureConverted = Units::convertPressure(PRESSURE_INCHES_HG, baroUnits, alt);
        if(_baroPressure == pressureConverted) return;
        _baroPressure = pressureConverted;
    }
    update();
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
    settings.setValue("range1", QString::number(_range1));
    settings.setValue("range2", QString::number(_range2));
    settings.setValue("thinbars", QString::number(_thinBars));
    settings.setValue("thickbars", QString::number(_thickBars));
    settings.setValue("numbers", QString::number(_numbers));
    settings.setValue("numbersscale", QString::number(_numbersScale));
}
void Altimeter::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    DistanceUnit unit = Units::distanceUnitForName(unitname);
    setUnit(unit);
    setRange1(settings.value("range1", 500).toFloat());
    setRange2(settings.value("range2", 5000).toFloat());
    setThinBars(settings.value("thinbars", 10).toFloat());
    setThickBars(settings.value("thickbars", 50).toFloat());
    setNumbers(settings.value("numbers", 100).toFloat());
    setNumbersScale(settings.value("numbersscale", 0.01).toFloat());
}

void Altimeter::createSettings(QGridLayout *layout) {
    QLabel *unitsLabel = new QLabel("Unit", layout->parentWidget());
    layout->addWidget(unitsLabel, layout->rowCount(), 0);
    DistanceUnitComboBox *unitsCombo = new DistanceUnitComboBox(layout->parentWidget(), units);
    connect(unitsCombo, SIGNAL(unitSelected(DistanceUnit)), this, SLOT(setUnit(DistanceUnit)));
    layout->addWidget(unitsCombo);
    QLabel *range1Label = new QLabel("Range", layout->parentWidget());
    layout->addWidget(range1Label);
    NumberInputLineEdit *range1Edit = new NumberInputLineEdit(layout->parentWidget());
    range1Edit->setText(QString::number(_range1));
    layout->addWidget(range1Edit);
    connect(range1Edit, SIGNAL(valueChangedFloat(float)), this, SLOT(setRange1(float)));

    QLabel *range2Label = new QLabel("Range 2", layout->parentWidget());
    layout->addWidget(range2Label);
    NumberInputLineEdit *range2Edit = new NumberInputLineEdit(layout->parentWidget());
    range2Edit->setText(QString::number(_range2));
    layout->addWidget(range2Edit);
    connect(range2Edit, SIGNAL(valueChangedFloat(float)), this, SLOT(setRange2(float)));

    QLabel *thinBarsLabel = new QLabel("Thin bars every", layout->parentWidget());
    layout->addWidget(thinBarsLabel);
    NumberInputLineEdit *thinBarsEdit = new NumberInputLineEdit(layout->parentWidget());
    thinBarsEdit->setText(QString::number(_thinBars));
    layout->addWidget(thinBarsEdit);
    connect(thinBarsEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setThinBars(float)));

    QLabel *thickBarsLabel = new QLabel("Thick bars every", layout->parentWidget());
    layout->addWidget(thickBarsLabel);
    NumberInputLineEdit *thickBarsEdit = new NumberInputLineEdit(layout->parentWidget());
    thickBarsEdit->setText(QString::number(_thickBars));
    layout->addWidget(thickBarsEdit);
    connect(thickBarsEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setThickBars(float)));

    layout->addWidget(new QLabel("Numbers every", layout->parentWidget()));
    NumberInputLineEdit *numbersEdit = new NumberInputLineEdit(layout->parentWidget());
    numbersEdit->setText(QString::number(_numbers));
    layout->addWidget(numbersEdit);
    connect(numbersEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setNumbers(float)));

    layout->addWidget(new QLabel("Number scale", layout->parentWidget()));
    NumberInputLineEdit *numbersScaleEdit = new NumberInputLineEdit(layout->parentWidget());
    numbersScaleEdit->setText(QString::number(_numbersScale));
    layout->addWidget(numbersScaleEdit);
    connect(numbersScaleEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setNumbersScale(float)));
}

void Altimeter::setThickBars(float v){
    _thickBars = v;
    update();
}

void Altimeter::setThinBars(float v) {
    _thinBars = v;
    update();
}

void Altimeter::setNumbersScale(float v) {
    _numbersScale = v;
    update();
}
