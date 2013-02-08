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
#include "simulateddatarefs/simulateddataref.h"

class ExtPlaneConnection : public QTcpSocket, public ClientDataRefProvicer {
    Q_OBJECT
public:
    explicit ExtPlaneConnection(QObject *parent = 0);
    void registerClient(ExtPlaneClient* client);
signals:
    void connectionMessage(QString text);
public slots:
    virtual ClientDataRef *subscribeDataRef(QString name, double accuracy=0);
    virtual void unsubscribeDataRef(ClientDataRef *ref);
    virtual void keyPress(int id);
    virtual void buttonPress(int id);
    virtual void buttonRelease(int id);
    virtual void setValue(QString name, QString value);
    virtual void setValue(ClientDataRef *ref);
    virtual void connectTo(QHostAddress addr, unsigned int port);
    void tickTime(double dt, int total);
private slots:
    void socketConnected();
    void socketError(QAbstractSocket::SocketError err);
    void readClient();
    void tryReconnect();
protected:
    void subRef(ClientDataRef *ref);
    void writeLine(QString line);
    virtual ClientDataRef *createDataRef(QString newName, double accuracy=0);
    QList<ExtPlaneClient*> clients;
    QMap<QString, ClientDataRef*> dataRefs;
    bool server_ok;
    QTimer reconnectTimer;
    QHostAddress _addr;
    unsigned int _port;
    QList<SimulatedDataRef*> simulatedRefs;
    bool enableSimulatedRefs;
};

#endif // EXTPLANECONNECTION_H

