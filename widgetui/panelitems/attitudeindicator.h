#ifndef ATTITUDEINDICATOR_H
#define ATTITUDEINDICATOR_H

#include "panelitem.h"
#include "../util/units.h"

#define GROUNDBROWN QColor(191,163,94)
#define LIGHTGROUNDBROWN QColor(231,203,134)

#define LIGHTSKYBLUE QColor(191,226,255)
#define VERYLIGHTBLUE QColor(221, 241, 255)
#define WHITEBLUE QColor(240, 248, 255)
#define SKYBLUE QColor(79, 129, 255)

#define pitchPixelsPerDegree 1.0
#define maxPitch 35

class AttitudeIndicator : public PanelItem
{
    Q_OBJECT
public:
    Q_INVOKABLE AttitudeIndicator(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void createSettings(QGridLayout *layout);
    virtual void itemSizeChanged(float w, float h);
    public slots:
    void refChanged(QString name, double alt);
private:
    void createCard(float w, float h);
    void createFrame(float w, float h);
    void createGlass(float w, float h);
    void createBackground(float w, float h);
    void createBezel(void);
    float value2Angle1(float value);
    float value2Angle2(float value);
private:
    ExtPlaneClient _client;
    DistanceUnit units;
    PressureUnit baroUnits;
    float _range1, _range2, _value;
    float _thickBars, _thinBars, _numbers;
    float _numbersScale;
    float _baroPressure;
    float _rollValue;
    float _pitchValue;
    QString _pitchRef;
    QString _rollRef;
    QPixmap _card;
    QPixmap _frame;
    QPixmap _glass;
    QPixmap _background;
};

#endif // ATTITUDEINDICATOR_H
