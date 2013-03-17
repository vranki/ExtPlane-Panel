#include "hardwarebinding.h"

HardwareBinding::HardwareBinding(QObject *parent) : QObject(parent) {
    inputMin_ = inputMax_ = outputMin_ = outputMax_ = 0;
}

void HardwareBinding::setName(QString newName) {
    name_ = newName;
}

QString HardwareBinding::name() {
    return name_;
}

void HardwareBinding::storeSettings(QSettings *panelSettings) {
    panelSettings->setValue("name", name());
}

void HardwareBinding::loadSettings(QSettings *panelSettings) {
    setName(panelSettings->value("name").toString());
}
