#ifndef PANELITEMFACTORY_H
#define PANELITEMFACTORY_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QMetaObject>

class PanelItem;
class ExtPlaneConnection;

typedef QMap<QString,const QMetaObject*> BaseFactoryMapType;

/**
 * Factory class for Panel Items. Implements a nice macro that can be used to register
 * PanelItem types and a function to query available items.
 *
 * @set PanelItem
 */
class PanelItemFactory
{
public:
    PanelItemFactory();
    PanelItemFactory(ExtPlaneConnection *conn);
    /**
     * @brief itemForName creates a new PanelItem
     * @param name Name of the item
     * @param parentObject Parent QObject of the created item
     * @return a new PanelItem
     */
    PanelItem *itemForName(QString name, QObject *parentObject);
    /**
     * @brief itemNames lists names of all available items
     * @return list of all available items
     */
    QStringList itemNames();

private:
    ExtPlaneConnection *connection;

protected:
    static BaseFactoryMapType *classMapping();
};

// Helper class which forces derived PanelItems which instante this to register with the PanelFactory
template<typename T>
struct RegisterWithPanelItemFactory : PanelItemFactory {
    RegisterWithPanelItemFactory(QString s) {
        classMapping()->insert(s, &T::staticMetaObject);
    }
};

// Helper macro used by derived classes of PanelItem to automatically register their name with the PanelFactory
#define REGISTER_WITH_PANEL_ITEM_FACTORY(CLASS,NAME) QString CLASS::typeName() {return NAME;} RegisterWithPanelItemFactory<CLASS> registerWithFactory_##CLASS(NAME);

#endif // PANELITEMFACTORY_H
