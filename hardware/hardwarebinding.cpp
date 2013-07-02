#include "hardwarebinding.h"
#include "clientdataref.h"
#include "extplaneconnection.h"
#include "../util/console.h"

HardwareBinding::HardwareBinding(QObject *parent, ExtPlaneConnection *conn) : QObject(parent), connection(conn), clientDataRef(0) {
    inputMin_ = inputMax_ = outputMin_ = outputMax_ = 0;
    accuracy_ = 0;
}

HardwareBinding::~HardwareBinding() {
    if(clientDataRef) {
        clientDataRef->unsubscribe();
        clientDataRef = 0;
    }
}

void HardwareBinding::activate() {
    if(clientDataRef) {
        clientDataRef->unsubscribe();
        clientDataRef = 0;
    }
    if(!refName().isEmpty()) {
        clientDataRef = connection->subscribeDataRef(refName(), accuracy());
        connect(clientDataRef, SIGNAL(changed(ClientDataRef*)), this, SLOT(refChanged(ClientDataRef*)));
        connect(clientDataRef, SIGNAL(destroyed()), this, SLOT(refDeleted()));
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

void HardwareBinding::setOutputValues(double min, double max)
{
    outputMin_ = min;
    outputMax_ = max;
}

double HardwareBinding::outputMin()
{
    return outputMin_;
}

double HardwareBinding::outputMax()
{
    return outputMax_;
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

void HardwareBinding::refChanged(ClientDataRef *ref) {
    bool ok;
    double refValue = ref->valueString().toDouble(&ok);
    if(!ok) {
        DEBUG << "Can't convert value " << ref->valueString() << " to double.";
        return;
    }
    bool invert = inputMax_ < inputMin_;
    // Limit value to input limits
    if(!invert) {
        refValue = qMin(refValue, inputMax_);
        refValue = qMax(refValue, inputMin_);
    } else {
        refValue = qMin(refValue, inputMin_);
        refValue = qMax(refValue, inputMax_);
    }
    double valueScaled = (refValue - inputMin_) / (inputMax_ - inputMin_); // [0-1]

    double valueOut = outputMin_ + (outputMax_ - outputMin_) * valueScaled;

    // Out value should be within limits
    valueOut = qMax(valueOut, outputMin_);
    valueOut = qMin(valueOut, outputMax_);

    emit outputValue(valueOut, output_);
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
    panelSettings->setValue("outputmin", outputMin());
    panelSettings->setValue("outputmax", outputMax());
    panelSettings->setValue("device", device());
    panelSettings->setValue("output", output());
}

void HardwareBinding::loadSettings(QSettings *panelSettings) {
    setName(panelSettings->value("name").toString());
    setRefName(panelSettings->value("refname").toString());
    setAccuracy(panelSettings->value("accuracy").toDouble());
    setInputValues(panelSettings->value("inputmin").toDouble(), panelSettings->value("inputmax").toDouble());
    setOutputValues(panelSettings->value("outputmin").toDouble(), panelSettings->value("outputmax").toDouble());
    setDevice(panelSettings->value("device").toInt());
    setOutput(panelSettings->value("output").toInt());
}
