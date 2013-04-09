#ifndef PANEL_H
#define PANEL_H

#include <QObject>

#include "settings.h"
#include "panelitemfactory.h"
#include "panelitems/panelitem.h"

namespace ExtPlane {
    class Panel : public QObject
    {
        Q_OBJECT
    public:
        explicit Panel(Settings *appSettings, QObject *parent = 0);
        ~Panel();

    signals:

    public slots:


    public:
        Settings *appSettings;
        QSettings *settings; // Contains all PanelItem settings
        PanelItemFactory itemFactory;
        QList<PanelItem *> items;
        int rotation; // Master rotation of the panel


    };
}

#endif // PANEL_H
