#include "hsi_ks55.h"

#include <QLabel>
#include <QCheckBox>
#include <QHash>

#include "extplaneclient.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(HSI,"indicator/heading/hsi_ks5");

HSI::HSI(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
    _client(this, typeName(), conn)
{
    
    // Init
    _thickBars = 10;
    _thinBars = 5;
    _numbers = 30;
    _heading = 45;
    _showDegrees = true;
    
    /*
     Data-Refs used:

     hsi_bearing_deg_mag_pilot          float	930+	no	degrees_magnetic	Indicated relative bearing to the pilot's HSI-selected navaid
     hsi_relative_bearing_deg_pilot     float	930+	no	degrees             Indicated relative bearing to the pilot's HSI-selected navaid
     hsi_flag_from_to_pilot             int     930+	no	enum                Nav-To-From indication, nav1, pilot, 0 is flag, 1 is to, 2 is from.
     hsi_hdef_dots_pilot                float	930+	no	dots                CDI lateral deflection in dots, nav1, pilot
     hsi_vdef_dots_pilot                float	930+	no	dots                CDI vertical deflection in dots, nav1, pilot
     hsi_has_dme_pilot                  int     930+	no	boolean             Is there a DME signal from nav1's DME? 1 if yes, 0 if no
     hsi_dme_distance_nm_pilot          float	930+	no	nautical_miles      nav1 DME distance in nautical miles, pilot HSI
     hsi_dme_speed_kts_pilot            float	930+	no	knots               nav1 DME speed in knots. pilot HSI
     hsi_dme_time_min_pilot             float	930+	no	minutes             nav1 DME time in minutes. pilot HSI
     hsi_flag_glideslope_pilot          int     940+	no	boolean             Glide slope flag, pilot side HSI
     hsi_display_horizontal_pilot       int     940+	no	boolean             Is there some kind of horizontal signal on pilot side HSI
     hsi_display_vertical_pilot         int     940+	no	boolean             Is there some kind of vertical signal on pilot side HSI
     */
    
    // DataRef container
    const static DataRefStruct dataRefs[] = {
        {"sim/cockpit2/gauges/indicators/heading_AHARS_deg_mag_pilot", 0.2, 
            drFloat, &_heading, true},
        {"sim/cockpit2/radios/actuators/hsi_obs_deg_mag_pilot", 0.2,    // direction of yellow line
            drFloat, &_course, true},
        {"sim/cockpit2/radios/indicators/hsi_hdef_dots_pilot",0.01,       
            drFloat, &_hdots, true},
        {"sim/cockpit2/radios/indicators/hsi_vdef_dots_pilot",0.05,
            drFloat, &_vdots, false}, // Note: unused
        {"sim/cockpit2/radios/indicators/hsi_dme_distance_nm_pilot",0.1,           //
            drFloat, &_dme, false}, // Note: unused
        {"sim/cockpit2/radios/indicators/hsi_flag_glideslope_pilot",0.1,           //
            drFloat, &_glideslopeFlag, false}, // Note: unused
        {"sim/cockpit2/radios/indicators/hsi_display_horizontal_pilot",0.2,
            drFloat, &_hasHorizontal, false}, // Note: unused
        {"sim/cockpit2/radios/indicators/hsi_display_vertical_pilot",0.2,      
            drFloat, &_hasVertical, false}, // Note: unused
        {"sim/cockpit2/radios/indicators/hsi_has_dme_pilot",0.2,          
            drFloat, &_hasDME, false}, // Note: unused
        {"sim/cockpit2/radios/indicators/hsi_dme_time_min_pilot",0.2,           
            drFloat, &_dmeTime, false}, // Note: unused
        {"sim/cockpit2/radios/indicators/hsi_flag_from_to_pilot",0.2,           
            drFloat, &_toFrom, false}, // Note: unused
        {"sim/cockpit2/autopilot/heading_dial_deg_mag_pilot",0.2,           // Orange bug
            drFloat, &_bug, true}
    };

    // Connect and subscribe
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));
    int nDataRefs = 12;
    for (int i=0;i<nDataRefs;i++){
        if(dataRefs[i].enabled) {
            _dataRefLookup.insert(dataRefs[i].name, dataRefs[i]);//.value);
            _client.subscribeDataRef(dataRefs[i].name, dataRefs[i].tolerance);
        }
    }
}

void HSI::refChanged(QString name, double value) {
    DataRefStruct   ref = _dataRefLookup[name];
    if (*(float *)ref.value == value) return;
    *(float *)ref.value = value;
    update();
}

void HSI::createCard(float w, float h){

    // Create a new image for working with
    int side = qMin(w, h);
    QImage cardImage = QImage(QSize(side,side), QImage::Format_ARGB32);
    cardImage.fill(0x00ff0000);
    
    // Init dimensions
    uint midx, midy, width, height;
    width = cardImage.width();
    midx = width/2;
    height = cardImage.height();
    midy = height/2;

    // Create and setup painter
    QPainter p;
    p.begin(&cardImage);
    setupPainter(&p);

    // The original code is based on a fixed 600px image. We need to transform scale for any
    // incomming size...
    p.translate(midx, midy);
    p.scale((double)side/600.0, (double)side/600.0);

    // Draw back black
    //p.setPen(Qt::black);
    //p.setBrush(Qt::black);
    //p.drawChord(-midx,-midy,width,height,0,360*16);
    
    // Draw bars
    p.setPen(Qt::white);
    p.setBrush(Qt::white);
    if(_thickBars > 0) {
        for (float i = 0 ; i <= 360; i+=_thickBars) {
            p.save(); {
                p.rotate(value2Angle(i));
                p.drawRect(-2.5, -300, 5.0, 30);
            } p.restore();
        }
    }
    if(_thinBars > 0) {
        for (float i = 0 ; i <= 360; i+=_thinBars) {
            p.save(); {
                p.rotate(value2Angle(i));
                p.drawRect(-1.0, -300, 2.0, 20);
            } p.restore();
        }
    }
    
    // Draw degree labels
    if(_showDegrees) {
        // Setup pen
        p.setPen(QColor(200,200,200));
        QFont font = defaultFont;
        font.setPointSizeF(defaultFont.pointSizeF()*4.0);
        p.setFont(font);
        // Loop through numbers
        for (float i = 0 ; i < 360; i+=_numbers) {
            p.save(); {
                p.rotate(value2Angle(i));
                QString lineNumber;
                switch (int(i)) {
                    case 0:
                        lineNumber = "N";
                        break;
                    case 90:
                        lineNumber = "E";
                        break;
                    case 180:
                        lineNumber = "S";
                        break;
                    case 270:
                        lineNumber = "W";
                        break;
                    default:
                        lineNumber = QString::number(i/10);
                        break;
                }
                p.translate(0,-234);
                int width = p.fontMetrics().width(lineNumber);
                int height = p.fontMetrics().height();
                p.drawText(-width/2,-height/2,width,height, Qt::AlignCenter,  lineNumber);
            } p.restore();
        }
    }
    
    // Save to bitmap
    p.end();    
    _card = QPixmap::fromImage(cardImage, Qt::AutoColor);
    
}


void HSI::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    // Shapes
    static const QPoint arrowHead[] = {
        QPoint(-4, -52),
        QPoint(4, -52),
        QPoint(0, -59)
    };
    static const QPoint triangle[] = {
        QPoint(0, -33),
        QPoint(5, -23),
        QPoint(-5, -23)
    };
    
    // Init
    int side = qMin(width(), height());

    painter->save(); {

        // Setup render hints for this gauge
        setupPainter(painter);

        // Draw the cached card with heading on it, we do this without any scale transformation to speed
        // things up on mobile devices...
        painter->save(); {
            painter->translate(_card.width()/2, _card.height()/2);
            painter->rotate(-_heading);
            painter->drawPixmap(-_card.width()/2,-_card.height()/2,_card);
        } painter->restore();

        // Scale and translate for all drawing of the rest of the guage
        painter->scale(side / 200.0, side / 200.0);
        painter->translate(100, 100);
        painter->scale(1.5, 1.5); // adjustment for fixed logical card size

        // Pen for plane
        QPen planePen(QColor(255,209,0)); // Nice orangy yellow
        planePen.setWidth(2);
        painter->setPen(planePen);
        painter->setBrush(QColor(255,209,0));

        // Draw dots, etc on rotating centre of compass card:
        painter->save(); {
            painter->rotate(value2Angle(_course-_heading));
            painter->drawLine(0,66, 0,36);
            painter->drawLine(0,-36, 0,-52);
            painter->drawConvexPolygon(arrowHead, 3);

            planePen.setColor(QColor(200,200,200));
            painter->setBrush(QColor(200,200,200));
            planePen.setWidthF(1.5);
            painter->setPen(Qt::white);

            float radius=1.5;
            float deflection =_hdots;
            if (deflection>2.5) deflection=2.5;
            if (deflection<-2.5) deflection=-2.5;

            for(int i=-5;i<=5;i++){
                painter->drawLine(QPointF(7*i,-5), QPointF(7*i,5));
                painter->drawChord(QRectF(7*i-radius, -radius, 2.*radius, 2.0*radius), 0, 360*16);

            }
            planePen.setColor(QColor(255,209,0)); // Nice orangy yellow
            planePen.setWidth(2);
            painter->setPen(planePen);
            painter->drawLine(QPointF(deflection*7*2., -33), QPointF(deflection*7*2., 33));

            painter->setPen(Qt::white);
            painter->setBrush(Qt::white);
            painter->drawConvexPolygon(triangle, 3);

        } painter->restore();

        // Draw bug rotating centre of compass card:
        planePen.setColor(QColor(255,132,16)); // Orange for bug
        planePen.setWidth(1);
        painter->setPen(planePen);
        painter->save(); {
            painter->rotate(value2Angle(_bug-_heading));
            painter->drawLine(-6,-67,  6,-67);
            painter->drawLine(-6,-67, -6,-63);
            painter->drawLine( 6,-67,  6,-63);
            painter->drawLine( 6,-63,  4,-63);
            painter->drawLine(-6,-64, -4,-63);
            painter->drawLine(-4,-63, 0,-59);
            painter->drawLine( 4,-63, 0,-59);
        } painter->restore();

        // Orange plane in centre of dial:
        if(_showPlane) {
            planePen.setColor(QColor(255,150,0)); // Orange for bug
            planePen.setWidthF(2.);
            painter->setPen(planePen);
            painter->drawLine(-13, -3, 13, -3);
            painter->drawLine( 0, -10, 0, +20);
            painter->drawLine(-3, +17, 3, +17);
        }

    } painter->restore();

    PanelItem::paint(painter, option, widget);
}

void HSI::itemSizeChanged(float w, float h) {
    // The guage has changed size: redraw the cached card.
    createCard(w,h);
}

void HSI::setShowDegrees(int state) {
    _showDegrees = state == Qt::Checked;
    createCard(this->width(),this->height());
    update();
}

void HSI::setShowPlane(int state) {
    _showPlane = state == Qt::Checked;
    update();
}

float HSI::value2Angle(float value) {
    return (value / 360.0) * 360.0;
}

void HSI::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);
    settings.setValue("showdegrees", _showDegrees);
    settings.setValue("showplane", _showPlane);
}
void HSI::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    _showDegrees = settings.value("showdegrees",true).toBool();
    _showPlane = settings.value("showplane",true).toBool();
}

void HSI::createSettings(QGridLayout *layout) {
    layout->addWidget(new QLabel("Degrees", layout->parentWidget()));
    QCheckBox *showDegreesCheckbox = new QCheckBox(layout->parentWidget());
    showDegreesCheckbox->setChecked(_showDegrees);
    layout->addWidget(showDegreesCheckbox);
    connect(showDegreesCheckbox, SIGNAL(stateChanged(int)), this, SLOT(setShowDegrees(int)));

    layout->addWidget(new QLabel("Plane", layout->parentWidget()));
    QCheckBox *showPlaneCheckbox = new QCheckBox(layout->parentWidget());
    showPlaneCheckbox->setChecked(_showPlane);
    layout->addWidget(showPlaneCheckbox);
    connect(showPlaneCheckbox, SIGNAL(stateChanged(int)), this, SLOT(setShowPlane(int)));
}

