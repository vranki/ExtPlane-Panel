#ifndef COMPASS_H
#define COMPASS_H

#include "extplaneclient.h"
#include "panelitem.h"

class Compass : public PanelItem
{
    Q_OBJECT
public:
    Q_INVOKABLE Compass(QObject *parent, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
signals:

public slots:
    void headingChanged(QString name, double hdg);
private:
    ExtPlaneClient _client;
    int _heading;
};

#endif // COMPASS_H
