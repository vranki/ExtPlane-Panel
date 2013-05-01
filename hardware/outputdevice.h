#ifndef OUTPUTDEVICE_H
#define OUTPUTDEVICE_H

#include <QObject>
#include <QString>
#include <QSettings>

class OutputDevice : public QObject
{
    Q_OBJECT
public:
    explicit OutputDevice(QObject *parent = 0);
    virtual bool init()=0;
    virtual void setEnabled(bool e);
    virtual void storeSettings(QSettings *panelSettings);
    virtual void loadSettings(QSettings *panelSettings);
    bool isAvailable();
    bool isEnabled();
    virtual int id()=0;
    QString statusString();
signals:
    void deviceEnabled(bool e);
public slots:
    virtual void outputValue(double value, int output)=0;
protected:
    int minValue, maxValue;
    bool enabled, available;
    QString status;
};

#endif // OUTPUTDEVICE_H
