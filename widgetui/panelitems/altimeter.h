#ifndef ALTIMETER_H
#define ALTIMETER_H

#include "panelitem.h"
#include "../util/units.h"

class Needle;

class Altimeter : public PanelItem
{
    Q_OBJECT
public:
    Q_INVOKABLE Altimeter(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    virtual void createSettings(QGridLayout *layout);
    void setLabel(QString text);
    void setBars(float thick, float thin);
    virtual void itemSizeChanged(float w, float h);
public slots:
    void refChanged(QString name, double alt);
    void setUnit(DistanceUnit un);
    void setRange1(float v);
    void setRange2(float v);
    void setThickBars(float v);
    void setThinBars(float v);
    void setNumbersScale(float v);
    void setNumbers(float div);
private:
    void repaintPixmap();
    float value2Angle1(float value);
    float value2Angle2(float value);
    ExtPlaneClient _client;
    DistanceUnit units;
    PressureUnit baroUnits;
    float _range1, _range2, _value;
    float _thickBars, _thinBars, _numbers;
    float _numbersScale;
    float _baroPressure;
    QString _label;
    Needle *longNeedle, *shortNeedle;
    QPixmap bottomPixmap;
    QRectF pressureWindow;
};

#endif // ALTIMETER_H
