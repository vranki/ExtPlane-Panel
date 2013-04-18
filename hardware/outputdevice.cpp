#include "outputdevice.h"
#include <QDebug>

OutputDevice::OutputDevice(QObject *parent) :
    QObject(parent), enabled(false), available(false)
{
}

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
            qDebug() << Q_FUNC_INFO << "Device not available!";
        }
    }
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
