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

//#include <boost/shared_ptr.hpp>

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
    
    //    _card = QPixmap(QString("junk"));//../images/engine_FUELP.png"));
    
    
    createCard();
    createGlass();
    
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));
    _client.subscribeDataRef(_pitchRef,0.1);
    _client.subscribeDataRef(_rollRef,0.1);
    
}

void AttitudeIndicator::createCard(void){
    QImage _cardImage = QImage(QSize(170,120), QImage::Format_ARGB32);
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
    QPen pen(Qt::SolidPattern, 3);
    pen.setColor(Qt::white);
    p.setPen(pen);
    p.drawLine(0,midy,width-1,midy);
    pen.setWidthF(1.5);
    p.setPen(pen);
    p.drawLine(midx,midy, width-1, midy+sin(30./180.*3.14159)*midx);
    p.drawLine(midx,midy, midx+cos(30./180.*3.14159)*midy, height-1);
    p.drawLine(midx,midy, 0, midy+sin(30./180.*3.14159)*midx);
    p.drawLine(midx,midy, midx-cos(30./180.*3.14159)*midy, height-1 );
    
    // Draw pitch up and down lines
    
    p.setPen(Qt::SolidPattern);
    pen.setColor(QColor(0,0,15)); // very slightly blue rather than black
    pen.setWidthF(2);
    p.setPen(pen);
    p.drawLine(midx-10, midy-20, midx+10,midy-20);
    p.drawLine(midx-5, midy-10, midx+5,midy-10);
             
    pen.setColor(Qt::white);
    p.setPen(pen);
    p.drawLine(midx-10, midy+20, midx+10,midy+20);
    p.drawLine(midx-5, midy+10, midx+5,midy+10);
    
    
    p.end();    
    
   
    
    _card = QPixmap::fromImage(_cardImage, Qt::AutoColor);
    
}

void AttitudeIndicator::createFrame(void){
/*    
    QImage _cardImage = QImage(QSize(170,120), QImage::Format_ARGB32);
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
    QPen pen(Qt::SolidPattern, 3);
    pen.setColor(Qt::white);
    p.setPen(pen);
    p.drawLine(0,midy,width-1,midy);
    pen.setWidthF(1.5);
    p.setPen(pen);
    p.drawLine(midx,midy, width-1, midy+sin(30./180.*3.14159)*midx);
    p.drawLine(midx,midy, midx+cos(30./180.*3.14159)*midy, height-1);
    p.drawLine(midx,midy, 0, midy+sin(30./180.*3.14159)*midx);
    p.drawLine(midx,midy, midx-cos(30./180.*3.14159)*midy, height-1 );
    
    // Draw pitch up and down lines
    
    p.setPen(Qt::SolidPattern);
    pen.setColor(QColor(0,0,15)); // very slightly blue rather than black
    pen.setWidthF(2);
    p.setPen(pen);
    p.drawLine(midx-10, midy-20, midx+10,midy-20);
    p.drawLine(midx-5, midy-10, midx+5,midy-10);
    
    pen.setColor(Qt::white);
    p.setPen(pen);
    p.drawLine(midx-10, midy+20, midx+10,midy+20);
    p.drawLine(midx-5, midy+10, midx+5,midy+10);
    
    
    p.end();    
    
    
    
    _card = QPixmap::fromImage(_cardImage, Qt::AutoColor);
*/    
}


void AttitudeIndicator::createGlass(void){
    QImage _glassImage = QImage(QSize(200,200), QImage::Format_ARGB32);
    _glassImage.fill(0x00ffffff);
    
    QPainter p;
    p.setRenderHint(QPainter::Antialiasing, true);
    p.begin(&_glassImage);


    
    // Cosmetics on glass: yellow arrows:
    p.setPen(QPen(QColor(79, 106, 25), 0, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));
    p.setBrush(Qt::SolidPattern);
    p.setBrush(Qt::yellow);
    
    static const QPointF bigArrow[] = {
        QPointF(100, 100),
        QPointF(145, 115),
        QPointF(55, 115)
    };
    int bigArrowNPts = sizeof(bigArrow)/sizeof(bigArrow[0]);
    
    p.drawConvexPolygon(bigArrow, bigArrowNPts);

    static const QPointF leftArrow[] = {
        QPointF(50, 150),
        QPointF(95, 130),
        QPointF(96, 131),
        QPointF(55,155),
        QPointF(50,155)
    };
    int leftArrowNPts = sizeof(leftArrow)/sizeof(leftArrow[0]);
    p.drawConvexPolygon(leftArrow, leftArrowNPts);

    static const QPointF rightArrow[] = {
        QPointF(150, 150),
        QPointF(105, 130),
        QPointF(104, 131),
        QPointF(145,155),
        QPointF(150,155)
    };
    

    p.drawConvexPolygon(rightArrow, leftArrowNPts);

    p.end();    
    
    _glass = QPixmap::fromImage(_glassImage, Qt::AutoColor);

}    

void AttitudeIndicator::setNumbers(float div) {
/*    _numbers = div;
    update();
*/
}

void AttitudeIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    
    
    QPainterPath pathTop;
    pathTop.moveTo(80,0);
    pathTop.lineTo(100,0);
    pathTop.arcTo(-100,-100,200,200, 0, 180);
    pathTop.lineTo(-80,0);
    pathTop.arcTo(-80,-80,160,160, 180, -180);

    
    
    //QPainterPath elipseTop;
    //    ellipseTop.
    
    
    
    //path.addRect(20, 20, 60, 60);
    
    //path.moveTo(0, 0);
    //path.cubicTo(99, 0,  50, 50,  99, 99);
    //path.cubicTo(0, 99,  50, 50,  0, 0);
    
    //QPainter painter(this);
    
    
    
    
    
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
    painter->rotate(-_rollValue);
    

 
    float pitch = _pitchValue;
    if (pitch>maxPitch) pitch=maxPitch;
    if (pitch<(0-maxPitch)) pitch=(0-maxPitch);
    pitch *= pitchPixelsPerDegree;

    
    // Draw background to moving disk as a gradient from sky to ground:
    QLinearGradient gradient(0,-85,0,85);
    gradient.setColorAt(0, SKYBLUE);
    gradient.setColorAt(1, GROUNDBROWN);
    
    QBrush gbrush(gradient); 
    painter->setBrush(gbrush);
    painter->drawChord(-85,-85,170,170, 0, 360*16);
    
    painter->drawPixmap(-_card.width()/2, pitch - _card.height()/2, _card.width(), _card.height(), _card);
    
    painter->setPen(QPen(QColor(79, 106, 25), 1, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));
    
    painter->setBrush(SKYBLUE);
    painter->drawPath(pathTop);
    

    
    // Draw bottom half of frame in ground brown:
    QPainterPath pathBottom;
    pathTop.moveTo(80,0);
    pathTop.lineTo(100,0);
    pathBottom.arcTo(-100,-100,200,200, 0, -180);
    pathBottom.lineTo(-80,0);
    pathBottom.arcTo(-80,-80,160,160, 180, 180);
    pathBottom.lineTo(80,0);
    
    painter->setPen(QPen(QColor(79, 106, 25), 0, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));
    painter->setBrush(GROUNDBROWN);
    
    painter->drawPath(pathBottom);
    painter->setPen(Qt::NoPen);

    painter->restore();     //Unroll
    
    
    // Cosmetics on glass: yellow arrows:
    painter->drawPixmap(-_glass.width()/2, -_glass.height()/2, _glass.width(), _glass.height(), _glass);
    //painter->drawPixmap(0, 0, _glass.width(), _glass.height(), _glass);
        
/*  
    static const QPointF bigArrow[] = {
        QPointF(0, 0),
        QPointF(45, 15),
        QPointF(-45, 15)
    };
    int bigArrowNPts = sizeof(bigArrow)/sizeof(bigArrow[0]);
    
    static const QPointF leftArrow[] = {
        QPointF(-50, 50),
        QPointF(-5, 30),
        QPointF(-4, 31),
        QPointF(-45,55),
        QPointF(-50,55)
    };
    int leftArrowNPts = sizeof(leftArrow)/sizeof(leftArrow[0]);
    
    static const QPointF rightArrow[] = {
        QPointF(50, 50),
        QPointF(5, 30),
        QPointF(4, 31),
        QPointF(45,55),
        QPointF(50,55)
    };
    
    
    painter->setPen(QPen(QColor(0, 255, 0), 0, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));
    painter->setBrush(Qt::SolidPattern);
    painter->setBrush(Qt::red);
    painter->drawConvexPolygon(bigArrow, bigArrowNPts);
    painter->drawConvexPolygon(leftArrow, leftArrowNPts);
    painter->drawConvexPolygon(rightArrow, leftArrowNPts);
*/    
    // Cosmetics on glass, little black pyramid and chord:
    
    
    
    painter->restore();     //Untranslate
    painter->restore();     //Unscale
    
    
    PanelItem::paint(painter, option, widget);
    
    
    
/*    
    painter->save();
    painter->rotate(- _value);
    
    painter->setPen(Qt::white);
    
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
*/
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


QString AttitudeIndicator::typeName() {
    return typeNameStatic();
}

QString AttitudeIndicator::typeNameStatic() {
    return "indicator/attitude/basic";
}

void AttitudeIndicator::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);
/*    settings.setValue("unit", Units::unitName(units));
    settings.setValue("range1", _range1);
    settings.setValue("range2", _range2);
    settings.setValue("thinbars", _thinBars);
    settings.setValue("thickbars", _thickBars);
    settings.setValue("numbers", _numbers);
    settings.setValue("numbersscale", _numbersScale);
*/
}
void AttitudeIndicator::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
/*    QString unitname = settings.value("unit").toString();
    DistanceUnit unit = Units::distanceUnitForName(unitname);
    setUnit(unit);
    setRange1(settings.value("range1", 500).toFloat());
    setRange2(settings.value("range2", 5000).toFloat());
    setThinBars(settings.value("thinbars", 10).toFloat());
    setThickBars(settings.value("thickbars", 50).toFloat());
    setNumbers(settings.value("numbers", 100).toFloat());
    setNumbersScale(settings.value("numbersscale", 0.01).toFloat());
*/ 
}

void AttitudeIndicator::createSettings(QGridLayout *layout) {
/*    
    QLabel *unitsLabel = new QLabel("Unit", layout->parentWidget());
    layout->addWidget(unitsLabel, layout->rowCount(), 0);
    DistanceUnitComboBox *unitsCombo = new DistanceUnitComboBox(layout->parentWidget(), units);
    connect(unitsCombo, SIGNAL(unitSelected(DistanceUnit)), this, SLOT(setUnit(DistanceUnit)));
    layout->addWidget(unitsCombo);
    QLabel *range1Label = new QLabel("Range", layout->parentWidget());
    layout->addWidget(range1Label);
    NumberInputLineEdit *range1Edit = new NumberInputLineEdit(layout->parentWidget());
    range1Edit->setText(QString::number(_range1));
    layout->addWidget(range1Edit);
    connect(range1Edit, SIGNAL(valueChangedFloat(float)), this, SLOT(setRange1(float)));
    
    QLabel *range2Label = new QLabel("Range 2", layout->parentWidget());
    layout->addWidget(range2Label);
    NumberInputLineEdit *range2Edit = new NumberInputLineEdit(layout->parentWidget());
    range2Edit->setText(QString::number(_range2));
    layout->addWidget(range2Edit);
    connect(range2Edit, SIGNAL(valueChangedFloat(float)), this, SLOT(setRange2(float)));
    
    QLabel *thinBarsLabel = new QLabel("Thin bars every", layout->parentWidget());
    layout->addWidget(thinBarsLabel);
    NumberInputLineEdit *thinBarsEdit = new NumberInputLineEdit(layout->parentWidget());
    thinBarsEdit->setText(QString::number(_thinBars));
    layout->addWidget(thinBarsEdit);
    connect(thinBarsEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setThinBars(float)));
    
    QLabel *thickBarsLabel = new QLabel("Thick bars every", layout->parentWidget());
    layout->addWidget(thickBarsLabel);
    NumberInputLineEdit *thickBarsEdit = new NumberInputLineEdit(layout->parentWidget());
    thickBarsEdit->setText(QString::number(_thickBars));
    layout->addWidget(thickBarsEdit);
    connect(thickBarsEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setThickBars(float)));
    
    layout->addWidget(new QLabel("Numbers every", layout->parentWidget()));
    NumberInputLineEdit *numbersEdit = new NumberInputLineEdit(layout->parentWidget());
    numbersEdit->setText(QString::number(_numbers));
    layout->addWidget(numbersEdit);
    connect(numbersEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setNumbers(float)));
    
    layout->addWidget(new QLabel("Number scale", layout->parentWidget()));
    NumberInputLineEdit *numbersScaleEdit = new NumberInputLineEdit(layout->parentWidget());
    numbersScaleEdit->setText(QString::number(_numbersScale));
    layout->addWidget(numbersScaleEdit);
    connect(numbersScaleEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setNumbersScale(float)));
*/
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
