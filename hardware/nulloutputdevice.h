#ifndef NULLOUTPUTDEVICE_H
#define NULLOUTPUTDEVICE_H

#include "outputdevice.h"

class NullOutputDevice : public OutputDevice
{
    Q_OBJECT
public:
    explicit NullOutputDevice(QObject *parent = 0);
    ~NullOutputDevice();
    virtual bool init();
    virtual int id();
    virtual void setEnabled(bool e);
public slots:
    void outputValue(double value, int output, int speed);
private:
    void safePosition();
};

#endif // NULLOUTPUTDEVICE_H
