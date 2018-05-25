#ifndef NAVDISPLAY_H
#define NAVDISPLAY_H

#include "displayinstrument.h"

// Forward declarations
class NavAid;

// Nav aid Types
#define NAVAID_TYPE_Unknown                         0
#define NAVAID_TYPE_Airport                         1
#define NAVAID_TYPE_NDB                             2
#define NAVAID_TYPE_VOR                             4
#define NAVAID_TYPE_ILS                             8
#define NAVAID_TYPE_Localizer                       16
#define NAVAID_TYPE_GlideSlope                      32
#define NAVAID_TYPE_OuterMarker                     64
#define NAVAID_TYPE_MiddleMarker                    128
#define NAVAID_TYPE_InnerMarker                     256
#define NAVAID_TYPE_Fix                             512
#define NAVAID_TYPE_DME                             1024
#define NAVAID_TYPE_LatLon                          2048

#define DATASOURCE_5KM 0
#define DATASOURCE_20KM 1
#define DATASOURCE_100KM 2

/**
 * Displays a digital navigation data panel.
 *
 * WORK IN PROGRESS
 */
class NavDisplay : public DisplayInstrument
{
    Q_OBJECT
public:
    Q_INVOKABLE NavDisplay(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void render(QPainter *painter, int width, int height);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);

signals:

public slots:
    void refChanged(QString name, double value);
    void refChanged(QString name, QStringList values);
    void refChanged(QString name, QString value);
    void setDisplayRange(int val) { _displayRange = val; }
    void setDisplayHeading(int val) { _displayHeading = val; }
    void setAutoRange(bool val) { _autoRange = val; }
    void setDataSource(int val);

protected:
    QList<NavAid*> _navAids;
    double _planeLocalX;
    double _planeLocalZ;
    double _heading;
    double _efisMapRange;

    float _displayRange;      // meters
    float _displayHeading;      // degrees
    int _dataSource;
    bool _autoRange;
};


/**
 * Class for storing a single nav aid point of data, similar to the XPLM API.
 */
class NavAid : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE NavAid(QString dataLine, QObject *parent = 0) : QObject(parent) {
        // A dataLine should look like this:
        // ID:TYPE:LAT:LON:LOCALX:LOCALZ:HEIGHT:FREQ:HEAD:NAME
        // 0  1    2   3   4      5      6      7    8    9
        QStringList data = dataLine.split(':');
        if(data.length() == 10) {
            id = data.at(0);
            type = data.at(1).toInt();
            lat = data.at(2).toFloat();
            lon = data.at(3).toFloat();
            localX = data.at(4).toFloat();
            localZ = data.at(5).toFloat();
            height = data.at(6).toFloat();
            frequency = data.at(7).toInt();
            heading = data.at(8).toFloat();
            name = data.at(9);
        }
    }

public:
    QString id;
    int type;
    QString name;
    float lat;          // decimal degrees
    float lon;          // decimal degrees
    float localX;       // meters
    float localZ;       // meters
    float height;
    float heading;
    int frequency;
};

#endif // NAVDISPLAY_H
