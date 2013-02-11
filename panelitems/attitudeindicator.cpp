/*
 *  attitudeindicator.cpp
 *  extplane-panel
 *
 *  Created by bobgates on 2011/07/08.
 *  Copyright 2011 DaffeySoft. All rights reserved.
 *
 */

#include "attitudeindicator.h"
#include "math.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(AttitudeIndicator,"indicator/attitude/basic");

AttitudeIndicator::AttitudeIndicator(QObject *parent, ExtPlaneConnection *conn) :
PanelItem(parent), _client(this, typeName(), conn)
{
/*    _value = 0;
    setThickBars(50);
    setThinBars(10);
    setRange1(500);
    setRange2(5000);
    setNumbers(50);
    setNumbersScale(0.01);
    units = DISTANCE_M;
    baroUnits = PRESSURE_HPA;
*/
  
    //     boost::shared_ptr<Sprite> _cardSprite(new Sprite);
    
    _pitchValue = 10;
    _rollValue = -20;
    
    _pitchRef = QString("sim/cockpit2/gauges/indicators/pitch_vacuum_deg_pilot");
    _rollRef = QString("sim/cockpit2/gauges/indicators/roll_vacuum_deg_pilot");
    
    createCard();
    createGlass();
    createFrame();
    createBackground();
    
    _bezel = QPixmap::fromImage(QImage(QString("../../images/bezel_square_.png")), Qt::AutoColor);

    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));
    _client.subscribeDataRef(_pitchRef,0.05);
    _client.subscribeDataRef(_rollRef,0.05);
    
}

void AttitudeIndicator::createCard(void){
    QImage _cardImage = QImage(QSize(510,360), QImage::Format_ARGB32);
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

    // Draw background 
    // Use linear left to right mirrored gradients to
    // get a lit in the middle effect that suggests a
    // curved surface
    p.setPen(Qt::NoPen);

    QLinearGradient gradient(5, midy-5,midx,midy-5);
    gradient.setSpread(QGradient::ReflectSpread);
    gradient.setColorAt(0, LIGHTSKYBLUE);
    gradient.setColorAt(1, WHITEBLUE);
    QBrush gbrush(gradient); 
    p.setBrush(gbrush);                    // blue sky
    
    p.drawChord(0,0,width, height, 0, 180*16);
    
    gradient.setColorAt(0, GROUNDBROWN);
    gradient.setColorAt(1, LIGHTGROUNDBROWN);
    QBrush gbrush2(gradient); 
    p.setBrush(gbrush2);
    p.drawChord(0,0, width, height, 180*16, 180*16);    
    
    
    // Draw horizon and angle cues 
    p.setPen(QPen(Qt::white, 7, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p.drawLine(0,midy,width-1,midy);

    p.setPen(QPen(Qt::white, 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    
    float sl = midx*0.76;
    float ll = midx*0.89;
    
    p.drawLine(midx,midy, 
               midx+cos(30./180.*3.14159)*ll, 
               midy+sin(30./180.*3.14159)*ll);
    p.drawLine(midx,midy, 
               midx+cos(60./180.*3.14159)*sl,
               midy+sin(60./180.*3.14159)*sl);
    p.drawLine(midx,midy, 
               midx-cos(30./180.*3.14159)*ll, 
               midy+sin(30./180.*3.14159)*ll);
    p.drawLine(midx,midy, 
               midx-cos(60./180.*3.14159)*sl,
               midy+sin(60./180.*3.14159)*sl);
    
    // Draw pitch up and down lines
    
    p.setPen(QPen(QColor(0,0,15), 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p.drawLine(midx-45, midy-60*pitchPixelsPerDegree, midx+45,midy-60*pitchPixelsPerDegree);
    p.drawLine(midx-24, midy-30*pitchPixelsPerDegree, midx+24,midy-30*pitchPixelsPerDegree);
             
    p.setPen(QPen(Qt::white, 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p.drawLine(midx-45, midy+60*pitchPixelsPerDegree, midx+45,midy+60*pitchPixelsPerDegree);
    p.drawLine(midx-24, midy+30*pitchPixelsPerDegree, midx+24,midy+30*pitchPixelsPerDegree);
    
    
    p.end();    
    _card = QPixmap::fromImage(_cardImage, Qt::AutoColor);
    
}

void AttitudeIndicator::createFrame(void){
    
    QImage _frameImage = QImage(QSize(600,600), QImage::Format_ARGB32);
    _frameImage.fill(0x00ff0000);
    
    uint midx, midy, width, height;
    width = _frameImage.width();
    midx = width/2;
    height = _frameImage.height();
    midy = height/2;
    
    QPainter p;
    p.setRenderHint(QPainter::Antialiasing, true);
    p.begin(&_frameImage);
    p.translate(300, 300);

    
  
     QPainterPath pathTop;
     pathTop.moveTo(240,0);
     pathTop.lineTo(300,0);
     pathTop.arcTo(-300,-300,600,600, 0, 180);
     pathTop.lineTo(-240,0);
     pathTop.arcTo(-240,-240,480,480, 180, -180);
     
    QPainterPath pathBottom;
    pathBottom.moveTo(240,0);
    pathBottom.lineTo(300,0);
    pathBottom.arcTo(-300,-300,600,600, 0, -180);
    pathBottom.lineTo(-240,0);
    pathBottom.arcTo(-240,-240,480,480, 180, 180);
    
    //    p.setPen(QPen(QColor(79, 106, 25), 0, Qt::SolidLine,
    //                     Qt::FlatCap, Qt::MiterJoin));
    p.setPen(QPen(QColor(0, 0, 75), 4, Qt::SolidLine,
                  Qt::FlatCap, Qt::MiterJoin));
    p.setBrush(GROUNDBROWN);
    
    p.drawPath(pathBottom);
    p.setPen(Qt::NoPen);
 
     p.setPen(QPen(QColor(0, 0, 75), 4, Qt::SolidLine,
                    Qt::FlatCap, Qt::MiterJoin));
     
     p.setBrush(SKYBLUE);
     p.drawPath(pathTop);
    
    
    p.setPen(QPen(Qt::white, 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    for(int i=70; i<=110; i+=10){
        p.drawLine(240.*cos(float(i)/180.*3.1415926),-240.*sin(float(i)/180.*3.1415926),
                   300.*cos(float(i)/180.*3.1415926),-300.*sin(float(i)/180.*3.1415926));
    }

    p.setPen(QPen(Qt::white, 10, Qt::SolidLine,
                  Qt::FlatCap, Qt::MiterJoin));
    for(int i=0; i<=180; i+=30){
        p.drawLine(240.*cos(float(i)/180.*3.1415926),-240.*sin(float(i)/180.*3.1415926),
                   300.*cos(float(i)/180.*3.1415926),-300.*sin(float(i)/180.*3.1415926));
    }
    
    p.end();    
    
    _frame = QPixmap::fromImage(_frameImage, Qt::AutoColor);
    
}

void AttitudeIndicator::createGlass(void){
    QImage _glassImage = QImage(QSize(500,500), QImage::Format_ARGB32);
    _glassImage.fill(0x00ffffff);
    
    QPainter p;
    p.setRenderHint(QPainter::Antialiasing, true);
    p.begin(&_glassImage);
    //p.scale(1./2.5, 1./2.5);
    

    
    // Cosmetics on glass: yellow arrows:
    p.setPen(QPen(QColor(79, 106, 25), 0, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));
    p.setBrush(Qt::SolidPattern);
    p.setBrush(Qt::yellow);
    
    static const QPointF bigArrow[] = {
        QPointF(250, 250),
        QPointF(362.5, 287.5),
        QPointF(137.5, 287.5)
    };
    int bigArrowNPts = sizeof(bigArrow)/sizeof(bigArrow[0]);
    
    p.drawConvexPolygon(bigArrow, bigArrowNPts);

    static const QPointF leftArrow[] = {
        QPointF(125, 375-10),
        QPointF(237.5, 320-10),
        QPointF(240, 322.5-10),
        QPointF(137.5, 387.5-10),
        QPointF(125,387.5-10)
    };
    int leftArrowNPts = sizeof(leftArrow)/sizeof(leftArrow[0]);
    p.drawConvexPolygon(leftArrow, leftArrowNPts);

    static const QPointF rightArrow[] = {
        QPointF(375, 375-10),
        QPointF(262.5, 320-10),
        QPointF(260, 322.5-10),
        QPointF(362.5,387.5-10),
        QPointF(375,387.5-10)
    };
    p.drawConvexPolygon(rightArrow, leftArrowNPts);
    
    // Upwards facing orange arrow at vertical up:

    static const QPointF orangeArrow[] = {
        QPointF(250, 50),
        QPointF(233, 88),
        QPointF(267, 88)
    };
    p.setBrush(QColor(255,116,0));
    p.drawConvexPolygon(orangeArrow, 3);
    
    // Little black pyramid and chord at bottom:
    p.setBrush(QColor(25,25,25));
    static const QPointF pyramid[] = {
        QPointF(250-19-60, 417+20),
        QPointF(250+19+60, 417+20),
        QPointF(250+25, 250+63+20),
        QPointF(250-25, 250+63+20)
    };
    
    p.drawConvexPolygon(pyramid, 4);
    p.setPen(QPen(Qt::black, 0, Qt::SolidLine,
                  Qt::FlatCap, Qt::MiterJoin));

    p.setBrush(QColor(76,76,76));
    p.drawChord(_glassImage.rect(), -40*16, -100*16);

    p.setBrush(QColor(25,25,25));
    p.drawChord(_glassImage.rect(), -42*16, -96*16);
    
    
    // Little vacuum text line
    p.setPen(QColor(200,200,200));
    p.setFont(QFont(QString("Helvetica"), 24, QFont::Bold, false));
    int width = p.fontMetrics().width(QString("VACUUM"));
    int height =p.fontMetrics().height();
    p.drawText(250-width/2,385,width, height, Qt::AlignCenter,  "VACUUM");

    
    
    p.end();    
    
    _glass = QPixmap::fromImage(_glassImage, Qt::AutoColor);

}    

void AttitudeIndicator::createBackground(void){
    QImage _glassImage = QImage(QSize(200,200), QImage::Format_ARGB32);
    _glassImage.fill(0x00ffffff);
    
    QPainter p;
    p.setRenderHint(QPainter::Antialiasing, true);
    p.begin(&_glassImage);
    p.translate(100, 100);
    
    QLinearGradient gradient(0,-85,0,85);
    gradient.setColorAt(0, SKYBLUE);
    gradient.setColorAt(1, GROUNDBROWN);
 
    QBrush gbrush(gradient); 
    p.setBrush(gbrush);
    p.drawChord(-85,-85,170,170, 0, 360*16);
     
    p.end();    
    
    _background = QPixmap::fromImage(_glassImage, Qt::AutoColor);
    
}    

void AttitudeIndicator::setNumbers(float div) {
/*    _numbers = div;
    update();
*/
}

void AttitudeIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    
    QColor needleColor(255, 255, 255);
    
    int side = qMin(width(), height());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();
    painter->scale(side / 200.0, side / 200.0);
    painter->save();
    painter->translate(100, 100);

    painter->setPen(Qt::white);
    
    painter->setBrush(Qt::NoBrush);
    
    painter->save();
    painter->scale(0.92, 0.92);
    
    
    painter->save();
    painter->rotate(-_rollValue);
    
    float pitch = _pitchValue;
    if (pitch>maxPitch) pitch=maxPitch;
    if (pitch<(0-maxPitch)) pitch=(0-maxPitch);
    pitch *= pitchPixelsPerDegree;

    
    // Draw background to moving disk as a gradient from sky to ground:
    
    painter->drawPixmap(-_background.width()/2, 
                        -_background.height()/2, 
                        _background.width(), 
                        _background.height(), 
                        _background);
    
    QRectF source(0.,0.,(float)_card.width(), (float)_card.height());
    
    QRectF target((float)-_card.width()/6., 
                  (float)pitch - (float)_card.height()/6., 
                  (float)_card.width()/3., 
                  (float)_card.height()/3);
    
    painter->drawPixmap(target, _card, source);
                        /*-_card.width()/6., 
                        pitch - _card.height()/6., 
                        _card.width()/3., 
                        _card.height()/3, 
                        _card); */
    
    painter->drawPixmap(-_frame.width()/6., 
                        -_frame.height()/6., 
                        _frame.width()/3., 
                        _frame.height()/3., 
                         _frame);

    painter->restore();     //Unroll
    
    // Cosmetics on glass: yellow arrows:
    painter->drawPixmap(-_glass.width()/5., 
                        -_glass.height()/5., 
                         _glass.width()/2.5, 
                         _glass.height()/2.5, 
                         _glass);
    

    if (0){
        // Print roll and pitch on AI for debugging, if needed.
        QString pitchS = QString::number(pitch, 'f', 2);
        int width = painter->fontMetrics().width(pitchS);
        int height =painter->fontMetrics().height();
        painter->drawText(-width/2,68,width, height, Qt::AlignCenter,  pitchS);
        pitchS = QString::number(_rollValue, 'f', 2);
        width = painter->fontMetrics().width(pitchS);
        height =painter->fontMetrics().height();
        painter->drawText(-width/2,80,width, height, Qt::AlignCenter,  pitchS);
    }

    painter->restore();
    painter->drawPixmap(-100,-100,200,200, _bezel);
    
    painter->restore();     //Untranslate
    painter->restore();     //Unscale


    
    
    PanelItem::paint(painter, option, widget);

}


void AttitudeIndicator::setLabel(QString text) {
    _label = text;
}

void AttitudeIndicator::setRange1(float r1) {
    _range1 = qMax(r1, 1.0f);
    update();
}

void AttitudeIndicator::setRange2(float r2) {
    _range2 = qMax(r2, 1.0f);
    update();
}

void AttitudeIndicator::setUnit(DistanceUnit unit) {
    units = unit;
    setLabel(Units::unitName(units));
    update();
}

void AttitudeIndicator::refChanged(QString name, double value) {
    if (name==_rollRef) {
        if(value == _rollValue) return;
        _rollValue = value;
    } else if (name==_pitchRef) {
        if(value == _pitchValue) return;
        _pitchValue = value;
    }
    update();
}

float AttitudeIndicator::value2Angle1(float value) {
    return (value / _range1) * 360.0;
}

void AttitudeIndicator::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);

}
void AttitudeIndicator::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

}

void AttitudeIndicator::createSettings(QGridLayout *layout) {

}

void AttitudeIndicator::setThickBars(float v){
    _thickBars = v;
    update();
}

void AttitudeIndicator::setThinBars(float v) {
    _thinBars = v;
    update();
}

void AttitudeIndicator::setNumbersScale(float v) {
    _numbersScale = v;
    update();
}
