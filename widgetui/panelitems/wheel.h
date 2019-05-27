#ifndef WHEEL_H
#define WHEEL_H

#include "panelitem.h"
#include <QPoint>

/**
 * A generic wheel that can set values
 */
class Wheel : public PanelItem
{
    Q_OBJECT
public:
    Q_INVOKABLE Wheel(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);
    virtual void applySettings();
protected:
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
signals:

public slots:
    void setChange(double ca);
    void setMinimum(float ca);
    void setMaximum(float ca);
    void setRef(QString ref);
private slots:
    void valueChanged(QString ref, double newValue);
    void setChangeString(QString ch);
    void setIndex1Set(bool input);
    void setIndex2Set(bool input);
    void setIndex3Set(bool input);
    void setIndex4Set(bool input);
    void setIndex5Set(bool input);
    void setIndex6Set(bool input);
    void setIndex7Set(bool input);
    void setIndex8Set(bool input);
    void setKnobColor(QColor ch);
private:
    ExtPlaneClient _client;
    QString _refname;
    double _value, _change, _valueStartPoint;
    double _draggedPixels;
    int _stripDistance;
    float _minimum, _maximum;
    bool _index1set;
    bool _index2set;
    bool _index3set;
    bool _index4set;
    bool _index5set;
    bool _index6set;
    bool _index7set;
    bool _index8set;
    QColor _knobcolor;
    ClientDataRef* _ref;
    QPointF _dragStartPoint;
    QPointF _dragValueStartPoint;
    double _turns;
};

#endif // WHEEL_H
