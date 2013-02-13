#include "hsi_ks55.h"

#include <QLabel>
#include "../widgets/distanceunitcombobox.h"
#include "../extplaneclient.h"

#include "widgets/numberinputlineedit.h"
#include <QHash>

REGISTER_WITH_PANEL_ITEM_FACTORY(HSI,"indicator/heading/hsi_ks5");

HSI::HSI(QObject *parent, ExtPlaneConnection *conn) :
PanelItem(parent), _client(this, typeName(), conn)
{
    
    _bezel = QPixmap::fromImage(QImage(QString("../../images/KCS_55_bezel.png")), Qt::AutoColor);
    
    _thickBars=10;
    _thinBars=5;
    _numbers=30;
    
    createCard(); 
   
    _heading=45;
    
    /*
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
    
    /*  {
     QString         name;
     float           tolerance;
     DataRefType     dataType;
     void *          value;
     }; */  

    //QList<DataRefStruct *> _dataRefs;
/*    
    bool    _hasDME;
    int     _toFrom;  // 0 = Flag, 1=To, 2=From
*/    
    
    const static DataRefStruct dataRefs[] = {
        {"sim/cockpit2/gauges/indicators/heading_AHARS_deg_mag_pilot", 0.2, 
            drFloat, &_heading},
        {"sim/cockpit2/radios/actuators/hsi_obs_deg_mag_pilot", 0.2,    // direction of yellow line
            drFloat, &_course},
        {"sim/cockpit2/radios/indicators/hsi_hdef_dots_pilot",0.01,       
            drFloat, &_hdots},
        {"sim/cockpit2/radios/indicators/hsi_vdef_dots_pilot",0.05,
            drFloat, &_vdots},
        {"sim/cockpit2/radios/indicators/hsi_dme_distance_nm_pilot",0.1,           //
            drFloat, &_dme},
        {"sim/cockpit2/radios/indicators/hsi_flag_glideslope_pilot",0.1,           //
            drFloat, &_glideslopeFlag},
        {"sim/cockpit2/radios/indicators/hsi_display_horizontal_pilot",0.2,
            drFloat, &_hasHorizontal},
        {"sim/cockpit2/radios/indicators/hsi_display_vertical_pilot",0.2,      
            drFloat, &_hasVertical},
        {"sim/cockpit2/radios/indicators/hsi_has_dme_pilot",0.2,          
            drFloat, &_hasDME},
        {"sim/cockpit2/radios/indicators/hsi_dme_time_min_pilot",0.2,           
            drFloat, &_dmeTime},
        {"sim/cockpit2/radios/indicators/hsi_flag_from_to_pilot",0.2,           
            drFloat, &_toFrom},
        {"sim/cockpit2/autopilot/heading_dial_deg_mag_pilot",0.2,           // Orange bug
            drFloat, &_bug}
    };

    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));
    int nDataRefs = 12;
    
    for (int i=0;i<nDataRefs;i++){
        _dataRefLookup.insert(dataRefs[i].name, dataRefs[i]);//.value);
        _client.subscribeDataRef(dataRefs[i].name, dataRefs[i].tolerance);
    }
}

void HSI::refChanged(QString name, double value) {
    
    
    DataRefStruct   ref = _dataRefLookup[name];

    qDebug() << Q_FUNC_INFO << "lookup name is: " << name << 
                                " ref name is: " << ref.name <<
                                " new value is: " << value;

    if (*(float *)ref.value == value) return;
    
    *(float *)ref.value = value;
    
    update();


}


void HSI::dataRefsChanged(QString name, QString valueString) {
/*    
    qDebug() << Q_FUNC_INFO << "valueString " << valueString;
    
    
    QStringList cmd = valueString.split(" ", QString::SkipEmptyParts);
    QString rpmStr = cmd.value(3);
    rpmStr.chop(1);
    
    qDebug() << Q_FUNC_INFO << "rpmStr " << rpmStr;
*/    
    
    //    setValue(rpmStr.toDouble());
}


void HSI::createCard(void){
    QImage _cardImage = QImage(QSize(600,600), QImage::Format_ARGB32);
    _cardImage.fill(0x00ff0000);
    //_cardImage.moveTo(10,10);
    
    uint midx, midy, width, height;
    width = _cardImage.width();
    midx = width/2;
    height = _cardImage.height();
    midy = height/2;
    
    
    QPainter p;
    p.setRenderHint(QPainter::Antialiasing, true);
    p.begin(&_cardImage);
    
    p.translate(midx, midy);
    p.setPen(Qt::black);
    
    p.setBrush(Qt::black);
    p.drawChord(-midx,-midy,width,height,0,360*16);
    
    
    p.setPen(Qt::white);
    p.setBrush(Qt::white);
    if(_thickBars > 0) {
        for (float i = 0 ; i <= 360; i+=_thickBars) {
            p.save();
            p.rotate(value2Angle(i));
            p.drawRect(-2.5, -300, 5.0, 30);
            p.restore();
        }
    }
    if(_thinBars > 0) {
        for (float i = 0 ; i <= 360; i+=_thinBars) {
            p.save();
            p.rotate(value2Angle(i));
            p.drawRect(-1.0, -300, 2.0, 20);
            p.restore();
        }
    }
    p.setPen(QColor(200,200,200));
    p.setFont(QFont(QString("Helvetica"), 48, QFont::Bold, false));
    
    if(1) {
        for (float i = 0 ; i < 360; i+=_numbers) {
            p.save();
            p.rotate(value2Angle(i));
            p.save();
            QString lineNumber;
            switch (int(i)) {
                case 0:
                    lineNumber = QString("N");
                    break;
                case 90:
                    lineNumber = QString("E");
                    break;
                case 180:
                    lineNumber = QString("S");
                    break;
                case 270:
                    lineNumber = QString("W");
                    break;
                default:
                    lineNumber = QString::number(i/10);
                    break;
            }
            p.translate(0,-234);
            int width = p.fontMetrics().width(lineNumber);
            int height = p.fontMetrics().height();
            p.drawText(-width/2,-height/2,width,height, Qt::AlignCenter,  lineNumber);
            p.restore();
            p.restore();
        }
    }
    
    
    
    p.end();    
    _card = QPixmap::fromImage(_cardImage, Qt::AutoColor);
    
}


void HSI::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    static const QPoint needle[3] = {
        QPoint(0, -90),
        QPoint(-5, -103),
        QPoint(5, -103)
    };
    
    static const QPoint Plane[] = {
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
    
    
    const int centreY = 9;          // In a 200x200 grid centred on zero 
    const int centreX = 0;
    
    
    QColor needleColor(255, 255, 255);
    
    int side = qMin(width(), height());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();
    painter->scale(side / 200.0, side / 200.0);
    painter->save();
    painter->translate(100, 100);
    
    painter->save();
    //painter->scale(0.91,0.91);
    
    painter->save();
    
    // Draw compass card at heading angle:
    painter->save();
    painter->translate(0, centreY);
    painter->rotate(- _heading);
    painter->drawPixmap(-67, -67, 134, 134, _card);
    painter->restore();
    
    painter->setPen(Qt::NoPen);
    
   
    QPen planePen(QColor(255,209,0)); // Nice orangy yellow
    planePen.setWidth(2);
    painter->setPen(planePen);
    painter->setBrush(QColor(255,209,0));
    
    // Draw dots, etc on rotating centre of compass card:
    painter->save();
        painter->translate(0, centreY);
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
    
    painter->restore();

    // Draw bug rotating centre of compass card:
    planePen.setColor(QColor(255,132,16)); // Orange for bug
    planePen.setWidth(1);
    painter->setPen(planePen);
    painter->save();
        painter->translate(0, centreY);
        painter->rotate(value2Angle(_bug-_heading));
    
        painter->drawLine(-6,-67,  6,-67);
        painter->drawLine(-6,-67, -6,-63);
        painter->drawLine( 6,-67,  6,-63);
        painter->drawLine( 6,-63,  4,-63);
        painter->drawLine(-6,-64, -4,-63);
        painter->drawLine(-4,-63, 0,-59);
        painter->drawLine( 4,-63, 0,-59);
    
    painter->restore();

    
    // Orange plane in centre of dial:
    planePen.setColor(QColor(255,150,0)); // Orange for bug
    planePen.setWidthF(2.);
    painter->setPen(planePen);
    
    painter->drawLine(-13, centreY-3, 13, centreY-3);
    painter->drawLine( 0, centreY-10, 0, centreY+20);
    painter->drawLine(-3, centreY+17, 3, centreY+17);
    
    
    painter->setPen(Qt::white);
  //painter->drawText(-40,-30, 100, 20, Qt::AlignRight | Qt::AlignVCenter, "hdg "+ QString::number(_heading));
  //painter->drawText(-40,-15, 100, 20, Qt::AlignRight | Qt::AlignVCenter, "crs "+ QString::number(_course));
  //painter->drawText(-40,0, 100, 20, Qt::AlignRight | Qt::AlignVCenter, "bug "+QString::number(_bug));
  //painter->drawText(-40,15, 100, 20, Qt::AlignRight | Qt::AlignVCenter, "hdt "+QString::number(_hdots));
  //    painter->drawText(-40,30, 100, 20, Qt::AlignRight | Qt::AlignVCenter, "gs "+QString::number(_glideslopeFlag));
    
    
    painter->restore();
    painter->setBrush(Qt::white);
    
    painter->restore();
    painter->drawPixmap(-100,-100,200,200, _bezel);
    
    painter->restore();
    
    painter->restore();

    PanelItem::paint(painter, option, widget);
}

void HSI::setLabel(QString text) {
    _label = text;
}

float HSI::value2Angle(float value) {
    return (value / 360.0) * 360.0;
}

void HSI::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);
    
}
void HSI::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    DistanceUnit unit = Units::distanceUnitForName(unitname);
}

void HSI::createSettings(QGridLayout *layout) {
    
}

