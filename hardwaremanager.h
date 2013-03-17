#ifndef HARDWAREMANAGER_H
#define HARDWAREMANAGER_H

#include <QObject>
#include <QList>
#include <QSettings>

class HardwareBinding;

class HardwareManager : public QObject
{
    Q_OBJECT
public:
    explicit HardwareManager(QObject *parent = 0);
    QList<HardwareBinding*>& bindings();
    void addBinding(HardwareBinding *binding);
    void saveSettings(QSettings *panelSettings);
    void loadSettings(QSettings *panelSettings);
signals:
    
public slots:
private:
    QList<HardwareBinding*> hwBindings;
};

#endif // HARDWAREMANAGER_H
