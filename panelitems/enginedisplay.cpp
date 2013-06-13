#include "enginedisplay.h"

#include <QLabel>
#include <QCheckBox>

#include "util/console.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(EngineDisplay,"display/engines")

#define DATAREF_RPM "sim/cockpit2/engine/indicators/engine_speed_rpm"
#define DATAREF_THROTTLE "sim/cockpit2/engine/indicators/engine_speed_throttle"

EngineDisplay::EngineDisplay(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        DisplayInstrument(panel,conn) {
    _engineCount = 0;
    _client.subscribeDataRef(DATAREF_RPM, 15.0);
    _client.subscribeDataRef(DATAREF_THROTTLE, 15.0);
    connect(&_client, SIGNAL(refChanged(QString,QString)), this, SLOT(refChanged(QString,QString)));

}


void EngineDisplay::refChanged(QString name, QString valueString) {
    if(name == DATAREF_RPM) {
        _rpmValues = valueString.split(" ", QString::SkipEmptyParts);
    } else if(name == DATAREF_THROTTLE) {
        _throttleValues = valueString.split(" ", QString::SkipEmptyParts);
    }
}

void EngineDisplay::render(QPainter *painter, int width, int height) {

    painter->save(); {

        // Painter init
        painter->setFont(this->defaultFont);

        // Geometry
        _engineCount = qMin(_rpmValues.count(),_throttleValues.count());
        double gaugeSpacing = width / (_engineCount*2.0);
        double gaugeWidth = gaugeSpacing * 0.4;
        double gaugeHeight = height;
        double xx = 0;
        double yy = 0;

        // Draw throttles
        if(true) {
            for(int i = 0; i < _engineCount; i++) {
                // Grab value
                double value = _throttleValues.at(i).toDouble();
                double maxValue = 10000.0;

                // Draw
                drawVerticalBarGauge(painter,xx,yy,gaugeWidth,gaugeHeight,value,maxValue,0,100,false,6);
                xx += gaugeSpacing;
            }
        }

        // Draw rpm
        if(true) {
            for(int i = 0; i < _engineCount; i++) {
                // Grab value
                double value = _rpmValues.at(i).toDouble();
                double maxValue = 60000.0;


                // Draw
                drawVerticalBarGauge(painter,xx,yy,gaugeWidth,gaugeHeight,value,maxValue,1.0,2.0,true,6);
                xx += gaugeSpacing;
            }
        }

    } painter->restore();

}

void EngineDisplay::storeSettings(QSettings &settings) {
    DisplayInstrument::storeSettings(settings);
}

void EngineDisplay::loadSettings(QSettings &settings) {
    DisplayInstrument::loadSettings(settings);
}

void EngineDisplay::createSettings(QGridLayout *layout) {
    DisplayInstrument::createSettings(layout);
}

