#include "panelitemfactory.h"

PanelItemFactory::PanelItemFactory(ExtPlaneConnection *conn)
{
    connection = conn;
}

PanelItem *PanelItemFactory::itemForName(QString name, QObject *parentObject) {
    PanelItem *g = 0;
    if(name == AirspeedIndicator::typeNameStatic()) {
        g = new AirspeedIndicator(parentObject, connection);
    } else if(name == Compass::typeNameStatic()) {
        g = new Compass(parentObject, connection);
    } else if(name == Variometer::typeNameStatic()) {
        g = new Variometer(parentObject, connection);
    } else if(name == Altimeter::typeNameStatic()) {
        g = new Altimeter(parentObject, connection);
    }
    return g;
}

QStringList PanelItemFactory::itemNames() {
    QStringList items;
    items << AirspeedIndicator::typeNameStatic() << Compass::typeNameStatic() << Variometer::typeNameStatic() << Altimeter::typeNameStatic();
    return items;
}
