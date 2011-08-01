/*
 *  HSI_KS55.cpp
 *  extplane-panel
 *
 *  Created by bobgates on 2011/08/01.
 *  Copyright 2011 DaffeySoft. All rights reserved.
 *
 */

#include "hsi_ks55.h"


HSI::HSI(QObject *parent, ExtPlaneConnection *conn) :
PanelItem(parent), _client(this, typeName(), conn)
{
    
    _bezel = QPixmap::fromImage(QImage(QString("../../images/KCS_55_bezel.png")), Qt::AutoColor);
    
    _thickBars=10;
    _thinBars=5;
    _numbers=30;
    
    createCard(); 
    
    
    _heading=45;
    
    
    _dataRef = QString("sim/cockpit2/radios/indicators/hsi_bearing_deg_mag_pilot");
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));
    _client.subscribeDataRef(_dataRef,0.1);
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
    
    painter->save();
    painter->translate(0, centreY);
    painter->rotate(- _heading);
    painter->drawPixmap(-67, -67, 134, 134, _card);
    painter->restore();
    
    painter->setPen(Qt::NoPen);
    
   
    QPen planePen(QColor(200,200,200));
    planePen.setWidth(3);
    painter->setPen(planePen);
    painter->setBrush(Qt::NoBrush);
    painter->save();
    painter->rotate(value2Angle(_heading)+180);
    //painter->scale(0.9,0.9);
    
    //painter->drawConvexPolygon(Plane, sizeof(Plane)/sizeof(Plane[0]));
    painter->restore();
    
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


void HSI::refChanged(QString name, double hdg) {
    if(name==_dataRef) {
        if(hdg == _heading) return;
        _heading = hdg;
    }
    update();
}

float HSI::value2Angle(float value) {
    return (value / 360.0) * 360.0;
}


QString HSI::typeName() {
    return typeNameStatic();
}

QString HSI::typeNameStatic() {
    return "indicator/heading/hsi_KS5";
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

