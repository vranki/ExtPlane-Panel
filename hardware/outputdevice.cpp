#include "outputdevice.h"
#include "../util/console.h"

OutputDevice::OutputDevice(QObject *parent) : QObject(parent)
  , minValue(0)
  , maxValue(1000)
  , enabled(false)
  , available(false)
{ }

void OutputDevice::setEnabled(bool e) {
    enabled = e;
    emit deviceEnabled(enabled);
}

void OutputDevice::storeSettings(QSettings *panelSettings)
{
    panelSettings->setValue("enabled", enabled);
}

void OutputDevice::loadSettings(QSettings *panelSettings)
{
    bool configEnabled = panelSettings->value("enabled").toBool();
    if(configEnabled) {
        if(available) {
            setEnabled(true);
        } else {
            // @todo Error dialog?
            DEBUG << "Device not available!";
        }
    }
}

void OutputDevice::safePosition()
{
}

bool OutputDevice::isAvailable()
{
    return available;
}

bool OutputDevice::isEnabled()
{
    return enabled;
}

QString OutputDevice::statusString()
{
    return status;
}
