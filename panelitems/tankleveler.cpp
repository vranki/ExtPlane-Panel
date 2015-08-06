#include "tankleveler.h"

#include <math.h>
#include "../util/console.h"
#include "extplaneclient.h"
#include "../widgets/numberinputlineedit.h"

REGISTER_WITH_PANEL_ITEM_FACTORY(TankLeveler,"indicator/engine/tank")


TankLeveler::TankLeveler(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
   _tankNumber(1),
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
        _client.subscribeDataRef("sim/cockpit2/fuel/fuel_quantity[0]", 1.0);
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

    QLabel *label3 = new QLabel("full Quantity", layout->parentWidget());
    layout->addWidget(label3);
    NumberInputLineEdit *maxValueEdit = new NumberInputLineEdit(layout->parentWidget());
    maxValueEdit->setText(QString::number(valueMax));
    layout->addWidget(maxValueEdit);
    connect(maxValueEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMaxValue(float)));

}


void TankLeveler::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    PanelItem::paint(painter, option, widget);
}

void TankLeveler::itemSizeChanged(float w, float h){
    this->drawBottomPixmap();
}


void TankLeveler::quantityChanged(QString name, QStringList values){}

void TankLeveler::setTankNumber(float val) {
    _tankNumber = (int)val;
    //TODO:need to unregister and register again...
}

void TankLeveler::setMaxValue(float mv){
    valueMax = (int)mv;
}

void TankLeveler::setShortDesignation(QString s) {
    tankShortDesignation = s;
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

    QPen pen;
    QBrush brush;

    //add description
    pa.save();
    pen.setColor(QColor(250,250,250)); //grey color
    pa.setPen(pen);
    pa.setBrush(brush);
    QFont font("Verdana", 5* this->scaleFactor);
    QFontMetrics fm(font);
    pa.setFont(font);

    pa.translate(w/2,h-h/6);

    pa.drawText(w/2, h-h/5, w/4, h/6, Qt::AlignCenter || Qt::AlignJustify  , tankShortDesignation, 0);

    pa.restore();



}

float TankLeveler::value2Angle(const float &p){}
