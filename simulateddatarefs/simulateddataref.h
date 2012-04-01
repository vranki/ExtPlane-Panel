#ifndef SIMULATEDDATAREF_H
#define SIMULATEDDATAREF_H

#include <QObject>
#include <QTimer>
#include "../clientdataref.h"

class SimulatedDataRef : public QObject
{
    Q_OBJECT
public:
    explicit SimulatedDataRef(QObject *parent, double minV, double maxV, QString refName);
    ClientDataRef *clientRef();
signals:
    
public slots:
    virtual void tickTime(double dt, int total);
private slots:
    void changeTimeout();
private:
    double minValue, maxValue, change, currentValue;
    QTimer changeTimer;
    ClientDataRef myClientRef;
};

#endif // SIMULATEDDATAREF_H
