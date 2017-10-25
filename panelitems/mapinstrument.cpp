#include "mapinstrument.h"

#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>

#include "../util/console.h"
#include "extplaneclient.h"

MapInstrument::MapInstrument(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeGauge, PanelItemShapeRectangular),
    _client(this, typeName(), conn) {

    // Init
    _mapSource = MAP_INSTRUMENT_SOURCE_GOOGLEMAPS;
    _mode = MAP_INSTRUMENT_MODE_DOWNLOAD_WAITING;
    _heading = 0;
    _range = 3;
    _showHeading = true;
    _googleMapsCropBottomPixels = 30;
    _googleMapsZoom = 12;
    _googleMapsMaximumUpdateRate = 1000;
    _googleMapsType = "";
    _googleMapsStyle = "feature:road.local|element:geometry|color:0x00ff00|weight:1|visibility:on&style=feature:landscape|element:geometry.fill|color:0x000000|visibility:on&style=feature:administrative|element:labels|weight:3.9|visibility:on|invert_lightness:true&style=feature:poi|visibility:simplified&style=feature:all|element:labels|visibility:off";
    _googleMapsAPIKey = "AIzaSyBrsi76jeAeAYJqHkPuxDn_feb_2g7eZQQ"; // Get your own API key at https://developers.google.com/maps/documentation/static-maps/
    _lastUpdateTime.restart();

    // Make connection and register data refs
    conn->registerClient(&_client);
    _client.subscribeDataRef("sim/flightmodel/position/latitude", 0);
    _client.subscribeDataRef("sim/flightmodel/position/longitude", 0);
    _client.subscribeDataRef(MAP_INSTRUMENT_HEADING_DATAREF, 0);
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(latlongChanged(QString,double)));

    // Setup network manager
    _netManager = new QNetworkAccessManager(this);
    connect(_netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(mapTileDownloadFinished(QNetworkReply*)));

    // Initial/default map size
    setSize(200,200);
}

MapInstrument::~MapInstrument() {
    if(_netManager) delete _netManager;
}

void MapInstrument::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    PanelItem::paint(painter, option, widget);
}

void MapInstrument::itemSizeChanged(float w, float h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
    _side = qMin(width(), height());
}

void MapInstrument::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

    settings.setValue("showheading", _showHeading);
    settings.setValue("mapsource", _mapSource);
    settings.setValue("maprange", _range);
}

void MapInstrument::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    setShowHeading(settings.value("showheading","1").toBool());
    setMapSource(settings.value("mapsource","0").toInt());
    setMapRange(settings.value("maprange","50").toInt());
}

void MapInstrument::createSettings(QGridLayout *layout) {
    PanelItem::createSettings(layout);

    // Heading
    createCheckboxSetting(layout,"Heading",_showHeading,SLOT(setShowHeading(bool)));

    // Source dropdown
    layout->addWidget(new QLabel("Source", layout->parentWidget()));
    QComboBox *mapSourceCombobox = new QComboBox(layout->parentWidget());
    mapSourceCombobox->addItem("Google Maps");
    //mapSourceCombobox->addItem("Radar"); //TODO use bitmap data directly from x-plane
    mapSourceCombobox->setCurrentIndex(_mapSource);
    layout->addWidget(mapSourceCombobox);
    connect(mapSourceCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMapSource(int)));

    // Api Key
    createLineEditSetting(layout, "Google Maps API Key",_googleMapsAPIKey,SLOT(setGoogleMapsAPIKey(QString)));

    // Note on api keys
    layout->addWidget(new QLabel("", layout->parentWidget()));
    QLabel *apiKeyNotes = new QLabel("The API key provided is for testing only and has a daily limit. You should get your own at https://developers.google.com/maps/documentation/static-maps/.", layout->parentWidget());
    apiKeyNotes->setWordWrap(true);
    layout->addWidget(apiKeyNotes);

    // Range
    createSliderSetting(layout,"Range",0,100,_range,SLOT(setMapRange(int)));

}

void MapInstrument::latlongChanged(QString name, double value) {

    if (name == MAP_INSTRUMENT_HEADING_DATAREF) {
        _heading = value;
        return;
    } else if(name == "sim/flightmodel/position/latitude") {
        _latitude = value;
    } else if (name == "sim/flightmodel/position/longitude") {
        _longitude = value;
    }
    if(value == 0) return;
    if(_mode == MAP_INSTRUMENT_MODE_DOWNLOAD_WAITING && _lastUpdateTime.elapsed() > _googleMapsMaximumUpdateRate) updateMap();
    //_gpsLocationRecieved = true;
}


void MapInstrument::updateMap() {

    // Change?
    if(_currentMapLatitude == _latitude && _currentMapLongitude == _longitude) return;

    // Update
    _mode = MAP_INSTRUMENT_MODE_DOWNLOAD_LOADING;
    _lastUpdateTime = QTime::currentTime();
    _lastUpdateTime.restart();
    _currentMapLatitude = _latitude;
    _currentMapLongitude = _longitude;

    // Size of tile to download
    int w = width();
    int h = height() + _googleMapsCropBottomPixels;

    // Download map tile
    QString url = QString("http://maps.googleapis.com/maps/api/staticmap?center=%1,%2&zoom=%3&size=%4x%5&maptype=%6&sensor=false&style=%7&key=%8")
            .arg(_latitude)
            .arg(_longitude)
            .arg(_googleMapsZoom)
            .arg(w)
            .arg(h)
            .arg(_googleMapsType)
            .arg(_googleMapsStyle)
            .arg(_googleMapsAPIKey);
    DEBUG << "will download" << url;
    _netManager->get(QNetworkRequest(QUrl(url)));
}

void MapInstrument::mapTileDownloadFinished(QNetworkReply *reply) {
    // Validate
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "could not download map tile:" << reply->errorString();
        _mode = MAP_INSTRUMENT_MODE_DOWNLOAD_WAITING;
        return;
    }

    // Read into pixmap and update state
    QByteArray data = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(data);
    _mapImage = pixmap;
    _mode = MAP_INSTRUMENT_MODE_DOWNLOAD_WAITING;;
    update();
}

void MapInstrument::drawHeading(QPainter *painter, int x, int y) {

    // Shapes
    static const QPoint plane[] = {
        QPoint(0, 70),
        QPoint(5, 60),
        QPoint(5, 30),
        QPoint(60,-10),
        QPoint(60,-20),
        QPoint(5,-10),
        QPoint(5,-30),
        QPoint(4,-40),
        QPoint(25,-55),
        QPoint(25,-65),
        QPoint(2,-60),
        QPoint(0,-65),
        QPoint(-2,-60),
        QPoint(-25,-65),
        QPoint(-25,-55),
        QPoint(-4,-40),
        QPoint(-5,-30),
        QPoint(-5,-10),
        QPoint(-60,-20),
        QPoint(-60,-10),
        QPoint(-5, 30),
        QPoint(-5, 60),
    };

    if(_showHeading) {
        painter->save(); {
            // Line
            painter->setClipRect(0,0,width(),height(),Qt::IntersectClip);
            painter->translate(x,y);
            painter->rotate(_heading-90);
            painter->setPen(QPen(Qt::green,4,Qt::DotLine));
            painter->drawLine(0,0,_side/1.0,0);
            // Plane
            double scale = 1.0/2.0;
            painter->scale(scale,scale);
            painter->rotate(-90);
            painter->setBrush(Qt::green);
            painter->setPen(Qt::NoPen);
            painter->drawConvexPolygon(plane, sizeof(plane)/sizeof(plane[0]));
        } painter->restore();
    }
}
