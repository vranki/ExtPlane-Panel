#include "extplaneclient.h"
#include <QDebug>

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
    double value;
    bool ok;
    
    value = ref->valueString().toDouble(&ok);
    if (ok){
        emit refChanged(ref->name(), value);
    } else {
        qDebug() << Q_FUNC_INFO << "unable to convert to double " << ref->valueString();
        emit refChanged(ref->name(), ref->valueString());
    }
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
