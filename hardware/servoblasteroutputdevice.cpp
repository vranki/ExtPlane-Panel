#include "servoblasteroutputdevice.h"
#include "../util/console.h"

ServoBlasterOutputDevice::ServoBlasterOutputDevice(QObject *parent) : OutputDevice(parent)
{
}

bool ServoBlasterOutputDevice::init()
{
    // @todo configurable
    int min = 50;
    int max = 220;

    if(min <= 0 || min >= max) {
        DEBUG << "Invalid range " << minValue << maxValue;
        return false;
    }
    devFile.setFileName("/dev/servoblaster");
    if(!devFile.exists()) {
        status = "Servoblaster device file doesn't exist";
        return false;
    }
    if(devFile.isWritable()) {
        status = "Servoblaster device file not writable";
        return false;
    }
    minValue = min;
    maxValue = max;
    status = "Ok";
    available = true;
    return true;
}

int ServoBlasterOutputDevice::id()
{
    return SERVOBLASTER_ID;
}

void ServoBlasterOutputDevice::outputValue(double value, int output, int speed)
{
    Q_UNUSED(speed);
    if(!enabled)
        return;

    double pos = value;
    pos = qMax(pos, minValue);
    pos = qMin(pos, maxValue);

    DEBUG << output << pos;

    QString outputString = QString("%1=%2\n").arg(output).arg(pos);

    if(devFile.open(QIODevice::WriteOnly)) {
        devFile.write(outputString.toUtf8());
        devFile.close();
    } else {
        status = "Unable to open device file for write!";
    }
}
