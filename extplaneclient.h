#ifndef EXTPLANECLIENT_H
#define EXTPLANECLIENT_H

#include <QObject>
#include <QString>
#include <QList>
#include "clientdataref.h"
#include "clientdatarefprovicer.h"

class ExtPlaneClient : public QObject {
    Q_OBJECT
public:
    explicit ExtPlaneClient(QObject *parent, QString name, ClientDataRefProvicer *drp);
    ~ExtPlaneClient();
    void subscribeDataRef(QString name, double accuracy);
    void unsubscribeDataRef(QString name);
signals:
    void refChanged(QString name, double value);
public slots:
    void cdrChanged(ClientDataRef *ref);
private:
    QString _name;
    QList<ClientDataRef*> _dataRefs;
    ClientDataRefProvicer *_connection;
};

#endif // EXTPLANECLIENT_H
