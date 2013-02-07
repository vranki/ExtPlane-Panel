#ifndef SIMULATEDEXTPLANECONNECTION_H
#define SIMULATEDEXTPLANECONNECTION_H

#include "extplaneconnection.h"



class SimulatedExtPlaneConnection : public ExtPlaneConnection {
    Q_OBJECT
public:
    explicit SimulatedExtPlaneConnection(QObject *parent = 0);
public slots:
    virtual void unsubscribeDataRef(ClientDataRef *ref);
    virtual void connectTo(QHostAddress addr, unsigned int port);
    void tickTime(double dt, int total);
private:
    virtual void writeLine(QString line);
    virtual ClientDataRef *createDataRef(QString name, double accuracy=0);
    QList<SimulatedDataRef*> simulatedRefs;
};

#endif // SIMULATEDEXTPLANECONNECTION_H
