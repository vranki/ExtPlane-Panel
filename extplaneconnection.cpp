#include "extplaneconnection.h"

ExtPlaneConnection::ExtPlaneConnection(QObject *parent) : QTcpSocket(parent) {
    connect(this, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(&reconnectTimer, SIGNAL(timeout()), this, SLOT(tryReconnect()));
    server_ok = false;
    enableSimulatedRefs = false;
}

void ExtPlaneConnection::connectTo(QHostAddress addr, unsigned int port) {
    close();
    _addr = addr;
    _port = port;
    abort();
    tryReconnect();
}

void ExtPlaneConnection::tryReconnect() {
    // qDebug() << Q_FUNC_INFO << _addr.toString() << _port;
    reconnectTimer.stop();
    connectToHost(_addr, _port);
}

void ExtPlaneConnection::socketConnected() {
    qDebug() << Q_FUNC_INFO ;
    emit connectionError("Connected to ExtPlane, waiting for handshake");
    reconnectTimer.stop();
}

void ExtPlaneConnection::socketError(QAbstractSocket::SocketError err) {
    // qDebug() << Q_FUNC_INFO << err;
    server_ok = false;
    emit connectionError(errorString() + " : " + peerName() + ":" + QString::number(peerPort()));
    reconnectTimer.setInterval(5000);
    reconnectTimer.start();
}

void ExtPlaneConnection::registerClient(ExtPlaneClient* client) {
    clients.append(client);
}

ClientDataRef *ExtPlaneConnection::subscribeDataRef(QString name, double accuracy) {
    ClientDataRef *ref = dataRefs.value(name);
    if(ref){
        qDebug() << Q_FUNC_INFO << "Ref already subscribed";
        ref->setSubscribers(ref->subscribers()+1);
        if(accuracy > ref->accuracy()) {
            // @todo update accuracy
        }
    } else {
        ref = createDataRef(name, accuracy);

        dataRefs[ref->name()] = ref;
        ref->setSubscribers(1);
        connect(ref, SIGNAL(unsubscribed(ClientDataRef*)), this, SLOT(unsubscribeDataRef(ClientDataRef*)));
        connect(ref, SIGNAL(valueSet(ClientDataRef*)), this, SLOT(setValue(ClientDataRef*)));
        if(server_ok)
            subRef(ref);
    }

    qDebug() << Q_FUNC_INFO << name << ref->subscribers() << server_ok;
    return ref;
}


ClientDataRef *ExtPlaneConnection::createDataRef(QString name, double accuracy) {
    return new ClientDataRef(this, name, accuracy);
}

void ExtPlaneConnection::unsubscribeDataRef(ClientDataRef *ref) {
    qDebug() << Q_FUNC_INFO << ref << ref->name() << ref->subscribers();
    ref->setSubscribers(ref->subscribers() - 1);
    if(ref->subscribers() > 0) return;
    qDebug() << Q_FUNC_INFO << "Ref not subscribed by anyone anymore";
    dataRefs.remove(ref->name());
    bool isSimulated = false;
    foreach(SimulatedDataRef *simRef, simulatedRefs) {
        if(simRef->clientRef()==ref) {
            simRef->deleteLater();
            simulatedRefs.removeOne(simRef);
            isSimulated = true;
        }
    }
    disconnect(ref, 0, this, 0);
    if(!isSimulated) {
        if(server_ok)
            writeLine("unsub " + ref->name());
        qDebug() << Q_FUNC_INFO << "Deleting ref " << ref->name() << ref;
        ref->deleteLater();
    }
    foreach(ClientDataRef *ref, dataRefs) {
        qDebug() << "refs now:" << ref->name();
    }
}

void ExtPlaneConnection::readClient() {
    while(canReadLine()) {
        QByteArray lineBA = readLine();
        QString line = QString(lineBA).trimmed();
        qDebug() << Q_FUNC_INFO << "Server says: " << line;
        if(!server_ok) {
            if(line=="EXTPLANE 1") {
                server_ok = true;
                emit connectionError("");
                // Sub all refs
                foreach(ClientDataRef *ref, dataRefs)
                    subRef(ref);
            }
            return;
        } else {
            QStringList cmd = line.split(" ", QString::SkipEmptyParts);
            if(cmd.size()==3) {
                if(cmd.value(0)=="uf") {
                    ClientDataRef *ref = dataRefs.value(cmd.value(1));
                    if(ref) {
                        ref->updateValue(cmd.value(2));
                    } else {
                        qDebug() << Q_FUNC_INFO << "ref not subscribed " << cmd.value(2);
                    }
                }
            }
        }
    }
}

void ExtPlaneConnection::subRef(ClientDataRef *ref) {
    qDebug() << Q_FUNC_INFO;
    Q_ASSERT(server_ok);
    QString subLine = "sub " + ref->name();
    if(ref->accuracy() != 0) {
        subLine += " " + QString::number(ref->accuracy());
    }
    writeLine(subLine);
}

void ExtPlaneConnection::writeLine(QString line) {
    if(!server_ok) {
        qDebug() << Q_FUNC_INFO << "Connection not ok! Tried to write" << line;
        return;
    }
    line +="\n";
    write(line.toUtf8());
    qDebug() << Q_FUNC_INFO << line;
}

void ExtPlaneConnection::keyPress(int id) {
    QString line = "key " + QString::number(id);
    writeLine(line);
}

void ExtPlaneConnection::buttonPress(int id) {
    QString line = "but " + QString::number(id);
    writeLine(line);
}

void ExtPlaneConnection::buttonRelease(int id) {
    QString line = "rel " + QString::number(id);
    writeLine(line);
}

void ExtPlaneConnection::setValue(QString name, QString value) {
    QString line = "set " + name + " " + value;
    writeLine(line);
}
void ExtPlaneConnection::setValue(ClientDataRef *ref) {
    setValue(ref->name(), ref->valueString());
}

void ExtPlaneConnection::tickTime(double dt, int total) {
    foreach(SimulatedDataRef *dr, simulatedRefs)
        dr->tickTime(dt, total);
}
