#ifndef SLIPINDICATOR_H
#define SLIPINDICATOR_H

#include "panelitem.h"

#define SLIP_RANGE_DEG 10

/**
 * Basic slip indicator.
 */
class SlipIndicator : public PanelItem
{
    Q_OBJECT
public:
    Q_INVOKABLE SlipIndicator(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
signals:

public slots:
    void slipChanged(QString name, double slip);
private:
    ExtPlaneClient _client;
    double _slip;
};

#endif // SLIPINDICATOR_H
