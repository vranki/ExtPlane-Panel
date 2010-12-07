#ifndef PANELITEMFACTORY_H
#define PANELITEMFACTORY_H

#include "panelitem.h"
#include <QString>
#include <QStringList>
#include "airspeedindicator.h"
#include "compass.h"
#include "extplaneconnection.h"

class PanelItemFactory
{
public:
    PanelItemFactory(ExtPlaneConnection *conn);
    PanelItem *itemForName(QString name, QObject *parentObject);
    QStringList itemNames();
private:
    ExtPlaneConnection *connection;
};

#endif // PANELITEMFACTORY_H
