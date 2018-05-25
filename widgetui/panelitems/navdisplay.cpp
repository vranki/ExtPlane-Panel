#include "navdisplay.h"

#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <qmath.h>

#include "../util/console.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(NavDisplay,"display/nav")

#define DATAREF_NAVDATA_5KM "extplane/navdata/5km"
#define DATAREF_NAVDATA_20KM "extplane/navdata/20km"
#define DATAREF_NAVDATA_100KM "extplane/navdata/100km"
#define DATAREF_LOCALX "sim/flightmodel/position/local_x"
#define DATAREF_LOCALZ "sim/flightmodel/position/local_z"
//#define DATAREF_HEADING "sim/flightmodel/position/hpath"
//#define DATAREF_HEADING "sim/cockpit/misc/compass_indicated"
#define DATAREF_HEADING "sim/cockpit2/gauges/indicators/heading_electric_deg_mag_pilot"
#define DATAREF_EFIS_MAP_RANGE "sim/cockpit2/EFIS/map_range" // or "sim/cockpit/switches/EFIS_map_range_selector"

NavDisplay::NavDisplay(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
        DisplayInstrument(panel,conn) {
    // Init
    _displayRange = 20000.0; //20km
    _displayHeading = 0.0;
    _dataSource = DATASOURCE_20KM;
    _efisMapRange = 0; // Map range, 1->6, where 6 is the longest range.
    _autoRange = false;

    // Connect
    _client.subscribeDataRef(DATAREF_LOCALX, 5.0);
    _client.subscribeDataRef(DATAREF_LOCALZ, 5.0);
    _client.subscribeDataRef(DATAREF_HEADING, 0.2);
    _client.subscribeDataRef(DATAREF_EFIS_MAP_RANGE, 1.0);
    connect(&_client, SIGNAL(refChanged(QString,QStringList)), this, SLOT(refChanged(QString,QStringList)));
    connect(&_client, SIGNAL(refChanged(QString,QString)), this, SLOT(refChanged(QString,QString)));
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));

}

void NavDisplay::refChanged(QString name, double value) {
    if(name == DATAREF_LOCALX) _planeLocalX = value;
    else if(name == DATAREF_LOCALZ) _planeLocalZ = value;
    else if(name == DATAREF_HEADING) _heading = value;
    else if(name == DATAREF_EFIS_MAP_RANGE) { _efisMapRange = value; }
}

void NavDisplay::refChanged(QString name, QString value) {
    if(name == DATAREF_NAVDATA_5KM || name == DATAREF_NAVDATA_20KM || name == DATAREF_NAVDATA_100KM) {
        // TODO: There is a lot to optimize here...
        int count = 0;
        QStringList lines = value.split('\n');
        foreach(NavAid *navAid, _navAids) navAid->deleteLater();
        _navAids.clear();
        foreach(QString line, lines) {
            // Create the navaid object for the data line. A data line looks like this:
            // ID:TYPE:LAT:LON:LOCALX:LOCALY:HEIGHT:FREQ:HEAD:NAME
            // KSEA:1:47.4475:-122.308:14461.6:-211.06:131.978:0:0:Seattle Tacoma Intl
            // IBFI:8:47.5155:-122.29:15776:-277.859:6.09532:11090:150.243:KBFI 13R ILS-cat-I
            // JUGBA:512:48.0517:-122.289:15717.4:-1094.2:0:0:0:JUGBA
            // WA53:1:47.604:-122.323:13271.5:-368.349:96.012:0:0:[H] Harborview Medical Center
            // ----:64:48.0528:-122.289:15719.3:-1096.42:172.212:0:196.948:KPAE 16R OM
            //DEBUG << "nav aid:" << line;
            NavAid *navAid = new NavAid(line);
            _navAids.append(navAid);
            count++;
        }
        //DEBUG << "total added:" << count;
    }
}

void NavDisplay::refChanged(QString name, QStringList values) {
    Q_UNUSED(name);
    Q_UNUSED(values);
}

void NavDisplay::render(QPainter *painter, int width, int height) {
    Q_UNUSED(width);
    Q_UNUSED(height);
    painter->save(); {

        // Painter init
        painter->setFont(this->defaultFont);

        // Init
        //painter->scale(scale,scale);
        double radarYOffsetPercent = 0.8;
        double radarYOffsetPixel = this->height()*radarYOffsetPercent;
        double radarRadiusPixel = radarYOffsetPixel-40;
        painter->translate(this->width()/2,radarYOffsetPixel);
        float displayRangeToUse = _displayRange;
        double crossBarMeters = 1000; // 1km
        if(_autoRange == true) {
            // "Map range, 1->6, where 6 is the longest range."
            if(_efisMapRange == 1)      displayRangeToUse = 5000; // 5 km
            else if(_efisMapRange == 2) displayRangeToUse = 10000; // 10 km
            else if(_efisMapRange == 3) displayRangeToUse = 20000; // 20 km
            else if(_efisMapRange == 4) displayRangeToUse = 50000; // 50 km
            else if(_efisMapRange == 5) displayRangeToUse = 100000; // 100 km
            else if(_efisMapRange == 6) displayRangeToUse = 200000; // 200 km
        }
        //if(displayRangeToUse > 50000) {
        //    crossBarMeters = 10000; // 10 km
        //}

        painter->save(); {

            // Mask for radar
            QPainterPath path;
            path.addEllipse(QPoint(0,0),radarRadiusPixel-20,radarRadiusPixel-20);
            painter->setClipPath(path);

            // Draw all navaids
            QList<NavAid*> navAids = _navAids; // Use a local copy
            foreach(NavAid *navAid, navAids) {

                // Filter
                if(navAid->type == NAVAID_TYPE_Fix || navAid->type == NAVAID_TYPE_Airport || navAid->type == NAVAID_TYPE_NDB || navAid->type == NAVAID_TYPE_VOR) {

                    // Pixel coords
                    double rx = navAid->localX - _planeLocalX;
                    double rz = -(navAid->localZ - _planeLocalZ);
                    double px = (rx/displayRangeToUse) * this->height();
                    double py = (rz/displayRangeToUse) * this->height();

                    // Rotate
                    double theta = (_heading+_displayHeading) * M_PI / 180.0;
                    double cs = qCos(theta);
                    double sn = qSin(theta);
                    double rpx = px * cs - py * sn;
                    double rpy = px * sn + py * cs;
                    rpy = -rpy;
                    double textOffsetY = 0;

                    // Draw icon
                    bool drawLabel = false;
                    bool drawDescription = false;
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
                        drawLabel = true;
                    } else if(navAid->type == NAVAID_TYPE_Airport) {
                        painter->setPen(Qt::white);
                        painter->drawEllipse(QPoint(rpx,rpy),8,8);
                        textOffsetY = 6;
                        drawLabel = true;
                        drawDescription = true;
                    } else if(navAid->type == NAVAID_TYPE_NDB) {
                        painter->setPen(QColor(71,187,206));
                        painter->drawEllipse(QPoint(rpx,rpy),8,8);
                        painter->drawEllipse(QPoint(rpx,rpy),3,3);
                        textOffsetY = 6;
                        drawLabel = true;
                        drawDescription = true;
                    } else if(navAid->type == NAVAID_TYPE_VOR) {
                        painter->setPen(QColor(0,218,255));
                        QPainterPath iconFixPath;
                        iconFixPath.moveTo(rpx-2*2,rpy+4*2);
                        iconFixPath.lineTo(rpx+2*2,rpy+4*2);
                        iconFixPath.lineTo(rpx+4*2,rpy+0*2);
                        iconFixPath.lineTo(rpx+2*2,rpy-4*2);
                        iconFixPath.lineTo(rpx-2*2,rpy-4*2);
                        iconFixPath.lineTo(rpx-4*2,rpy+0*2);
                        iconFixPath.lineTo(rpx-2*2,rpy+4*2);
                        painter->drawPath(iconFixPath);
                        textOffsetY = 6;
                        drawLabel = true;
                        drawDescription = true;
                    }

                    // Draw label
                    if(drawLabel) {
                        double textWidth = 100;
                        painter->setPen(Qt::white);
                        painter->drawText(QRect(rpx-textWidth/2,rpy+textOffsetY,textWidth,20),Qt::AlignCenter,navAid->id);
                        textOffsetY += 12;
                    }
                    if(drawDescription) {
                        double textWidth = 200;
                        painter->setPen(Qt::white);
                        painter->drawText(QRect(rpx-textWidth/2,rpy+textOffsetY,textWidth,20),Qt::AlignCenter,navAid->name);
                    }
                }
            }

        } painter->restore();

        {
            // Draw plane
            double triangleSize = 30;
            QPainterPath iconFixPath;
            iconFixPath.moveTo(0,0-triangleSize/2);
            iconFixPath.lineTo(0-triangleSize/3,0+triangleSize/2);
            iconFixPath.lineTo(0+triangleSize/3,0+triangleSize/2);
            iconFixPath.lineTo(0,0-triangleSize/2);
            painter->setPen(QPen(Qt::white,1));
            painter->drawPath(iconFixPath);

            // Pos guide and cross bars
            int guideTop = -radarRadiusPixel-10;
            painter->drawLine(0,-triangleSize/2-4,0,guideTop);
            int numCrossbars = (int)(displayRangeToUse/crossBarMeters);
            for(int i = 1; i < numCrossbars; i++) {
                //double yy = (radarRadiusPixel/4) * i;
                double meters = (crossBarMeters)*i;
                int crossBarWidth = 5;
                if(((int)meters) % 10000 == 0) crossBarWidth = 10;
                double yy = meters/displayRangeToUse * this->height();
                if(yy > radarRadiusPixel) break;
                painter->drawLine(-crossBarWidth,-yy,crossBarWidth,-yy);
            }

            // Heading box
            int headingBoxW = 60;
            painter->drawLine(-headingBoxW/2,guideTop,headingBoxW/2,guideTop);
            painter->drawLine(-headingBoxW/2,guideTop,-headingBoxW/2,-radarYOffsetPixel);
            painter->drawLine(+headingBoxW/2,guideTop,+headingBoxW/2,-radarYOffsetPixel);
            QFont headingFont(defaultFont);
            headingFont.setPixelSize(28.0*(1.0+itemFontSize()));
            painter->setFont(headingFont);
            painter->drawText(QRect(-headingBoxW/2,-radarYOffsetPixel,headingBoxW,radarYOffsetPixel+guideTop),Qt::AlignCenter,QString("%1").arg((int)_heading));
        }

        painter->save(); {
            // Draw heading card
            painter->setPen(QPen(Qt::white,1));
            painter->drawEllipse(QPointF(0,0),radarRadiusPixel,radarRadiusPixel);
            painter->rotate(-_heading);
            QFont cardFont(defaultFont);
            cardFont.setPixelSize(28.0*(1.0+itemFontSize()));
            painter->setFont(cardFont);
            for(int a = 0; a < 360; a = a + 5) {
                int tickLength = 10;
                if(a % 10 == 0) tickLength = 15;
                painter->drawLine(0,-radarRadiusPixel,0,-radarRadiusPixel+tickLength);
                if(a % 30 == 0) {
                    painter->drawText(QRect(-50,-radarRadiusPixel+tickLength,100,30),Qt::AlignCenter,QString("%1").arg(a/10));
                }
                painter->rotate(5);
            }

        } painter->restore();

    } painter->restore();

}

void NavDisplay::storeSettings(QSettings &settings) {
    DisplayInstrument::storeSettings(settings);

    settings.setValue("datasource", _dataSource);
    settings.setValue("displayrange", _displayRange);
    settings.setValue("displayheading", _displayHeading);
    settings.setValue("autorange", _autoRange);

}

void NavDisplay::loadSettings(QSettings &settings) {
    DisplayInstrument::loadSettings(settings);

    setDataSource(settings.value("datasource","1").toInt());
    setDisplayRange(settings.value("displayrange","20000").toInt());
    setDisplayHeading(settings.value("displayheading","0").toInt());
    setAutoRange(settings.value("autorange","0").toBool());

}

void NavDisplay::createSettings(QGridLayout *layout) {
    DisplayInstrument::createSettings(layout);

    createSliderSetting(layout,"Range",100,200000,_displayRange,SLOT(setDisplayRange(int)));
    createCheckboxSetting(layout,"Set Range from EFIS",_autoRange,SLOT(setAutoRange(bool)));

    createSliderSetting(layout,"Heading Offset",0,360,_displayHeading,SLOT(setDisplayHeading(int)));

    layout->addWidget(new QLabel("Data Source", layout->parentWidget()));
    QComboBox *combobox = new QComboBox(layout->parentWidget());
    combobox->addItem("5km");
    combobox->addItem("20km");
    combobox->addItem("100km");
    combobox->setCurrentIndex(_dataSource);
    layout->addWidget(combobox);
    connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDataSource(int)));
}

void NavDisplay::setDataSource(int val) {
    _dataSource = val;
    if(_client.isDataRefSubscribed(DATAREF_NAVDATA_5KM)) _client.unsubscribeDataRef(DATAREF_NAVDATA_5KM);
    if(_client.isDataRefSubscribed(DATAREF_NAVDATA_20KM)) _client.unsubscribeDataRef(DATAREF_NAVDATA_20KM);
    if(_client.isDataRefSubscribed(DATAREF_NAVDATA_100KM)) _client.unsubscribeDataRef(DATAREF_NAVDATA_100KM);
    if(_dataSource == DATASOURCE_5KM) {
        _client.subscribeDataRef(DATAREF_NAVDATA_5KM, 1000);
    } else if(_dataSource == DATASOURCE_20KM) {
        _client.subscribeDataRef(DATAREF_NAVDATA_20KM, 1000);
    } else if(_dataSource == DATASOURCE_100KM) {
        _client.subscribeDataRef(DATAREF_NAVDATA_100KM, 1000);
    }
}
