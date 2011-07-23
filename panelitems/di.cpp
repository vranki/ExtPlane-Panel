/*
*  di.cpp
*  extplane-panel
*
*  Created by Bob Gates on 2011/07/08.
*  Copyright 2011 DaffeySoft. All rights reserved.
*
*/


#include "di.h"

DirectionIndicator::DirectionIndicator(QObject *parent, ExtPlaneConnection *conn) :
PanelItem(parent), _client(this, typeName(), conn)
{
    _value = 0;
    setThickBars(50);
    setThinBars(10);
    setRange1(500);
    setRange2(5000);
    setNumbers(50);
    setNumbersScale(0.01);
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

void DirectionIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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
    
    
    QColor needleColor(255, 255, 255);
    
    int side = qMin(width(), height());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();
    painter->scale(side / 200.0, side / 200.0);
    painter->save();
    painter->translate(100, 100);

    painter->save();
    painter->rotate(- _value);

    painter->setPen(Qt::white);
    
    painter->setBrush(Qt::black);
    painter->drawChord(-100,-100,200,200,0,360*16);

    
    painter->setBrush(Qt::white);
    if(_thickBars > 0) {
        for (float i = 0 ; i <= _range1; i+=_thickBars) {
            painter->save();
            painter->rotate(value2Angle1(i));
            painter->drawRect(-0.5, -100, 1.0, 10);
            painter->restore();
        }
    }
    if(_thinBars > 0) {
        for (float i = 0 ; i <= _range2; i+=_thinBars) {
            painter->save();
            painter->rotate(value2Angle1(i));
            painter->drawRect(-0.25, -100, 0.5, 8);
            painter->restore();
        }
    }
    painter->setPen(QColor(200,200,200));
    painter->setFont(QFont(QString("Helvetica"), 16, QFont::Bold, false));
    
    if(_numbers != 0) {
        for (float i = 0 ; i < _range1; i+=_numbers) {
            painter->save();
            painter->rotate(value2Angle1(i));
            painter->save();
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
            painter->translate(0,-78);
            int width = painter->fontMetrics().width(lineNumber);
            int height = painter->fontMetrics().height();
            painter->drawText(-width/2,-height/2,width,height, Qt::AlignCenter,  lineNumber);
            painter->restore();
            painter->restore();
        }
    }
    painter->setPen(Qt::NoPen);

    painter->setBrush(needleColor);
    for(int i=0; i<360;i+=45){
        painter->save();
        painter->rotate(value2Angle1(_value)+i);
        painter->drawConvexPolygon(needle, 3);
        painter->restore();
    }

    QPen planePen(QColor(200,200,200));
    planePen.setWidth(3);
    painter->setPen(planePen);
    painter->setBrush(Qt::NoBrush);
    painter->save();
    painter->rotate(value2Angle1(_value)+180);
    painter->scale(0.9,0.9);
    painter->drawConvexPolygon(Plane, sizeof(Plane)/sizeof(Plane[0]));
    painter->restore();
    
    painter->restore();
    painter->setBrush(Qt::white);
    

    painter->restore();
    
    painter->restore();
    
    
    
    PanelItem::paint(painter, option, widget);
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


QString DirectionIndicator::typeName() {
    return typeNameStatic();
}

QString DirectionIndicator::typeNameStatic() {
    return "indicator/heading/basic";
}

void DirectionIndicator::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

}
void DirectionIndicator::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
    QString unitname = settings.value("unit").toString();
    DistanceUnit unit = Units::distanceUnitForName(unitname);
    setUnit(unit);
    setRange1(settings.value("range1", 360).toFloat());
    setRange2(settings.value("range2", 360).toFloat());
    setThinBars(settings.value("thinbars", 10).toFloat());
    setThickBars(settings.value("thickbars", 30).toFloat());
    setNumbers(settings.value("numbers", 30).toFloat());
    setNumbersScale(settings.value("numbersscale", 0.1).toFloat());
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
