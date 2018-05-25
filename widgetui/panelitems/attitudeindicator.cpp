/*
 *  Created by bobgates on 2011/07/08.
 */

#include "attitudeindicator.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "../widgets/distanceunitcombobox.h"
#include "extplaneclient.h"
#include "../widgets/numberinputlineedit.h"
#include "../util/console.h"
#include <QLabel>

#define ATTITUDE_COLOR_BROWN_LIGHT QColor(164,147,108)
#define ATTITUDE_COLOR_BROWN_DARK QColor(106,91,60)
#define ATTITUDE_COLOR_BLUE_LIGHT QColor(161,189,255)
#define ATTITUDE_COLOR_BLUE_DARK QColor(98,116,165)

REGISTER_WITH_PANEL_ITEM_FACTORY(AttitudeIndicator,"indicator/attitude/basic");

AttitudeIndicator::AttitudeIndicator(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
    _client(this, typeName(), conn)
{
    // Init
    _pitchValue = 10;
    _rollValue = -20;
    _pitchRef = QString("sim/cockpit2/gauges/indicators/pitch_vacuum_deg_pilot");
    _rollRef = QString("sim/cockpit2/gauges/indicators/roll_vacuum_deg_pilot");

    // Subscribe refs
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));
    _client.subscribeDataRef(_pitchRef,0.05);
    _client.subscribeDataRef(_rollRef,0.05);
}

void AttitudeIndicator::createCard(float w, float h){
    // Init dimensions
    double size = 0.85;
    double targetW = qMin(w*size,h*size);
    double targetH = targetW*0.70;
    double realH = targetH*1.3;
    double midX = targetW / 2.0;
    double midY = targetH / 2.0;

    // Create image
    QImage _cardImage = QImage(QSize(targetW,realH), QImage::Format_ARGB32);
    _cardImage.fill(0x00ff0000);

    QPainter p;
    p.begin(&_cardImage);
    setupPainter(&p);
    p.translate(0,(realH-targetH)/2.0);

    // Shadow
    p.save(); {
        double shadowSize = 0.86;
        p.setPen(Qt::NoPen);
        double shadowRH = realH/2.0*shadowSize;
        QRadialGradient radial(QPointF(midX, midY), shadowRH);
        p.scale(1.8,1.0);
        p.translate(-midX*0.4,0);
        radial.setColorAt(0.6, Qt::black); //I want the center to be black
        radial.setColorAt(1, Qt::transparent); //I want the sides to be white
        p.setBrush(QBrush(radial));
        p.setOpacity(0.4);
        p.drawEllipse(QPointF(midX,midY),targetW/2.0*0.8,shadowRH);
    } p.restore();

    // Draw upper horizon
    QLinearGradient gradient(5, midY-5,midX,midY-5);
    gradient.setSpread(QGradient::ReflectSpread);
    gradient.setColorAt(0, ATTITUDE_COLOR_BLUE_DARK);
    gradient.setColorAt(1, ATTITUDE_COLOR_BLUE_LIGHT);
    QBrush gbrush(gradient);
    p.setPen(QPen(QColor(0,0,0,50),targetW*0.004));
    p.setBrush(gbrush);
    p.drawChord(0,0,targetW, targetH, 0, 180*16);

    // Draw lower ground
    gradient.setColorAt(0, ATTITUDE_COLOR_BROWN_DARK);
    gradient.setColorAt(1, ATTITUDE_COLOR_BROWN_LIGHT);
    QBrush gbrush2(gradient);
    p.setBrush(gbrush2);
    p.drawChord(0,0, targetW, targetH, 180*16, 180*16);

    p.save(); {
        // Original drawing code was 510x360, so we to keep that code we need to scale for it
        double origW = 510.0;
        double origH = 360;
        double origMidX = origW / 2.0;
        double origMidY = origH / 2.0;
        double scale = targetW/origW;
        p.scale(scale,scale);

        // Draw horizon and angle cues
        p.setPen(QPen(Qt::white, 7, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        p.drawLine(0,origMidY,origW-1,origMidY);
        p.setPen(QPen(Qt::white, 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        float sl = origMidX*0.72;
        float ll = origMidX*0.84;
        p.drawLine(origMidX,origMidY,origMidX+cos(30./180.*3.14159)*ll,origMidY+sin(30./180.*3.14159)*ll);
        p.drawLine(origMidX,origMidY,origMidX+cos(60./180.*3.14159)*sl,origMidY+sin(60./180.*3.14159)*sl);
        p.drawLine(origMidX,origMidY,origMidX-cos(30./180.*3.14159)*ll,origMidY+sin(30./180.*3.14159)*ll);
        p.drawLine(origMidX,origMidY,origMidX-cos(60./180.*3.14159)*sl,origMidY+sin(60./180.*3.14159)*sl);

        // Draw pitch up and down lines
        p.setPen(QPen(QColor(0,0,15), 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        p.drawLine(origMidX-45, origMidY-60*pitchPixelsPerDegree, origMidX+45,origMidY-60*pitchPixelsPerDegree);
        p.drawLine(origMidX-24, origMidY-30*pitchPixelsPerDegree, origMidX+24,origMidY-30*pitchPixelsPerDegree);
        p.setPen(QPen(Qt::white, 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        p.drawLine(origMidX-35, origMidY+80*pitchPixelsPerDegree, origMidX+35,origMidY+80*pitchPixelsPerDegree);
        p.drawLine(origMidX-17, origMidY+46*pitchPixelsPerDegree, origMidX+17,origMidY+46*pitchPixelsPerDegree);

    } p.restore();

    // Save as pixmap
    p.end();
    _card = QPixmap::fromImage(_cardImage, Qt::AutoColor);
}

void AttitudeIndicator::createFrame(float w, float h){
    // Init dimensions
    double origW = 600.0;
    double targetW = qMin(w,h);
    double targetH = targetW;

    // Create image
    QImage _frameImage = QImage(QSize(targetW, targetH), QImage::Format_ARGB32);
    _frameImage.fill(0x00ff0000);

    // Init painter and scale to the original drawing size at 600x600
    QPainter p;
    p.begin(&_frameImage);
    setupPainter(&p);
    double scale = targetW/origW;
    p.scale(scale,scale);
    p.translate(300, 300);

    // Shadow
    double shadowSize = 0.8;
    QPainterPath pathShadow;
    pathShadow.moveTo(240,0);
    pathShadow.lineTo(300,0);
    pathShadow.arcTo(-290,-290,580,580, 0, 360);
    pathShadow.lineTo(-240,0);
    pathShadow.arcTo(-240*shadowSize,-240*shadowSize,480*shadowSize,480*shadowSize, 0, 360);
    p.setPen(Qt::NoPen);
    QRadialGradient radial(QPointF(0, 0), 290);
    radial.setColorAt(0.74, Qt::transparent); //I want the center to be black
    radial.setColorAt(1, Qt::black); //I want the sides to be white
    p.setBrush(QBrush(radial));
    p.drawPath(pathShadow);

    // Upper half
    QPainterPath pathTop;
    pathTop.moveTo(240,0);
    pathTop.lineTo(300,0);
    pathTop.arcTo(-300,-300,600,600, 0, 180);
    pathTop.lineTo(-240,0);
    pathTop.arcTo(-240,-240,480,480, 180, -180);
    p.setPen(QPen(QColor(0, 0, 0), 4, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p.setBrush(SKYBLUE);
    p.drawPath(pathTop);
     
    // Lower half
    QPainterPath pathBottom;
    pathBottom.moveTo(240,0);
    pathBottom.lineTo(300,0);
    pathBottom.arcTo(-300,-300,600,600, 0, -180);
    pathBottom.lineTo(-240,0);
    pathBottom.arcTo(-240,-240,480,480, 180, 180);
    p.setPen(QPen(QColor(0, 0, 0), 4, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p.setBrush(ATTITUDE_COLOR_BROWN_DARK);
    p.drawPath(pathBottom);

    // Line markings
    double l1 = 243.0;
    double l2 = 298.0;
    p.setPen(QPen(Qt::white, 5, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    for(int i=70; i<=110; i+=10){
        p.drawLine(l1*cos(float(i)/180.*M_PI),-l1*sin(float(i)/180.*M_PI),
                   l2*cos(float(i)/180.*M_PI),-l2*sin(float(i)/180.*M_PI));
    }
    p.setPen(QPen(Qt::white, 10, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    for(int i=0; i<=180; i+=30){
        p.drawLine(l1*cos(float(i)/180.*M_PI),-l1*sin(float(i)/180.*M_PI),
                   l2*cos(float(i)/180.*M_PI),-l2*sin(float(i)/180.*M_PI));
    }

    // Save as pixmap
    p.end();
    _frame = QPixmap::fromImage(_frameImage, Qt::AutoColor);
}

void AttitudeIndicator::createGlass(float w, float h){

    // Init dimensions
    double origW = 500.0;
    double targetW = qMin(w,h);
    double targetH = targetW;

    // Create image
    QImage _glassImage = QImage(QSize(targetW,targetH), QImage::Format_ARGB32);
    _glassImage.fill(0x00ff0000);

    // Init painter and scale to the original drawing size at 500x500
    QPainter p;
    p.begin(&_glassImage);
    setupPainter(&p);
    double scale = targetW/origW;
    p.scale(scale,scale);

    // Cosmetics on glass: yellow arrows:
    p.setPen(QPen(QColor(0, 0, 0,50), 3, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p.setBrush(QColor(255,116,0));
    static const QPointF bigArrow[] = {
        QPointF(250, 250),
        QPointF(362.5, 287.5),
        QPointF(137.5, 287.5)
    };
    int bigArrowNPts = sizeof(bigArrow)/sizeof(bigArrow[0]);
    p.drawConvexPolygon(bigArrow, bigArrowNPts);
    p.setBrush(Qt::yellow);
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
    p.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    p.setBrush(QColor(76,76,76));
    p.drawChord(QRect(0,0,origW,origW), -40*16, -100*16);
    p.setBrush(QColor(25,25,25));
    p.drawChord(QRect(0,0,origW,origW), -42*16, -96*16);

    // Little vacuum text line
    p.setPen(QColor(200,200,200));
    QFont font(defaultFont);
    font.setPixelSize(20);
    font.setBold(true);
    p.setFont(font);
    int width = p.fontMetrics().width("VACUUM");
    int height = p.fontMetrics().height();
    p.drawText(250-width/2, 385, width, height, Qt::AlignCenter, "VACUUM");

    // Save as pixmap
    p.end();
    _glass = QPixmap::fromImage(_glassImage, Qt::AutoColor);
}    

void AttitudeIndicator::createBackground(float w, float h){
    // Init dimensions
    double origW = 200.0;
    double targetW = qMin(w,h);
    double targetH = targetW;

    // Create image
    QImage _glassImage = QImage(QSize(targetW,targetH), QImage::Format_ARGB32);
    _glassImage.fill(0x00ffffff);
    
    // Init painter and scale to the original drawing size at 200x200
    QPainter p;
    p.begin(&_glassImage);
    setupPainter(&p);
    double scale = targetW/origW;
    p.scale(scale,scale);
    p.translate(100, 100);
    
    // Draw back face
    QLinearGradient gradient(0,-85,0,85);
    gradient.setColorAt(0, SKYBLUE);
    gradient.setColorAt(1, GROUNDBROWN);
    QBrush gbrush(gradient); 
    p.setBrush(gbrush);
    p.drawChord(-85,-85,170,170, 0, 360*16);

    // Save as pixmap
    p.end();    
    _background = QPixmap::fromImage(_glassImage, Qt::AutoColor);
}    

void AttitudeIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    // Init
    setupPainter(painter);
    double side = qMin(width(), height());
    double pitch = _pitchValue;
    if (pitch > maxPitch) pitch = maxPitch;
    if (pitch < (0-maxPitch)) pitch = (0-maxPitch);

    // Do drawing of cached pixmaps
    painter->save(); {

        // Make sure to clip the widget because of moving card
        painter->setClipRect(0,0,width(),height());

        // Move to center and draw background
        painter->translate(width()/2, height()/2);
        painter->drawPixmap(-_background.width()/2,-_background.height()/2,_background);

        // Rotate to roll and draw moving card
        painter->rotate(-_rollValue);
        double pitchPixelsY = pitch*(side*0.005f);
        //DEBUG << pitchPixelsY;
        painter->drawPixmap(QPointF(-_card.width()/2.0,-_card.height()/2.0 + pitchPixelsY),_card);

        // Draw frame
        painter->drawPixmap(-_frame.width()/2,-_frame.height()/2,_frame);

        // Undo roll and draw glass
        painter->rotate(+_rollValue);
        painter->drawPixmap(-_glass.width()/2,-_glass.height()/2,_glass);

    } painter->restore();

    PanelItem::paint(painter, option, widget);
}

void AttitudeIndicator::itemSizeChanged(float w, float h) {
    // The guage has changed size: redraw the cached pixmaps.
    createCard(w,h);
    createFrame(w,h);
    createGlass(w,h);
    createBackground(w,h);
}

void AttitudeIndicator::refChanged(QString name, double value) {
    if (name == _rollRef) {
        _rollValue = value;
    } else if (name == _pitchRef) {
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
    Q_UNUSED(layout);
}
