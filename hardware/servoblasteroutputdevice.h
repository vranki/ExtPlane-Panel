#ifndef SERVOBLASTEROUTPUTDEVICE_H
#define SERVOBLASTEROUTPUTDEVICE_H

#include <QFile>
#include "outputdevice.h"

class ServoBlasterOutputDevice : public OutputDevice
{
public:
    ServoBlasterOutputDevice(QObject *parent = 0);
    virtual bool init();
public slots:
    void outputValue(double value, int output);
private:
    QFile devFile;
};

#endif // SERVOBLASTEROUTPUTDEVICE_H
