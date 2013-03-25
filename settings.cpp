#include "settings.h"

#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

Settings::Settings(const QString &organization, const QString &application, QObject *parent) :
    QSettings(organization, application, parent)
{
    // We pre-parse these incase the app makes heavy use of some settings...

    // Parse the command line, storing the values in parsedArguments
    QString key = QString::null;
    foreach(QString arg, QCoreApplication::arguments()) {
        // Is this a new parameter?
        if(arg.startsWith("--")) {
            // Extract the key, and pre-emptively set as a flag
            key = arg.mid(2);
            parsedArguments.insert(key,"true");
        } else {
            if(!key.isNull()) {
                // Store value using the key
                parsedArguments.insert(key,arg);
                key = QString::null;
            }
        }
    }

    // Show what happened
    foreach(QString arg, parsedArguments.keys()) {
        qDebug() << "detected command line argument" << arg << "with value" << parsedArguments.value(arg);
    }
}

QVariant Settings::valueFromSettingsOrCommandLine(const QString &key, const QVariant &defaultValue) {
    // Is this a setting which is being overided by the command line arguments?
    if(parsedArguments.contains(key))   return parsedArguments.value(key);
    else                                return QSettings::value(key,defaultValue);
}
