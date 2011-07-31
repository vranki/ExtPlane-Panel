#ifndef NEEDLEINSTRUMENT_H
#define NEEDLEINSTRUMENT_H

#include "panelitem.h"
#include <QLabel>
#include "widgets/numberinputlineedit.h"
#include <math.h>

class ScaleRangeStruct{
    bool    use;
    bool    linear;
    float   minValue;
    float   minAngle;
    float   maxValue;
    float   maxAngle;
};

class Arc : public QObject {
    Q_OBJECT
public:
    explicit Arc(QObject *parent = 0, PanelItem *panel=0);
    void update(void);
    
    bool    _use;
    float   _minValue;
    float   _maxValue;
    float   _radius_inner;
    float   _radius_outer;
    QColor  _color;
    
public slots:
    void setUse(bool);
    void setMin(float);
    void setMax(float);
    void setInner(float);
    void setOuter(float);
    void setColor(QString);
    
    //private:
   
private:
    QObject * _parent;
    PanelItem *_panel;
};


class NeedleInstrument : public PanelItem {
    Q_OBJECT
public:
    explicit NeedleInstrument(QObject *parent = 0);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setScale(float zeroAngle, float zeroValue, float maxAngle, float maxValue);
    void setBars(float thick, float thin);
    void setNumbers(float div);
    void setNumberMult(float ns);
    void setLabel(QString text);
    virtual QString typeName() = 0;

    
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout); 
    void addArc(Arc *arc);
    void clearArcs(void);

    void paintArcs(QPainter *painter);
    
    void CreateNumberEntry(QGridLayout *layout, QString legend, 
                           float value, QObject *which, 
                           char const * slot_method);
    int numberOfArcs(void);
    Arc * getArc(int arcNo);


public slots:
    void setValue(float value);
    void setMinValue(float value);
    void setMinTextValue(float value);
    void setMaxValue(float value);    
    void setMinAngle(float value);
    void setMaxAngle(float value);   
    void setNumberFontsize(int fs);
    void setArcMin(int arcNumber, float value);

private:
    float _thickBars, _thinBars;
    float _numbers;
    float _numberScale;
    float value2Angle(float value);
    
    float _value;
    float _minAngle, _maxAngle;
    float _minValue, _maxValue;
    float _minTextValue;        // Values less than this aren't labelled

    QString _numberFontname;
    int _numberFontsize;
    QPixmap _bezel;
    QString _label;
    
    int _numberOfArcs;
    //    ArcStruct * _arcs;
    QList<Arc*> _arcs;
    // Arc *_arcs;
};

#endif // NEEDLEINSTRUMENT_H
