#ifndef HARDWAREBINDING_H
#define HARDWAREBINDING_H

#include <QObject>
#include <QString>
#include <QSettings>

class ExtPlaneConnection;
class ClientDataRef;

class HardwareBinding : public QObject
{
    Q_OBJECT
public:
    explicit HardwareBinding(QObject *parent, ExtPlaneConnection *conn);
    ~HardwareBinding();
    void activate();
    void storeSettings(QSettings *panelSettings);
    void loadSettings(QSettings *panelSettings);
    void setName(QString newName);
    QString name();
    void setRefName(QString newName);
    QString refName();
    void setAccuracy(double newAcc);
    double accuracy();
    void setInputValues(double min, double max);
    double inputMin();
    double inputMax();
    void setOutputValues(double min, double max);
    double outputMin();
    double outputMax();
signals:
    
public slots:
private slots:
    void refChanged(ClientDataRef *ref);
    void refDeleted();
private:
    ExtPlaneConnection *connection;
    ClientDataRef *clientDataRef;
    QString name_, refName_;
    int device_, output_;
    double inputMin_, inputMax_, outputMin_, outputMax_;
    double accuracy_;
};

#endif // HARDWAREBINDING_H
