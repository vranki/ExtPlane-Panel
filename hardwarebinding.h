#ifndef HARDWAREBINDING_H
#define HARDWAREBINDING_H

#include <QObject>
#include <QString>
#include <QSettings>
#include "clientdataref.h"

class HardwareBinding : public QObject
{
    Q_OBJECT
public:
    explicit HardwareBinding(QObject *parent = 0);
    void setName(QString newName);
    QString name();
    void storeSettings(QSettings *panelSettings);
    void loadSettings(QSettings *panelSettings);
signals:
    
public slots:

private:
    ClientDataRef *clientDataRef;
    QString name_;
    int device_, output_;
    double inputMin_, inputMax_, outputMin_, outputMax_;
};

#endif // HARDWAREBINDING_H
