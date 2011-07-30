#include "needleinstrument.h"

NeedleInstrument::NeedleInstrument(QObject *parent) :
    PanelItem(parent)
{
    _value = 0;
    _minValue = 0;
    _minAngle = 0;
    _maxValue = 1.;
    _maxAngle = 360.;
    _thickBars = _thinBars = 0;
    _numbers = 0;
    _numberScale = 1;
    
    _numberFontname = "Helvetica";
    _numberFontsize = 16;
   
}

void NeedleInstrument::setNumbers(float div) {
    _numbers = div;
}

void NeedleInstrument::setNumberMult(float ns) {
    _numberScale = ns;
}


void NeedleInstrument::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    static const QPoint needle[3] = {
        QPoint(6, 8),
        QPoint(-6, 8),
        QPoint(0, -95)
    };

    QColor needleColor(255, 255, 255); //0, 0); //255, 255);

    int side = qMin(width(), height());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();
    painter->scale(side / 200.0, side / 200.0);
    painter->save();
    painter->translate(100, 100);
    
    painter->setBrush(Qt::black);
    painter->drawChord(-100,-100,200,200,0,360*16);

    
    painter->setPen(Qt::NoPen);
    painter->setBrush(needleColor);

    painter->save();
    float needleValue = qMax(qMin(_value, _maxValue), _minValue);

    painter->rotate(value2Angle(needleValue));
    painter->drawConvexPolygon(needle, 3);
    painter->restore();

    painter->setPen(Qt::white);
    if(!_label.isEmpty()) {
        int textwidth = painter->fontMetrics().width(_label);
        painter->drawText(-textwidth/2,-70, textwidth, 200, Qt::AlignCenter, _label);
    }
    if(_thickBars != 0) {
        for (float i = _minValue ; i <= _maxValue; i+=_thickBars) {
            painter->save();
            painter->rotate(value2Angle(i));
            painter->drawRect(-1, -100, 2, 14);
            painter->restore();
        }
    }
    if(_thinBars != 0) {
        for (float i = _minValue ; i <= _maxValue; i+=_thinBars) {
            painter->save();
            painter->rotate(value2Angle(i));
            painter->drawRect(-0.3, -100, 0.6, 8);
            painter->restore();
        }
    }
    painter->setPen(QColor(200,200,200));
    if(_numbers != 0) {
        for (float i = _minValue ; i <= _maxValue; i+=_numbers) {
            painter->save();
            painter->rotate(value2Angle(i));
            painter->save();
            QString lineNumber = QString::number(i * _numberScale);
            painter->translate(0,-73);
            painter->rotate(-value2Angle(i));

            painter->setPen(QColor(200,200,200));
            painter->setFont(QFont(QString(_numberFontname), 
                                   _numberFontsize, QFont::Bold, false));
            
            int width = painter->fontMetrics().width(lineNumber);
            int height =painter->fontMetrics().height();
            painter->drawText(-width/2,-height/2,width,height, Qt::AlignCenter,  lineNumber);
            painter->restore();
            painter->restore();
        }
    }
    QPixmap _bezel = QPixmap::fromImage(QImage(QString("../../images/bezel2.png")), Qt::AutoColor);//_bezelImage, Qt::AutoColor);
    painter->drawPixmap(-_bezel.width()/4.2-3,-_bezel.height()/4.1 +5, 
                        _bezel.width()/2.1, _bezel.height()/2.05, 
                        _bezel);

    painter->restore();

    painter->restore();
    PanelItem::paint(painter, option, widget);
}


void NeedleInstrument::setLabel(QString text) {
    _label = text;
}

void NeedleInstrument::setScale(float zeroAngle, float zeroValue, float maxAngle, float maxValue) {
    Q_ASSERT(maxAngle != 0);
    _minAngle = zeroAngle;
    _minValue = zeroValue;
    _maxAngle = maxAngle;
    _maxValue = maxValue;
    update();
}


void NeedleInstrument::setValue(float value) {
    if(value == _value) return;
    _value = value;
    update();
}

void NeedleInstrument::setMinValue(float value) {
    if(value == _minValue) return;
    _minValue = value;
    update();
}

void NeedleInstrument::setMaxValue(float value) {
    if(value == _maxValue) return;
    _maxValue = value;
    update();
}

void NeedleInstrument::setMinAngle(float value) {
    if(value == _minAngle) return;
    _minAngle = value;
    update();
}

void NeedleInstrument::setMaxAngle(float value) {
    if(value == _maxAngle) return;
    _maxAngle = value;
    update();
}

void NeedleInstrument::setBars(float thick, float thin) {
    _thinBars = thin;
    _thickBars = thick;
}

void NeedleInstrument::setNumberFontsize(int fs)
{
    _numberFontsize = fs;
    update();
}

float NeedleInstrument::value2Angle(float value) {
    return _minAngle +  ((value - _minValue) / (_maxValue - _minValue)) * (_maxAngle - _minAngle);
}

void NeedleInstrument::storeSettings(QSettings &settings){
    PanelItem::storeSettings(settings);
    settings.setValue("minValue", _minValue);
    settings.setValue("maxValue", _maxValue);
    settings.setValue("minAngle", _minAngle);
    settings.setValue("maxAngle", _maxAngle);
}

void NeedleInstrument::loadSettings(QSettings &settings){
    PanelItem::loadSettings(settings);
    setMinValue(settings.value("minValue", 30).toFloat());
    setMinAngle(settings.value("minAngle", 30).toFloat());
    setMaxValue(settings.value("maxValue", 190).toFloat());
    setMaxAngle(settings.value("maxAngle", 330).toFloat());
}




void NeedleInstrument::createSettings(QGridLayout *layout){
    QLabel *zeroLabel = new QLabel("Minimum value", layout->parentWidget());
    layout->addWidget(zeroLabel);
    NumberInputLineEdit *zeroValueEdit = new NumberInputLineEdit(layout->parentWidget());
    zeroValueEdit->setText(QString::number(_minValue));
    layout->addWidget(zeroValueEdit);
    connect(zeroValueEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMinValue(float)));

    QLabel *minALabel = new QLabel("Angle of min value", layout->parentWidget());
    layout->addWidget(minALabel);
    NumberInputLineEdit *minAngleEdit = new NumberInputLineEdit(layout->parentWidget());
    minAngleEdit->setText(QString::number(_minAngle));
    layout->addWidget(minAngleEdit);
    connect(minAngleEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMinAngle(float)));
    
    
    QLabel *maxLabel = new QLabel("Maximum value", layout->parentWidget());
    layout->addWidget(maxLabel);
    NumberInputLineEdit *maxValueEdit = new NumberInputLineEdit(layout->parentWidget());
    maxValueEdit->setText(QString::number(_maxValue));
    layout->addWidget(maxValueEdit);
    connect(maxValueEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMaxValue(float)));

    QLabel *maxALabel = new QLabel("Angle of max value", layout->parentWidget());
    layout->addWidget(maxALabel);
    NumberInputLineEdit *maxAngleEdit = new NumberInputLineEdit(layout->parentWidget());
    maxAngleEdit->setText(QString::number(_maxAngle));
    layout->addWidget(maxAngleEdit);
    connect(maxAngleEdit, SIGNAL(valueChangedFloat(float)), this, SLOT(setMaxAngle(float)));
    
    
    
}

