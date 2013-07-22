#ifndef HARDWAREMANAGER_H
#define HARDWAREMANAGER_H

#include <QObject>
#include <QList>
#include <QSettings>

class HardwareBinding;
class ExtPlaneConnection;
class OutputDevice;

class HardwareManager : public QObject
{
    Q_OBJECT
public:
    explicit HardwareManager(QObject *parent, ExtPlaneConnection *conn);
    QList<HardwareBinding*>& bindings();
    QMap<int, OutputDevice*>& devices();
    ExtPlaneConnection *connection();
    void addBinding(HardwareBinding *binding);
    void deleteBinding(HardwareBinding *binding);
    void saveSettings(QSettings *panelSettings);
    void loadSettings(QSettings *panelSettings);
signals:
    void deviceAvailable(int dev, bool avail);
public slots:
    void tickTime(double dt, int total);
private slots:
    void deviceChanged(HardwareBinding *binding, int device);
    void deviceEnabled(int dev, bool enable);
private:
    QList<HardwareBinding*> hwBindings;
    QMap<int, OutputDevice*> outputDevices;
    ExtPlaneConnection *connection_;
};

#endif // HARDWAREMANAGER_H
