#include "indicatordisplay.h"

#include <QLabel>
#include <QCheckBox>
#include <QComboBox>

#include "../util/console.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(IndicatorDisplay,"display/indicator")

#define COLOR_WHITE Qt::white
#define COLOR_BLUE QColor(0,161,255)
#define COLOR_GREEN QColor(110,254,2)

IndicatorDisplay::IndicatorDisplay(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        DisplayInstrument(panel,conn) {
    // Init
    _labelOn = "BRAKES";
    _labelOff = "BRAKES";
    _labelColor = COLOR_GREEN;
    _valueColor = COLOR_WHITE;
    _datarefName = "";
    _threshold = 0.1;
    _strengthOn = 100;
    _strengthOff = 0;
    _on = false;
    _valueDivisor = 1;

    // Make connection
    //conn->registerClient(&_client);
    connect(&_client, SIGNAL(refChanged(QString,QString)), this, SLOT(dataRefChanged(QString,QString)));
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(dataRefChanged(QString,double)));

    // Defaults
    setDataRefName("sim/flightmodel/controls/parkbrake");
    setSize(100,30);

}

void IndicatorDisplay::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("datarefname", _datarefName);
    settings.setValue("threshold", _threshold);
    settings.setValue("strengthon", _strengthOn);
    settings.setValue("strengthoff", _strengthOff);
    settings.setValue("labelon", _labelOn);
    settings.setValue("labeloff", _labelOff);
    settings.setValue("labelcolor", _labelColor.name());
    settings.setValue("valuecolor", _valueColor.name());
    settings.setValue("showvalue", _showValue);
    settings.setValue("valuedivisor", _valueDivisor);
}

void IndicatorDisplay::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    setDataRefName(settings.value("datarefname","sim/cockpit/misc/compass_indicated").toString());
    setThreshold(settings.value("threshold","0.1").toDouble());
    setStrengthOn(settings.value("strengthon","100").toInt());
    setStrengthOff(settings.value("strengthoff","20").toInt());
    setLabelOn(settings.value("labelon","BRAKES").toString());
    setLabelOff(settings.value("labeloff","BRAKES").toString());
    setLabelColor(QColor(settings.value("labelcolor","white").toString()));
    setValueColor(QColor(settings.value("valuecolor","white").toString()));
    setShowValue(settings.value("showvalue","false").toString()=="true");
    setValueDivisor(settings.value("valuedivisor","1").toInt());

    DEBUG << _datarefName;
}

void IndicatorDisplay::createSettings(QGridLayout *layout) {
    DisplayInstrument::createSettings(layout);



    // Presets
    // Warning: do not change the order
    layout->addWidget(new QLabel("Load Preset", layout->parentWidget()));
    QComboBox *combobox = new QComboBox(layout->parentWidget());
    combobox->addItem("");
    // On/off types
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
    // Custom types
    combobox->addItem("");
    combobox->addItem("Custom Dataref");
    combobox->addItem("Custom Label");
    // Value types
    combobox->addItem("");
    combobox->addItem("Cabin Alt");
    combobox->addItem("Total Fuel");
    combobox->addItem("Outside Temp");
    combobox->addItem("Cabin Pressure Rate");
    combobox->addItem("Cabin AP Rate");

    // Add to layout and connect
    layout->addWidget(combobox);
    connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPreset(int)));
    connect(combobox, SIGNAL(currentIndexChanged(int)), layout->parentWidget()->window(), SLOT(close())); // This is kindof a hack, but we need to close the window to reflect the changes to the gui

    // Standard settings
    createLineEditSetting(layout,"DataRef",_datarefName,SLOT(setDataRefName(QString)));
    createNumberInputSetting(layout,"Threshold",_threshold,SLOT(setThreshold(float)));
    createSliderSetting(layout,"Strength On",0,100,_strengthOn,SLOT(setStrengthOn(int)));
    createSliderSetting(layout,"Strength Off",0,100,_strengthOff,SLOT(setStrengthOff(int)));
    createLineEditSetting(layout,"Label On",_labelOn,SLOT(setLabelOn(QString)));
    createLineEditSetting(layout,"Label Off",_labelOff,SLOT(setLabelOff(QString)));
    createColorSetting(layout,"Label Color",_labelColor,SLOT(setLabelColor(QColor)));
    createCheckboxSetting(layout,"Show Value",_showValue,SLOT(setShowValue(bool)));
    createColorSetting(layout,"Value Color",_valueColor,SLOT(setValueColor(QColor)));
    createNumberInputSetting(layout,"Value Divisor",_valueDivisor,SLOT(setValueDivisor(float)));
}

void IndicatorDisplay::loadPreset(int val) {
    // Defaults (On/off type)
    _valueColor = COLOR_WHITE;
    _labelColor = COLOR_GREEN;
    _strengthOn = 100;
    _strengthOff = 0;
    _threshold = 0.1;
    _valueDivisor = 1;
    _showValue = false;
    if(val == 1) {
        _labelOn = "BRAKES";
        _labelOff = "BRAKES";
        setDataRefName("sim/flightmodel/controls/parkbrake");
    } else if(val == 2) {
        _labelOn = "GEAR";
        _labelOff = "GEAR";
        setDataRefName("sim/cockpit/switches/gear_handle_status");
    } else if(val == 3) {
        _labelOn = "BATTERY";
        _labelOff = "BATTERY";
        setDataRefName("sim/cockpit/electrical/battery_on");
    } else if(val == 4) {
        _labelOn = "ENGINE FIRE";
        _labelOff = "ENGINE FIRE";
        setDataRefName("sim/cockpit/warnings/annunciators/engine_fire");
    } else if(val == 5) {
        _labelOn = "FUEL PRESSURE";
        _labelOff = "FUEL PRESSURE";
        setDataRefName("sim/cockpit/warnings/annunciators/fuel_pressure");
    } else if(val == 6) {
        _labelOn = "OIL PRESSURE";
        _labelOff = "OIL PRESSURE";
        setDataRefName("sim/cockpit/warnings/annunciators/oil_pressure");
    } else if(val == 7) {
        _labelOn = "OIL TEMP";
        _labelOff = "OIL TEMP";
        setDataRefName("sim/cockpit/warnings/annunciators/oil_temperature");
    } else if(val == 8) {
        _labelOn = "FUEL LOW";
        _labelOff = "FUEL LOW";
        setDataRefName("sim/cockpit/warnings/annunciators/fuel_quantity");
    } else if(val == 9) {
        _labelOn = "ICE";
        _labelOff = "ICE";
        setDataRefName("sim/cockpit/warnings/annunciators/ice");
    } else if(val == 10) {
        _labelOn = "BEACON";
        _labelOff = "BEACON";
        setDataRefName("sim/cockpit/electrical/beacon_lights_on");
    } else if(val == 11) {
        _labelOn = "LANDING";
        _labelOff = "LANDING";
        setDataRefName("sim/cockpit/electrical/landing_lights_on");
    } else if(val == 12) {
        _labelOn = "NAV";
        _labelOff = "NAV";
        setDataRefName("sim/cockpit/electrical/nav_lights_on");
    } else if(val == 13) {
        _labelOn = "TAXI";
        _labelOff = "TAXI";
        setDataRefName("sim/cockpit/electrical/taxi_light_on");
    } else if(val == 14) {
        _labelOn = "STROBE";
        _labelOff = "STROBE";
        setDataRefName("sim/cockpit/electrical/strobe_lights_on");
    } else if(val == 16) {
        _labelOn = "CUSTOM DATAREF";
        _labelOff = "CUSTOM DATAREF";
        setDataRefName("sim/custom");
    } else if(val == 17) {
        _labelOn = "CUSTOM LABEL";
        _labelOff = "CUSTOM LABEL";
        _labelColor = COLOR_BLUE;
        _strengthOn = 100;
        _strengthOff = 100;
        setDataRefName("sim/custom");
    } else if(val == 19) {
        _labelOn = "CAB ALT";
        _labelOff = "CAB ALT";
        _strengthOn = 100;
        _strengthOff = 100;
        _labelColor = COLOR_BLUE;
        _valueColor = COLOR_WHITE;
        _showValue = true;
        setDataRefName("sim/cockpit2/pressurization/indicators/cabin_altitude_ft");
    } else if(val == 19) {
        _labelOn = "LNG ALT";
        _labelOff = "LNG ALT";
        _strengthOn = 100;
        _strengthOff = 100;
        _labelColor = COLOR_BLUE;
        _valueColor = COLOR_WHITE;
        _showValue = true;
        setDataRefName("sim/cockpit2/pressurization/indicators/cabin_altitude_ft");
    } else if(val == 20) {
        _labelOn = "TOTAL FUEL";
        _labelOff = "TOTAL FUEL";
        _strengthOn = 100;
        _strengthOff = 100;
        _labelColor = COLOR_BLUE;
        _valueColor = COLOR_WHITE;
        _showValue = true;
        _valueDivisor = 10000;
        setDataRefName("sim/flightmodel/weight/m_fuel_total");
    } else if(val == 21) {
        _labelOn = "TEMP";
        _labelOff = "TEMP";
        _strengthOn = 100;
        _strengthOff = 100;
        _labelColor = COLOR_BLUE;
        _valueColor = COLOR_WHITE;
        _showValue = true;
        setDataRefName("sim/cockpit2/temperature/outside_air_temp_degc");
    } else if(val == 22) {
        _labelOn = "RATE";
        _labelOff = "RATE";
        _strengthOn = 100;
        _strengthOff = 100;
        _labelColor = COLOR_BLUE;
        _valueColor = COLOR_WHITE;
        _showValue = true;
        setDataRefName("sim/cockpit/pressure/cabin_vvi_set_m_msec");
    } else if(val == 23) {
        _labelOn = "AP";
        _labelOff = "AP";
        _strengthOn = 100;
        _strengthOff = 100;
        _labelColor = COLOR_BLUE;
        _valueColor = COLOR_WHITE;
        _showValue = true;
        setDataRefName("sim/cockpit/pressure/cabin_vvi_actual_m_msec");
    }
}

void IndicatorDisplay::setDataRefName(QString name) {

    // Unsubscribe old
    if(_datarefName != "" && _client.isDataRefSubscribed(_datarefName)) _client.unsubscribeDataRef(_datarefName); //TODO: there seems to be something wrong with unsubscribing...
    _datarefName = name;
    _datarefValue = "";

    // Subscribe new
    _client.subscribeDataRef(name, 0);
}

void IndicatorDisplay::dataRefChanged(QString name, QString val) {
    if(name != _datarefName) return;

    _datarefValue = val;
    //update();
}

void IndicatorDisplay::dataRefChanged(QString name, double val) {
    if(name != _datarefName) return;

    // On or off?
    //DEBUG << name << val;
    int valInt = (int)val;
    if(_valueDivisor > 1) valInt = valInt/_valueDivisor;
    if(_showValue) _datarefValue = QString("%1").arg(valInt);

    // Set on/off based on threshhold
    bool newOn = (val > _threshold);

    // Override if we have no avionics power
    if(!this->panel()->hasAvionicsPower) newOn = false;

    // Update if diff
    if(_on != newOn) {
        _on = newOn;
        //update();
    }
}

void IndicatorDisplay::render(QPainter *painter, int width, int height) {

    painter->save(); {

        // Painter init
        QFont font = defaultFont;
        font.setBold(true);
        font.setPixelSize(height*0.7+itemFontSize());
        painter->setFont(font);


        // Init

        // Setup label
        int strengh = _strengthOn;
        QString label = _labelOn;
        if(!_on) {
            strengh = _strengthOff;
            label = _labelOff;
        }
        painter->setOpacity(strengh/100.0);

        // Value?
        int labelAlign = Qt::AlignCenter | Qt::AlignVCenter;
        if(_showValue) {
            labelAlign = Qt::AlignLeft | Qt::AlignVCenter;
            painter->setPen(_valueColor);
            painter->drawText(QRect(0,0,width, height), Qt::AlignRight | Qt::AlignVCenter, _datarefValue);
            DEBUG << _datarefValue;
        }

        // Draw label
        painter->setPen(_labelColor);
        painter->drawText(QRect(0,0,width, height), labelAlign, label);

    } painter->restore();

}




