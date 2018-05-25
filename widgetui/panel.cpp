#include "panel.h"

ExtPlanePanel::ExtPlanePanel(Settings *appSettings, QObject *parent) : QObject(parent), appSettings(appSettings)
{
    _items = new PanelItemList();
    settings = NULL;
    rotation = 0;
    hasAvionicsPower = true;
}

ExtPlanePanel::~ExtPlanePanel() {
    if(settings) delete settings;
    if(_items) delete _items;
}


