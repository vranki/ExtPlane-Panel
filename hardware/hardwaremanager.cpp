#include "hardwaremanager.h"
#include <QDebug>
#include "hardwarebinding.h"
#include "servoblasteroutputdevice.h"
#include "pololuoutputdevice.h"

HardwareManager::HardwareManager(QObject *parent, ExtPlaneConnection *conn) : QObject(parent), connection_(conn) {
    ServoBlasterOutputDevice *sbo = new ServoBlasterOutputDevice(this);
    outputDevices.insert(sbo->id(), sbo);
    PololuOutputDevice *pod = new PololuOutputDevice(this);
    outputDevices.insert(pod->id(), pod);
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
    connect(binding, SIGNAL(deviceChanged(HardwareBinding*,int)), this, SLOT(deviceChanged(HardwareBinding*,int)));
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
    foreach(OutputDevice *device, outputDevices)
        emit deviceAvailable(device->id(), device->init());

    int bn = panelSettings->value("bindingCount").toInt();
    for(int i=0;i<bn;i++) {
        panelSettings->beginGroup("binding-" + QString::number(i));
        HardwareBinding *binding = new HardwareBinding(this, connection());
        binding->loadSettings(panelSettings);
        panelSettings->endGroup();
        addBinding(binding);
//        hwBindings.append(binding);
        binding->activate();
    }
}

void HardwareManager::deviceChanged(HardwareBinding *binding, int device)
{
    Q_ASSERT(outputDevices.contains(binding->device()));
    connect(binding, SIGNAL(outputValue(double,int)), outputDevices.value(binding->device()), SLOT(outputValue(double,int)));
    qDebug() << Q_FUNC_INFO << "connected output of binding" << binding->name() << "to device " << outputDevices.value(binding->device())->id();
}

void HardwareManager::deviceEnabled(int dev, bool enable)
{
    outputDevices.value(dev)->setEnabled(enable);
}
