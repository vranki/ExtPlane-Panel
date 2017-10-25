#include "hardwaremanager.h"
#include <QDebug>
#include "hardwarebinding.h"
#include "servoblasteroutputdevice.h"
#include "pololuoutputdevice.h"
#include "chromaoutputdevice.h"
#include "nulloutputdevice.h"
#include "../util/console.h"

HardwareManager::HardwareManager(QObject *parent, ExtPlaneConnection *conn) : QObject(parent), connection_(conn) {
    NullOutputDevice *no = new NullOutputDevice(this);
    outputDevices.insert(no->id(), no);
    ServoBlasterOutputDevice *sbo = new ServoBlasterOutputDevice(this);
    outputDevices.insert(sbo->id(), sbo);
    PololuOutputDevice *pod = new PololuOutputDevice(this);
    outputDevices.insert(pod->id(), pod);
    ChromaOutputDevice *chd = new ChromaOutputDevice(this);
    outputDevices.insert(chd->id(), chd);
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
    int n = 0;
    foreach(HardwareBinding *binding, bindings()) {
        panelSettings->beginGroup("binding-" + QString::number(n));
        binding->storeSettings(panelSettings);
        panelSettings->endGroup();
        n++;
    }
    panelSettings->setValue("deviceCount", outputDevices.values().count());
    foreach(OutputDevice *device, devices()) {
        panelSettings->beginGroup("device-" + QString::number(device->id()));
        device->storeSettings(panelSettings);
        panelSettings->endGroup();
    }
}

void HardwareManager::loadSettings(QSettings *panelSettings) {
    foreach(OutputDevice *device, outputDevices) {
        emit deviceAvailable(device->id(), device->init());
    }

    int n = panelSettings->value("bindingCount").toInt();
    for(int i=0;i<n;i++) {
        panelSettings->beginGroup("binding-" + QString::number(i));
        HardwareBinding *binding = new HardwareBinding(this, connection());
        binding->loadSettings(panelSettings);
        panelSettings->endGroup();
        addBinding(binding);
        binding->activate();
    }
    foreach(OutputDevice *device, outputDevices.values()) {
        panelSettings->beginGroup("device-" + QString::number(device->id()));
        device->loadSettings(panelSettings);
        panelSettings->endGroup();
    }
}

void HardwareManager::tickTime(double dt, int total)
{
    foreach(HardwareBinding *binding, bindings())
        binding->tickTime(dt, total);
}

void HardwareManager::deviceChanged(HardwareBinding *binding, int device)
{
    Q_UNUSED(device);
    Q_ASSERT(outputDevices.contains(binding->device()));
    connect(binding, SIGNAL(outputValue(double, int, int)), outputDevices.value(binding->device()), SLOT(outputValue(double, int, int)));
    DEBUG << "connected output of binding" << binding->name() << "to device" << outputDevices.value(binding->device())->id();
}

void HardwareManager::deviceEnabled(int dev, bool enable)
{
    outputDevices.value(dev)->setEnabled(enable);
}
