#include "engine_battery.h"

#include <math.h>
#include "../util/console.h"
#include "extplaneclient.h"
#include "../widgets/numberinputlineedit.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(EngineBattery,"indicator/engine/battery")


EngineBattery::EngineBattery(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
   _batteryNumber(0),
   amperageValue(0),
   valueMin(-30),
   valueMax(40),
   scaleFactor(1),
   _client(this, typeName(), conn),
   bottomImage(":/images/DR400_Battery_Amp.png"),
   bottomPixmap(0)
   {
        //init
        //subscibe to dataref
        _client.subscribeDataRef("sim/cockpit2/electrical/battery_amps", 0.1);
        connect(&_client, SIGNAL(refChanged(QString,QStringList)), this, SLOT(amperageChanged(QString,QStringList)));

        //set size
        if (! bottomImage.isNull()) {
        //dimension the item with the default background image
        this->setSize(bottomImage.width(),bottomImage.height());
        }
    }



EngineBattery::~EngineBattery() {
    //nothing to do
}

void EngineBattery::storeSettings(QSettings &settings){
    PanelItem::storeSettings(settings);
    settings.setValue("minvalue", QString::number(valueMin));
    settings.setValue("maxvalue", QString::number(valueMax));
    settings.setValue("batteryNumber", QString::number(_batteryNumber));
}

void EngineBattery::loadSettings(QSettings &settings){
    PanelItem::loadSettings(settings);
    setMinValue(settings.value("minvalue",-25).toInt());
    setMaxValue(settings.value("maxvalue",+25).toInt());
    setBatteryNumber(settings.value("batteryNumber",0).toInt());
}

void EngineBattery::createSettings(QGridLayout *layout){
    QLabel *label1 = new QLabel("battery number [0-9] ", layout->parentWidget());
    layout->addWidget(label1);
    NumberInputLineEdit *batteryNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    batteryNumberEdit->setText(QString::number(_batteryNumber));
    layout->addWidget(batteryNumberEdit);
    connect(batteryNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setBatteryNumber(float)));

    QLabel *label2 = new QLabel("max negative value on discharge", layout->parentWidget());
    layout->addWidget(label2);
    NumberInputLineEdit *minEdit = new NumberInputLineEdit(layout->parentWidget());
    minEdit->setText(QString::number(valueMin));
    layout->addWidget(minEdit);
    connect(minEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMinValue(float)));

    QLabel *label3 = new QLabel("max value on charge", layout->parentWidget());
    layout->addWidget(label3);
    NumberInputLineEdit *maxValueEdit = new NumberInputLineEdit(layout->parentWidget());
    maxValueEdit->setText(QString::number(valueMax));
    layout->addWidget(maxValueEdit);
    connect(maxValueEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMaxValue(float)));

}


void EngineBattery::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    painter->save();
    //paint the background
    painter->drawPixmap(QPoint(0,0), *bottomPixmap);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    //paint the needle
    int dimension;
    dimension=qMin(this->width(), this -> height());

    painter->save();
    QPen pen;
    QBrush brush;

    pen.setColor(QColor(0xc0,0x45,0x28)); //orange color
    pen.setWidth(5+dimension/153); //pen width increase when dimension increase

    painter->setClipping(true);
    painter->setClipRect(0,0,dimension,dimension*89/153);

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->translate(dimension/2,dimension*3/4);
    painter->rotate(30);
    painter->drawLine(0,-1*dimension*1/2,0,0);
    painter->rotate(-30);
    painter->drawLine(0,-1*dimension*1/2,0,0);
    painter->rotate(-30);
    painter->drawLine(0,-1*dimension*1/2,0,0);

//    if ( ! needleImage.isNull()) {

//        int ww, hh;
//        ww = needleImage.width() * this->scaleFactor;
//        hh = needleImage.height() * this->scaleFactor;
//        //center of the needle image is center of width background image
//        //  and  12/19 of height
//        painter->translate((dimension-ww)/2, (dimension*12/19-hh/2));

//        int x,y,a;
//        a=this->value2Angle(this->quantityValue); // rotation degree

//        //make some math to rotate the picture needle by his center
//        float alpha = a*3.14159/180;
//        x = (cos(alpha)*ww + sin(alpha)*hh)/2;
//        y = (-sin(alpha)*ww + cos(alpha)*hh )/2;

//        x = x - ww/2;
//        y = y - hh/2;

//       painter->save();
//            painter->rotate(a);
//            painter->translate(x,y);
//            painter->scale(this->scaleFactor,this->scaleFactor);
//            painter->drawPixmap(0,0,needleImage);
//       painter->restore();


//    }


    painter->restore();

    PanelItem::paint(painter, option, widget);

}

void EngineBattery::itemSizeChanged(float w, float h){
    this->drawBottomPixmap();
}


void EngineBattery::amperageChanged(QString name, QStringList values){
    qDebug() << " tank value  list changed, name =  " << name << " values " <<  values;
    if (values.size() > _batteryNumber) {
        amperageValue = values[_batteryNumber].toFloat();
        this->update();
    }
}

void EngineBattery::setBatteryNumber(float val) {
    if (_batteryNumber != (int)val) {
         //setValue
        _batteryNumber = (int)val;
        //refresh subscription in order to call quantityChanged(xx)
        _client.unsubscribeDataRef("sim/cockpit2/electrical/battery_amps");
        _client.subscribeDataRef("sim/cockpit2/electrical/battery_amps", 0.1);
    }
}

void EngineBattery::setMinValue(float mv){
    if ( mv != valueMin && mv < valueMax ){
        valueMax = (int)mv;
        this->update();
    }
}

void EngineBattery::setMaxValue(float mv){
    if (mv > 0 and mv != valueMax && mv > valueMin){
        valueMax = (int)mv;
        this->update();
    }
}



/*
 * draw background function to the ressource default image,
 * set this pixmap scale to feet the item size
 * and add short description
 */
void EngineBattery::drawBottomPixmap(){
    int w,h; //width; height
    w=qMin(this->width(), this -> height());
    h=w; //this is a square !

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



}

float EngineBattery::value2Angle(const float &p){
    float rval = 0;
    float qt = qMax(qMin(p, this->valueMax), 0.0f);
    //gauge is not linear...
    //for value beetwen half and full :
    if ( valueMax/2 <= qt ) {
        //angle is 35degree between half value and full value
        float angleRange = 35;
        //and begin 10 degree right
        float angleBegin = 10;
        rval = angleBegin + ( 2*qt - valueMax ) / valueMax * angleRange;

    //value is more than 5 kg and less than half load
    }else if ( 5 < qt ) {
        //angle is 53 degree between half value and full value
        float angleRange = 53;
        //and begin 43 degree left
        float angleBegin = -43;
        rval = angleBegin + ( qt - 5) / ((valueMax/2) - 5) * angleRange;

    //value is less than 5 kg
    }else{
        //angle is 24 degree between 5 kg and null
        float angleRange = 24;
        //and begin 67 degree left
        float angleBegin = -67;
        rval = angleBegin + ( qt / 5 ) * angleRange;

    }

    return rval;
}
