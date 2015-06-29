#include "engine_fuel_p.h"

#include <math.h>
#include "../util/console.h"
#include "extplaneclient.h"



REGISTER_WITH_PANEL_ITEM_FACTORY(EngineFuelP,"indicator/engine/pressure")

EngineFuelP::EngineFuelP(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
                 PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
                _pressureNumber(1),
                _engineNumber(1),
                pressureValue(0),
                pressureValueMin(0),
                pressureValueMax(400),
                scaleFactor(1),
                _client(this, typeName(), conn),
                bottomImage(":/images/DR400_engine_FUELP.png"),
                bottomPixmap(0),
                needleImage(":/images/DR400_engine_FUELP_needle.png")
                {


    //init
    //subscibe to dataref
    _client.subscribeDataRef("sim/cockpit2/engine/indicators/fuel_pressure_psi", 15.0);
    connect(&_client, SIGNAL(refChanged(QString,QStringList)), this, SLOT(pressureChanged(QString,QStringList)));



    //set size
    if (! bottomImage.isNull()) {
        //dimension the item with the default background image
        this->setSize(bottomImage.width(),bottomImage.height());
    }
}

EngineFuelP::~EngineFuelP() {
    delete(bottomPixmap);
}

void EngineFuelP::storeSettings(QSettings &settings) {
    PanelItem::storeSettings(settings);
}

void EngineFuelP::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);
}

void EngineFuelP::createSettings(QGridLayout *layout) {
   
    QLabel *unitsLabel = new QLabel("Nothing to set for this time", layout->parentWidget());
    layout->addWidget(unitsLabel, layout->rowCount(), 0);
}

/*
 * draw background function to the ressource default image,
 * add arcs and set this pixmap scale to feet the item size
 */
void EngineFuelP::drawBottomPixmap() {

    int w,h; //width; height
    float arcGauge = 0.45;
    w=qMin(this->width(), this -> height());
    h=w;

    //destroy last bottomPixmap
    if ( ! (bottomPixmap == NULL) ) delete(bottomPixmap);
    //and create a new with the good dimension
    bottomPixmap = new QPixmap(w, h);
    bottomPixmap->fill(Qt::black);


    //set  painter
    QPainter pa(bottomPixmap);    
    pa.setRenderHint(QPainter::Antialiasing);

    //resize image
    this->scaleFactor = (float) w/bottomImage.width();
    pa.save();
    pa.scale(this->scaleFactor,this->scaleFactor);
    pa.drawImage(QPoint(0,0),bottomImage);
    pa.restore();

    QPen pen;
    QBrush brush;

    pa.save();
    //draw graduations
    pen.setWidth(1+w/250); //pen width increase when w increase
    pen.setColor(QColor(200,200,200)); //grey color
    pa.setPen(pen);
    pa.setBrush(brush);
    pa.translate(w/2,h/2);
    pa.rotate(-30);
    for (int i=0; i< 9; i++) {
        int l1 = 5;
        int l2 = 0;
        if ( i % 4 == 0 ) {
            l1 = 10;
            l2 = 10;
        }
        pa.drawLine( (w -  (l1 * this->scaleFactor)) * arcGauge/2, 0, (w + (l2 * this->scaleFactor))*arcGauge/2, 0);
        pa.rotate(-15.0);
    }
    pa.restore();

    //draw text
    pa.save();
    pen.setColor(QColor(250,250,250)); //grey color
    pa.setPen(pen);
    pa.setBrush(brush);
    QFont font("Verdana", 5* this->scaleFactor);
    QFontMetrics fm(font);
    pa.setFont(font);
    pa.translate(w/2,h/2);
    QString sMin, sMax, sMed;
    sMin.setNum(pressureValueMin,'f',0);
    sMed.setNum(pressureValueMax/2,'f',0);
    sMax.setNum(pressureValueMax,'f',0);

    pa.drawText(  cos(3.14159*150/180)*(w*arcGauge+8*scaleFactor)/-2 , sin(3.14159*150/180)*(w*arcGauge+8*scaleFactor)/-2 ,  sMax);
    pa.drawText(  cos(3.14159* 90/180)*(w*arcGauge+8*scaleFactor)/-2 - fm.width(sMed)/2 , sin(3.14159* 90/180)*(w*arcGauge+8*scaleFactor)/-2 ,sMed);
    pa.drawText(  cos(3.14159* 30/180)*(w*arcGauge+8*scaleFactor)/-2 - fm.width(sMin), sin(3.14159* 30/180)*(w*arcGauge+8*scaleFactor)/-2 ,sMin);

    pa.restore();

    //draw green arc
    pen.setWidth(1+w/150); //pen width increase when w increase
    pen.setColor(Qt::green);
    brush.setColor(Qt::green);
    pa.setPen(pen);
    pa.setBrush(brush);

    double startAngle = (45)*16.0;
    double endAngle = (75)*16.0;
    int x,y,ww,hh;
    ww=w*arcGauge;
    hh=h*arcGauge;
    x = (w - ww)/2;
    y = (h - hh)/2;
    pa.drawArc(x,y,ww,hh, startAngle, endAngle );

}

void EngineFuelP::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    int w,h;
    w=qMin(this->width(), this -> height());
    h=w;

    painter->save();
    //paint the background
    painter->drawPixmap(QPoint(0,0), *bottomPixmap);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);


    //paint the needle;
    if ( ! needleImage.isNull()) {

        int ww, hh;
        ww = needleImage.width() * this->scaleFactor;
        hh = needleImage.height() * this->scaleFactor;
        //center of the needle image is center of background image
        painter->translate((w-ww)/2, (h-hh)/2);

        int x,y,a;
        a=this->value2Angle(this->pressureValue); // rotation degree

        float alpha = a*3.14159/180;
        x = (cos(alpha)*ww + sin(alpha)*hh)/2;
        y = (-sin(alpha)*ww + cos(alpha)*hh )/2;

        x = x - ww/2;
        y = y - hh/2;

       painter->save();
            painter->rotate(a);
            painter->translate(x,y);
            painter->scale(this->scaleFactor,this->scaleFactor);
            painter->drawPixmap(0,0,needleImage);
        painter->restore();


    }
    painter->restore();
    PanelItem::paint(painter, option, widget);
}


float EngineFuelP::value2Angle(const float &p) {
    float pressure = qMax(qMin(p, this->pressureValueMax), this->pressureValueMin);
    // 0 = -60° , pressureValueMax/2 = 0° , pressureValueMax = 60 °
    // range = 120 °
    float r = (float) 120/(this->pressureValueMax);
    float convert = pressure * r -60;
    return convert;
}


void EngineFuelP::itemSizeChanged(float w, float h) {
    this->drawBottomPixmap();
}


void EngineFuelP::pressureChanged(QString name, QStringList values) {
    int val = 0;
    if (! values.isEmpty()) {
        val = values.first().toFloat();
        if (val != this->pressureValue) {
            this->pressureValue=val;
            this->update();
        }
    }

}
