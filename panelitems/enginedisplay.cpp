#include "enginedisplay.h"

#include <QLabel>
#include <QCheckBox>

#include "util/console.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(EngineDisplay,"display/engines")

#define DATAREF_NUMENGINES "sim/aircraft/engine/acf_num_engines"
#define DATAREF_N1 "sim/cockpit2/engine/indicators/N1_percent"
#define DATAREF_EPR "sim/cockpit2/engine/indicators/EPR_ratio"
#define DATAREF_EGT "sim/cockpit2/engine/indicators/EGT_deg_C"

EngineDisplay::EngineDisplay(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        DisplayInstrument(panel,conn) {
    // Init
    _barLabels = 6;
    _engineCount = 0;

    _n1Enabled = true;
    _n1DatarefMinimum = 0.0;
    _n1DatarefMaximum = 100;
    _n1RangeMinimum = 0.0;
    _n1RangeMaximum = 100.0;
    _n1Color = Qt::green;

    _eprEnabled = true;
    _eprDatarefMinimum = 1.0;
    _eprDatarefMaximum = 2.0;
    _eprRangeMinimum = 1.0;
    _eprRangeMaximum = 2.0;
    _eprColor = Qt::blue;

    _egtEnabled = true;
    _egtDatarefMinimum = 0.0;
    _egtDatarefMaximum = 700.0;
    _egtRangeMinimum = 0.0;
    _egtRangeMaximum = 700.0;
    _egtColor = Qt::yellow;

    // Connect
    _client.subscribeDataRef(DATAREF_N1, 1.0);
    _client.subscribeDataRef(DATAREF_EPR, 0.01);
    _client.subscribeDataRef(DATAREF_EGT, 1.0);
    _client.subscribeDataRef(DATAREF_NUMENGINES, 0.0);
    connect(&_client, SIGNAL(refChanged(QString,QStringList)), this, SLOT(refChanged(QString,QStringList)));
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));

}

void EngineDisplay::refChanged(QString name, double value) {
    if(name == DATAREF_NUMENGINES) {
        _engineCount = (int)value;
    }
}

void EngineDisplay::refChanged(QString name, QStringList values) {
    if(name == DATAREF_N1) {
        _n1Values = values;
    } else if(name == DATAREF_EPR) {
        _eprValues = values;
    } else if(name == DATAREF_EGT) {
        _egtValues = values;
    }
}

void EngineDisplay::render(QPainter *painter, int width, int height) {
    if(_engineCount <= 0) return;

    painter->save(); {

        // Painter init
        painter->setFont(this->defaultFont);

        // Geometry
        double gaugeSets = 0;
        if(_n1Enabled) gaugeSets += 1;
        if(_eprEnabled) gaugeSets += 1;
        if(_egtEnabled) gaugeSets += 1;
        double paddingLeft = 15;
        double gaugeSpacing = (width-paddingLeft) / (_engineCount*gaugeSets);
        double gaugeWidth = gaugeSpacing * 0.4;
        double gaugeHeight = height;
        double xx = paddingLeft;
        double yy = 0;

        // Draw N1
        if(_n1Enabled) {
            for(int i = 0; i < _engineCount; i++) {
                // Grab value
                double value = _n1Values.at(i).toDouble();
                // Draw
                drawVerticalBarGauge(painter,_n1Color,xx,yy,gaugeWidth,gaugeHeight,value,_n1DatarefMinimum,_n1DatarefMaximum,_n1RangeMinimum,_n1RangeMaximum,false,_barLabels);
                xx += gaugeSpacing;
            }
        }

        // Draw EPR
        if(_eprEnabled) {
            for(int i = 0; i < _engineCount; i++) {
                // Grab value
                double value = _eprValues.at(i).toDouble();
                // Draw
                drawVerticalBarGauge(painter,_eprColor,xx,yy,gaugeWidth,gaugeHeight,value,_eprDatarefMinimum,_eprDatarefMaximum,_eprRangeMinimum,_eprRangeMaximum,true,_barLabels);
                xx += gaugeSpacing;
            }
        }

        // Draw EGT
        if(_egtEnabled) {
            for(int i = 0; i < _engineCount; i++) {
                // Grab value
                double value = _egtValues.at(i).toDouble();
                // Draw
                drawVerticalBarGauge(painter,_egtColor,xx,yy,gaugeWidth,gaugeHeight,value,_egtDatarefMinimum,_egtDatarefMaximum,_egtRangeMinimum,_egtRangeMaximum,true,_barLabels);
                xx += gaugeSpacing;
            }
        }

    } painter->restore();

}

void EngineDisplay::storeSettings(QSettings &settings) {
    DisplayInstrument::storeSettings(settings);

    settings.setValue("barLabels", _barLabels);
}

void EngineDisplay::loadSettings(QSettings &settings) {
    DisplayInstrument::loadSettings(settings);

    setBarLabels(settings.value("barLabels","6").toInt());
}

void EngineDisplay::createGaugeSetSettings(QGridLayout *layout, QString name, bool enabled, double dataRefMin, double dataRefMax, double rangeMin, double rangeMax, QColor color) {
    createCheckboxSetting(layout,name+" Enabled",enabled,QString("1set"+name+"Enabled(bool)").toStdString().c_str());
    createNumberInputSetting(layout,name+" DataRef Min",dataRefMin,QString("1set"+name+"DatarefMinimum(float)").toStdString().c_str());
    createNumberInputSetting(layout,name+" DataRef Max",dataRefMax,QString("1set"+name+"DatarefMaximum(float)").toStdString().c_str());
    createNumberInputSetting(layout,name+" Range Min",rangeMin,QString("1set"+name+"RangeMinimum(float)").toStdString().c_str());
    createNumberInputSetting(layout,name+" Range Max",rangeMax,QString("1set"+name+"RangeMaximum(float)").toStdString().c_str());
    createColorSetting(layout,name+" Color",color,QString("1set"+name+"Color(QColor)").toStdString().c_str());
}

void EngineDisplay::createSettings(QGridLayout *layout) {
    DisplayInstrument::createSettings(layout);

    createSliderSetting(layout,"Number of Labels",0,11,_barLabels,SLOT(setBarLabels(int)));

    createGaugeSetSettings(layout,"N1",_n1Enabled,_n1DatarefMinimum,_n1DatarefMaximum,_n1RangeMinimum,_n1RangeMaximum,_n1Color);
    createGaugeSetSettings(layout,"EPR",_eprEnabled,_eprDatarefMinimum,_eprDatarefMaximum,_eprRangeMinimum,_eprRangeMaximum,_eprColor);
    createGaugeSetSettings(layout,"EGT",_egtEnabled,_egtDatarefMinimum,_egtDatarefMaximum,_egtRangeMinimum,_egtRangeMaximum,_egtColor);

}

