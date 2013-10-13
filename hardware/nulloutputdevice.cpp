#include "nulloutputdevice.h"
#include <QDebug>

NullOutputDevice::NullOutputDevice(QObject *parent) :
    OutputDevice(parent)
{
}

NullOutputDevice::~NullOutputDevice()
{
}

bool NullOutputDevice::init()
{
    minValue = 0;
    maxValue = 1000;
    status = "Ok";
    available = true;
    return true;
}

int NullOutputDevice::id()
{
    return 0;
}

void NullOutputDevice::setEnabled(bool e)
{
    OutputDevice::setEnabled(e);
}

void NullOutputDevice::outputValue(double value, int output, int speed) {
    qDebug() << Q_FUNC_INFO << value << output << speed;
}

void NullOutputDevice::safePosition() {
}
