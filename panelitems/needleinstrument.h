#ifndef NEEDLEINSTRUMENT_H
#define NEEDLEINSTRUMENT_H

#include "panelitem.h"
#include <QFont>
#include "../needles/needle.h"

/**
 * Represents an arc with different properties used in
 * round instruments.
 *
 * @todo move to own class file(s)
 */
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

private:
    QObject * _parent;
    PanelItem *_panel;
};

/**
 * Base class for round instruments with one (or more) needles.
 */
class NeedleInstrument : public PanelItem {
    Q_OBJECT
public:
    explicit NeedleInstrument(QObject *parent = 0);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setScale(float zeroAngle, float zeroValue, float maxAngle, float maxValue);
    void setBars(float thick, float thin);
    void setNumbers(float div);
    void setNumberScale(float ns);
    void setLabel(QString text);
    virtual QString typeName() = 0;
    float value2Angle(float value);

    void addArc(Arc *arc);
    void clearArcs();
    void paintArcs(QPainter *painter);
    int numberOfArcs();
    Arc *getArc(int arcNo);
    void setArcMin(int arcNumber, float value);

public slots:
    void setValue(float value);
    void setNeedle(Needle * newNeedle);
protected:
    float _thickBars, _thinBars;
    float _numbers;
    float _numberScale;
    float _value, _zeroangle, _maxAngle, _zeroValue, _maxValue;
    QString _label;
    QList<Arc*> _arcs;
    Needle *needle;
};

#endif // NEEDLEINSTRUMENT_H
