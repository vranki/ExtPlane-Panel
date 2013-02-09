#include "simulatedextplaneconnection.h"




SimulatedExtPlaneConnection::SimulatedExtPlaneConnection(QObject *parent) : ExtPlaneConnection(parent) {
    enableSimulatedRefs = true;
}

void SimulatedExtPlaneConnection::connectTo(QHostAddress addr, unsigned int port) {
    _addr = addr;
    _port = port;
    server_ok = true;
    emit connectionMessage("Connected to ExtPlane (simulated)");
}

ClientDataRef *SimulatedExtPlaneConnection::createDataRef(QString name, double accuracy) {
    SimulatedDataRef *simRef = 0;
    if(name=="sim/cockpit2/gauges/indicators/airspeed_kts_pilot") {
	simRef = new SimulatedDataRef(this, 0, 200, name);
    } else if(name=="sim/flightmodel/position/vh_ind") {
	simRef = new SimulatedDataRef(this, -5, 5, name);
    } else if(name=="sim/cockpit2/gauges/indicators/total_energy_fpm") {
	simRef = new SimulatedDataRef(this, -500, 500, name);
    } else if(name=="sim/flightmodel/position/latitude") {
	simRef = new SimulatedDataRef(this, 47.3, 47.4, name);
    }  else if(name=="sim/flightmodel/position/longitude") {
	simRef = new SimulatedDataRef(this, 8.5, 8.6, name);
    } else {
        // Fallback
        qWarning() << Q_FUNC_INFO << "the dataref " << name << "is not supported by simulation";
	simRef = new SimulatedDataRef(this, 0, 0, name);
    }
    if(simRef) {
	simulatedRefs.append(simRef);
	return simRef->clientRef();
    }
    return 0;
}

void SimulatedExtPlaneConnection::unsubscribeDataRef(ClientDataRef *ref) {
    qDebug() << Q_FUNC_INFO << ref << ref->name() << ref->subscribers();
    ref->setSubscribers(ref->subscribers() - 1);
    if(ref->subscribers() > 0) return;
    qDebug() << Q_FUNC_INFO << "Ref not subscribed by anyone anymore";
    dataRefs.remove(ref->name());

    foreach(SimulatedDataRef *simRef, simulatedRefs) {
	if(simRef->clientRef()==ref) {
	    simRef->deleteLater();
            simulatedRefs.removeOne(simRef);
	}
    }
    disconnect(ref, 0, this, 0);

    foreach(ClientDataRef *ref, dataRefs) {
	qDebug() << "refs now:" << ref->name();
    }
}




void SimulatedExtPlaneConnection::writeLine(QString line) {
    qDebug() << Q_FUNC_INFO << line << "(simulated)";
}



void SimulatedExtPlaneConnection::tickTime(double dt, int total) {
    foreach(SimulatedDataRef *dr, simulatedRefs)
	dr->tickTime(dt, total);
}
