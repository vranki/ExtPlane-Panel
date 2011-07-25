#ifndef NEEDLEINSTRUMENT_H
#define NEEDLEINSTRUMENT_H

#include "panelitem.h"
#include <QLabel>
#include "widgets/numberinputlineedit.h"

struct ScaleRangeStruct {
    bool    use;
    bool    linear;
    float   minValue;
    float   minAngle;
    float   maxValue;
    float   maxAngle;
};

struct ArcStruct {
    bool    use;
    float   minValue;
    float   maxValue;
    float   radius_inner;
    float   radius_outer;
    QColor  color;
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

public slots:
    void setValue(float value);
    void setMinValue(float value);
    void setMaxValue(float value);    
    void setMinAngle(float value);
    void setMaxAngle(float value);    

private:
    float _thickBars, _thinBars;
    float _numbers;
    float _numberScale;
    float value2Angle(float value);
    
    float _value, _minAngle, _maxAngle, _minValue, _maxValue;

    
    QString _label;
};

#endif // NEEDLEINSTRUMENT_H
