#ifndef ROTARYKNOB_H
#define ROTARYKNOB_H

#include "panelitem.h"
#include <QPoint>

class RotaryKnob : public PanelItem
{
    Q_OBJECT
public:
    RotaryKnob(QObject *parent, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    static QString typeNameStatic();
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
    void setRef(QString ref);
private slots:
    void valueChanged(QString ref, double newValue);
    void setChangeString(QString ch);
private:
    ExtPlaneClient _client;
    QString _refname;
    double _value, _change, _valueStartPoint;
    ClientDataRef* _ref;
    QPointF dragStartPoint;
};

#endif // ROTARYKNOB_H
