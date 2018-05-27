#include "hardwarebinding.h"
#include "clientdataref.h"
#include "extplaneconnection.h"
#include "../util/console.h"
#include "../util/interpolation.h"

HardwareBinding::HardwareBinding(QObject *parent, ExtPlaneConnection *conn) : QObject(parent), connection(conn), clientDataRef(0), interpolator_(0,0) {
    inputMin_ = 0;
    inputMax_ = 100;
    accuracy_ = interpolationSpeed_ = 0;
    device_ = 0;
    output_ = 0;
    invert_ = false;
    speed_ = 0;
    for(int i=0; i < OUTPUT_CURVE_SIZE; i ++)
        outputCurve_.append(0);
    outputCurve_.replace(OUTPUT_CURVE_SIZE - 1, 100);
    resetOutputCurve();
}

HardwareBinding::~HardwareBinding() {
    if(clientDataRef) {
        clientDataRef->unsubscribe();
        clientDataRef = 0;
    }
}

void HardwareBinding::tickTime(double dt, int total)
{
    interpolator_.tickTime(dt, total);
}

void HardwareBinding::activate() {
    if(clientDataRef) {
        clientDataRef->unsubscribe();
        clientDataRef = 0;
    }
    if(!refName().isEmpty()) {
        clientDataRef = connection->subscribeDataRef(refName(), accuracy());
        connect(clientDataRef, SIGNAL(changed(ClientDataRef*)), this, SLOT(refChanged(ClientDataRef*)));
        connect(&interpolator_, SIGNAL(interpolatedValueChanged(QString,double)), this, SLOT(refValueChanged(QString,double)));
        connect(clientDataRef, SIGNAL(destroyed()), this, SLOT(refDeleted()));
        interpolator_.setSpeed(interpolationSpeed_);
        interpolator_.setEnabled(interpolationSpeed_ > 0);
    }
}

void HardwareBinding::setName(QString newName) {
    name_ = newName;
}

QString HardwareBinding::name() {
    return name_;
}

void HardwareBinding::setRefName(QString newName)
{
    refName_ = newName;
}

QString HardwareBinding::refName()
{
    return refName_;
}

void HardwareBinding::setAccuracy(double newAcc)
{
    accuracy_ = newAcc;
}

double HardwareBinding::accuracy()
{
    return accuracy_;
}

void HardwareBinding::setInputValues(double min, double max)
{
    inputMin_ = min;
    inputMax_ = max;
}

double HardwareBinding::inputMin()
{
    return inputMin_;
}

double HardwareBinding::inputMax()
{
    return inputMax_;
}

void HardwareBinding::setDevice(int dev)
{
    device_ = dev;
    emit deviceChanged(this, device_);
}

int HardwareBinding::device()
{
    return device_;
}

void HardwareBinding::setOutput(int output)
{
    output_ = output;
}

int HardwareBinding::output()
{
    return output_;
}

void HardwareBinding::setInterpolationSpeed(double speed)
{
    if(speed < 0) speed = 0;
    interpolationSpeed_ = speed;
}

double HardwareBinding::interpolationSpeed()
{
    return interpolationSpeed_;
}

void HardwareBinding::setInverted(bool inv)
{
    invert_ = inv;
}

bool HardwareBinding::isInverted()
{
    return invert_;
}

double HardwareBinding::invertValueIfNeeded(double val)
{
    if(!invert_) return val;
    return outputMax() - (val - outputMin());
}

void HardwareBinding::setSpeed(int spd)
{
    speed_ = spd;
}

int HardwareBinding::speed()
{
    return speed_;
}

QList<double> &HardwareBinding::outputCurve()
{
    return outputCurve_;
}

double HardwareBinding::calculateOutValue(double inValue)
{
    double valueOut = -66666;
    if(inValue <= inputMin()) {
        valueOut = outputMin();
    } else if(inValue >= inputMax()) {
        valueOut = outputMax();
    } else {
        double inputStep = inputRange() / 9;

        int i=0;
        bool valueFound = false;
        while(i < 9 && !valueFound) {
            double in1 = inputMin_ + i * inputStep;
            double in2 = inputMin_ + (i+1) * inputStep;

            if(inValue < in2) {
                double out1 = outputCurve_.at(i);
                double out2 = outputCurve_.at(i+1);
                valueOut = Interpolation::linear(in1, in2, out1, out2, inValue);
                valueFound = true;
                break;
            }
            i++;
        }
        if(!valueFound)
            valueOut = outputMax();
    }
    valueOut = invertValueIfNeeded(valueOut);
    return valueOut;
}

double HardwareBinding::outputMin()
{
    return outputCurve_.first();
}

double HardwareBinding::outputMax()
{
    return outputCurve().last();
}

double HardwareBinding::outputRange() {
    return outputMax() - outputMin();
}

double HardwareBinding::inputRange()
{
    return inputMax() - inputMin();
}

void HardwareBinding::resetOutputCurve()
{
    double d = outputRange() / 9;
    double v = outputMin();
    for(int i=1; i<OUTPUT_CURVE_SIZE - 1; i++) {
        v += d;
        outputCurve_.replace(i, v);
    }
}

void HardwareBinding::refValueChanged(QString, double refValue)
{
    INFO << Q_FUNC_INFO << refValue << calculateOutValue(refValue) << output() << speed_;
    emit outputValue(calculateOutValue(refValue), output_, speed_);
}

void HardwareBinding::refDeleted() {
    clientDataRef = 0;
}

void HardwareBinding::storeSettings(QSettings *panelSettings) {
    panelSettings->setValue("name", name());
    panelSettings->setValue("refname", refName());
    panelSettings->setValue("accuracy", accuracy());
    panelSettings->setValue("inputmin", inputMin());
    panelSettings->setValue("inputmax", inputMax());
    panelSettings->setValue("device", device());
    panelSettings->setValue("output", output());
    panelSettings->setValue("speed", speed());
    panelSettings->setValue("invert", isInverted());
    panelSettings->setValue("interpolationspeed", interpolationSpeed());
    panelSettings->beginWriteArray("outputcurve", outputCurve_.size());
    for (int i = 0; i < outputCurve_.size(); ++i) {
        panelSettings->setArrayIndex(i);
        panelSettings->setValue("curvevalue", outputCurve_.at(i));
    }
    panelSettings->endArray();
}

void HardwareBinding::loadSettings(QSettings *panelSettings) {
    setName(panelSettings->value("name").toString());
    setRefName(panelSettings->value("refname").toString());
    setAccuracy(panelSettings->value("accuracy").toDouble());
    setInputValues(panelSettings->value("inputmin").toDouble(), panelSettings->value("inputmax").toDouble());
    setDevice(panelSettings->value("device").toInt());
    setOutput(panelSettings->value("output").toInt());
    setSpeed(panelSettings->value("speed").toInt());
    setInverted(panelSettings->value("invert").toBool());
    setInterpolationSpeed(panelSettings->value("interpolationspeed").toDouble());
    int size = panelSettings->beginReadArray("outputcurve");
    if(size == OUTPUT_CURVE_SIZE) {
        outputCurve_.clear();
        outputCurve_.reserve(size);
        for (int i = 0; i < size; ++i) {
            panelSettings->setArrayIndex(i);
            outputCurve_.append(panelSettings->value("curvevalue").toDouble());
        }
    }
    panelSettings->endArray();
}


void HardwareBinding::refChanged(ClientDataRef *ref)
{
    bool ok = true;
    double value = ref->value().toDouble(&ok);
    if(ok) {
        interpolator_.valueChanged(ref->name(), value);
    } else {
        DEBUG << Q_FUNC_INFO << "unable to convert ref value " << ref->value() << " to double";
    }
}
