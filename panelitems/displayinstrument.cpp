#include "displayinstrument.h"

#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>
#include <QTime>

#include "../util/console.h"
#include "extplaneclient.h"

DisplayInstrument::DisplayInstrument(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeGauge, PanelItemShapeRectangular),
    _client(this, typeName(), conn) {

    // Init
    _monitorFade = 80;
    _displayStrength = 80;
    _resolution = 100;
    _lastRender.start();
    _refreshHerz = 30;

    // Make connection and register data refs
    conn->registerClient(&_client);
    //TODO: avionics on/off dataref for powering instrument
    //_client.subscribeDataRef(MAP_INSTRUMENT_HEADING_DATAREF, 0);
    //connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(latlongChanged(QString,double)));

}

DisplayInstrument::~DisplayInstrument() {
}

void DisplayInstrument::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    // Just setup the painter and draw our radar image
    setupPainter(painter);

    // Do we need to downscale for pixelation effect?
    double _resolutionForScale = 1.0+(1.0-_resolution/100.0)*3.0;
    if(_resolutionForScale != 1.0) {
        // Scale down the render then upscale again
        // TODO: is there a more efficient way to do this without adding too much complexity?
        double scale = 1.0/_resolutionForScale;
        painter->save(); {
            painter->setRenderHint(QPainter::SmoothPixmapTransform,false);
            QPixmap scaled = _monitorImage.scaled(_monitorImage.width()*scale,_monitorImage.height()*scale,Qt::IgnoreAspectRatio,Qt::FastTransformation);
            painter->drawPixmap(0,0,width(),height(),scaled);
        } painter->restore();
    } else {
        // Paint directly
        painter->drawPixmap((width()-_monitorImage.width())/2,(height()-_monitorImage.height())/2,_monitorImage,0,0,_monitorImage.width(),_monitorImage.height());
    }
    PanelItem::paint(painter, option, widget);
}

void DisplayInstrument::itemSizeChanged(float w, float h) {
    // Reload pixmaps
    _monitorImage = QPixmap(w,h);
    _monitorImage.fill(Qt::black);
}

void DisplayInstrument::tickTime(double dt, int total) {
    // Is it time to redraw?
    if(_lastRender.elapsed() > 1000/_refreshHerz) {
        // Init painters
        QPainter pixmapPainter;
        pixmapPainter.begin(&_monitorImage);
        setupPainter(&pixmapPainter);

        // Draw black over monitor for fade
        double monitorFadePercent = 1.0 - _monitorFade/100.0;
        if(monitorFadePercent != 0.0) {
            pixmapPainter.setOpacity(monitorFadePercent);
            pixmapPainter.fillRect(0,0,_monitorImage.width(),_monitorImage.height(), Qt::black);
        }

        // Render display
        double displayStrengthPercent = _displayStrength/100.0;
        if(displayStrengthPercent != 1.0) {
            pixmapPainter.setOpacity(displayStrengthPercent);
        }
        this->render(&pixmapPainter, this->width(), this->height());

        // Call for repaint
        _lastRender.restart();
        update();
    }
}

void DisplayInstrument::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("monitorFade", _monitorFade);
    settings.setValue("displayStrength", _displayStrength);
    settings.setValue("resolution", _resolution);
    settings.setValue("refreshHerz", _refreshHerz);
}

void DisplayInstrument::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    setMonitorFade(settings.value("monitorFade","80").toInt());
    setDisplayStrength(settings.value("displayStrength","80").toInt());
    setResolution(settings.value("resolution","100").toInt());
    setRefreshHerz(settings.value("refreshHerz","20").toInt());
}

void DisplayInstrument::createSettings(QGridLayout *layout) {
    PanelItem::createSettings(layout);

    createSliderSetting(layout,"Monitor Fade",0,100,_monitorFade,SLOT(setMonitorFade(int)));
    createSliderSetting(layout,"Display Strength",0,100,_displayStrength,SLOT(setDisplayStrength(int)));
    createSliderSetting(layout,"Resolution",0,100,_resolution,SLOT(setResolution(int)));
    createSliderSetting(layout,"Refresh Rate",1,60,_refreshHerz,SLOT(setRefreshHerz(int)));

}

void DisplayInstrument::drawVerticalBarGauge(
        QPainter *painter,
        QColor color,
        double x,
        double y,
        double width,
        double height,
        double value,
        double minValue,
        double maxValue,
        double rangeStart,
        double rangeEnd,
        bool decimalPrecision,
        int labelCount) {

    // Init
    double labelHeight = 20;
    double valueHeight = 30;
    double barHeight = height - valueHeight - labelHeight - labelHeight;
    double strokeWidth = 2.0;
    double percent = (value-minValue) / (maxValue-minValue);
    if(percent < 0.0) percent = 0.0;
    if(percent > 1.0) percent = 1.0;

    // Draw bar outline
    QPen pen(Qt::white,strokeWidth);
    painter->setPen(pen);
    painter->setBrush(Qt::transparent);
    painter->drawRect(x,valueHeight+labelHeight/2,width,barHeight);

    // Draw bar inner
    painter->setPen(Qt::transparent);
    painter->setBrush(color);
    double barInnerHeight = barHeight-strokeWidth-strokeWidth;
    double barInnerValue = barInnerHeight * percent;
    painter->drawRect(x+strokeWidth,
                      y+valueHeight+labelHeight/2+strokeWidth+(barInnerHeight - barInnerValue),
                      width-strokeWidth-strokeWidth,
                      barInnerValue);

    // Draw labels
    painter->setPen(Qt::white);
    int labelCountLogical = labelCount-1;
    for(int i = 0; i <= labelCountLogical; i++) {
        double labelValuePercent = ((double)i/(double)labelCountLogical);
        double labelValueRanged = rangeStart + (labelValuePercent*(rangeEnd-rangeStart));
        QString text;
        if(decimalPrecision) {
            // Decimal precision
            text = QString("%1").arg(labelValueRanged,0,'f',2);
        } else {
            // int precision
            text = QString("%1").arg((int)labelValueRanged);
        }
        painter->setFont(this->defaultFont);
        painter->drawText(x+width+labelHeight/3.0,
                          y+valueHeight+barHeight-(i*(barHeight/(double)labelCountLogical)),
                          labelHeight*3,
                          labelHeight,
                          Qt::AlignVCenter,
                          text);
    }

    // Draw value
    double valuePercent = (value-minValue) / (maxValue-minValue);
    double valueRanged = rangeStart + (valuePercent*(rangeEnd-rangeStart));
    QString valueText;
    if(decimalPrecision) {
        // Decimal precision
        valueText = QString("%1").arg(valueRanged,0,'f',2);
    } else {
        // int precision
        valueText = QString("%1").arg((int)valueRanged,3,10,QChar('0'));
    }
    painter->setFont(QFont(this->defaultFont.family(),this->defaultFont.pointSize()*2.0));
    painter->drawText(x,
                      y,
                      valueHeight*3,
                      valueHeight,
                      Qt::AlignBottom,
                      valueText);

}


