#include "turnbank.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <QLabel>
#include "../widgets/distanceunitcombobox.h"
#include "extplaneclient.h"
#include "../util/units.h"
#include "../widgets/numberinputlineedit.h"

#define GROUNDBROWN QColor(191,163,94)
#define LIGHTGROUNDBROWN QColor(231,203,134)

#define LIGHTSKYBLUE QColor(191,226,255)
#define VERYLIGHTBLUE QColor(221, 241, 255)
#define WHITEBLUE QColor(240, 248, 255)
#define SKYBLUE QColor(79, 129, 255)

REGISTER_WITH_PANEL_ITEM_FACTORY(TurnAndBank,"indicator/turnbank/basic");

TurnAndBank::TurnAndBank(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
    _client(this, typeName(), conn)
{
    _rollValue = 10;
    _slipValue = -20;
    
    //    _rollRef = QString("sim/cockpit2/gauges/indicators/turn_rate_roll_deg_pilot");
    _rollRef = QString("sim/cockpit2/gauges/indicators/roll_electric_deg_pilot");   // Matches XP's C172
    //_rollRef = QString("sim/cockpit2/gauges/indicators/turn_rate_heading_deg_pilot");
    _slipRef = QString("sim/cockpit2/gauges/indicators/slip_deg");
    
    //    _card = QPixmap(QString("junk"));//../images/engine_FUELP.png"));

    createCard();
    //createGlass();
    createFrame();
    createBall();
    //_bezel = QPixmap::fromImage(QImage(QString("../images/bezel_square_.png")), Qt::AutoColor);
    
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(refChanged(QString,double)));
    _client.subscribeDataRef(_slipRef,0.02);
    _client.subscribeDataRef(_rollRef,0.1);
    
}

void TurnAndBank::createCard(void){
    QImage _cardImage = QImage(QSize(500,150), QImage::Format_ARGB32);
    _cardImage.fill(0x00ff0000);
    
    uint midx, midy, width, height;
    width = _cardImage.width();
    midx = width/2;
    height = _cardImage.height();
    midy = height/2;
    
    
    QPainter p;
    p.setRenderHint(QPainter::Antialiasing, true);
    p.begin(&_cardImage);
    
    p.setPen(QPen(Qt::white, 7, Qt::SolidLine,
                  Qt::FlatCap, Qt::MiterJoin));
    p.setBrush(Qt::SolidPattern);
    p.setBrush(Qt::white);
    
    
    p.drawChord(midx-30, midy-30, 60, 60, 0, 360*16);
    
    p.drawLine(midx-60, midy-30,  midx+60, midy-30);
    p.drawLine(midx, midy-30,  midx, midy-80);
    p.drawLine(midx-240, midy,  midx+240, midy);
    
    
    p.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    static const QPointF rightWing[] = {
        QPointF(midx, midy-2),
        QPointF(midx+245, midy-3),
        QPointF(midx+244, midy+3),
        QPointF(midx, midy+15)
    };
    p.drawConvexPolygon(rightWing, 4);

    static const QPointF leftWing[] = {
        QPointF(midx, midy-2),
        QPointF(midx-245, midy-3),
        QPointF(midx-244, midy+3),
        QPointF(midx, midy+15)
    };
    p.drawConvexPolygon(leftWing, 4);
    
    
    
    p.end();
    _card = QPixmap::fromImage(_cardImage, Qt::AutoColor);
    
}

void TurnAndBank::createFrame(void){
    QImage _frameImage = QImage(QSize(600,600), QImage::Format_ARGB32);
    _frameImage.fill(0x00ff0000);
    
    
    int outerR = _frameImage.width()/2;
    const int innerR = _frameImage.width()/2-60;
    
    QPainter p;
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    p.begin(&_frameImage);
    p.translate(300, 300);

    p.setPen(Qt::NoPen);
    p.setBrush(Qt::SolidPattern);
    p.setBrush(QColor(225,225,225));

    QRadialGradient gradient(0, 0, outerR, 0, 0);
    gradient.setSpread(QGradient::ReflectSpread);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::black);
    QBrush gbrush(gradient);
    p.setBrush(gbrush);
    p.drawChord(-outerR, -outerR, 2*outerR, 2*outerR, 0, 360*16);

    //Ring outside of intstrument with white line:
    if (0){
        p.setPen(QPen(QColor(225,225,225), 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        p.setBrush(Qt::NoBrush);
        p.drawChord(-outerR, -outerR, 2*outerR, 2*outerR, 0, 360*16);
    }
    
    // Create black inner core:
    p.setBrush(Qt::black);
    p.setPen(Qt::NoPen);
    p.drawChord(-innerR, -innerR, 2*innerR, 2*innerR, 0, 360*16);

    
    // Put horizontal and 2minute lines on outside:
    p.setPen(QPen(Qt::white, 7, Qt::SolidLine,
                  Qt::FlatCap, Qt::MiterJoin));
    
    const float angle = 15.;
    
    outerR = outerR-20;

    p.drawLine(innerR*cos(angle/180*M_PI), innerR*sin(angle/180*M_PI),
               outerR*cos(angle/180*M_PI), outerR*sin(angle/180*M_PI));
    p.drawLine(-innerR*cos(angle/180*M_PI), innerR*sin(angle/180*M_PI),
               -outerR*cos(angle/180*M_PI), outerR*sin(angle/180*M_PI));

    p.drawLine(innerR, 0, outerR, 0);
    p.drawLine(-innerR, 0, -outerR, 0);
    
    // Little vacuum text line
    p.setPen(QColor(200,200,200));

    QFont topLabelFont = defaultFont;
    topLabelFont.setBold(true);
    p.setFont(topLabelFont);

    QString legend = "NO PITCH";
    int width = p.fontMetrics().width(legend);
    int height = p.fontMetrics().height();
    p.drawText(0-width/2,245,width, height, Qt::AlignCenter, legend);

    legend = "INFORMATION";
    width = p.fontMetrics().width(legend);
    p.drawText(0-width/2,267,width, height, Qt::AlignCenter, legend);

    legend = "D. C. ELEC.";
    width = p.fontMetrics().width(legend);
    height =p.fontMetrics().height();
    p.drawText(0-width/2,-275,width, height, Qt::AlignCenter, legend);
    
    legend = "2 MIN";
    width = p.fontMetrics().width(legend);
    height =p.fontMetrics().height();
    p.drawText(0-width/2,205,width, height, Qt::AlignCenter, legend);
    
    legend = "TURN COORDINATOR";
    width = p.fontMetrics().width(legend);
    height =p.fontMetrics().height();
    p.drawText(0-width/2,-150,width, height, Qt::AlignCenter, legend);
    
    QFont lrFont = defaultFont;
    lrFont.setPointSizeF(defaultFont.pointSizeF() * 2);
    lrFont.setBold(true);
    p.setFont(lrFont);

    legend = "L";
    width = p.fontMetrics().width(legend);
    height = p.fontMetrics().height();
    p.drawText(-265*cos(18./180.*M_PI)-width/2+5, 265*sin(18./180.*M_PI),
               width, height, Qt::AlignCenter, legend);
    
    legend = "R";
    width = p.fontMetrics().width(legend);
    height = p.fontMetrics().height();
    p.drawText(265*cos(18./180.*M_PI)-width/2-5, 265*sin(18./180.*M_PI),
               width, height, Qt::AlignCenter, legend);
    
    p.end();
    
    _frame = QPixmap::fromImage(_frameImage, Qt::AutoColor);
    
}

void TurnAndBank::createGlass(void){
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

void TurnAndBank::createBall(void){
    QImage _glassImage = QImage(QSize(800,800), QImage::Format_ARGB32);
    _glassImage.fill(0x00ffffff);
    
    QPainter p;
    
    p.setRenderHint(QPainter::Antialiasing, true);
    p.begin(&_glassImage);
    p.translate(400, 400);
    
    // QLinearGradient gradient(0,101,0,199);
    // gradient.setColorAt(0, SKYBLUE);
    // gradient.setColorAt(1, GROUNDBROWN);

    QRadialGradient gradient(0,-4500, 4750, 0, 30000);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::green);
    gradient.setSpread(QGradient::ReflectSpread);
    
    QBrush gbrush(gradient);
    p.setBrush(gbrush);
    //    p.drawRect(-350, 120, 700, 150);
    
    
    QPainterPath pathBottom;
    pathBottom.moveTo(-310, 150);
    pathBottom.arcTo(-2500+50,-3830-1000,5000,5000, -96, 12);
    //pathBottom.lineTo(310, 250);
    pathBottom.arcTo(-2000,-3700,4000,4000, -81, -18);
    //pathBottom.lineTo(-310, 150);
    
    //    pathBottom.lineTo(-240,0);
    //pathBottom.arcTo(-240,-240,480,480, 180, 180);
    
    //    p.setPen(QPen(QColor(79, 106, 25), 0, Qt::SolidLine,
    //                     Qt::FlatCap, Qt::MiterJoin));
    p.setPen(QPen(Qt::white, 4, Qt::SolidLine,
                  Qt::FlatCap, Qt::MiterJoin));
    p.setPen(Qt::NoPen);
    p.setBrush(gbrush);
    // p.setBrush(GROUNDBROWN);
    
    p.drawPath(pathBottom);
    
    p.setBrush(Qt::black);
    
    
    p.setPen(QPen(Qt::black, 4, Qt::SolidLine,
                  Qt::FlatCap, Qt::MiterJoin));
    p.drawLine(-50,170, -50, 300);
    p.drawLine(50,170, 50, 300);
    
    p.end();
    
    _ball = QPixmap::fromImage(_glassImage, Qt::AutoColor);
    
}    

void TurnAndBank::setNumbers(float div) {
    Q_UNUSED(div);
    /*    _numbers = div;
     update();
     */
}

void TurnAndBank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    int side = qMin(width(), height());
    setupPainter(painter);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->save();
    painter->scale(side / 200.0, side / 200.0);
    painter->save();
    painter->translate(100, 100);
    
    painter->setBrush(Qt::black);
    painter->drawChord(-95,-95,190,190,0,360*16);
    
    painter->save();
    painter->scale(0.92,0.92);
    
    painter->setPen(Qt::white);
    
    painter->setBrush(Qt::NoBrush);
    
    painter->drawPixmap(-_frame.width()/6.,
                        -_frame.height()/6.,
                        _frame.width()/3.,
                        _frame.height()/3.,
                        _frame);
    
    painter->drawPixmap(-80, -(_ball.height()*160./_ball.width())/2.,
                        160, _ball.height()*160./_ball.width(),
                        _ball);
    
    float x=0., y=0;
    const float pixelDeflectionPerDegree = 7.;
    
    if (_slipValue>maxOffbalance) _slipValue=maxOffbalance;
    if (_slipValue<(0-maxOffbalance)) _slipValue=0-maxOffbalance;
    
    
    x-=pixelDeflectionPerDegree*_slipValue;
    
    
    
    y-=fabs(_slipValue)/3.1;
    painter->setBrush(Qt::black);
    painter->drawEllipse(QRectF(x-10.,35.+y, 20., 25.));
    
    
    painter->save();
    float roll = _rollValue;
    if (roll>maxRoll) roll=maxRoll;
    if (roll<(0-maxRoll)) roll=(0-maxRoll);

    
    
    painter->rotate(roll);
    
    painter->drawPixmap(-80, -(_card.height()*160./_card.width())/2.,
                        160, _card.height()*160./_card.width(),
                        _card);
    
    painter->restore();     //Unroll
    
    if (0){
        // Print roll and pitch on AI for debugging, if needed.
        QString pitchS = QString::number(_slipValue, 'f', 2);
        int width = painter->fontMetrics().width(pitchS);
        int height =painter->fontMetrics().height();
        painter->drawText(-width/2,68,width, height, Qt::AlignCenter,  pitchS);
    }

    painter->restore();
    painter->drawPixmap(-100,-100,200,200, _bezel);
    
    painter->restore();     //Untranslate
    painter->restore();     //Unscale
    
    
    PanelItem::paint(painter, option, widget);
    
}


void TurnAndBank::setLabel(QString text) {
    _label = text;
}

void TurnAndBank::setRange1(float r1) {
    _range1 = qMax(r1, 1.0f);
    update();
}

void TurnAndBank::setRange2(float r2) {
    _range2 = qMax(r2, 1.0f);
    update();
}

void TurnAndBank::setUnit(DistanceUnit unit) {
    units = unit;
    setLabel(Units::unitName(units));
    update();
}

void TurnAndBank::refChanged(QString name, double value) {
    if (name==_rollRef) {
        if(value == _rollValue) return;
        _rollValue = value;
    } else if (name==_slipRef) {
        if(value == _slipValue) return;
        _slipValue = value;
    }
    update();
}

float TurnAndBank::value2Angle1(float value) {
    return (value / _range1) * 360.0;
}

void TurnAndBank::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);
    
}
void TurnAndBank::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
}

void TurnAndBank::createSettings(QGridLayout *layout) {
    Q_UNUSED(layout);
}

void TurnAndBank::setThickBars(float v){
    _thickBars = v;
    update();
}

void TurnAndBank::setThinBars(float v) {
    _thinBars = v;
    update();
}

void TurnAndBank::setNumbersScale(float v) {
    _numbersScale = v;
    update();
}
