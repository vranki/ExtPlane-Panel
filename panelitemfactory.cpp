#include "panelitemfactory.h"

#include <QDebug>

PanelItemFactory::PanelItemFactory() {
    connection = NULL;
}

PanelItemFactory::PanelItemFactory(ExtPlaneConnection *conn) {
    connection = conn;
}

PanelItem *PanelItemFactory::itemForName(QString name, QObject *parentObject) {
    if(classMapping()->contains(name)) {
        const QMetaObject *meta = classMapping()->find(name).value();
        return (PanelItem*) (meta->newInstance(Q_ARG(QObject*,parentObject),Q_ARG(ExtPlaneConnection*,connection)));
    } else {
        qWarning() << Q_FUNC_INFO << "the panel item " << name << "is not recognized";
        return NULL;
    }
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
