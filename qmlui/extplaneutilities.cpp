#include "extplaneutilities.h"
#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

ExtplaneUtilities::ExtplaneUtilities(QObject *parent) : QObject(parent)
{

}

void ExtplaneUtilities::copySettings(QObject *from, QObject *to)
{
    qDebug() << Q_FUNC_INFO << from << to;
    qDebug() << from->property("children");
    auto mo = from->metaObject();
    for(int i=0;i< mo->propertyCount(); i++) {
        qDebug() << mo->property(i).name();
    }
}
