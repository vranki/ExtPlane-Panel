#include "hardwaremanager.h"
#include "hardwarebinding.h"

HardwareManager::HardwareManager(QObject *parent, ExtPlaneConnection *conn) : QObject(parent), connection_(conn) {
}

QList<HardwareBinding *> &HardwareManager::bindings() {
    return hwBindings;
}

ExtPlaneConnection *HardwareManager::connection()
{
    return connection_;
}

void HardwareManager::addBinding(HardwareBinding *binding) {
    Q_ASSERT(binding->parent()==this);
    hwBindings.append(binding);
}

void HardwareManager::deleteBinding(HardwareBinding *binding)
{
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
