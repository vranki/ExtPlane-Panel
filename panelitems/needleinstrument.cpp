#include "needleinstrument.h"

Arc::Arc(QObject *parent, PanelItem *panel) : QObject(parent){
    _parent = parent;
    _panel = panel;
}

void Arc::update(void){
    _panel->update();
}

void Arc::setMin(float value){
    if (value == _minValue) return;
    _minValue = value;
    update();
}

void Arc::setUse(bool use){

}

void Arc::setMax(float value){
    if (value == _maxValue) return;
    _maxValue = value;
    update();
}
void Arc::setInner(float value){
    if (value == _radius_inner) return;
    _radius_inner = value;
    update();
}
void Arc::setOuter(float value){
    if (value == _radius_outer) return;
    _radius_outer = value;
    update();
}

void Arc::setColor(QString colorString){

}

NeedleInstrument::NeedleInstrument(QObject *parent) :
    PanelItem(parent)
{
    _value = 0;
    _minValue = 0;
    _minAngle = 0;
    _maxValue = 1.;
    _maxAngle = 360.;
    _minTextValue = 0;
    _thickBars = _thinBars = 0;
    _numbers = 0;
    _numberScale = 1;
    
    _numberFontname = "Helvetica";
    _numberFontsize = 16;
    _numberOfArcs = 0;
    //    _arcs = new QList<Arc *>();
  
    _bezel = QPixmap::fromImage(QImage(QString("../../images/bezel_square_.png")), Qt::AutoColor);
    
}

void NeedleInstrument::setNumbers(float div) {
    _numbers = div;
}

void NeedleInstrument::setNumberMult(float ns) {
    _numberScale = ns;
}

void NeedleInstrument::paintArcs(QPainter *painter)
{
    float       startAngleD, endAngleD, sweep;
    float       startAngle, endAngle;
    float       outer, inner;
    Arc         *arc;
   
    qDebug() << Q_FUNC_INFO << "______painting arc. No of arcs " << _numberOfArcs;
    
    if (_numberOfArcs){
        for(int i=0;i<_numberOfArcs;i++){
            arc = _arcs[i];
            if (arc->_use) {
                
                startAngleD = 90-value2Angle(arc->_minValue);
                endAngleD = 90-value2Angle(arc->_maxValue);
                sweep = 0.-(endAngleD - startAngleD);
                
                startAngle = startAngleD * 3.1415926/180.0;
                endAngle = endAngleD * 3.1415926/180.0;
                
                
                inner = arc->_radius_inner * 100.;
                outer = arc->_radius_outer * 100.;
                
                QPainterPath arcPath;
                
                arcPath.moveTo(inner * cos(startAngle), -inner * sin(startAngle));
                arcPath.lineTo(outer * cos(startAngle), -outer * sin(startAngle));
                arcPath.arcTo(-outer,-outer,2*outer,2*outer, startAngleD, -sweep);
                //arcPath.lineTo(outer * cos(endAngle), -outer * sin(endAngle));
                arcPath.lineTo(inner * cos(endAngle), -inner * sin(endAngle));
                arcPath.arcTo(-inner,-inner,2*inner,2*inner, endAngleD, sweep);
                arcPath.closeSubpath();
                
                /*
                arcPath.moveTo(inner * cos(startAngle), -inner * sin(startAngle));
                arcPath.lineTo(outer * cos(startAngle), -outer * sin(startAngle));
                arcPath.arcTo(-outer,-outer,2*outer,2*outer, 90-startAngleD, -sweep);
                arcPath.lineTo(inner * cos(endAngle), -inner * sin(endAngle));
                arcPath.arcTo(-inner,-inner,2*inner,2*inner, 90-endAngleD, sweep);
                arcPath.closeSubpath();
                */
                
                painter->setPen(Qt::NoPen); 
                painter->setBrush(arc->_color);
                
                painter->drawPath(arcPath);
                
            }
        }
    }
    
}

void NeedleInstrument::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    static const QPoint needle[3] = {
        QPoint(6, 8),
        QPoint(-6, 8),
        QPoint(0, -95)
    };

    QColor needleColor(255, 255, 255); //0, 0); //255, 255);

    int side = qMin(width(), height());
    painter->save();
    painter->scale(side / 200.0, side / 200.0);
    painter->save();
    painter->translate(100, 100);
    
    painter->setBrush(Qt::black);
    painter->setRenderHint(QPainter::Antialiasing);
    qDebug() << Q_FUNC_INFO << "setting hint "; 
    painter->drawChord(-95,-95,190,190,0,360*16);

    
    painter->setPen(Qt::NoPen);
    painter->setBrush(needleColor);

    painter->save();
    
    float needleValue = qMax(qMin(_value, _maxValue), _minValue);

    painter->scale(0.90, 0.90); // Scale to draw within bezel
    painter->save();
    
    painter->rotate(value2Angle(needleValue));
    painter->drawConvexPolygon(needle, 3);
    painter->restore();

    painter->setPen(Qt::white);
    if(!_label.isEmpty()) {
        int textwidth = painter->fontMetrics().width(_label);
        painter->drawText(-textwidth/2,-70, textwidth, 200, Qt::AlignCenter, _label);
    }
    // painter->drawRect(-100,0,1,20); Test line for fitting bezel
    
    paintArcs(painter);
    painter->setPen(Qt::white);
    painter->setBrush(Qt::white);
    
    if(_thickBars != 0) {
        for (float i = _minValue ; i <= _maxValue; i+=_thickBars) {
            if (i>=_minTextValue){
                painter->save();
                painter->rotate(value2Angle(i));
                painter->drawRect(-1, -100, 2, 14);
                painter->restore();
            }
        }
    }
    if(_thinBars != 0) {
        for (float i = _minValue ; i <= _maxValue; i+=_thinBars) {
            if (i>=_minTextValue){
                painter->save();
                painter->rotate(value2Angle(i));
                painter->drawRect(-0.3, -100, 0.6, 8);
                painter->restore();
            }
        }
    }
    painter->setPen(QColor(200,200,200));
    if(_numbers != 0) {
        for (float i = _minValue ; i <= _maxValue; i+=_numbers) {
            if (i>=_minTextValue){
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
    }
    
    painter->restore();     // Restore to full 200x200 size
    
    painter->drawPixmap(-100, -100, 200, 200, _bezel);
    
    painter->restore();

    painter->restore();
    qDebug() << Q_FUNC_INFO << "finishing paint "; 
    PanelItem::paint(painter, option, widget);
    qDebug() << Q_FUNC_INFO << "after supercall "; 

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

int NeedleInstrument::numberOfArcs(void)
{
    return _numberOfArcs;
}

Arc * NeedleInstrument::getArc(int arcNo)
{
    return _arcs[arcNo];
}


void NeedleInstrument::clearArcs(void)
{
    foreach(Arc *g, _arcs) {
        qDebug() << Q_FUNC_INFO << "deleting arc "; 
        _arcs.removeOne(g);
        delete g;
    }
    _numberOfArcs=0;
}

void NeedleInstrument::addArc(Arc *arc)
{
    _arcs.append(arc);
    _numberOfArcs+=1;
    int dummy = _arcs.count();
    qDebug() << Q_FUNC_INFO << "Now: " << dummy << " arcs in _arc"; 
    
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

void NeedleInstrument::setMinTextValue(float value) {
    if(value == _minTextValue) return;
    _minTextValue = value;
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
    settings.setValue("minValue", QString::number(_minValue));
    settings.setValue("minTextValue", QString::number(_minTextValue));
    settings.setValue("maxValue", QString::number(_maxValue));
    settings.setValue("minAngle", QString::number(_minAngle));
    settings.setValue("maxAngle", QString::number(_maxAngle));
    
/*    settings.setValue("numberOfArcs", QString::number(_numberOfArcs));
    
    if (_numberOfArcs){
        for(int i=0;i<_numberOfArcs;i++){
            settings.beginGroup("arc" + QString::number(i));
            settings.setValue("use", QString::number(_arcs[i]->_use));
            settings.setValue("minValue", QString::number(_arcs[i]->_minValue));
            settings.setValue("maxValue", QString::number(_arcs[i]->_maxValue));
            settings.setValue("innerRadius", QString::number(_arcs[i]->_radius_inner));
            settings.setValue("outerRadius", QString::number(_arcs[i]->_radius_outer));
            settings.setValue("color", _arcs[i]->_color.name());
            settings.endGroup();
        }
    }
*/
}

    
void NeedleInstrument::loadSettings(QSettings &settings){
    PanelItem::loadSettings(settings);
    setMinValue(settings.value("minValue", 30).toFloat());
    setMinTextValue(settings.value("minTextValue", 0).toFloat());
    setMinAngle(settings.value("minAngle", 30).toFloat());
    setMaxValue(settings.value("maxValue", 190).toFloat());
    setMaxAngle(settings.value("maxAngle", 330).toFloat());
}

void NeedleInstrument::CreateNumberEntry(QGridLayout *layout, QString legend, 
                                         float value, 
                                         QObject *which,
                                         char const * slot_method)
{
    QLabel *junkLabel = new QLabel(legend, layout->parentWidget());
    layout->addWidget(junkLabel);
    NumberInputLineEdit *junkValueEdit = new NumberInputLineEdit(layout->parentWidget());
    junkValueEdit->setText(QString::number(value));
    layout->addWidget(junkValueEdit);
    connect(junkValueEdit, SIGNAL(valueChangedFloat(float)), which, slot_method);
}
                                    
void NeedleInstrument::setArcMin(int arcNumber, float value)
{   
    if (_arcs[arcNumber]->_minValue==value) return;
    _arcs[arcNumber]->_minValue=value;
    update();
}

void NeedleInstrument::createSettings(QGridLayout *layout){
    
    CreateNumberEntry(layout, "Minimum value", 
                      _minValue, this, SLOT(setMinValue(float)));
    CreateNumberEntry(layout, "Minimum text value", 
                      _minTextValue, this, SLOT(setMinTextValue(float)));
    CreateNumberEntry(layout, "Angle of min value", 
                      _minAngle, this, SLOT(setMinAngle(float)));
    CreateNumberEntry(layout, "Maximum value", 
                      _maxValue, this, SLOT(setMaxValue(float)));
    CreateNumberEntry(layout, "Angle of max value", 
                      _maxAngle, this, SLOT(setMaxAngle(float)));
    

    QString legend;

    if (_numberOfArcs){
        for(int i=0;i<_numberOfArcs;i++){
            if (_arcs[i]->_use){
                legend = QString("Arc ")+QString::number(i)+QString(" min value:");
                CreateNumberEntry(layout, legend, 
                                  _arcs[i]->_minValue, _arcs[i],
                                  SLOT(setMin(float)));
                legend = QString("Arc ")+QString::number(i)+QString(" max value:");
                CreateNumberEntry(layout, legend, 
                                  _arcs[i]->_maxValue, _arcs[i],
                                  SLOT(setMax(float)));
                legend = QString("Arc ")+QString::number(i)+QString(" min radius:");
                CreateNumberEntry(layout, legend, 
                                  _arcs[i]->_radius_inner, _arcs[i],
                                  SLOT(setInner(float)));
                legend = QString("Arc ")+QString::number(i)+QString(" max radius:");
                CreateNumberEntry(layout, legend, 
                                  _arcs[i]->_radius_outer, _arcs[i],
                                  SLOT(setOuter(float)));
            }
        }
    }
}

