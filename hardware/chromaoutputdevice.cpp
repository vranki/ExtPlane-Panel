#include "chromaoutputdevice.h"
#include <QFile>
#include <fcntl.h>
#include "../util/console.h"

ChromaOutputDevice::ChromaOutputDevice(QObject *parent) : OutputDevice(parent)
{
}

ChromaOutputDevice::~ChromaOutputDevice()
{
    if(enabled)
        setEnabled(false);

}

bool ChromaOutputDevice::init()
{
    minValue = -1000;
    maxValue = 1000;

    devFile.setFileName("/dev/ttyUSB0");

    if(!devFile.exists()) {
        status = "Device file doesn't exist";
        return false;
    }

    status = "Ok";
    available = true;
    return true;
}

int ChromaOutputDevice::id()
{
    return CHROMA_ID;
}

void ChromaOutputDevice::setEnabled(bool e)
{
    OutputDevice::setEnabled(e);
    if(devFile.isOpen())
        foreach(int servo, servopos.keys())
            setpos(servo,0);
    if(enabled) {
        //open the device(com port) to be non-blocking (read will return immediately)
        devFile.open(QIODevice::ReadWrite | QIODevice::Text);

        if(!devFile.isWritable()) {
            DEBUG << "Device file not writable";
            status = "Device file not writable";
            return;
        }

        int fd = devFile.handle();

        fcntl(fd, F_SETFL, 0);
    } else {
        devFile.close();
    }
}

void ChromaOutputDevice::outputValue(double value, int output)
{
    if(!enabled)
        return;

    int pos = value;
    pos = qMax(pos, minValue);
    pos = qMin(pos, maxValue);

    setpos(output, pos);
}

void ChromaOutputDevice::setpos(int servo, int pos) {
    if(!devFile.isWritable()) return;
    if(servopos.contains(servo) && servopos[servo] == pos) return;
//    int speed = 4;
//    QString outstring = QString("s%0 %1 %2\n").arg(servo).arg(pos).arg(speed);
    QString outstring = QString("s%0 %1\n").arg(servo).arg(pos);
    // DEBUG << servo << pos;
    devFile.write(outstring.toUtf8());
    servopos[servo] = pos;
    devFile.flush();
}
