#include "variometer.h"
#include <QLabel>
#include <QCheckBox>
#include "../util/units.h"
#include "../widgets/velocityunitcombobox.h"
#include "../widgets/numberinputlineedit.h"
#include "../util/valueinterpolator.h"
#include "../needles/gabalancedneedle.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(Variometer,"indicator/variometer/round");

Variometer::Variometer(ExtPlanePanel *panel, ExtPlaneConnection *conn) : NeedleInstrument(panel),
    _client(this, typeName(), conn), interpolator(0, 3) {
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/flightmodel/position/vh_ind", VARIOMETER_ACCURACY);
    connect(&_client, SIGNAL(refChanged(QString,double)), &interpolator, SLOT(valueChanged(QString,double)));
    connect(&interpolator, SIGNAL(interpolatedValueChanged(QString,double)), this, SLOT(velocityChanged(QString,double)));
    setBars(1, 0.5);
    setNumbers(1);
    setUnit(VELOCITY_MS);
    setMaxValue(5);
    isTotalEnergy = false;
    setIsTotalEnergy(false);
    setNeedle(new GABalancedNeedle(this));
}

void Variometer::velocityChanged(QString name, double speed) {
    Q_UNUSED(name);
    if(isTotalEnergy) {
        setValue(Units::convertSpeed(VELOCITY_FPM, units, speed));
    } else {
        setValue(Units::convertSpeed(VELOCITY_MS, units, speed));
    }
}

void Variometer::setUnit(VelocityUnit unit) {
    units = unit;
    repaintPixmaps();
}

void Variometer::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("unit", Units::unitName(units));
    settings.setValue("maxvalue", maxValue);
    settings.setValue("totalenergy", isTotalEnergy);
}

void Variometer::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    VelocityUnit unit = Units::velocityUnitForName(unitname);
    setUnit(unit);
    setMaxValue(settings.value("maxvalue", 300).toDouble());
    setIsTotalEnergy(settings.value("totalenergy", false).toBool());
}

void Variometer::setMaxValue(float mv) {
    maxValue = mv;
    setScale(180-45, -maxValue, 360+45, maxValue);
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

    QCheckBox *totalCheckbox = new QCheckBox("Total energy", layout->parentWidget());
    totalCheckbox->setChecked(isTotalEnergy);
    layout->addWidget(totalCheckbox);
    connect(totalCheckbox, SIGNAL(clicked(bool)), this, SLOT(setIsTotalEnergy(bool)));
}

void Variometer::setIsTotalEnergy(bool te) {
    if(te == isTotalEnergy) return;
    isTotalEnergy = te;
    if(isTotalEnergy) {
        _client.unsubscribeDataRef("sim/flightmodel/position/vh_ind");
        _client.subscribeDataRef("sim/cockpit2/gauges/indicators/total_energy_fpm", VARIOMETER_ACCURACY_TE);
    } else {
        _client.unsubscribeDataRef("sim/cockpit2/gauges/indicators/total_energy_fpm");
        _client.subscribeDataRef("sim/flightmodel/position/vh_ind", VARIOMETER_ACCURACY);
    }
}

void Variometer::tickTime(double dt, int total) {
    interpolator.tickTime(dt, total);
}
void Variometer::setInterpolationEnabled(bool ie) {
    interpolator.setEnabled(ie);
}

void Variometer::paintTopPixmap() {
    topPixmap = QPixmap(width(),height());
    topPixmap.fill(Qt::transparent);

    QPainter painter(&topPixmap);
    setupPainter(&painter);

    painter.save();
    double side = qMin(width(), height());
    // Round center
    painter.setPen(Qt::NoPen);
    painter.setBrush(darkGrayColor);
    painter.drawEllipse(QPointF(side/2, side/2), side/5, side/5);
    painter.setPen(Qt::white);

    // Unit text
    QTextOption textOption(Qt::AlignCenter);
    QFont unitFont = defaultFont;
    unitFont.setPointSizeF(side/20);
    painter.setFont(unitFont);

    painter.drawText(QRect(side/2 + side/5, 0, side/2 - side/5, side), Units::unitName(units), textOption);

    // Up and down arrows
    QFont arrowFont = defaultFont;
    arrowFont.setPointSizeF(side/10);
    painter.setFont(arrowFont);
    int textstartX = side/2 - side/5;
    int textWidth = side/2.5f;
    int textHeight = side/5.f;
    painter.drawText(QRect(textstartX, textstartX, textWidth, textHeight), QString::fromUtf8("↑"), textOption);
    painter.drawText(QRect(textstartX, textstartX + textHeight, textWidth, textHeight), QString::fromUtf8("↓"), textOption);
    painter.restore();
}
