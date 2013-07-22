#include "navdisplay.h"

#include <QLabel>
#include <QCheckBox>
#include <qmath.h>

#include "util/console.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(NavDisplay,"display/nav")

#define DATAREF_NUMENGINES "sim/aircraft/engine/acf_num_engines"
#define DATAREF_N1 "sim/cockpit2/engine/indicators/N1_percent"
#define DATAREF_EPR "sim/cockpit2/engine/indicators/EPR_ratio"
#define DATAREF_EGT "sim/cockpit2/engine/indicators/EGT_deg_C"
#define DATAREF_NAVDATA "extplane/navdata/20km"
#define DATAREF_LOCALX "sim/flightmodel/position/local_x"
#define DATAREF_LOCALZ "sim/flightmodel/position/local_z"
#define DATAREF_HEADING "sim/flightmodel/position/hpath"

NavDisplay::NavDisplay(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        DisplayInstrument(panel,conn) {
    // Init
    _displayRange = 20000.0; //20km
    _displayHeading = 0.0;

    // Connect
    _client.subscribeDataRef(DATAREF_NAVDATA, 1000);
    _client.subscribeDataRef(DATAREF_LOCALX, 5.0);
    _client.subscribeDataRef(DATAREF_LOCALZ, 5.0);
    _client.subscribeDataRef(DATAREF_HEADING, 0.2);
    connect(&_client, SIGNAL(refChanged(QString,QStringList)), this, SLOT(refChanged(QString,QStringList)));
    connect(&_client, SIGNAL(refChanged(QString,QString)), this, SLOT(refChanged(QString,QString)));
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));

}

void NavDisplay::refChanged(QString name, double value) {
    if(name == DATAREF_LOCALX) _planeLocalX = value;
    else if(name == DATAREF_LOCALZ) _planeLocalZ = value;
    else if(name == DATAREF_HEADING) _heading = value;
}

void NavDisplay::refChanged(QString name, QString value) {
    if(name == DATAREF_NAVDATA) {
        QStringList lines = value.split('\n');
        foreach(NavAid *navAid, _navAids) navAid->deleteLater();
        _navAids.clear();
        foreach(QString line, lines) {
            // Create the data line for this navaid. A data line looks like this:
            // ID:TYPE:LAT:LON:LOCALX:LOCALY:HEIGHT:FREQ:HEAD:NAME
            // KSEA:1:47.4475:-122.308:14461.6:-211.06:131.978:0:0:Seattle Tacoma Intl
            // IBFI:8:47.5155:-122.29:15776:-277.859:6.09532:11090:150.243:KBFI 13R ILS-cat-I
            // JUGBA:512:48.0517:-122.289:15717.4:-1094.2:0:0:0:JUGBA
            // WA53:1:47.604:-122.323:13271.5:-368.349:96.012:0:0:[H] Harborview Medical Center
            // ----:64:48.0528:-122.289:15719.3:-1096.42:172.212:0:196.948:KPAE 16R OM
            DEBUG << "nav aid:" << line;
            NavAid *navAid = new NavAid(line);
            _navAids.append(navAid);
        }
    }
}

void NavDisplay::refChanged(QString name, QStringList values) {
    if(name == DATAREF_N1) {
    }
}

void NavDisplay::render(QPainter *painter, int width, int height) {

    painter->save(); {

        // Painter init
        painter->setFont(this->defaultFont);

        // Init
        //painter->scale(scale,scale);
        double radarYOffset = 0.8;
        painter->translate(this->width()/2,this->height()*radarYOffset);

        // Draw plane pos
        painter->fillRect(-5,-5,10,10,Qt::red);

        // Draw all navaids
        QList<NavAid*> navAids = _navAids; // Use a local copy
        foreach(NavAid *navAid, navAids) {

            // Filter
            if(navAid->type == NAVAID_TYPE_Fix || navAid->type == NAVAID_TYPE_Airport) {
                // Pixel coords
                double rx = navAid->localX - _planeLocalX;
                double rz = -(navAid->localZ - _planeLocalZ);
                double px = (rx/_displayRange) * this->height();
                double py = (rz/_displayRange) * this->height();

                // Rotate
                double theta = (_heading+_displayHeading) * M_PI / 180.0;
                double cs = qCos(theta);
                double sn = qSin(theta);
                double rpx = px * cs - py * sn;
                double rpy = px * sn + py * cs;
                rpy = -rpy;
                double textOffsetY = 0;

                // Draw icon
                if(navAid->type == NAVAID_TYPE_Fix) {
                    double triangleSize = 10;
                    QPainterPath iconFixPath;
                    iconFixPath.moveTo(rpx,rpy-triangleSize/2);
                    iconFixPath.lineTo(rpx-triangleSize/2,rpy+triangleSize/2);
                    iconFixPath.lineTo(rpx+triangleSize/2,rpy+triangleSize/2);
                    iconFixPath.lineTo(rpx,rpy-triangleSize/2);
                    painter->setPen(Qt::green);
                    painter->drawPath(iconFixPath);
                    textOffsetY = 4;
                } else if(navAid->type == NAVAID_TYPE_Airport) {
                    painter->setPen(Qt::white);
                    painter->drawEllipse(QPoint(rpx,rpy),8,8);
                    textOffsetY = 6;
                }

                // Draw label
                double textWidth = 100;
                painter->setPen(Qt::white);
                painter->drawText(QRect(rpx-textWidth/2,rpy+textOffsetY,textWidth,20),Qt::AlignCenter,navAid->id);
            }
        }


    } painter->restore();

}

void NavDisplay::storeSettings(QSettings &settings) {
    DisplayInstrument::storeSettings(settings);

    //settings.setValue("barLabels", _barLabels);

}

void NavDisplay::loadSettings(QSettings &settings) {
    DisplayInstrument::loadSettings(settings);

    //setBarLabels(settings.value("barLabels","6").toInt());


}

void NavDisplay::createSettings(QGridLayout *layout) {
    DisplayInstrument::createSettings(layout);

    createSliderSetting(layout,"Range",100,20000,_displayRange,SLOT(setDisplayRange(int)));
    createSliderSetting(layout,"Heading",0,360,_displayHeading,SLOT(setDisplayHeading(int)));

}
