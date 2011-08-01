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
    } else if(name == DirectionIndicator::typeNameStatic()) {
        g = new DirectionIndicator(parentObject, connection);
    } else if(name == HSI::typeNameStatic()) {
        g = new HSI(parentObject, connection);
    } else if(name == AttitudeIndicator::typeNameStatic()) {
        g = new AttitudeIndicator(parentObject, connection);
    } else if(name == TurnAndBank::typeNameStatic()) {
        g = new TurnAndBank(parentObject, connection);
    } else if(name == EngineRPM::typeNameStatic()) {
        g = new EngineRPM(parentObject, connection);
    } else if(name == Button::typeNameStatic()) {
        g = new Button(parentObject, connection);
    }
    return g;
}

QStringList PanelItemFactory::itemNames() {
    QStringList items;
    items << AirspeedIndicator::typeNameStatic() <<
             Altimeter::typeNameStatic() << 
             AttitudeIndicator::typeNameStatic() << 
             Compass::typeNameStatic() << 
             DirectionIndicator::typeNameStatic() << 
             EngineRPM::typeNameStatic() << 
             HSI::typeNameStatic() <<
             TurnAndBank::typeNameStatic() << 
             Variometer::typeNameStatic() << 
             Button::typeNameStatic();
    return items;
}
