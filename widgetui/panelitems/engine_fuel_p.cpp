#include "engine_fuel_p.h"

#include <math.h>
#include "../util/console.h"
#include "extplaneclient.h"
#include "../util/valueinterpolator.h"
#include "../widgets/numberinputlineedit.h"



REGISTER_WITH_PANEL_ITEM_FACTORY(EngineFuelP,"indicator/engine/pressure")

EngineFuelP::EngineFuelP(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
                 PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
                _engineNumber(0),
                pressureValue(0),
                pressureValueMin(0),
                pressureValueMax(400),
                scaleFactor(1),
                pressureGreenBegin(100),
                pressureGreenEnd(350),
                allowUpdateBottomPixmap(true),
                _client(this, typeName(), conn),
                bottomImage(":/images/DR400_engine_FUELP.png"),
                bottomPixmap(0),
                needleImage(":/images/DR400_engine_generic_needle.png")
                {


    //init
    //subscibe to dataref
    _client.subscribeDataRef("sim/cockpit2/engine/indicators/fuel_pressure_psi", 5.0);

    conn->registerClient(&_client);
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
    settings.setValue("maxvalue", QString::number(pressureValueMax));
    settings.setValue("greenBegin", QString::number(pressureGreenBegin));
    settings.setValue("greenEnd", QString::number(pressureGreenEnd));
    settings.setValue("engineNumber", QString::number(_engineNumber));
}

void EngineFuelP::loadSettings(QSettings &settings) {
    PanelItem::loadSettings(settings);

    allowUpdateBottomPixmap = false;

    setMaxValue(settings.value("maxvalue",400).toInt());
    setGreenBeginValue(settings.value("greenBegin",100).toInt());
    setGreenEndValue(settings.value("greenEnd",350).toInt());
    setEngineNumber(settings.value("engineNumber",0).toInt());

    allowUpdateBottomPixmap = true;
}

void EngineFuelP::createSettings(QGridLayout *layout) {

    QLabel *label1 = new QLabel("Engine number [0-9] ", layout->parentWidget());
    layout->addWidget(label1);
    NumberInputLineEdit *EngineNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    EngineNumberEdit->setText(QString::number(_engineNumber));
    layout->addWidget(EngineNumberEdit);
    connect(EngineNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setEngineNumber(float)));

    QLabel *maxPressure = new QLabel("Maximum psi range", layout->parentWidget());
    layout->addWidget(maxPressure);
    NumberInputLineEdit *maxValueEdit = new NumberInputLineEdit(layout->parentWidget());
    maxValueEdit->setText(QString::number(pressureValueMax));
    layout->addWidget(maxValueEdit);
    connect(maxValueEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMaxValue(float)));

    QLabel *greenBeginPressure = new QLabel("Green value begins", layout->parentWidget());
    layout->addWidget(greenBeginPressure);
    NumberInputLineEdit *greenBeginPressureEdit = new NumberInputLineEdit(layout->parentWidget());
    greenBeginPressureEdit->setText(QString::number(pressureGreenBegin));
    layout->addWidget(greenBeginPressureEdit);
    connect(greenBeginPressureEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setGreenBeginValue(float)));

    QLabel *greenEndPressure = new QLabel("Green value ends", layout->parentWidget());
    layout->addWidget(greenEndPressure);
    NumberInputLineEdit *greenEndPressureEdit = new NumberInputLineEdit(layout->parentWidget());
    greenEndPressureEdit->setText(QString::number(pressureGreenEnd));
    layout->addWidget(greenEndPressureEdit);
    connect(greenEndPressureEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setGreenEndValue(float)));

}

void EngineFuelP::setMaxValue(float mv) {
    if (mv != pressureValueMax && mv > 0) {
        pressureValueMax = mv;
        this->drawBottomPixmap();
    }
}

void EngineFuelP::setGreenBeginValue(float mv) {
    if (mv != pressureGreenBegin && mv > 0) {
        //pressureGreenBegin value must be between pressureValueMin and pressureGreenEnd
        pressureGreenBegin = qMax(qMin(mv, this->pressureGreenEnd), this->pressureValueMin);
        this->drawBottomPixmap();
    }
}

void EngineFuelP::setGreenEndValue(float mv) {
    if (mv != pressureGreenEnd && mv > 0) {
        //pressureGreenEnd value must be between pressureGreenBegin and pressureValueMax
        pressureGreenEnd = qMax(qMin(mv, this->pressureValueMax), this->pressureGreenBegin);
        this->drawBottomPixmap();
    }
}

/*
 * draw background function to the ressource default image,
 * add arcs and set this pixmap scale to feet the item size
 */
void EngineFuelP::drawBottomPixmap() {

    if (allowUpdateBottomPixmap) {
        int w,h; //width; height
        float arcGauge = 0.45; //distance ration between edge and arc
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
        //arc amplitude is 120 degrees

        pen.setWidth(1+w/250); //pen width increase when w increase
        pen.setColor(QColor(0xb4,0xba,0xbd)); //grey color
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
        pen.setColor(QColor(0xb4,0xba,0xbd)); //grey color
        pa.setPen(pen);
        pa.setBrush(brush);
        QFont font("Verdana", this->defaultFont.pointSizeF() /1.3f * this->scaleFactor);
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

        double startAngle = (90 - this->value2Angle(this->pressureGreenBegin))*16.0;
        double arcAngle = ( this->value2Angle(this->pressureGreenBegin) - this->value2Angle(this->pressureGreenEnd) )*16.0;
        int x,y,ww,hh;
        ww=w*arcGauge;
        hh=h*arcGauge;
        x = (w - ww)/2;
        y = (h - hh)/2;
        pa.drawArc(x,y,ww,hh, startAngle, arcAngle );

    }
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
    Q_UNUSED(w);
    Q_UNUSED(h);
    this->drawBottomPixmap();
}

void EngineFuelP::setEngineNumber(float val) {
    if (_engineNumber != (int)val) {
        _engineNumber = (int)val;
        //refresh subscription in order to call pressureChanged(xx)
        _client.unsubscribeDataRef("sim/cockpit2/engine/indicators/fuel_pressure_psi");
        _client.subscribeDataRef("sim/cockpit2/engine/indicators/fuel_pressure_psi", 5.0);

    }
}

void EngineFuelP::pressureChanged(QString name, QStringList values) {
    Q_UNUSED(name);
    int val = 0;
    if (values.size() > _engineNumber) {
        val = values[_engineNumber].toFloat();
        if (val != this->pressureValue) {
            this->pressureValue=val;
            this->update();
        }
    }
}

