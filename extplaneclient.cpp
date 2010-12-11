#include "extplaneclient.h"

ExtPlaneClient::ExtPlaneClient(QObject *parent, QString name, ClientDataRefProvicer *drp) :
    QObject(parent), _name(name), _connection(drp)
{
}
ExtPlaneClient::~ExtPlaneClient() {
    foreach(ClientDataRef *ref, _dataRefs) {
        _connection->unsubscribeDataRef(ref);
    }
    _dataRefs.clear();
}

void ExtPlaneClient::subscribeDataRef(QString name, double accuracy) {
    ClientDataRef *ref = _connection->subscribeDataRef(name, accuracy);
    connect(ref, SIGNAL(changed(ClientDataRef*)), this, SLOT(cdrChanged(ClientDataRef*)));
    _dataRefs.append(ref);
}

void ExtPlaneClient::cdrChanged(ClientDataRef *ref) {
    emit refChanged(ref->name(), ref->valueString().toDouble());
}

void ExtPlaneClient::unsubscribeDataRef(QString name) {
    foreach(ClientDataRef *ref, _dataRefs) {
        if(ref->name() == name) {
//            disconnect(ref, 0, this, 0);
            _connection->unsubscribeDataRef(ref);
            _dataRefs.removeOne(ref);
            return;
        }
    }
}
