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

ClientDataRef* ExtPlaneClient::subscribeDataRef(QString name, double accuracy) {
    ClientDataRef *ref = _connection->subscribeDataRef(name, accuracy);
    connect(ref, SIGNAL(changed(ClientDataRef*)), this, SLOT(cdrChanged(ClientDataRef*)));
    _dataRefs.append(ref);
    return ref;
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

void ExtPlaneClient::keyPress(int id) {
    _connection->keyPress(id);
}

void ExtPlaneClient::buttonPress(int id) {
    _heldButtons.insert(id);
    _connection->buttonPress(id);
}

void ExtPlaneClient::buttonRelease(int id) {
    if(!_heldButtons.contains(id)) return;
    _heldButtons.remove(id);
    _connection->buttonRelease(id);
}

void ExtPlaneClient::valueSet(ClientDataRef *ref) {
    _connection->setValue(ref->name(), ref->valueString());
}

void ExtPlaneClient::unsubscribed(ClientDataRef *ref) {
    unsubscribeDataRef(ref->name());
}
