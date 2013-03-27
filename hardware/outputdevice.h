#ifndef OUTPUTDEVICE_H
#define OUTPUTDEVICE_H

#include <QObject>
#include <QString>

class OutputDevice : public QObject
{
    Q_OBJECT
public:
    explicit OutputDevice(QObject *parent = 0);
    virtual bool init()=0;
    virtual void setEnabled(bool e);
    bool isAvailable();
    bool isEnabled();
    virtual int id()=0;
    QString statusString();
public slots:
    virtual void outputValue(double value, int output)=0;
protected:
    int minValue, maxValue;
    bool enabled, available;
    QString status;
};

#endif // OUTPUTDEVICE_H
