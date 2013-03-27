#ifndef OUTPUTDEVICE_H
#define OUTPUTDEVICE_H

#include <QObject>

class OutputDevice : public QObject
{
    Q_OBJECT
public:
    explicit OutputDevice(QObject *parent = 0);
    virtual bool init()=0;
public slots:
    virtual void outputValue(double value, int output)=0;
protected:
    int minValue, maxValue;
};

#endif // OUTPUTDEVICE_H
