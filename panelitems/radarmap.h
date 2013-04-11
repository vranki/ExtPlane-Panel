#ifndef RADARMAP_H
#define RADARMAP_H

#include "mapinstrument.h"

class RadarMap : public MapInstrument
{
    Q_OBJECT

public:
    Q_INVOKABLE RadarMap(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void itemSizeChanged(float w, float h);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);

public slots:
    virtual void tickTime(double dt, int total);
    void setMonitorFade(int val) {_radarMonitorFade=val;}
    void setSweepSpeed(int val) {_radarSweepSpeed=val;}

private:
    // Internal variables
    QPixmap         _mask;
    QPixmap         _radarImage;
    double          _radarCurrentAngle;

    // Configuration variables
    double          _radarMonitorFade;
    double          _radarSweepSpeed;

};

#endif // RADARMAP_H
