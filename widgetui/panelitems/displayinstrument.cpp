#include "displayinstrument.h"

#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>
#include <QTime>

#include "../util/console.h"
#include "extplaneclient.h"

DisplayInstrument::DisplayInstrument(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeGauge, PanelItemShapeRectangular),_client(this, typeName(), conn) {

    // Init
    _monitorFade = 93;
    _displayStrength = 39;
    _resolution = 100;
    _lastRender.start();
    _refreshHerz = 30;

    // Make connection and register data refs
    conn->registerClient(&_client);

}

DisplayInstrument::~DisplayInstrument() {
}

void DisplayInstrument::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    // Just setup the painter and draw our image
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
    _bufferImage = QPixmap(w,h);
    //_bufferImage.fill(Qt::black);
}

void DisplayInstrument::tickTime(double dt, int total) {
    Q_UNUSED(dt);
    Q_UNUSED(total);
    // Is it time to redraw?
    if(_lastRender.elapsed() > 1000/_refreshHerz) {
        // Init painters
        QPainter pixmapPainter;
        pixmapPainter.begin(&_monitorImage);
        setupPainter(&pixmapPainter);

        // Draw black over monitor for fade
        double monitorFadePercent = 1.0 - _monitorFade/100.0;
        if(!this->panel()->hasAvionicsPower) monitorFadePercent = 0.1;
        if(monitorFadePercent != 0.0) {
            pixmapPainter.setOpacity(monitorFadePercent);
            pixmapPainter.fillRect(0,0,_monitorImage.width(),_monitorImage.height(), Qt::black);
        }

        // Render display
        if(this->panel()->hasAvionicsPower) {
            // Render to buffer
            QPainter bufferPainter;
            bufferPainter.begin(&_bufferImage);
            bufferPainter.fillRect(_bufferImage.rect(),Qt::black);
            setupPainter(&bufferPainter);
            this->render(&bufferPainter, this->width(), this->height());
            // Render to pixamp
            double displayStrengthPercent = _displayStrength/100.0;
            if(displayStrengthPercent != 1.0) {
                pixmapPainter.setOpacity(displayStrengthPercent);
            }
            pixmapPainter.drawPixmap(0,0,_bufferImage);
        }

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




