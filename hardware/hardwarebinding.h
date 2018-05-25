#ifndef HARDWAREBINDING_H
#define HARDWAREBINDING_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QList>
#include "../util/valueinterpolator.h"

class ExtPlaneConnection;
class ClientDataRef;

#define OUTPUT_CURVE_SIZE 10

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
    void setDevice(int dev);
    int device();
    void setOutput(int output);
    int output();
    void setInterpolationSpeed(double speed);
    double interpolationSpeed();

    void setInverted(bool inv);
    bool isInverted();
    double invertValueIfNeeded(double val);

    void setSpeed(int spd);
    int speed();

    QList<double> & outputCurve();
    double calculateOutValue(double inValue);
    // Helpers
    double outputMin();
    double outputMax();
    double outputRange();
    double inputRange();
    void resetOutputCurve();
public slots:
    void tickTime(double dt, int total);
    void refValueChanged(QString name, double refValue); // Name is ignored

signals:
    void outputValue(double value, int output, int speed);
    void deviceChanged(HardwareBinding *binding, int device);

private slots:
    void refChanged(ClientDataRef *ref);
    void refDeleted();

private:
    ExtPlaneConnection *connection;
    ClientDataRef *clientDataRef;
    QString name_, refName_;
    int device_, output_;
    double inputMin_, inputMax_;
    double accuracy_, interpolationSpeed_;
    ValueInterpolator interpolator_;
    int speed_; // HW dependent
    bool invert_; // Invert output?
    // [Output min], [8 values between] [Output max], total 10 values
    QList<double> outputCurve_;
};

#endif // HARDWAREBINDING_H
