#ifndef DIAL_H
#define DIAL_H

#include "panelitem.h"

class Dial : public PanelItem {
    Q_OBJECT
public:
    Q_INVOKABLE Dial(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);
    virtual void applySettings();
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:

public slots:
    void setLabel(QString txt);
    void setRef(QString ref);
private slots:
    void valueChanged(QString ref, double newValue);
private:
    ExtPlaneClient _client;
    int _dialPosition;
    int _dialPositions;
    QString _label;
    QString _refname;
    double _value;
    ClientDataRef* _ref;
};

#endif // DIAL_H
