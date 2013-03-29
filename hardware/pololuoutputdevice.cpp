#include "pololuoutputdevice.h"
#include <QFile>
#include <QDebug>
#include <termios.h>

PololuOutputDevice::PololuOutputDevice(QObject *parent) : OutputDevice(parent)
{
}

PololuOutputDevice::~PololuOutputDevice()
{
    if(enabled)
        setEnabled(false);
    devFile.close();
}

bool PololuOutputDevice::init()
{
    // @todo configurable
    int min = 0;
    int max = 255;

    if(min < 0 || min >= max) {
        qDebug() << Q_FUNC_INFO << "Invalid range " << min << max;
        return false;
    }
    devFile.setFileName("/dev/ttyUSB0");
    if(!devFile.exists()) {
        status = "Device file doesn't exist";
        return false;
    }
    if(devFile.isWritable()) {
        status = "Device file not writable";
        return false;
    }

    struct termios newtio;

    //open the device(com port) to be non-blocking (read will return immediately)
    devFile.open(QIODevice::WriteOnly);
    int fd = devFile.handle();
    //    (())                fd = open(devicename, O_WRONLY | O_NOCTTY);
    // set new port settings for canonical input processing
    newtio.c_cflag = B9600 | CRTSCTS | CS8 | 1 | 0 | 0 | CLOCAL/* | CREAD*/;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;       //ICANON;
    newtio.c_cc[VMIN]=1;
    newtio.c_cc[VTIME]=0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW, &newtio);
    setpos(0, 127);
    setpos(1, 127);

    minValue = min;
    maxValue = max;
    status = "Ok";
    available = true;
    return true;
}

int PololuOutputDevice::id()
{
    return POLOLU_ID;
}

void PololuOutputDevice::setEnabled(bool e)
{
    OutputDevice::setEnabled(e);
    if(!enabled) {
        // Reset all to 127
        foreach(int servo, servopos.keys())
            setpos(servo,127);
    }
}

void PololuOutputDevice::outputValue(double value, int output)
{
    if(!enabled)
        return;

    int pos = value;
    pos = qMax(pos, minValue);
    pos = qMin(pos, maxValue);

    setpos(output, pos);
}

void PololuOutputDevice::setpos(int servo, int pos) {
    if(!devFile.isWritable()) return;
    if(servopos.contains(servo) && servopos[servo] == pos) return;
    qDebug() << Q_FUNC_INFO << servo << pos;
    char foo[3];
    foo[0] = 0xff;
    foo[1] = servo + 8;
    foo[2] = pos;
    devFile.write(foo, 3);
    servopos[servo] = pos;
    devFile.flush();
}
