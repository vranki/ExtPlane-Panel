#include "outputdevice.h"

OutputDevice::OutputDevice(QObject *parent) :
    QObject(parent), enabled(false), available(false)
{
}

void OutputDevice::setEnabled(bool e) {
    enabled = e;
}

bool OutputDevice::isAvailable()
{
    return available;
}

bool OutputDevice::isEnabled()
{
    return enabled;
}

QString OutputDevice::statusString()
{
    return status;
}
