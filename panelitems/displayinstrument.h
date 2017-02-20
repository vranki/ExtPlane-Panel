#ifndef DISPLAYINSTRUMENT_H
#define DISPLAYINSTRUMENT_H

#include "panelitem.h"
#include "extplaneclient.h"

#include <QTime>

/**
 * Base class for any CRT display instrument or panel. This class
 * allows a PanelItem to easily mimick any digital display instrument.
 * It provides a refresh/rendering mechanism and monitor fade effect
 * as well as helper function to draw common gui element.
 *
 * @todo: Add common functionality for on/off state of battery/avionics...
 */
class DisplayInstrument : public PanelItem
{
    Q_OBJECT
public:
    explicit DisplayInstrument(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual ~DisplayInstrument();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void render(QPainter *painter, int width, int height) = 0;
    virtual void itemSizeChanged(float w, float h);
    virtual QString typeName() { return ""; } // Note: not really a nice solution but allows constructor to take care of client init and dataref subscribe
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);

protected:


signals:
    
public slots:
    virtual void tickTime(double dt, int total);
    void setMonitorFade(int val) {_monitorFade=val;}
    void setDisplayStrength(int val) {_displayStrength=val;}
    void setResolution(int val) {_resolution=val;}
    void setRefreshHerz(int val) {_refreshHerz=val;}

protected:
    // Internal variables
    ExtPlaneClient  _client;
    QPixmap         _monitorImage;      // Image stores currently rendered image
    QPixmap         _bufferImage;       // Image for a fresh render
    int             _monitorFade;       // 0.0 - 1.0
    int             _displayStrength;   // 0.0 - 1.0
    int             _resolution;        // 1.0 - 3.0
    QTime           _lastRender;
    int             _refreshHerz;       // 1 - 60
    
};

#endif // DISPLAYINSTRUMENT_H
