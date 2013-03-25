#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QMap>

/**
 * @brief Settings helper class
 *
 * An extended QSettings object which allows for overriding specific
 * settings from the command line. This is especially useful for users
 * who wish to load multiple ExtPlane-Panel instances on a single machine.
 *
 * Note: Settings from the command line argument do not take into account groups
 * in order to simplify specification on the command line.
 *
 * @see QSettings
 */
class Settings : public QSettings
{
    Q_OBJECT
public:
    explicit Settings(const QString &organization, const QString &application, QObject *parent = 0);
    QVariant valueFromSettingsOrCommandLine(const QString &key, const QVariant &defaultValue = QVariant());

signals:
    
public slots:

private:
    QMap<QString,QString> parsedArguments;
    
};

#endif // SETTINGS_H
