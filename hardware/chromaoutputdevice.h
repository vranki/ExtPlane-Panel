#ifndef CHROMAOUTPUTDEVICE_H
#define CHROMAOUTPUTDEVICE_H

#include <QObject>
#include <QFile>
#include <QMap>
#include "outputdevice.h"
#define CHROMA_ID 3

class ChromaOutputDevice : public OutputDevice
{
    Q_OBJECT
public:
    explicit ChromaOutputDevice(QObject *parent = 0);
    ~ChromaOutputDevice();
    virtual bool init();
    virtual int id();
    virtual void setEnabled(bool e);
    virtual void safePosition();
public slots:
    void outputValue(double value, int output, int speed);
private:
    void setpos(int servo, int pos, int speed);
    QFile devFile;
    QMap<int, int> servopos;
};

#endif // CHROMAOUTPUTDEVICE_H
