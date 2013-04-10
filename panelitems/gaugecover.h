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
    void drawCoverImage();
    void drawCoverImageCircularFitting(QPainter *painter, int centerX, int centerY, int radius);
    void drawCoverImageRectangularFitting(QPainter *painter, int centerX, int centerY, int width, int height);

public slots:
    void setGaugesH(int val) {_gaugesH = val;}
    void setGaugesV(int val) {_gaugesV = val;}
    void setGaugesSize(int val) {_gaugesSize = val;}
    void setGrainEnabled(bool val) {_grainEnabled = val;}
    void setGrainStrength(int val) {_grainStrengh = val;}
    void setGrainZoom(int val) {_grainZoom = val;}
    void setGrainPersistance(int val) {_grainPersistance = val;}
    void setGrainQuality(int val) {_grainQuality = val;}
    void setBackgroundColor(QString val) {_backgroundColor = val;}

private:
    //QPixmap _coverMask;
    QImage      _coverImage;        // The image buffer
    int         _gaugesH;           // Number of gauges to draw horizontally (1-10)
    int         _gaugesV;           // Number of gauges to draw vertically (1-10)
    int         _gaugesSize;        // The size of the gauges (1-100)
    int         _gaugesInset;       // The inset of the gauge (shadow/border/engravement)
    bool        _grainEnabled;
    int         _grainStrengh;
    int         _grainZoom;
    int         _grainPersistance;
    int         _grainQuality;
    QString     _backgroundColor;
};

#endif // GAUGECOVER_H
