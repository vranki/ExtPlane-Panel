/*
*  Created by Bob Gates on 2011/07/08.
*/


#include "di.h"
#include <QLabel>
#include "../widgets/distanceunitcombobox.h"
#include "extplaneclient.h"

#include "widgets/numberinputlineedit.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(DirectionIndicator,"indicator/heading/basic");

DirectionIndicator::DirectionIndicator(QObject *parent, ExtPlaneConnection *conn) :
    PanelItem(parent), _client(this, typeName(), conn)
{
    _value = 0;
    setThickBars(10);
    setThinBars(5);
    setRange1(360);
    setRange2(360);
    setNumbers(30);
    setNumbersScale(0.1);
    units = DISTANCE_M;
    baroUnits = PRESSURE_HPA;

    _dataRef = QString("sim/cockpit2/gauges/indicators/heading_vacuum_deg_mag_pilot");
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));
    _client.subscribeDataRef(_dataRef,0.1);
}

void DirectionIndicator::setNumbers(float div) {
    _numbers = div;
    update();
}

void DirectionIndicator::createCard(float w, float h){

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
    p.scale(DIRECTION_INDICATOR_HEADING_SCALE, DIRECTION_INDICATOR_HEADING_SCALE);

    //TODO: dankrusi: I've disabled this. Black on black is invisible...
    //p.setPen(Qt::blue);
    //p.setBrush(Qt::red);
    //p.drawChord(-midx,-midy,width,height,0,360*16);

    // Draw the bars
    p.setPen(Qt::white);
    p.setBrush(Qt::white);
    if(_thickBars > 0) {
        for (float i = 0 ; i <= _range1; i+=_thickBars) {
            p.save();
            p.rotate(value2Angle1(i));
            p.drawRect(-2.5, -300, 5.0, 30);
            p.restore();
        }
    }
    if(_thinBars > 0) {
        for (float i = 0 ; i <= _range2; i+=_thinBars) {
            p.save();
            p.rotate(value2Angle1(i));
            p.drawRect(-1.0, -300, 2.0, 20);
            p.restore();
        }
    }

    // Draw the numbers and NESW labels
    p.setPen(QColor(200,200,200));
    p.setFont(QFont(QString("Helvetica"), defaultFont.pointSize()*4.5, QFont::Bold, false)); //TODO: dankrusi: should we reference such explicit fonts?
    if(_numbers != 0) {
        for (float i = 0 ; i < _range1; i+=_numbers) {
            p.save();
            p.rotate(value2Angle1(i));
            p.save();
            QString lineNumber;
            switch (int(i*_numbersScale)) {
            case 0:
                lineNumber = QString("N");
                break;
            case 9:
                lineNumber = QString("E");
                break;
            case 18:
                lineNumber = QString("S");
                break;
            case 27:
                lineNumber = QString("W");
                break;
            default:
                lineNumber = QString::number(i*_numbersScale);
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

    // Export painter work as pixmap
    p.end();
    _card = QPixmap::fromImage(cardImage, Qt::AutoColor);

}


void DirectionIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    static const QPoint needle[3] = {
        QPoint(0, -90),
        QPoint(-5, -103),
        QPoint(5, -103)
    };
    
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
    
    // Init
    QColor needleColor(255, 255, 255);
    int side = qMin(width(), height());

    painter->save(); {

        // Setup render hints for this gauge
        setupPainter(painter);

        // Draw the cached card with heading on it, we do this without any scale transformation to speed
        // things up on mobile devices...
        painter->save(); {
            painter->translate(_card.width()/2, _card.height()/2);
            painter->rotate(-_value);
            painter->drawPixmap(-_card.width()/2,-_card.height()/2,_card);
        } painter->restore();

        // Scale and translate for all drawing of the guage
        painter->scale(side / 200.0, side / 200.0);
        painter->translate(100, 100);

        // Draw fixed heading pins
        painter->save(); {
            painter->scale(DIRECTION_INDICATOR_FIXED_HEADING_SCALE,DIRECTION_INDICATOR_FIXED_HEADING_SCALE);
            painter->setPen(Qt::NoPen);
            painter->setBrush(needleColor);
            for(int i=0; i<360;i+=45){
                painter->rotate(45);
                painter->drawConvexPolygon(needle, 3);
            }
        } painter->restore();

        // Draw the plane polygon
        QPen planePen(QColor(200,200,200));
        planePen.setWidth(3);
        painter->setPen(planePen);
        painter->setBrush(Qt::NoBrush);
        painter->save(); {
            painter->rotate(180);
            painter->scale(DIRECTION_INDICATOR_PLANE_SCALE,DIRECTION_INDICATOR_PLANE_SCALE);
            painter->drawConvexPolygon(plane, sizeof(plane)/sizeof(plane[0]));
        } painter->restore();

    } painter->restore();

    PanelItem::paint(painter, option, widget);
}

void DirectionIndicator::itemSizeChanged(float w, float h) {
    // The guage has changed size: redraw the cached card.
    createCard(w, h);
}

void DirectionIndicator::setLabel(QString text) {
    _label = text;
}

void DirectionIndicator::setRange1(float r1) {
    _range1 = qMax(r1, 1.0f);
    update();
}

void DirectionIndicator::setRange2(float r2) {
    _range2 = qMax(r2, 1.0f);
    update();
}

void DirectionIndicator::setUnit(DistanceUnit unit) {
    units = unit;
    setLabel(Units::unitName(units));
    update();
}

void DirectionIndicator::refChanged(QString name, double hdg) {
    if(name==_dataRef) {
        if(hdg == _value) return;
        _value = hdg;
    }
    update();
}

float DirectionIndicator::value2Angle1(float value) {
    return (value / _range1) * 360.0;
}

void DirectionIndicator::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

}
void DirectionIndicator::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    DistanceUnit unit = Units::distanceUnitForName(unitname);
    setUnit(unit);
    setRange1(settings.value("range1", 360).toDouble());
    setRange2(settings.value("range2", 360).toDouble());
    setThinBars(settings.value("thinbars", 10).toDouble());
    setThickBars(settings.value("thickbars", 30).toDouble());
    setNumbers(settings.value("numbers", 30).toDouble());
    setNumbersScale(settings.value("numbersscale", 0.1).toDouble());
}

void DirectionIndicator::createSettings(QGridLayout *layout) {

}

void DirectionIndicator::setThickBars(float v){
    _thickBars = v;
    update();
}

void DirectionIndicator::setThinBars(float v) {
    _thinBars = v;
    update();
}

void DirectionIndicator::setNumbersScale(float v) {
    _numbersScale = v;
    update();
}
