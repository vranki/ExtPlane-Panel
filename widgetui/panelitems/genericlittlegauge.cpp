#include "genericlittlegauge.h"

#include <math.h>
#include "extplaneclient.h"
#include "../util/console.h"
#include "../util/valueinterpolator.h"
#include "../widgets/numberinputlineedit.h"
#include "../util/interpolation.h"
#include <QCheckBox>


REGISTER_WITH_PANEL_ITEM_FACTORY(GenericLittleGauge,"indicator/engine/generic_little_gauge")

GenericLittleGauge::GenericLittleGauge(ExtPlanePanel *panel, ExtPlaneConnection *conn) :
    PanelItem(panel, PanelItemTypeGauge, PanelItemShapeCircular),
    _indexNumber(0),
    value(0),
    valueMin(50),
    valueMax(150),
    scaleFactor(1),
    greenBeginValue(60),
    greenEndValue(120),
    greenThickness(3),
    orangeBeginValue(120),
    orangeEndValue(130),
    orangeThickness(3),
    redBeginValue(130),
    redEndValue(150),
    redThickness(4),
    upDesignation("Temp°"),
    downDesignation("OIL"),
    isTableValues(true),
    dataRefsString(""),
    accuracy(1),
    nbBigGraduations(3),
    nbThinGraduations(7),
    isDisplayValues(true),
    isLogDisplayStyle(false),
    allowUpdateBottomPixmap(true),
    arcDegreeAmplitude(120),
    arcRatioPosition(0.5),
    _client(this, typeName(), conn),
    bottomImage(":/images/DR400_engine_generic.png"),
    bottomPixmap(0),
    needleImage(":/images/DR400_engine_generic_needle.png")
{

    //init

    //subscibe to dataref
    setDataRefString("sim/cockpit2/engine/indicators/oil_temperature_deg_C");

    conn->registerClient(&_client);
    connect(&_client, SIGNAL(refChanged(QString,QStringList)), this, SLOT(valueChanged(QString,QStringList)));
    connect(&_client, SIGNAL(refChanged(QString,double)), this, SLOT(valueChanged(QString,double)));


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
    settings.setValue("isDisplayValues", QString::number(isDisplayValues));
    settings.setValue("isLogDisplayStyle", QString::number(isLogDisplayStyle));
    settings.setValue("arcDegreeAmplitude", QString::number(arcDegreeAmplitude));

}

void GenericLittleGauge::loadSettings(QSettings &settings){

    this->allowUpdateBottomPixmap = false;

    PanelItem::loadSettings(settings);

    setIndexNumber(settings.value("indexNumber",0).toFloat());
    setMinValue(settings.value("valueMin", 50).toFloat());
    setMaxValue(settings.value("valueMax", 150).toFloat());
    setGreenBeginValue(settings.value("greenBeginValue", 60).toFloat());
    setGreenEndValue(settings.value("greenEndValue", 120).toFloat());
    setGreenThickness(settings.value("greenThickness", 3).toFloat());
    setOrangeBeginValue(settings.value("orangeBeginValue", 120).toFloat());
    setOrangeEndValue(settings.value("orangeEndValue", 130).toFloat());
    setOrangeThickness(settings.value("orangeThickness", 3).toFloat());
    setRedBeginValue(settings.value("redBeginValue", 130).toFloat());
    setRedEndValue(settings.value("redEndValue", 150).toFloat());
    setRedThickness(settings.value("redThickness", 3).toFloat());
    setUpDesignation(settings.value("upDesignation","Temp").toString());
    setDownDesignation(settings.value("downDesignation","OIL").toString());
    setIsTableValues(settings.value("isTableValues", 1).toBool());
    setDataRefString(settings.value("dataRefsString", "sim/cockpit2/engine/indicators/oil_temperature_deg_C").toString());
    setAccuracy(settings.value("accuracy", 1).toFloat());
    setNbOfBigGraduations(settings.value("nbBigGraduations", 3).toInt());
    setNbOfThinGraduations(settings.value("nbThinGraducations", 7).toInt());
    setIsDisplayValue(settings.value("isDisplayValues",1).toBool());
    setLogDisplayStyle(settings.value("isLogDisplayStyle", 0).toBool());
    setArcValueAmplitudeDisplay(settings.value("arcDegreeAmplitude",120).toFloat());


    this->allowUpdateBottomPixmap = true;
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

    layout->addWidget(new QLabel("datarefs values are in table", layout->parentWidget()));
    QCheckBox *qcb1 = new QCheckBox(layout->parentWidget());
    qcb1->setChecked(isTableValues);
    layout->addWidget(qcb1);
    connect(qcb1, SIGNAL(clicked(bool)), this, SLOT(setIsTableValues(bool)));

    layout->addWidget(new QLabel("selected index for value", layout->parentWidget()));
    NumberInputLineEdit *indexNumberEdit = new NumberInputLineEdit(layout->parentWidget());
    indexNumberEdit->setText(QString::number(_indexNumber));
    layout->addWidget(indexNumberEdit);
    connect(indexNumberEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setIndexNumber(float)));

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

    layout->addWidget(new QLabel("arc amplitude to display", layout->parentWidget()));
    NumberInputLineEdit *arcRangeEdit = new NumberInputLineEdit(layout->parentWidget());
    arcRangeEdit->setText(QString::number(arcDegreeAmplitude));
    layout->addWidget(arcRangeEdit);
    connect(arcRangeEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setArcValueAmplitudeDisplay(float)));

    layout->addWidget(new QLabel(" _________________ ", layout->parentWidget()));
    layout->addWidget(new QLabel("graduations settings : ", layout->parentWidget()));

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

    layout->addWidget(new QLabel("display values", layout->parentWidget()));
    QCheckBox *isValuesDisplayEdit = new QCheckBox(layout->parentWidget());
    isValuesDisplayEdit->setChecked(isDisplayValues);
    layout->addWidget(isValuesDisplayEdit);
    connect(isValuesDisplayEdit, SIGNAL(clicked(bool)), this, SLOT(setIsDisplayValue(bool)));

    layout->addWidget(new QLabel("display into Logarithmic scale", layout->parentWidget()));
    QCheckBox *qcb2 = new QCheckBox(layout->parentWidget());
    qcb2->setChecked(isLogDisplayStyle);
    layout->addWidget(qcb2);
    connect(qcb2, SIGNAL(clicked(bool)), this, SLOT(setLogDisplayStyle(bool)));

    layout->addWidget(new QLabel(" _________________ ", layout->parentWidget()));
    layout->addWidget(new QLabel("green arc settings : ", layout->parentWidget()));

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

    layout->addWidget(new QLabel(" _________________ ", layout->parentWidget()));
    layout->addWidget(new QLabel("orange arc settings : ", layout->parentWidget()));

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

    layout->addWidget(new QLabel(" _________________ ", layout->parentWidget()));
    layout->addWidget(new QLabel("red arc settings : ", layout->parentWidget()));

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

}

void GenericLittleGauge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->save();
    //paint the background
    painter->drawPixmap(QPoint(0,0), *bottomPixmap);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    //paint the needle;
    if ( ! needleImage.isNull()) {
        int w, h, ww, hh;
        w=qMin(this->width(), this -> height());
        h=w;
        ww = needleImage.width() * this->scaleFactor;
        hh = needleImage.height() * this->scaleFactor;
        //center of the needle image is center of background image
        painter->translate((w-ww)/2, (h-hh)/2);

        int x,y,a;
        a = qMax(qMin(value2Angle(value), value2Angle(valueMax)), value2Angle(valueMin)) ;
        a = a - arcDegreeAmplitude/2;
        a = a - value2Angle(valueMin); // rotation degree


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

void GenericLittleGauge::drawBottomPixmap(){

    if (allowUpdateBottomPixmap) {

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
        QFont font("Verdana", defaultFont.pointSizeF() * 0.8 * this->scaleFactor);
        font.setItalic(true);
        QFontMetrics fm1(font);
        pa.setFont(font);
        pa.translate(w/2,2*h/5);
        pa.drawText(-1*fm1.width(upDesignation)/2, 0, upDesignation);
        //font size a little bit higher
        font.setPointSizeF(defaultFont.pointSizeF() * this->scaleFactor);
        pa.setFont(font);
        QFontMetrics fm2(font);
        pa.translate(0,2*h/6);
        pa.drawText(-1*fm2.width(downDesignation)/2, 0, downDesignation);

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
        for (int i=0; i < nbThinGraduations; i++) {
            float valueAtGrad= Interpolation::linear(0,nbThinGraduations -1,valueMin, valueMax, i );
            pa.save();
            pa.rotate(value2Angle(valueAtGrad) - value2Angle(valueMin));
            pa.drawLine( (w -  (5 * this->scaleFactor)) * arcRatioPosition/2, 0, w * arcRatioPosition/2, 0);
            pa.restore();
        }
        pa.restore();


        //draw big graduations
        pa.save();

        pen.setWidth(1+w/250); //pen width increase when w increase
        pen.setColor(QColor(0xb4,0xba,0xbd)); //grey color
        pa.setPen(pen);
        pa.translate(w/2,h/2);
        pa.rotate(-1 * (180 + arcDegreeAmplitude) / 2 );
        for (int i=0; i < nbBigGraduations; i++) {
            float valueAtGrad= Interpolation::linear(0,nbBigGraduations -1,valueMin, valueMax, i );
            pa.save();
            pa.rotate(value2Angle(valueAtGrad) - value2Angle(valueMin));
            pa.drawLine( (w -  (10 * this->scaleFactor)) * arcRatioPosition/2, 0, ( w + ( 10 * this->scaleFactor)) * arcRatioPosition/2, 0);
            pa.restore();
        }
        pa.restore();

        //draw values of big graduations
        if (isDisplayValues) {
            pa.save();

            pen.setColor(QColor(0xb4,0xba,0xbd)); //grey color
            pa.setPen(pen);
            QFont font("Verdana", this->defaultFont.pointSizeF() /1.3 * this->scaleFactor);
            QFontMetrics fm(font);
            pa.setFont(font);
            pa.translate(w/2,h/2);
            for (int i=0; i< nbBigGraduations; i++) {
                float valueDisplay = Interpolation::linear(0,nbBigGraduations -1,valueMin, valueMax, i );
                QString textDisplay = QString::number( (int)valueDisplay) ;
                float x,y;
                x =  cos(3.14159/180 * (value2Angle(valueDisplay) - value2Angle(valueMin) -  (180 + arcDegreeAmplitude) / 2) ) ;
                x = x * (w*arcRatioPosition+8*scaleFactor)/2 ;
                //if text is on the left side, make offset
                if ( (value2Angle(valueDisplay) - value2Angle(valueMin)) < ( arcDegreeAmplitude / 2) ) {
                    x = x - fm.width(textDisplay) ;
                //else if text is on center of panel (+/- 1 degree) then make half offset
                }else if (  abs ( ((value2Angle(valueDisplay) - value2Angle(valueMin)) - ( arcDegreeAmplitude / 2) ) )  < 1) {
                    x = x - fm.width(textDisplay)/2 ;
                }
                y = sin(3.14159/180* ( value2Angle(valueDisplay) - value2Angle(valueMin) -  (180 + arcDegreeAmplitude) / 2 ) );
                y = y *(w*arcRatioPosition+8*scaleFactor)/2;

                pa.drawText( x, y , textDisplay);
            }
            pa.restore();

        }




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

    float beginAbsoluteAngle = (180 - arcDegreeAmplitude) / 2 + arcDegreeAmplitude + value2Angle(valueMin) ;

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
    Q_UNUSED(w);
    Q_UNUSED(h);
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
    if (mv != greenBeginValue ) {
        greenBeginValue = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setGreenEndValue(float mv){
    if (mv != greenEndValue ) {
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
    if (mv != orangeBeginValue ) {
        orangeBeginValue = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setOrangeEndValue(float mv){
    if (mv != orangeEndValue ) {
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
    if (mv != redBeginValue ) {
        redBeginValue = mv;
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setRedEndValue(float mv){
    if (mv != redEndValue ) {
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

    if (QString::compare(s,dataRefsString) != 0 ) {

        // Unsubscribe old
        if (_client.isDataRefSubscribed(dataRefsString)) {
            _client.unsubscribeDataRef(dataRefsString);
        }

        dataRefsString = s;
        value = 0;

        // Subscribe new
        if(QString::compare("",s) != 0 ) _client.subscribeDataRef(s, accuracy);
        update();
    }
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
        this->drawBottomPixmap();
    }
}

void GenericLittleGauge::setIsDisplayValue(bool yes){
    isDisplayValues = yes;
    this->drawBottomPixmap();
}

void GenericLittleGauge::setLogDisplayStyle(bool log){
    isLogDisplayStyle=log;
    this->drawBottomPixmap();
}

void GenericLittleGauge::setArcValueAmplitudeDisplay(float mv){
    if ( (int)mv != arcDegreeAmplitude ) {
        arcDegreeAmplitude = (int)mv;
        this->drawBottomPixmap();
    }
}
