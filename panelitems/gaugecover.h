#ifndef GAUGECOVER_H
#define GAUGECOVER_H

#include "panelitem.h"

#include <QBitmap>
#include <QImage>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>

class GaugeCover : public PanelItem {

    Q_OBJECT

public:
    Q_INVOKABLE GaugeCover(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void itemSizeChanged(float w, float h);
    virtual void setEditMode(bool em);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void createSettings(QGridLayout *layout);
    virtual void settingChanged();
    void drawCoverImageCircularFitting(QPainter *painter, int centerX, int centerY, int radius);
    void drawCoverImageRectangularFitting(QPainter *painter, int centerX, int centerY, int width, int height);
    void drawCoverImageScrew(QPainter *painter, int centerX, int centerY);

public slots:
    void drawCoverImage();
    void setGaugesInset(int val) {_gaugesInset = val;}
    void setGaugesSize(int val) {_gaugesSize = val;}
    void setGrainEnabled(bool val) {_grainEnabled = val;}
    void setGrainStrength(int val) {_grainStrengh = val;}
    void setGrainZoom(int val) {_grainZoom = val;}
    void setGrainPersistance(int val) {_grainPersistance = val;}
    void setGrainQuality(int val) {_grainQuality = val;}
    void setGrainTiles(int val) {_grainTiles = val;}
    void setBackgroundColor(QColor val) {_backgroundColor = val;}
    void setScrewsEnabled(bool val) {_screwsEnabled = val;}
    void setScrewsSize(int val) {_screwsSize = val;}
    void setScrewsOffset(int val) {_screwsOffset = val;}

private:
    // Internal variables
    QImage      _coverImage;        // The image buffer

    // Configuration variables
    QColor      _backgroundColor;
    int         _gaugesSize;        // The size of the gauges (1-100)
    int         _gaugesInset;       // The inset of the gauge (shadow/border/engravement)
    bool        _grainEnabled;
    int         _grainStrengh;
    int         _grainZoom;
    int         _grainPersistance;
    int         _grainQuality;
    int         _grainTiles;
    bool        _screwsEnabled;
    int        _screwsSize;
    int        _screwsOffset;
};

#endif // GAUGECOVER_H
