#include "chromaoutputdevice.h"
#include <QFile>
#include "../util/console.h"

#ifdef TERMIOS_AVAILABLE
    #include <fcntl.h>
#endif

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
    minValue = 0;
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
        safePosition();
    if(enabled) {
        //open the device(com port) to be non-blocking (read will return immediately)
        devFile.open(QIODevice::ReadWrite | QIODevice::Text);

        if(!devFile.isWritable()) {
            DEBUG << "Device file not writable";
            status = "Device file not writable";
            return;
        }

        int fd = devFile.handle();

        // TODO: What about windows? This should be ported to QSerialPort
        #ifdef TERMIOS_AVAILABLE
            fcntl(fd, F_SETFL, 0);
        #endif
            // Servo enable
            QString outstring = "se\n";
            devFile.write(outstring.toUtf8());
            devFile.flush();
    } else {
        if(devFile.isWritable()) {
            // Servo disable
            QString outstring = "sd\n";
            devFile.write(outstring.toUtf8());
            devFile.flush();
        }
        devFile.close();
    }
}

void ChromaOutputDevice::outputValue(double value, int output, int speed) {
    INFO << Q_FUNC_INFO << value << output << enabled;
    if(!enabled)
        return;

    double pos = value;
    pos = qMax(pos, minValue);
    pos = qMin(pos, maxValue);

    // Convert to real range -1000 to 1000
    pos = pos * 2 - 1000;

    if(speed > 0 ) {
        speed = speed - 1;
    } else {
        speed = 3;
    }

    setpos(output, pos, speed);
}

void ChromaOutputDevice::safePosition() {
    foreach(int servo, servopos.keys())
        setpos(servo, (minValue + maxValue) / 2, 4);
}

void ChromaOutputDevice::setpos(int servo, int pos, int speed) {
    if(!devFile.isWritable()) return;
    if(servopos.contains(servo) && servopos[servo] == pos) return;

    QString outstring = QString("s%0 %1 %2\n").arg(servo).arg(pos).arg(speed);
    INFO << servo << pos;
    devFile.write(outstring.toUtf8());
    servopos[servo] = pos;
    devFile.flush();
}
