#include "panelitemfactory.h"
#include <QDebug>
#include "panelitems/panelitem.h"

PanelItemFactory::PanelItemFactory() {}

PanelItem *PanelItemFactory::itemForName(QString name, ExtPlanePanel *panel, ExtPlaneClient *client) {
    Q_ASSERT(panel != NULL);
    if(classMapping()->contains(name)) {
        const QMetaObject *meta = classMapping()->find(name).value();
        PanelItem *newItem = qobject_cast<PanelItem*> (meta->newInstance(Q_ARG(ExtPlanePanel*,panel),Q_ARG(ExtPlaneClient*, client)));
        if(newItem) {
            return  newItem;
        } else {
            qWarning() << Q_FUNC_INFO << "the panel item " << name << " cannot be created!";
            return  nullptr;
        }
    }
    qWarning() << Q_FUNC_INFO << "the panel item " << name << "is not recognized";
    return nullptr;
}

QStringList PanelItemFactory::itemNames() {
    QStringList items;
    foreach(QString k,classMapping()->keys()) items << k;
    return items;
}

BaseFactoryMapType *PanelItemFactory::classMapping() {
    static BaseFactoryMapType *_classMapping; // Statically allocated, lives for the entire period of the executable
    if(!_classMapping) {
        _classMapping = new BaseFactoryMapType;
    }
    return _classMapping;
}
