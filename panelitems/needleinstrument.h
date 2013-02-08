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
    float value2Angle(float value);
public slots:
    void setValue(float value);
protected:
private:
    float _thickBars, _thinBars;
    float _numbers;
    float _value, _zeroangle, _maxAngle, _zeroValue, _maxValue;
    QString _label;
    QFont numberFont;
};

#endif // NEEDLEINSTRUMENT_H
