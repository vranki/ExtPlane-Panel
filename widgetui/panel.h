#ifndef PANEL_H
#define PANEL_H

#include <QObject>
#include <QList>
#include <QDebug>

#include "../settings.h"

// Forward declarations
class PanelItem;

// Typedefs
typedef QList<PanelItem*> PanelItemList;

// Defs
#define PANEL_PANELITEM_COVER_ZVALUE 9000
#define PANEL_PANELITEM_GLOW_ZVALUE 9100
#define PANEL_PANELINFO_ZVALUE 9500

/**
 * @brief A logical Panel
 *
 * Contains references to the panel's current PanelItems. The Panel is
 * shared between all the PanelItems.
 *
 * Note: Called ExtPlanePanel instead of Panel because of Qt's QFrame::Panel
 * name conflict. We might consider using namespaces?
 *
 * @see PanelItem, PanelWindow
 */
class ExtPlanePanel : public QObject
{
    Q_OBJECT

public:
    explicit ExtPlanePanel(Settings *appSettings, QObject *parent = 0);
    ~ExtPlanePanel();
    inline PanelItemList* items() { return _items; }

signals:

public slots:


public:
    Settings *appSettings;
    QSettings *settings; // Contains all PanelItem settings
    int rotation; // Master rotation of the panel
    QString name;
    QString groupName;
    bool hasAvionicsPower;

private:
    PanelItemList *_items;


};


#endif // PANEL_H
