#include "gpsmap.h"

#include <QLabel>
#include <QCheckBox>

REGISTER_WITH_PANEL_ITEM_FACTORY(GPSMap,"map/gps")

GPSMap::GPSMap(QObject *parent, ExtPlaneConnection *conn) :
        MapInstrument(parent,conn) {
}

void GPSMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    painter->save(); {

        // Just paint the map image
        setupPainter(painter);
        painter->drawPixmap(0,0,_mapImage,0,0,width(),height());
        drawHeading(painter);

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
