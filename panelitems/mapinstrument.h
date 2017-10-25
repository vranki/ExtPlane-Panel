#ifndef MAPINSTRUMENT_H
#define MAPINSTRUMENT_H

#include "panelitem.h"
#include "extplaneclient.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTime>
#include <QBitmap>

#define MAP_INSTRUMENT_MODE_DOWNLOAD_WAITING 1
#define MAP_INSTRUMENT_MODE_DOWNLOAD_LOADING 2
#define MAP_INSTRUMENT_MODE_DOWNLOAD_FINISHED 3

#define MAP_INSTRUMENT_SOURCE_GOOGLEMAPS 0
#define MAP_INSTRUMENT_SOURCE_RADAR 1

//#define MAP_INSTRUMENT_HEADING_DATAREF "sim/cockpit2/gauges/indicators/heading_vacuum_deg_mag_pilot"
#define MAP_INSTRUMENT_HEADING_DATAREF "sim/flightmodel/position/hpath" // This is the true north heading


class MapInstrument : public PanelItem {
    Q_OBJECT
public:
    explicit MapInstrument(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual ~MapInstrument();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void itemSizeChanged(float w, float h);
    virtual QString typeName() { return ""; } // Note: not really a nice solution but allows constructor to take care of client init and dataref subscribe
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);

public slots:
    virtual void tickTime(double dt, int total) { Q_UNUSED(dt);Q_UNUSED(total); };
    void latlongChanged(QString name, double value);
    void updateMap();
    void mapTileDownloadFinished(QNetworkReply *reply);
    void setShowHeading(bool val) {_showHeading = val;}
    void setMapSource(int val) {_mapSource = val;}
    void setMapRange(int val) {_range = val;_googleMapsZoom=(int)((1.0-val/100.0)*16.0);}
    void setGoogleMapsAPIKey(QString val) { _googleMapsAPIKey = val; }

protected:
    void drawHeading(QPainter *painter, int x, int y);

protected:
    // Internal variables
    QNetworkAccessManager *_netManager;
    ExtPlaneClient  _client;
    QPixmap         _mapImage;
    int             _side;
    int             _range;
    double          _currentMapLatitude;
    double          _currentMapLongitude;
    QTime           _lastUpdateTime;
    int             _mode;

    // Dataref variables
    double          _heading;
    double          _latitude;
    double          _longitude;

    // Configuration variables
    int             _mapSource;
    bool            _showHeading;
    int             _googleMapsCropBottomPixels;          // Downloads tiles with extra pixels at bottom so drawing is cropped from bottom up...
    int             _googleMapsMaximumUpdateRate;         // The maximum map update rate in ms
    int             _googleMapsZoom;
    QString         _googleMapsType;
    QString         _googleMapsStyle;
    QString         _googleMapsAPIKey;
};

#endif // MAPINSTRUMENT_H
