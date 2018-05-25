#include "indicatorlight.h"

#include <QLineEdit>
#include <QLabel>
#include <QGraphicsScene>
#include <QImage>
#include <QBitmap>
#include <QComboBox>

#include "../util/console.h"
#include "extplaneclient.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(IndicatorLight,"indicator/light/basic")

IndicatorLight::IndicatorLight(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        PanelItem(panel, PanelItemTypeDisplay, PanelItemShapeRectangular),
        _client(this, typeName(), conn) {

    // Init
    _labelOn = "BRAKES";
    _labelOff = "BRAKES";
    _labelColor = Qt::red;
    _datarefName = "";
    _threshold = 0.1;
    _strengthOn = 100;
    _strengthOff = 20;
    _on = false;
    _labelGlowItem = NULL;

    // Make connection
    conn->registerClient(&_client);
    connect(&_client, SIGNAL(refChanged(QString,QString)), this, SLOT(dataRefChanged(QString,QString)));
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(dataRefChanged(QString,double)));

    // Defaults
    setGlowStrength(80);
    setDataRefName("sim/flightmodel/controls/parkbrake");
    setSize(200,60);
}

IndicatorLight::~IndicatorLight() {
    if(_labelGlowItem && this->scene()) {
        _labelGlowItem->scene()->removeItem(_labelGlowItem);
    }
}

void IndicatorLight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    // Init painter
    setupPainter(painter);
    painter->save(); {

        // Draw the dataref name and value
        painter->setBrush(Qt::NoBrush);
        painter->setPen(_labelColor);
        QFont font = defaultFont;
        font.setBold(true);
        font.setPixelSize(height()*0.7+itemFontSize());
        painter->setFont(font);
        if(_on) {
            painter->setOpacity(_strengthOn/100.0);
            painter->drawText(QRect(0,0,width(), height()), Qt::AlignCenter, _labelOn);
        } else {
            painter->setOpacity(_strengthOff/100.0);
            painter->drawText(QRect(0,0,width(), height()), Qt::AlignCenter, _labelOff);
        }

    } painter->restore();

    // Update the glow if enabled
    if(_glowEnabled && _labelGlowItem) {
        _labelGlowItem->setVisible(_on);
    }

    PanelItem::paint(painter, option, widget);
}

void IndicatorLight::createLabel(int w, int h) {
    if(_glowEnabled && this->scene()) {

        // Draw label glow
        QPixmap pixmap = QPixmap(w,h);
        pixmap.fill(Qt::transparent);
        QPainter painter;
        painter.begin(&pixmap); {
            setupPainter(&painter);
            painter.setBrush(_labelColor);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(0,0,w,h,20,20);
        } painter.end();

        // Setup the graphics item for glow
        // This has a special z-value ontop of other graphics items so that it can glow above the panel cover...
        if(_labelGlowItem) {
            this->scene()->removeItem(_labelGlowItem);
        }
        _labelGlowItem = new QGraphicsPixmapItem(NULL);
        _labelGlowItem->setPixmap(pixmap);
        _labelGlowItem->setOpacity(_glowStrength/100.0);
        _labelGlowItem->setX(this->x());
        _labelGlowItem->setY(this->y());
        _labelGlowItem->setZValue(PANEL_PANELITEM_GLOW_ZVALUE);
        QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
        effect->setBlurRadius(40);
        _labelGlowItem->setGraphicsEffect(effect);
        this->scene()->addItem(_labelGlowItem);
    }
}

void IndicatorLight::itemSizeChanged(float w, float h) {
    createLabel(w,h);
}

void IndicatorLight::settingChanged() {
    createLabel(width(),height());
    update();
}

void IndicatorLight::setPos(int x, int y) {
    PanelItem::setPos(x,y);
    if(_glowEnabled && _labelGlowItem) {
        _labelGlowItem->setX(x);
        _labelGlowItem->setY(y);
    }
}

void IndicatorLight::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("datarefname", _datarefName);
    settings.setValue("threshold", _threshold);
    settings.setValue("strengthon", _strengthOn);
    settings.setValue("strengthoff", _strengthOff);
    settings.setValue("labelon", _labelOn);
    settings.setValue("labeloff", _labelOff);
    settings.setValue("labelcolor", _labelColor.name());
    settings.setValue("glowstrength", _glowStrength);
}

void IndicatorLight::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    setDataRefName(settings.value("datarefname","sim/cockpit/misc/compass_indicated").toString());
    setThreshold(settings.value("threshold","0.1").toDouble());
    setStrengthOn(settings.value("strengthon","100").toInt());
    setStrengthOff(settings.value("strengthoff","20").toInt());
    setLabelOn(settings.value("labelon","BRAKES").toString());
    setLabelOff(settings.value("labeloff","BRAKES").toString());
    setLabelColor(QColor(settings.value("labelcolor","red").toString()));
    setGlowStrength(settings.value("glowstrength","80").toInt());

    DEBUG << _datarefName;
}

void IndicatorLight::createSettings(QGridLayout *layout) {
    PanelItem::createSettings(layout);

    // Standard settings
    createLineEditSetting(layout,"DataRef",_datarefName,SLOT(setDataRefName(QString)));
    createNumberInputSetting(layout,"Threshold",_threshold,SLOT(setThreshold(float)));
    createSliderSetting(layout,"Strength On",0,100,_strengthOn,SLOT(setStrengthOn(int)));
    createSliderSetting(layout,"Strength Off",0,100,_strengthOff,SLOT(setStrengthOff(int)));
    createLineEditSetting(layout,"Label On",_labelOn,SLOT(setLabelOn(QString)));
    createLineEditSetting(layout,"Label Off",_labelOff,SLOT(setLabelOff(QString)));
    createColorSetting(layout,"Label Color",_labelColor,SLOT(setLabelColor(QColor)));
    createSliderSetting(layout,"Label Glow",0,100,_glowStrength,SLOT(setGlowStrength(int)));

    // Presets
    layout->addWidget(new QLabel("Load Preset", layout->parentWidget()));
    QComboBox *combobox = new QComboBox(layout->parentWidget());
    combobox->addItem("");
    combobox->addItem("Brakes");
    combobox->addItem("Gear");
    combobox->addItem("Battery");
    combobox->addItem("Engine Fire");
    combobox->addItem("Fuel Pressure");
    combobox->addItem("Oil Pressure");
    combobox->addItem("Oil Temperature");
    combobox->addItem("Fuel Low");
    combobox->addItem("Ice Detected");
    combobox->addItem("Beacon Lights");
    combobox->addItem("Landing Lights");
    combobox->addItem("Navigation Lights");
    combobox->addItem("Taxi Lights");
    combobox->addItem("Strobe Lights");
    layout->addWidget(combobox);
    connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPreset(int)));
    connect(combobox, SIGNAL(currentIndexChanged(int)), layout->parentWidget()->window(), SLOT(close())); // This is kindof a hack, but we need to close the window to reflect the changes to the gui

}

void IndicatorLight::loadPreset(int val) {
    if(val == 1) {
        _labelOn = "BRAKES";
        _labelOff = "BRAKES";
        _labelColor = Qt::red;
        _threshold = 0.1;
        _strengthOn = 100;
        _strengthOff = 20;
        setGlowStrength(80);
        setDataRefName("sim/flightmodel/controls/parkbrake");
    } else if(val == 2) {
        _labelOn = "GEAR";
        _labelOff = "GEAR";
        _labelColor = Qt::white;
        _threshold = 0.1;
        _strengthOn = 100;
        _strengthOff = 20;
        setGlowStrength(50);
        setDataRefName("sim/cockpit/switches/gear_handle_status");
    } else if(val == 3) {
        _labelOn = "BATTERY";
        _labelOff = "BATTERY";
        _labelColor = Qt::white;
        _threshold = 0.1;
        _strengthOn = 50;
        _strengthOff = 20;
        setGlowStrength(50);
        setDataRefName("sim/cockpit/electrical/battery_on");
    } else if(val == 4) {
        _labelOn = "ENGINE FIRE";
        _labelOff = "ENGINE FIRE";
        _labelColor = Qt::red;
        _threshold = 0.1;
        _strengthOn = 100;
        _strengthOff = 20;
        setGlowStrength(80);
        setDataRefName("sim/cockpit/warnings/annunciators/engine_fire");
    } else if(val == 5) {
        _labelOn = "FUEL PRESSURE";
        _labelOff = "FUEL PRESSURE";
        _labelColor = Qt::red;
        _threshold = 0.1;
        _strengthOn = 100;
        _strengthOff = 20;
        setGlowStrength(80);
        setDataRefName("sim/cockpit/warnings/annunciators/fuel_pressure");
    } else if(val == 6) {
        _labelOn = "OIL PRESSURE";
        _labelOff = "OIL PRESSURE";
        _labelColor = Qt::red;
        _threshold = 0.1;
        _strengthOn = 100;
        _strengthOff = 20;
        setGlowStrength(80);
        setDataRefName("sim/cockpit/warnings/annunciators/oil_pressure");
    } else if(val == 7) {
        _labelOn = "OIL TEMP";
        _labelOff = "OIL TEMP";
        _labelColor = Qt::red;
        _threshold = 0.1;
        _strengthOn = 100;
        _strengthOff = 20;
        setGlowStrength(80);
        setDataRefName("sim/cockpit/warnings/annunciators/oil_temperature");
    } else if(val == 8) {
        _labelOn = "FUEL LOW";
        _labelOff = "FUEL LOW";
        _labelColor = Qt::red;
        _threshold = 0.1;
        _strengthOn = 100;
        _strengthOff = 20;
        setGlowStrength(80);
        setDataRefName("sim/cockpit/warnings/annunciators/fuel_quantity");
    } else if(val == 9) {
        _labelOn = "ICE";
        _labelOff = "ICE";
        _labelColor = Qt::red;
        _threshold = 0.1;
        _strengthOn = 100;
        _strengthOff = 20;
        setGlowStrength(80);
        setDataRefName("sim/cockpit/warnings/annunciators/ice");
    } else if(val == 10) {
        _labelOn = "BEACON";
        _labelOff = "BEACON";
        _labelColor = Qt::white;
        _threshold = 0.1;
        _strengthOn = 50;
        _strengthOff = 20;
        setGlowStrength(50);
        setDataRefName("sim/cockpit/electrical/beacon_lights_on");
    } else if(val == 11) {
        _labelOn = "LANDING";
        _labelOff = "LANDING";
        _labelColor = Qt::white;
        _threshold = 0.1;
        _strengthOn = 50;
        _strengthOff = 20;
        setGlowStrength(50);
        setDataRefName("sim/cockpit/electrical/landing_lights_on");
    } else if(val == 12) {
        _labelOn = "NAV";
        _labelOff = "NAV";
        _labelColor = Qt::white;
        _threshold = 0.1;
        _strengthOn = 50;
        _strengthOff = 20;
        setGlowStrength(50);
        setDataRefName("sim/cockpit/electrical/nav_lights_on");
    } else if(val == 13) {
        _labelOn = "TAXI";
        _labelOff = "TAXI";
        _labelColor = Qt::white;
        _threshold = 0.1;
        _strengthOn = 50;
        _strengthOff = 20;
        setGlowStrength(50);
        setDataRefName("sim/cockpit/electrical/taxi_light_on");
    } else if(val == 14) {
        _labelOn = "STROBE";
        _labelOff = "STROBE";
        _labelColor = Qt::white;
        _threshold = 0.1;
        _strengthOn = 50;
        _strengthOff = 20;
        setGlowStrength(50);
        setDataRefName("sim/cockpit/electrical/strobe_lights_on");
    }
}

void IndicatorLight::setDataRefName(QString name) {

    // Unsubscribe old
    if(_datarefName != "" && _client.isDataRefSubscribed(_datarefName)) _client.unsubscribeDataRef(_datarefName); //TODO: there seems to be something wrong with unsubscribing...
    _datarefName = name;
     _datarefValue = 0;

    // Subscribe new
    _client.subscribeDataRef(name, 0);
    createLabel(width(),height());
}

void IndicatorLight::dataRefChanged(QString name, QString val) {
    dataRefChanged(name,val.toDouble());
}

void IndicatorLight::dataRefChanged(QString name, double val) {
    if(name != _datarefName) return;

    // On or off?
    DEBUG << name << val;
    _datarefValue = val;
    bool newOn = (_datarefValue > _threshold);
    if(!this->panel()->hasAvionicsPower) newOn = false;
    if(_on != newOn) {
        _on = newOn;
        update();
    }
}
