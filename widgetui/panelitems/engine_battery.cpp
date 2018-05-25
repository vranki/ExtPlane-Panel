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
   valueMin(-5),
   valueMax(1),
   scaleFactor(1),
   _client(this, typeName(), conn),
   bottomImage(":/images/DR400_Battery_Amp.png"),
   bottomPixmap(0)
   {
        //init
        //subscibe to dataref
        _client.subscribeDataRef("sim/cockpit2/electrical/battery_amps", 0.1);

        conn->registerClient(&_client);
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
    setMinValue(settings.value("minvalue",-5).toFloat());
    setMaxValue(settings.value("maxvalue",+2).toFloat());
    setBatteryNumber(settings.value("batteryNumber",0).toInt());
}

void EngineBattery::createSettings(QGridLayout *layout){
    QLabel *label1 = new QLabel("battery number [0-9] ", layout->parentWidget());
    layout->addWidget(label1);
    NumberInputLineEdit *batteryNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    batteryNumberEdit->setText(QString::number(_batteryNumber));
    layout->addWidget(batteryNumberEdit);
    connect(batteryNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setBatteryNumber(float)));

    QLabel *label2 = new QLabel("max negative Amperage on discharge", layout->parentWidget());
    layout->addWidget(label2);
    NumberInputLineEdit *minEdit = new NumberInputLineEdit(layout->parentWidget());
    minEdit->setText(QString::number(valueMin));
    layout->addWidget(minEdit);
    connect(minEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMinValue(float)));

    QLabel *label3 = new QLabel("max Amperage on charge", layout->parentWidget());
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

    painter->setClipping(true); //draw only in this area, the bottom of the needle is hidden
    painter->setClipRect(0,0,dimension,dimension*89/153);

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->translate(dimension/2,dimension*3/4);
    painter->rotate(value2Angle(amperageValue));
    painter->drawLine(0,-1*dimension*1/2,0,0);

    painter->restore();

    PanelItem::paint(painter, option, widget);

}

void EngineBattery::itemSizeChanged(float w, float h){
    Q_UNUSED(w);
    Q_UNUSED(h);
    this->drawBottomPixmap();
}


void EngineBattery::amperageChanged(QString name, QStringList values){
    Q_UNUSED(name);
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
        valueMin = (float)mv;
        this->update();
    }
}

void EngineBattery::setMaxValue(float mv){
    if (mv > 0 and mv != valueMax && mv > valueMin){
        valueMax = (float)mv;
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
    float qt = qMax(qMin(p, this->valueMax), this->valueMin);
    //from zero to maxValue, the needle is in Ok zone
    if ( 0 < qt ) {
        //angle is 48degree and gebin at -18degree
        float angleRange = 48;
        //and begin 10 degree right
        float angleBegin = -18;
        rval = angleBegin +  qt / valueMax * angleRange;

    //value is less than zero
    }else {
        //angle is -12 degree and begin at -18
        float angleRange = -12;
        //and begin 43 degree left
        float angleBegin = -18;
        rval = angleBegin + qt / (valueMin) * angleRange;
    }
    return rval;
}
