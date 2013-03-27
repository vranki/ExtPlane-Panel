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
    ExtPlaneConnection *connection();
    void addBinding(HardwareBinding *binding);
    void deleteBinding(HardwareBinding *binding);
    void saveSettings(QSettings *panelSettings);
    void loadSettings(QSettings *panelSettings);
signals:
    
public slots:
private:
    QList<HardwareBinding*> hwBindings;
    OutputDevice *outputDevice; // @todo list when we have more than one
    ExtPlaneConnection *connection_;
};

#endif // HARDWAREMANAGER_H
