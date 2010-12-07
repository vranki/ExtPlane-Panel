#ifndef NEEDLEINSTRUMENT_H
#define NEEDLEINSTRUMENT_H

#include "panelitem.h"

class NeedleInstrument : public PanelItem {
    Q_OBJECT
public:
    explicit NeedleInstrument(QObject *parent = 0);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setScale(float zeroAngle, float zeroValue, float maxAngle, float maxValue);
    void setBars(float thick, float thin);
    void setNumbers(float div);
    void setLabel(QString text);
    virtual QString typeName() = 0;
public slots:
    void setValue(float value);
private:
    float _thickBars, _thinBars;
    float _numbers;
    float value2Angle(float value);
    float _value, _zeroangle, _maxAngle, _zeroValue, _maxValue;
    QString _label;
};

#endif // NEEDLEINSTRUMENT_H
