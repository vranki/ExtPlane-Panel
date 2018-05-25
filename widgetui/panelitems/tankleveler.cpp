#include "tankleveler.h"

#include <math.h>
#include "../util/console.h"
#include "extplaneclient.h"
#include "../widgets/numberinputlineedit.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(TankLeveler,"indicator/engine/tank")


TankLeveler::TankLeveler(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
   _tankNumber(0),
   tankShortDesignation("C"),
   quantityValue(0),
   valueMax(110),
   scaleFactor(1),
   _client(this, typeName(), conn),
   bottomImage(":/images/DR400_engine_FUEL_Tank.png"),
   bottomPixmap(0),
   needleImage(":/images/DR400_engine_FUEL_Tank_needle.png")
   {


        //init
        //subscibe to dataref
        _client.subscribeDataRef("sim/cockpit2/fuel/fuel_quantity", 1.0);

        conn->registerClient(&_client);
        connect(&_client, SIGNAL(refChanged(QString,QStringList)), this, SLOT(quantityChanged(QString,QStringList)));

        //set size
        if (! bottomImage.isNull()) {
        //dimension the item with the default background image
        this->setSize(bottomImage.width(),bottomImage.height());
        }

    }



TankLeveler::~TankLeveler() {
    delete(bottomPixmap);
}

void TankLeveler::storeSettings(QSettings &settings){
    PanelItem::storeSettings(settings);
    settings.setValue("maxvalue", QString::number(valueMax));
    settings.setValue("tankNumber", QString::number(_tankNumber));
    settings.setValue("tankShortDesignation",tankShortDesignation);
}

void TankLeveler::loadSettings(QSettings &settings){
    PanelItem::loadSettings(settings);
    setMaxValue(settings.value("maxvalue",110).toInt());
    setTankNumber(settings.value("tankNumber",0).toInt());
    setShortDesignation(settings.value("tankShortDesignation").toString());
}

void TankLeveler::createSettings(QGridLayout *layout){
    QLabel *label1 = new QLabel("tank number [0-9] ", layout->parentWidget());
    layout->addWidget(label1);
    NumberInputLineEdit *tankNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    tankNumberEdit->setText(QString::number(_tankNumber));
    layout->addWidget(tankNumberEdit);
    connect(tankNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setTankNumber(float)));

    QLabel *label2 = new QLabel("Short designation to draw", layout->parentWidget());
    layout->addWidget(label2);
    QLineEdit *ShortDesEdit = new QLineEdit(layout->parentWidget());
    ShortDesEdit->setText(tankShortDesignation);
    layout->addWidget(ShortDesEdit);
    connect(ShortDesEdit, SIGNAL(textChanged(QString)), this, SLOT(setShortDesignation(QString)));

    QLabel *label3 = new QLabel("full capacity (kg)", layout->parentWidget());
    layout->addWidget(label3);
    NumberInputLineEdit *maxValueEdit = new NumberInputLineEdit(layout->parentWidget());
    maxValueEdit->setText(QString::number(valueMax));
    layout->addWidget(maxValueEdit);
    connect(maxValueEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMaxValue(float)));

}


void TankLeveler::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    painter->save();
    //paint the background
    painter->drawPixmap(QPoint(0,0), *bottomPixmap);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    //paint the needle
    int dimension;
    dimension=qMin(this->width(), this -> height());
    //paint the needle;
    if ( ! needleImage.isNull()) {

        int ww, hh;
        ww = needleImage.width() * this->scaleFactor;
        hh = needleImage.height() * this->scaleFactor;
        //center of the needle image is center of width background image
        //  and  12/19 of height
        painter->translate((dimension-ww)/2, (dimension*12/19-hh/2));

        int x,y,a;
        a=this->value2Angle(this->quantityValue); // rotation degree

        //make some math to rotate the picture needle by his center
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

void TankLeveler::itemSizeChanged(float w, float h){
    Q_UNUSED(w);
    Q_UNUSED(h);
    this->drawBottomPixmap();
}


void TankLeveler::quantityChanged(QString name, QStringList values){
    Q_UNUSED(name);
    //qDebug() << " tank value  list changed, name =  " << name << " values " <<  values;
    if (values.size() > _tankNumber) {
        quantityValue = values[_tankNumber].toFloat();
        this->update();
    }
}

void TankLeveler::setTankNumber(float val) {
    if (_tankNumber != (int)val) {
         //setValue
        _tankNumber = (int)val;
        //refresh subscription in order to call quantityChanged(xx)
        _client.unsubscribeDataRef("sim/cockpit2/fuel/fuel_quantity");
        _client.subscribeDataRef("sim/cockpit2/fuel/fuel_quantity", 1.0);

    }
}

void TankLeveler::setMaxValue(float mv){
    if (mv > 0 and mv != valueMax){
        valueMax = (int)mv;
        this->update();
    }
}

void TankLeveler::setShortDesignation(QString s) {
    tankShortDesignation = s;    
    this->drawBottomPixmap();
    this->update();
}

/*
 * draw background function to the ressource default image,
 * set this pixmap scale to feet the item size
 * and add short description
 */
void TankLeveler::drawBottomPixmap(){
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


    //add description
    pa.save();
    QPen pen;
    QBrush brush;
    pen.setColor(QColor(0xb4,0xba,0xbd)); //grey color
    pa.setPen(pen);
    pa.setBrush(brush);
    QFont font("Verdana", defaultFont.pointSizeF() * this->scaleFactor);
    font.setItalic(true);
    QFontMetrics fm(font);
    pa.setFont(font);

    pa.translate(w/2,4*h/5);
    pa.drawText(-1*fm.width(tankShortDesignation)/2, 0, tankShortDesignation);

    pa.restore();



}

float TankLeveler::value2Angle(const float &p){
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
