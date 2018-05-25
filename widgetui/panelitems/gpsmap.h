#ifndef GPSMAP_H
#define GPSMAP_H

#include "mapinstrument.h"

class GPSMap : public MapInstrument
{
    Q_OBJECT

public:
    Q_INVOKABLE GPSMap(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);

};

#endif // GPSMAP_Hb
