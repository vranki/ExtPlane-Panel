#include "panelitemfactory.h"
#include "panelitems/airspeedindicator.h"
#include "panelitems/compass.h"
#include "panelitems/variometer.h"
#include "panelitems/altimeter.h"
#include "panelitems/button.h"
#include "panelitems/switch.h"
#include "panelitems/rotaryknob.h"

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
    } else if(name == Button::typeNameStatic()) {
        g = new Button(parentObject, connection);
    } else if(name == Switch::typeNameStatic()) {
        g = new Switch(parentObject, connection);
    } else if(name == RotaryKnob::typeNameStatic()) {
        g = new RotaryKnob(parentObject, connection);
    }
    return g;
}

QStringList PanelItemFactory::itemNames() {
    QStringList items;
    items << AirspeedIndicator::typeNameStatic() << Compass::typeNameStatic() << Variometer::typeNameStatic()
          << Altimeter::typeNameStatic() << Button::typeNameStatic() << Switch::typeNameStatic() << RotaryKnob::typeNameStatic();
    return items;
}
