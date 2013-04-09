#include "panel.h"

ExtPlane::Panel::Panel(Settings *appSettings, QObject *parent) : QObject(parent), appSettings(appSettings)
{
    settings = NULL;
    rotation = 0;
}

ExtPlane::Panel::~Panel() {
    if(settings) delete settings;
}


