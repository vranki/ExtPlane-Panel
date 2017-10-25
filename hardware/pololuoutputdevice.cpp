#include "pololuoutputdevice.h"
#include <QFile>
#include "../util/console.h"

#ifdef TERMIOS_AVAILABLE
    #include <termios.h>
#endif

PololuOutputDevice::PololuOutputDevice(QObject *parent) : OutputDevice(parent)
{
}

PololuOutputDevice::~PololuOutputDevice()
{
    if(enabled)
        setEnabled(false);

}

bool PololuOutputDevice::init()
{
    minValue = 0;
    maxValue = 255;

    devFile.setFileName("/dev/ttyUSB0");

    if(!devFile.exists()) {
        status = "Device file doesn't exist";
        return false;
    }
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
    if(enabled) {
        //open the device(com port) to be non-blocking (read will return immediately)
        devFile.open(QIODevice::WriteOnly);

        if(!devFile.isWritable()) {
            DEBUG << "Device file not writable";
            status = "Device file not writable";
            return;
        }

        int fd = devFile.handle();

        // Set new port settings for canonical input processing
        // TODO: What about windows? This should be ported to QSerialPort
        #ifdef TERMIOS_AVAILABLE
            struct termios newtio;
            newtio.c_cflag = B9600 | CRTSCTS | CS8 | 1 | 0 | 0 | CLOCAL/* | CREAD*/;
            newtio.c_iflag = IGNPAR;
            newtio.c_oflag = 0;
            newtio.c_lflag = 0;       //ICANON;
            newtio.c_cc[VMIN]=1;
            newtio.c_cc[VTIME]=0;
            tcflush(fd, TCIFLUSH);
            tcsetattr(fd,TCSANOW, &newtio);
        #endif

        setpos(0, 127);
        setpos(1, 127);

    } else {
        // Reset all to 127
        foreach(int servo, servopos.keys())
            setpos(servo,127);
        devFile.close();
    }
}

void PololuOutputDevice::outputValue(double value, int output, int speed)
{
    Q_UNUSED(speed);
    if(!enabled)
        return;

    double pos = value;
    pos = qMax(pos, minValue);
    pos = qMin(pos, maxValue);

    setpos(output, pos);
}

void PololuOutputDevice::setpos(int servo, int pos) {
    if(!devFile.isWritable()) return;
    if(servopos.contains(servo) && servopos[servo] == pos) return;
    // DEBUG << servo << pos;
    QByteArray data(3, 0);
    data[0] = 0xff;
    data[1] = servo + 8;
    data[2] = pos;
    devFile.write(data);
    servopos[servo] = pos;
    devFile.flush();
}
