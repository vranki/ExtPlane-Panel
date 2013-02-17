/*
 *  Created by Bob Gates on 2011/07/08.
 */

#ifndef DIRECTIONINDICATOR_H
#define DIRECTIONINDICATOR_H

#include "panelitem.h"
#include "../units.h"

#define DIRECTION_INDICATOR_PLANE_SCALE 0.88
#define DIRECTION_INDICATOR_HEADING_SCALE 0.90
#define DIRECTION_INDICATOR_FIXED_HEADING_SCALE 0.96

class DirectionIndicator : public PanelItem
{
    Q_OBJECT
public:
    Q_INVOKABLE DirectionIndicator(QObject *parent, ExtPlaneConnection *conn);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void updateForNewSize(float w, float h);
    virtual QString typeName();
    virtual void createSettings(QGridLayout *layout);
    void setLabel(QString text);
    void setBars(float thick, float thin);
    public slots:
    void refChanged(QString name, double alt);
    void setUnit(DistanceUnit un);
    void setRange1(float v);
    void setRange2(float v);
    void setThickBars(float v);
    void setThinBars(float v);
    void setNumbersScale(float v);
    void setNumbers(float div);
    void createCard(float w, float h);
    
private:
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
    QString _dataRef;
    QPixmap _card; // A cached bitmap of the background rotating card
};

#endif // DIRECTIONINDICATOR_H
