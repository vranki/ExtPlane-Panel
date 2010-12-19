#ifndef EXTPLANECLIENT_H
#define EXTPLANECLIENT_H

#include <QObject>
#include <QString>
#include <QList>
#include <QSet>
#include "clientdataref.h"
#include "clientdatarefprovicer.h"

class ExtPlaneClient : public QObject {
    Q_OBJECT
public:
    explicit ExtPlaneClient(QObject *parent, QString name, ClientDataRefProvicer *drp);
    ~ExtPlaneClient();
    void subscribeDataRef(QString name, double accuracy=0);
    void unsubscribeDataRef(QString name);
    void keyPress(int id);
    void buttonPress(int id);
    void buttonRelease(int id);
signals:
    void refChanged(QString name, double value);
public slots:
    void cdrChanged(ClientDataRef *ref);
private:
    QString _name;
    QList<ClientDataRef*> _dataRefs;
    QSet<int> _heldButtons;
    ClientDataRefProvicer *_connection;
};

#endif // EXTPLANECLIENT_H
