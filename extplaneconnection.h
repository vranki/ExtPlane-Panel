#ifndef EXTPLANECONNECTION_H
#define EXTPLANECONNECTION_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QTimer>
#include <QStringList>
#include "extplaneclient.h"
#include "clientdataref.h"
#include "clientdatarefprovicer.h"

class ExtPlaneConnection : public QTcpSocket, public ClientDataRefProvicer {
    Q_OBJECT
public:
    explicit ExtPlaneConnection(QObject *parent = 0);
    void registerClient(ExtPlaneClient* client);
    virtual ClientDataRef *subscribeDataRef(QString name, double accuracy=0);
    virtual void unsubscribeDataRef(ClientDataRef *ref);
signals:
    void connectionError(QString text);
public slots:
    void socketConnected();
    void socketError(QAbstractSocket::SocketError err);
    void readClient();
    void connectTo(QHostAddress addr, unsigned int port);
    void tryReconnect();
private:
    void subRef(ClientDataRef *ref);
    void writeLine(QString line);
    QList<ExtPlaneClient*> clients;
    QMap<QString, ClientDataRef*> dataRefs;
    bool server_ok;
    QTimer reconnectTimer;
    QHostAddress _addr;
    unsigned int _port;
};

#endif // EXTPLANECONNECTION_H

