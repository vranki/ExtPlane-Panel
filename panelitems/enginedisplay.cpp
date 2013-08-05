#include "enginedisplay.h"

#include <QLabel>
#include <QCheckBox>

#include "../util/console.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(EngineDisplay,"display/engines")

#define DATAREF_NUMENGINES "sim/aircraft/engine/acf_num_engines"
#define DATAREF_N1 "sim/cockpit2/engine/indicators/N1_percent"
#define DATAREF_N2 "sim/cockpit2/engine/indicators/N2_percent"
#define DATAREF_EPR "sim/cockpit2/engine/indicators/EPR_ratio"
#define DATAREF_EGT "sim/cockpit2/engine/indicators/EGT_deg_C"
#define DATAREF_FF "sim/cockpit2/engine/indicators/fuel_flow_kg_sec"

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

    _n2Enabled = false;
    _n2DatarefMinimum = 0.0;
    _n2DatarefMaximum = 100;
    _n2RangeMinimum = 0.0;
    _n2RangeMaximum = 100.0;
    _n2Color = Qt::green;

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

    _ffEnabled = true;
    _ffDatarefMinimum = 0.0;
    _ffDatarefMaximum = 1.0;
    _ffRangeMinimum = 0.0;
    _ffRangeMaximum = 1.0;
    _ffColor = Qt::white;

    // Connect
    _client.subscribeDataRef(DATAREF_N1, 1.0);
    _client.subscribeDataRef(DATAREF_EPR, 0.01);
    _client.subscribeDataRef(DATAREF_EGT, 1.0);
    _client.subscribeDataRef(DATAREF_NUMENGINES, 0.0);
    _client.subscribeDataRef(DATAREF_FF, 0.0);
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
    } else if(name == DATAREF_N2) {
        _n2Values = values;
    } else if(name == DATAREF_EPR) {
        _eprValues = values;
    } else if(name == DATAREF_EGT) {
        _egtValues = values;
    } else if(name == DATAREF_FF) {
        _ffValues = values;
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
        if(_n2Enabled) gaugeSets += 1;
        if(_eprEnabled) gaugeSets += 1;
        if(_egtEnabled) gaugeSets += 1;
        if(_ffEnabled) gaugeSets += 1;
        double paddingLeft = 15;
        double gaugeSpacing = (width-paddingLeft) / (_engineCount*gaugeSets);
        double gaugeWidth = gaugeSpacing * 0.4;
        double gaugeHeight = height;
        double xx = paddingLeft;
        double yy = 0;

        // Draw N1
        if(_n1Enabled) {
            for(int i = 0; i < qMin(_engineCount,_n1Values.count()); i++) {
                // Grab value
                double value = _n1Values.at(i).toDouble();
                // Draw
                drawVerticalBarGauge(painter,_n1Color,xx,yy,gaugeWidth,gaugeHeight,value,_n1DatarefMinimum,_n1DatarefMaximum,_n1RangeMinimum,_n1RangeMaximum,false,_barLabels);
                xx += gaugeSpacing;
            }
        }

        // Draw N2
        if(_n2Enabled) {
            for(int i = 0; i < qMin(_engineCount,_n2Values.count()); i++) {
                // Grab value
                double value = _n2Values.at(i).toDouble();
                // Draw
                drawVerticalBarGauge(painter,_n2Color,xx,yy,gaugeWidth,gaugeHeight,value,_n2DatarefMinimum,_n2DatarefMaximum,_n2RangeMinimum,_n2RangeMaximum,false,_barLabels);
                xx += gaugeSpacing;
            }
        }

        // Draw EPR
        if(_eprEnabled) {
            for(int i = 0; i < qMin(_engineCount,_eprValues.count()); i++) {
                // Grab value
                double value = _eprValues.at(i).toDouble();
                // Draw
                drawVerticalBarGauge(painter,_eprColor,xx,yy,gaugeWidth,gaugeHeight,value,_eprDatarefMinimum,_eprDatarefMaximum,_eprRangeMinimum,_eprRangeMaximum,true,_barLabels);
                xx += gaugeSpacing;
            }
        }

        // Draw EGT
        if(_egtEnabled) {
            for(int i = 0; i < qMin(_engineCount,_egtValues.count()); i++) {
                // Grab value
                double value = _egtValues.at(i).toDouble();
                // Draw
                drawVerticalBarGauge(painter,_egtColor,xx,yy,gaugeWidth,gaugeHeight,value,_egtDatarefMinimum,_egtDatarefMaximum,_egtRangeMinimum,_egtRangeMaximum,true,_barLabels);
                xx += gaugeSpacing;
            }
        }

        // Draw FF
        if(_ffEnabled) {
            for(int i = 0; i < qMin(_engineCount,_ffValues.count()); i++) {
                // Grab value
                double value = _ffValues.at(i).toDouble();
                // Draw
                drawVerticalBarGauge(painter,_ffColor,xx,yy,gaugeWidth,gaugeHeight,value,_ffDatarefMinimum,_ffDatarefMaximum,_ffRangeMinimum,_ffRangeMaximum,true,_barLabels);
                xx += gaugeSpacing;
            }
        }

    } painter->restore();

}

void EngineDisplay::storeSettings(QSettings &settings) {
    DisplayInstrument::storeSettings(settings);

    settings.setValue("barLabels", _barLabels);

    settings.setValue("n1enabled",_n1Enabled);
    settings.setValue("n1datarefmin",_n1DatarefMinimum);
    settings.setValue("n1datarefmax",_n1DatarefMaximum);
    settings.setValue("n1rangemin",_n1RangeMinimum);
    settings.setValue("n1rangemax",_n1RangeMaximum);
    settings.setValue("n1color",_n1Color.name());

    settings.setValue("n2enabled",_n2Enabled);
    settings.setValue("n2datarefmin",_n2DatarefMinimum);
    settings.setValue("n2datarefmax",_n2DatarefMaximum);
    settings.setValue("n2rangemin",_n2RangeMinimum);
    settings.setValue("n2rangemax",_n2RangeMaximum);
    settings.setValue("n2color",_n2Color.name());

    settings.setValue("eprenabled",_eprEnabled);
    settings.setValue("eprdatarefmin",_eprDatarefMinimum);
    settings.setValue("eprdatarefmax",_eprDatarefMaximum);
    settings.setValue("eprrangemin",_eprRangeMinimum);
    settings.setValue("eprrangemax",_eprRangeMaximum);
    settings.setValue("eprcolor",_eprColor.name());

    settings.setValue("egtenabled",_egtEnabled);
    settings.setValue("egtdatarefmin",_egtDatarefMinimum);
    settings.setValue("egtdatarefmax",_egtDatarefMaximum);
    settings.setValue("egtrangemin",_egtRangeMinimum);
    settings.setValue("egtrangemax",_egtRangeMaximum);
    settings.setValue("egtcolor",_egtColor.name());

    settings.setValue("ffenabled",_ffEnabled);
    settings.setValue("ffdatarefmin",_ffDatarefMinimum);
    settings.setValue("ffdatarefmax",_ffDatarefMaximum);
    settings.setValue("ffrangemin",_ffRangeMinimum);
    settings.setValue("ffrangemax",_ffRangeMaximum);
    settings.setValue("ffcolor",_ffColor.name());
}

void EngineDisplay::loadSettings(QSettings &settings) {
    DisplayInstrument::loadSettings(settings);

    setBarLabels(settings.value("barLabels","6").toInt());

    setN1Enabled(settings.value("n1enabled","true").toBool());
    setN1DatarefMinimum(settings.value("n1datarefmin","0").toDouble());
    setN1DatarefMaximum(settings.value("n1datarefmax","100").toDouble());
    setN1RangeMinimum(settings.value("n1rangemin","0").toDouble());
    setN1RangeMaximum(settings.value("n1rangemax","100").toDouble());
    setN1Color(QColor(settings.value("n1color","#00FF00").toString()));

    setN1Enabled(settings.value("n2enabled","false").toBool());
    setN1DatarefMinimum(settings.value("n2datarefmin","0").toDouble());
    setN1DatarefMaximum(settings.value("n2datarefmax","100").toDouble());
    setN1RangeMinimum(settings.value("n2rangemin","0").toDouble());
    setN1RangeMaximum(settings.value("n2rangemax","100").toDouble());
    setN1Color(QColor(settings.value("n2color","#00FF00").toString()));

    setEPREnabled(settings.value("eprenabled","true").toBool());
    setEPRDatarefMinimum(settings.value("eprdatarefmin","1.0").toDouble());
    setEPRDatarefMaximum(settings.value("eprdatarefmax","2.0").toDouble());
    setEPRRangeMinimum(settings.value("eprrangemin","1.0").toDouble());
    setEPRRangeMaximum(settings.value("eprrangemax","2.0").toDouble());
    setEPRColor(QColor(settings.value("eprcolor","blue").toString()));

    setEGTEnabled(settings.value("egtenabled","true").toBool());
    setEGTDatarefMinimum(settings.value("egtdatarefmin","0").toDouble());
    setEGTDatarefMaximum(settings.value("egtdatarefmax","700").toDouble());
    setEGTRangeMinimum(settings.value("egtrangemin","0").toDouble());
    setEGTRangeMaximum(settings.value("egtrangemax","700").toDouble());
    setEGTColor(QColor(settings.value("egtcolor","yellow").toString()));

    setFFEnabled(settings.value("ffenabled","false").toBool());
    setFFDatarefMinimum(settings.value("ffdatarefmin","0").toDouble());
    setFFDatarefMaximum(settings.value("ffdatarefmax","1").toDouble());
    setFFRangeMinimum(settings.value("ffrangemin","0").toDouble());
    setFFRangeMaximum(settings.value("ffrangemax","1").toDouble());
    setFFColor(QColor(settings.value("ffcolor","white").toString()));
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
    createGaugeSetSettings(layout,"N2",_n2Enabled,_n2DatarefMinimum,_n2DatarefMaximum,_n2RangeMinimum,_n2RangeMaximum,_n2Color);
    createGaugeSetSettings(layout,"EPR",_eprEnabled,_eprDatarefMinimum,_eprDatarefMaximum,_eprRangeMinimum,_eprRangeMaximum,_eprColor);
    createGaugeSetSettings(layout,"EGT",_egtEnabled,_egtDatarefMinimum,_egtDatarefMaximum,_egtRangeMinimum,_egtRangeMaximum,_egtColor);
    createGaugeSetSettings(layout,"FF",_ffEnabled,_ffDatarefMinimum,_ffDatarefMaximum,_ffRangeMinimum,_ffRangeMaximum,_ffColor);

}

