#ifndef POLOLUOUTPUTDEVICE_H
#define POLOLUOUTPUTDEVICE_H

#include "outputdevice.h"
#include <QFile>
#include <QMap>
#define POLOLU_ID 2

class PololuOutputDevice : public OutputDevice
{
public:
    PololuOutputDevice(QObject *parent = 0);
    ~PololuOutputDevice();
    virtual bool init();
    virtual int id();
    virtual void setEnabled(bool e);
public slots:
    void outputValue(double value, int output, int speed);
private:
    void setpos(int servo, int pos);
    QFile devFile;
    QMap<int, int> servopos;
};

#endif // POLOLUOUTPUTDEVICE_H
