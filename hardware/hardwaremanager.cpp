#include "hardwaremanager.h"
#include "hardwarebinding.h"
#include "servoblasteroutputdevice.h"

HardwareManager::HardwareManager(QObject *parent, ExtPlaneConnection *conn) : QObject(parent), connection_(conn) {
    ServoBlasterOutputDevice *sbo = new ServoBlasterOutputDevice(this);
    outputDevices.insert(sbo->id(), sbo);
}

QList<HardwareBinding *> &HardwareManager::bindings() {
    return hwBindings;
}

QMap<int, OutputDevice *> &HardwareManager::devices()
{
    return outputDevices;
}

ExtPlaneConnection *HardwareManager::connection()
{
    return connection_;
}

void HardwareManager::addBinding(HardwareBinding *binding) {
    Q_ASSERT(binding->parent()==this);
    hwBindings.append(binding);
    deviceChanged(binding, binding->device());
}

void HardwareManager::deleteBinding(HardwareBinding *binding)
{
    disconnect(binding, 0, outputDevices.value(binding->device()), 0);
    hwBindings.removeOne(binding);
    binding->deleteLater();
}

void HardwareManager::saveSettings(QSettings *panelSettings) {
    panelSettings->group().clear();
    panelSettings->setValue("bindingCount", bindings().count());
    int bn = 0;
    foreach(HardwareBinding *binding, bindings()) {
        panelSettings->beginGroup("binding-" + QString::number(bn));
        binding->storeSettings(panelSettings);
        panelSettings->endGroup();
        bn++;
    }
}

void HardwareManager::loadSettings(QSettings *panelSettings) {
    // @todo load settings for devices
    emit deviceAvailable(0, outputDevices.value(0)->init());

    int bn = panelSettings->value("bindingCount").toInt();
    for(int i=0;i<bn;i++) {
        panelSettings->beginGroup("binding-" + QString::number(i));
        HardwareBinding *binding = new HardwareBinding(this, connection());
        binding->loadSettings(panelSettings);
        panelSettings->endGroup();
        hwBindings.append(binding);
        binding->activate();
    }
}

void HardwareManager::deviceChanged(HardwareBinding *binding, int device)
{
    connect(binding, SIGNAL(outputValue(double,int)), outputDevices.value(binding->device()), SLOT(outputValue(double,int)));
}

void HardwareManager::deviceEnabled(int dev, bool enable)
{
    outputDevices.value(dev)->setEnabled(enable);
}
