#include "radarmap.h"

#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>

REGISTER_WITH_PANEL_ITEM_FACTORY(RadarMap,"map/radar")

RadarMap::RadarMap(ExtPlanePanel *panel, ExtPlaneConnection *conn) : MapInstrument(panel,conn)
{
    // Init
    setItemShape(PanelItemShapeCircular);
    _radarMonitorFade = 50;
    _radarSweepSpeed = 60;
    _radarCurrentAngle = 0;
}

void RadarMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    // Just setup the painter and draw our radar image
    setupPainter(painter);
    painter->drawPixmap((width()-_radarImage.width())/2,(height()-_radarImage.height())/2,_radarImage,0,0,_radarImage.width(),_radarImage.height());

    PanelItem::paint(painter, option, widget);
}

void RadarMap::itemSizeChanged(float w, float h) {
    MapInstrument::itemSizeChanged(w,h);

    // Reload pixmaps
    _mask = QPixmap(_side,_side);
    _radarImage = QPixmap(_side,_side);
    _radarImage.fill(Qt::black);
}

void RadarMap::tickTime(double dt, int total) {
    Q_UNUSED(total);
    // Update radar update position
    double sweepSpeed = (1.0-_radarSweepSpeed/100.0)*4.0;
    double degreesPerSecond = 360.0/sweepSpeed;
    double degreesForDt = dt * (degreesPerSecond/1.0);
    _radarCurrentAngle += degreesForDt;
    if(_radarCurrentAngle > 360) _radarCurrentAngle = 0;

    // Init painters
    QPainter maskPainter;
    maskPainter.begin(&_mask);
    maskPainter.setPen(Qt::NoPen);
    QPainter pixmapPainter;
    pixmapPainter.begin(&_radarImage);
    setupPainter(&pixmapPainter);

    // Draw monitor refresh fade
    double monitorFade = (_radarMonitorFade/100.0)*3.0;
    pixmapPainter.setOpacity(monitorFade*dt);
    pixmapPainter.fillRect(0,0,_side,_side,Qt::black);
    pixmapPainter.setOpacity(1.0);

    // Mask update pie region and draw
    if(panel()->hasAvionicsPower) {
        maskPainter.fillRect(0,0,_side,_side,Qt::white);
        maskPainter.setBrush(Qt::black);
        maskPainter.drawPie(0,0,_side,_side,_radarCurrentAngle*16,(60.0/sweepSpeed)*16);
        maskPainter.end();
        pixmapPainter.setClipRegion(QRegion(_mask));
        pixmapPainter.drawPixmap(0,0,_mapImage,0,0,width(),height());

        // Draw heading (masked)
        drawHeading(&pixmapPainter,_radarImage.width()/2,_radarImage.height()/2);
    }

    // Call for repaint
    update();
}

void RadarMap::storeSettings(QSettings &settings) {
    MapInstrument::storeSettings(settings);

    settings.setValue("monitorfade", _radarMonitorFade);
    settings.setValue("sweepspeed", _radarSweepSpeed);
}

void RadarMap::loadSettings(QSettings &settings) {
    MapInstrument::loadSettings(settings);

    setMonitorFade(settings.value("monitorfade","50").toInt());
    setSweepSpeed(settings.value("sweepspeed","60").toInt());
}

void RadarMap::createSettings(QGridLayout *layout) {
    MapInstrument::createSettings(layout);

    createSliderSetting(layout,"Monitor Fade",0,100,_radarMonitorFade,SLOT(setMonitorFade(int)));
    createSliderSetting(layout,"Sweep Speed",0,100,_radarSweepSpeed,SLOT(setSweepSpeed(int)));
}

