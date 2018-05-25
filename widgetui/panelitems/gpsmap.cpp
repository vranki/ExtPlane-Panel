#include "gpsmap.h"

#include <QLabel>
#include <QCheckBox>

REGISTER_WITH_PANEL_ITEM_FACTORY(GPSMap,"map/gps")

GPSMap::GPSMap(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        MapInstrument(panel,conn) {
}

void GPSMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    painter->save(); {

        // Just paint the map image
        if(panel()->hasAvionicsPower) {
            setupPainter(painter);
            if(!_mapImage.isNull()) painter->drawPixmap(0,0,_mapImage,0,0,width(),height());
            drawHeading(painter,width()/2,height()/2);
        }

    } painter->restore();

    MapInstrument::paint(painter, option, widget);
}

void GPSMap::storeSettings(QSettings &settings) {
    MapInstrument::storeSettings(settings);
}

void GPSMap::loadSettings(QSettings &settings) {
    MapInstrument::loadSettings(settings);
}

void GPSMap::createSettings(QGridLayout *layout) {
    MapInstrument::createSettings(layout);
}
