#include "genericlittlegauge.h"

#include <math.h>
#include "../util/console.h"
#include "extplaneclient.h"
#include "../valueinterpolator.h"
#include "../widgets/numberinputlineedit.h"
#include <QCheckBox>


REGISTER_WITH_PANEL_ITEM_FACTORY(GenericLittleGauge,"indicator/engine/generic_little_gauge")

GenericLittleGauge::GenericLittleGauge(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
    _indexNumber(0),
    value(0),
    valueMin(0),
    valueMax(100),
    scaleFactor(1),
    greenBeginValue(50),
    greenEndValue(70),
    greenThickness(2),
    orangeBeginValue(70),
    orangeEndValue(75),
    orangeThickness(3),
    redBeginValue(75),
    redEndValue(100),
    redThickness(4),
    upDesignation("what"),
    downDesignation("Generic"),
    isTableValues(true),
    dataRefsString("sim/cockpit2/engine/indicators/oil_temperature_deg_C"),
    accuracy(1),
    nbBigGraduations(3),
    nbThinGraduations(6),
    isLogDisplayStyle(false),
    updateBottomPixmap(true),
    arcDegreeAmplitude(120),
    arcRatioPosition(0.65),
    _client(this, typeName(), conn),
    bottomImage(":/images/DR400_engine_generic.png"),
    bottomPixmap(0),
    needleImage(":/images/DR400_engine_generic_needle.png")
{

    //init
    stringDisplayValues[0]="0";
    stringDisplayValues[1]="50";
    stringDisplayValues[2]="100";
    stringDisplayValues[3]="";
    stringDisplayValues[4]="";

    //subscibe to dataref
    _client.subscribeDataRef(dataRefsString, accuracy);
    connect(&_client, SIGNAL(refChanged(QString,QStringList)), this, SLOT(valueChanged(QString,QStringList)));


    //set size
    if (! bottomImage.isNull()) {
        //dimension the item with the default background image
        this->setSize(bottomImage.width(),bottomImage.height());
    }

}

GenericLittleGauge::~GenericLittleGauge(){
    delete(bottomPixmap);
}

void GenericLittleGauge::storeSettings(QSettings &settings){
    PanelItem::storeSettings(settings);

    settings.setValue("indexNumber", QString::number(_indexNumber));
    settings.setValue("valueMin", QString::number(valueMin));
    settings.setValue("valueMax", QString::number(valueMax));
    settings.setValue("greenBeginValue", QString::number(greenBeginValue));
    settings.setValue("greenEndValue", QString::number(greenEndValue));
    settings.setValue("greenThickness", QString::number(greenThickness));
    settings.setValue("orangeBeginValue", QString::number(orangeBeginValue));
    settings.setValue("orangeEndValue", QString::number(orangeEndValue));
    settings.setValue("orangeThickness", QString::number(orangeThickness));
    settings.setValue("redBeginValue", QString::number(redBeginValue));
    settings.setValue("redEndValue", QString::number(redEndValue));
    settings.setValue("redThickness", QString::number(redThickness));
    settings.setValue("upDesignation",upDesignation);
    settings.setValue("downDesignation",downDesignation);
    settings.setValue("isTableValues",QString::number(isTableValues));
    settings.setValue("dataRefsString", dataRefsString);
    settings.setValue("accuracy", QString::number(accuracy));
    settings.setValue("nbBigGraduations", QString::number(nbBigGraduations));
    settings.setValue("nbThinGraducations", QString::number(nbThinGraduations));
    settings.setValue("stringDisplayValues[0]",stringDisplayValues[0]);
    settings.setValue("stringDisplayValues[1]",stringDisplayValues[1]);
    settings.setValue("stringDisplayValues[2]",stringDisplayValues[2]);
    settings.setValue("stringDisplayValues[3]",stringDisplayValues[3]);
    settings.setValue("stringDisplayValues[4]",stringDisplayValues[4]);
    settings.setValue("isLogDisplayStyle", QString::number(isLogDisplayStyle));

}

void GenericLittleGauge::loadSettings(QSettings &settings){

    this->updateBottomPixmap = false;

    PanelItem::loadSettings(settings);

    setIndexNumber(settings.value("indexNumber",0).toFloat());
    setMinValue(settings.value("valueMin", 0).toFloat());
    setMaxValue(settings.value("valueMax", 100).toFloat());
    setGreenBeginValue(settings.value("greenBeginValue", 0).toFloat());
    setGreenEndValue(settings.value("greenEndValue", 0).toFloat());
    setOrangeBeginValue(settings.value("orangeBeginValue", 0).toFloat());
    setOrangeEndValue(settings.value("orangeEndValue", 0).toFloat());
    setRedBeginValue(settings.value("redBeginValue", 0).toFloat());
    setRedEndValue(settings.value("redEndValue", 0).toFloat());
    setUpDesignation(settings.value("upDesignation","upDesignation""").toString());
    setDownDesignation(settings.value("downDesignation","downDesignation""").toString());
    setIsTableValues(settings.value("isTableValues", 0).toBool());
    setDataRefString(settings.value("dataRefsString", "").toString());
    setAccuracy(settings.value("accuracy", 1).toFloat());
    setNbOfBigGraduations(settings.value("nbBigGraduations", 2).toInt());
    setNbOfThinGraduations(settings.value("nbThinGraducations", 2).toInt());
    setStringDisplayValue(settings.value("stringDisplayValues[0]","").toString(),0);
    setStringDisplayValue(settings.value("stringDisplayValues[1]","").toString(),1);
    setStringDisplayValue(settings.value("stringDisplayValues[2]","").toString(),2);
    setStringDisplayValue(settings.value("stringDisplayValues[3]","").toString(),3);
    setStringDisplayValue(settings.value("stringDisplayValues[4]","").toString(),4);
    setLogDisplayStyle(settings.value("isLogDisplayStyle", 0).toBool());


    this->updateBottomPixmap = true;
}

void GenericLittleGauge::createSettings(QGridLayout *layout){

    layout->addWidget(new QLabel("Designation line 1/2", layout->parentWidget()));
    QLineEdit *des1Edit = new QLineEdit(layout->parentWidget());
    des1Edit->setText(upDesignation);
    layout->addWidget(des1Edit);
    connect(des1Edit, SIGNAL(textChanged(QString)), this, SLOT(setUpDesignation(QString)));

    layout->addWidget(new QLabel("Designation line 2/2", layout->parentWidget()));
    QLineEdit *des2Edit = new QLineEdit(layout->parentWidget());
    des2Edit->setText(downDesignation);
    layout->addWidget(des2Edit);
    connect(des2Edit, SIGNAL(textChanged(QString)), this, SLOT(setDownDesignation(QString)));

    layout->addWidget(new QLabel("dataRef String", layout->parentWidget()));
    QLineEdit *dataRefEdit = new QLineEdit(layout->parentWidget());
    dataRefEdit->setText(dataRefsString);
    layout->addWidget(dataRefEdit);
    connect(dataRefEdit, SIGNAL(textChanged(QString)), this, SLOT(setDataRefString(QString)));

    layout->addWidget(new QLabel("accuracy", layout->parentWidget()));
    NumberInputLineEdit *accuracyEdit = new NumberInputLineEdit(layout->parentWidget());
    accuracyEdit->setText(QString::number(accuracy));
    layout->addWidget(accuracyEdit);
    connect(accuracyEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setAccuracy(float)));

    layout->addWidget(new QLabel("is multiple values", layout->parentWidget()));
    QCheckBox *qcb1 = new QCheckBox(layout->parentWidget());
    qcb1->setChecked(isTableValues);
    layout->addWidget(qcb1);
    connect(qcb1, SIGNAL(clicked(bool)), this, SLOT(setIsTableValues(bool)));

    layout->addWidget(new QLabel("show value [0-9]", layout->parentWidget()));
    NumberInputLineEdit *indexNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    indexNumberEdit->setText(QString::number(_indexNumber));
    layout->addWidget(indexNumberEdit);
    connect(indexNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setIndexNumber(float)));

    layout->addWidget(new QLabel("display Logarithmic scale", layout->parentWidget()));
    QCheckBox *qcb2 = new QCheckBox(layout->parentWidget());
    qcb2->setChecked(isLogDisplayStyle);
    layout->addWidget(qcb2);
    connect(qcb2, SIGNAL(clicked(bool)), this, SLOT(setLogDisplayStyle(bool)));

    layout->addWidget(new QLabel("value Min", layout->parentWidget()));
    NumberInputLineEdit *vminEdit = new NumberInputLineEdit(layout->parentWidget());
    vminEdit->setText(QString::number(valueMin));
    layout->addWidget(vminEdit);
    connect(vminEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMinValue(float)));

    layout->addWidget(new QLabel("value Max", layout->parentWidget()));
    NumberInputLineEdit *vmaxEdit = new NumberInputLineEdit(layout->parentWidget());
    vmaxEdit->setText(QString::number(valueMax));
    layout->addWidget(vmaxEdit);
    connect(vmaxEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMaxValue(float)));

    layout->addWidget(new QLabel("green arc value begins at", layout->parentWidget()));
    NumberInputLineEdit *greenMinNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    greenMinNumberEdit->setText(QString::number(greenBeginValue));
    layout->addWidget(greenMinNumberEdit);
    connect(greenMinNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setGreenBeginValue(float)));

    layout->addWidget(new QLabel("green arc value ends at", layout->parentWidget()));
    NumberInputLineEdit *greenMaxNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    greenMaxNumberEdit->setText(QString::number(greenEndValue));
    layout->addWidget(greenMaxNumberEdit);
    connect(greenMaxNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setGreenEndValue(float)));

    layout->addWidget(new QLabel("green arc thickness", layout->parentWidget()));
    NumberInputLineEdit *greenThickNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    greenThickNumberEdit->setText(QString::number(greenThickness));
    layout->addWidget(greenThickNumberEdit);
    connect(greenThickNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setGreenThickness(float)));

    layout->addWidget(new QLabel("orange arc value begins at", layout->parentWidget()));
    NumberInputLineEdit *orangeMinNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    orangeMinNumberEdit->setText(QString::number(orangeBeginValue));
    layout->addWidget(orangeMinNumberEdit);
    connect(orangeMinNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setOrangeBeginValue(float)));

    layout->addWidget(new QLabel("orange arc value ends at", layout->parentWidget()));
    NumberInputLineEdit *orangeMaxNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    orangeMaxNumberEdit->setText(QString::number(orangeEndValue));
    layout->addWidget(orangeMaxNumberEdit);
    connect(orangeMaxNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setOrangeEndValue(float)));

    layout->addWidget(new QLabel("orange arc thickness", layout->parentWidget()));
    NumberInputLineEdit *orangeThickNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    orangeThickNumberEdit->setText(QString::number(orangeThickness));
    layout->addWidget(orangeThickNumberEdit);
    connect(orangeThickNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setOrangeThickness(float)));

    layout->addWidget(new QLabel("red arc value begins at", layout->parentWidget()));
    NumberInputLineEdit *redMinNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    redMinNumberEdit->setText(QString::number(redBeginValue));
    layout->addWidget(redMinNumberEdit);
    connect(redMinNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setRedBeginValue(float)));

    layout->addWidget(new QLabel("red arc value ends at", layout->parentWidget()));
    NumberInputLineEdit *redMaxNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    redMaxNumberEdit->setText(QString::number(redEndValue));
    layout->addWidget(redMaxNumberEdit);
    connect(redMaxNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setRedEndValue(float)));

    layout->addWidget(new QLabel("red arc thickness", layout->parentWidget()));
    NumberInputLineEdit *redThickNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    redThickNumberEdit->setText(QString::number(redThickness));
    layout->addWidget(redThickNumberEdit);
    connect(redThickNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setRedThickness(float)));

    layout->addWidget(new QLabel("number of big graduations", layout->parentWidget()));
    NumberInputLineEdit *nbBigEdit = new NumberInputLineEdit(layout->parentWidget());
    nbBigEdit->setText(QString::number(nbBigGraduations));
    layout->addWidget(nbBigEdit);
    connect(nbBigEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setNbOfBigGraduations(float)));

    layout->addWidget(new QLabel("number of thingraduations", layout->parentWidget()));
    NumberInputLineEdit *nbThinEdit = new NumberInputLineEdit(layout->parentWidget());
    nbThinEdit->setText(QString::number(nbThinGraduations));
    layout->addWidget(nbThinEdit);
    connect(nbThinEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setNbOfThinGraduations(float)));

    layout->addWidget(new QLabel("String value 1 to display", layout->parentWidget()));
    QLineEdit *stringValue1DisplayEdit = new QLineEdit(layout->parentWidget());
    stringValue1DisplayEdit->setText(stringDisplayValues[0]);
    layout->addWidget(stringValue1DisplayEdit);
    connect(stringValue1DisplayEdit, SIGNAL(textChanged(QString)), this, SLOT(setStringDisplayValue(QString,uint=0)));


}

void GenericLittleGauge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->save();
    //paint the background
    painter->drawPixmap(QPoint(0,0), *bottomPixmap);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);


    painter->restore();
    PanelItem::paint(painter, option, widget);
}

void GenericLittleGauge::drawBottomPixmap(){

    if (updateBottomPixmap) {

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
        pen.setColor(QColor(0xb4,0xba,0xbd)); //grey color
        pa.setPen(pen);
        pa.setBrush(brush);
        QFont font("Verdana", defaultFont.pointSizeF() * this->scaleFactor);
        font.setItalic(true);
        QFontMetrics fm(font);
        pa.setFont(font);

        pa.translate(w/2,2*h/5);
        pa.drawText(-1*fm.width(upDesignation)/2, 0, upDesignation);
        pa.translate(0,2*h/6);
        pa.drawText(-1*fm.width(downDesignation)/2, 0, downDesignation);

        pa.restore();

        //general Arcs values
        int startAngle16, spanAngle16;
        QRect *qr = new QRect();
        //add green arc
        this->values2valuesArc(greenBeginValue, greenEndValue, startAngle16, spanAngle16, qr);
        pen.setColor(QColor("green"));
        pen.setWidthF( greenThickness * scaleFactor);
        pa.setPen(pen);
        pa.drawArc(*qr,startAngle16,spanAngle16);

        //add orange Arc
        this->values2valuesArc(orangeBeginValue, orangeEndValue, startAngle16, spanAngle16, qr);
        pen.setColor(QColor("yellow"));
        pen.setWidthF( orangeThickness * scaleFactor);
        pa.setPen(pen);
        pa.drawArc(*qr,startAngle16,spanAngle16);

        //add red Arc
        this->values2valuesArc(redBeginValue, redEndValue, startAngle16, spanAngle16, qr);
        pen.setColor(QColor("red"));
        pen.setWidthF( redThickness * scaleFactor);
        pa.setPen(pen);
        pa.drawArc(*qr,startAngle16,spanAngle16);

        //end arcs drawing, destroy rectangle
        delete(qr);


        //draw thin graduations
        pa.save();

        pen.setWidth(1+w/250); //pen width increase when w increase
        pen.setColor(QColor(0xb4,0xba,0xbd)); //grey color
        pa.setPen(pen);
        pa.translate(w/2,h/2);
        pa.rotate(-1 * (180 + arcDegreeAmplitude) / 2 );
        for (int i=0; i<=  nbThinGraduations; i++) {
            pa.drawLine( (w -  (5 * this->scaleFactor)) * arcRatioPosition/2, 0, w * arcRatioPosition/2, 0);
            pa.rotate(value2Angle( (valueMax-valueMin)/nbThinGraduations ));
        }
        pa.restore();


        //draw big graduations
        pa.save();

        pen.setWidth(1+w/250); //pen width increase when w increase
        pen.setColor(QColor(0xb4,0xba,0xbd)); //grey color
        pa.setPen(pen);
        pa.translate(w/2,h/2);
        pa.rotate(-1 * (180 + arcDegreeAmplitude) / 2 );
        for (int i=0; i<=  nbBigGraduations; i++) {
            pa.drawLine( (w -  (10 * this->scaleFactor)) * arcRatioPosition/2, 0, ( w + ( 10 * this->scaleFactor)) * arcRatioPosition/2, 0);
            pa.rotate(value2Angle( (valueMax-valueMin)/nbBigGraduations ));
        }
        pa.restore();




    } // end if updateBottomPixmap
}


float GenericLittleGauge::value2Angle(const float &p) {

    float rangeAngle = arcDegreeAmplitude; // 120 degree between min and max value
    float angle;
    if (isLogDisplayStyle) {
        angle = log10( ( p / (valueMax - valueMin))*9 + 1 ) * rangeAngle;
    }else{
        angle =  ( p / (valueMax - valueMin)) * rangeAngle ;
    }
    return angle;
}

void GenericLittleGauge::values2valuesArc(const float &begin, const float &end, int &returnStartAngle, int &returnSpanAngle, QRect *qrect) {

    returnStartAngle=0;
    returnSpanAngle=0;

    float beginAbsoluteAngle = (180 - arcDegreeAmplitude) / 2 + arcDegreeAmplitude ;

    //set the returned rectangle
    if (qrect == 0) {
        qrect = new QRect();
    }
    int w=qMin(this->width(), this -> height());
    //translate rect for his center is the center of the panelint w,h; //width; height
    qrect->setTopLeft(QPoint( w * (1 - arcRatioPosition) / 2, w * (1 - arcRatioPosition) / 2 ));
    //set width and height of the rectangel (must be set after left corner)
    qrect->setWidth(w*arcRatioPosition);
    qrect->setHeight(w*arcRatioPosition);

    //
    returnStartAngle = this->value2Angle(begin);
    returnSpanAngle = this->value2Angle(end) - returnStartAngle ;

    //translate angle to abslute angle coordonnate
    returnStartAngle = beginAbsoluteAngle - returnStartAngle;
    //span is clockwithg, so convert to negative value;
    returnSpanAngle *= -1;

    // 1/16 degree as says qt documentation
    returnStartAngle *= 16;
    returnSpanAngle *= 16;


}

void GenericLittleGauge::itemSizeChanged(float w, float h){
    this->drawBottomPixmap();
}


void GenericLittleGauge::valueChanged(QString name, QStringList values){
    if (isTableValues && QString::compare(name,dataRefsString)==0) {
        if ( (unsigned int)values.size() > _indexNumber) {
            value = values[_indexNumber].toFloat();
            this->update();
        }
    }
}

void GenericLittleGauge::valueChanged(QString name, double valueD){
    if (! isTableValues && QString::compare(name,dataRefsString)==0) {
        value = (float)valueD;
        this->update();
    }
}


void GenericLittleGauge::setIndexNumber(float val){
    if (_indexNumber != (unsigned int)val) {
        _indexNumber = (unsigned int)val;
        //refresh subscription in order to call valueChanged(xx)
        _client.unsubscribeDataRef(dataRefsString);
        _client.subscribeDataRef(dataRefsString, accuracy);

    }
}


void GenericLittleGauge::setMaxValue(float mv){
    if (mv != valueMax && mv > valueMin) {
        valueMax = mv;
        this->drawBottomPixmap();
    }

}

void GenericLittleGauge::setMinValue(float mv){
    if (mv != valueMin && mv < valueMax) {
        valueMin = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setGreenBeginValue(float mv){
    if (mv != greenBeginValue && mv <= greenEndValue ) {
        greenBeginValue = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setGreenEndValue(float mv){
    if (mv != greenEndValue && mv >= greenBeginValue) {
        greenEndValue = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setGreenThickness(float mv){
    if (mv != greenThickness ) {
        greenThickness = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setOrangeBeginValue(float mv){
    if (mv != orangeBeginValue && mv <= orangeEndValue) {
        orangeBeginValue = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setOrangeEndValue(float mv){
    if (mv != orangeEndValue && mv >= orangeBeginValue) {
        orangeEndValue = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setOrangeThickness(float mv){
    if (mv != orangeThickness ) {
        orangeThickness = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setRedBeginValue(float mv){
    if (mv != redBeginValue && mv <= redEndValue) {
        redBeginValue = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setRedEndValue(float mv){
    if (mv != redEndValue && mv >= redBeginValue) {
        redEndValue = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setRedThickness(float mv){
    if (mv != redThickness ) {
        redThickness = mv;
        this->drawBottomPixmap();
    }
}
void GenericLittleGauge::setUpDesignation(QString s){
    upDesignation = s;
    this->drawBottomPixmap();
}

void GenericLittleGauge::setDownDesignation(QString s){
    downDesignation =s;
    this->drawBottomPixmap();
}


void GenericLittleGauge::setIsTableValues(bool yes){
    isTableValues = yes;
}

void GenericLittleGauge::setDataRefString(QString s){
    dataRefsString = s;
}

void GenericLittleGauge::setAccuracy(float f){
    accuracy = f;
}

void GenericLittleGauge::setNbOfBigGraduations(float n){
    if (n != nbBigGraduations) {
        nbBigGraduations = (int)n;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setNbOfThinGraduations(float n){
    if (n != nbThinGraduations) {
        nbThinGraduations = (int)n;
    }
}

void GenericLittleGauge::setStringDisplayValue(QString value, unsigned int index){
    if ( index < (sizeof(stringDisplayValues)/sizeof(*stringDisplayValues)) ) {
        stringDisplayValues[index] = value;
    }

}

void GenericLittleGauge::setLogDisplayStyle(bool log){
    isLogDisplayStyle=log;
}

