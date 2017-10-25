#include "altimeter.h"
#include <QLabel>
#include "../widgets/distanceunitcombobox.h"
#include "extplaneclient.h"
#include "../widgets/numberinputlineedit.h"
#include "../needles/gabalancedneedle.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(Altimeter,"indicator/altitude/basic");

Altimeter::Altimeter(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
        _client(this, typeName(), conn) {
    // Init
    _value = 0;
    _thickBars = 50;
    _thinBars = 10;
    _range1 = 500;
    _range2 = 5000;
    _numbers = 50;
    _numbersScale = 0.01;
    _baroPressure = 1013.25;
    units = DISTANCE_M;
    baroUnits = PRESSURE_HPA;
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));
    _client.subscribeDataRef("sim/flightmodel/misc/h_ind", 1);
    _client.subscribeDataRef("sim/cockpit2/gauges/actuators/barometer_setting_in_hg_pilot");
    GABalancedNeedle *sn = new GABalancedNeedle(this);
    sn->setNeedleLength(0.6);
    shortNeedle = sn;
    longNeedle = new GABalancedNeedle(this);
    repaintPixmap();
}

void Altimeter::setNumbers(float div) {
    _numbers = div;
    repaintPixmap();
}

void Altimeter::repaintPixmap() {
    // Paint bottom pixmap
    bottomPixmap = QPixmap(width(),height());
    bottomPixmap.fill(Qt::transparent);

    QPainter painter(&bottomPixmap);
    painter.setFont(defaultFont);

    double side = qMin(width(), height());

    pressureWindow = QRect(side * 0.7, side * 0.45, side*0.2, side*0.1);

    painter.setRenderHint(QPainter::Antialiasing);

    // Pressure window
    painter.setPen(Qt::white);
    painter.setBrush(Qt::black);
    painter.drawRect(pressureWindow);

    painter.save();
    painter.scale(side / 200.0, side / 200.0);
    painter.save();
    painter.translate(100, 100);

    if(!_label.isEmpty()) {
        int textwidth = painter.fontMetrics().width(_label);
        painter.drawText(-textwidth/2,-70, textwidth, 200, Qt::AlignCenter, _label);
    }
    int textwidth = painter.fontMetrics().width("Altitude");
    painter.drawText(-textwidth/2,-130, textwidth, 200, Qt::AlignCenter, "Altitude");


    painter.setFont(defaultFont);
    painter.setBrush(Qt::white);
    if(_thickBars > 0) {
        for (float i = 0 ; i <= _range1; i+=_thickBars) {
            painter.save();
            painter.rotate(value2Angle1(i));
            painter.drawRect(-1, -100, 2, 14);
            painter.restore();
        }
    }
    if(_thinBars > 0) {
        for (float i = 0 ; i <= _range2; i+=_thinBars) {
            painter.save();
            painter.rotate(value2Angle1(i));
            painter.drawRect(-0.3, -100, 0.6, 8);
            painter.restore();
        }
    }
    painter.setPen(QColor(200,200,200));

    if(_numbers != 0) {
        for (float i = 0 ; i < _range1; i+=_numbers) {
            painter.save();
            painter.rotate(value2Angle1(i));
            painter.save();
            QString lineNumber = QString::number(i*_numbersScale);
            painter.translate(0,-70);
            painter.rotate(-value2Angle1(i));
            int width = painter.fontMetrics().width(lineNumber);
            int height =painter.fontMetrics().height();
            painter.drawText(-width/2,-height/2,width,height, Qt::AlignCenter,  lineNumber);
            painter.restore();
            painter.restore();
        }
    }

    painter.restore();

    painter.restore();
    update();
}

void Altimeter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    setupPainter(painter);
    painter->drawPixmap(0,0, bottomPixmap);

    double side = qMin(width(), height());

    painter->save();
    painter->setBrush(Qt::white);
    painter->setPen(Qt::white);

    // Pressure text
    QFont pressureFont = defaultFont;
    pressureFont.setPointSizeF(defaultFont.pointSizeF() * side/300.f);
    painter->setFont(pressureFont);
    QString pressureText = QString::number(_baroPressure);

    painter->drawText(pressureWindow, Qt::AlignRight | Qt::AlignVCenter, pressureText);

    painter->restore();

    // Draw needles
    painter->save();
    painter->translate(side/2,side/2);
    painter->scale(side/2,side/2);
    painter->save();
    painter->rotate(value2Angle2(_value));
    shortNeedle->paint(painter);
    painter->restore();
    painter->save();
    painter->rotate(value2Angle1(_value));
    longNeedle->paint(painter);
    painter->restore();
    painter->restore();

    PanelItem::paint(painter, option, widget);
}


void Altimeter::setLabel(QString text) {
    _label = text;
    repaintPixmap();
}

void Altimeter::setRange1(float r1) {
    _range1 = qMax(r1, 1.0f);
    repaintPixmap();
}

void Altimeter::setRange2(float r2) {
    _range2 = qMax(r2, 1.0f);
    repaintPixmap();
}

void Altimeter::setUnit(DistanceUnit unit) {
    units = unit;
    setLabel(Units::unitName(units));
    repaintPixmap();
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

void Altimeter::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);
    settings.setValue("unit", Units::unitName(units));
    settings.setValue("range1", _range1);
    settings.setValue("range2", _range2);
    settings.setValue("thinbars", _thinBars);
    settings.setValue("thickbars", _thickBars);
    settings.setValue("numbers", _numbers);
    settings.setValue("numbersscale", _numbersScale);
}

void Altimeter::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    DistanceUnit unit = Units::distanceUnitForName(unitname);
    setUnit(unit);
    setRange1(settings.value("range1", 500).toDouble());
    setRange2(settings.value("range2", 5000).toDouble());
    setThinBars(settings.value("thinbars", 10).toDouble());
    setThickBars(settings.value("thickbars", 50).toDouble());
    setNumbers(settings.value("numbers", 100).toDouble());
    setNumbersScale(settings.value("numbersscale", 0.01).toDouble());
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
    repaintPixmap();
}

void Altimeter::setThinBars(float v) {
    _thinBars = v;
    repaintPixmap();
}

void Altimeter::setNumbersScale(float v) {
    _numbersScale = v;
    repaintPixmap();
}

void Altimeter::itemSizeChanged(float w, float h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
    repaintPixmap();
}
