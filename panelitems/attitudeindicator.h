/*
 *  attitudeindicator.h
 *  extplane-panel
 *
 *  Created by bobgates on 2011/07/08.
 *  Copyright 2011 DaffeySoft. All rights reserved.
 *
 */


#ifndef AI_H
#define AI_H

#include <QLabel>
#include "../widgets/distanceunitcombobox.h"
#include "../extplaneclient.h"
#include "panelitem.h"
#include "../units.h"
#include "widgets/numberinputlineedit.h"


#define GROUNDBROWN QColor(191,163,94)
#define LIGHTGROUNDBROWN QColor(231,203,134)

#define LIGHTSKYBLUE QColor(191,226,255)
#define VERYLIGHTBLUE QColor(221, 241, 255)
#define WHITEBLUE QColor(240, 248, 255)
#define SKYBLUE QColor(79, 129, 255)

class AttitudeIndicator : public PanelItem
{
    Q_OBJECT
public:
    Q_INVOKABLE AttitudeIndicator(QObject *parent, ExtPlaneConnection *conn);
    virtual QString typeName();
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
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
private:
    void createCard(void);
    void createFrame(void);
    void createGlass(void);
    void createBackground(void);
    void createBezel(void);
    
    float value2Angle1(float value);
    float value2Angle2(float value);
    ExtPlaneClient _client;
    DistanceUnit units;
    PressureUnit baroUnits;
    float _range1, _range2, _value;
    float _thickBars, _thinBars, _numbers;
    float _numbersScale;
    float _baroPressure;
    
    static const float pitchPixelsPerDegree=1.0;
    static const float maxPitch=35;
    
    float _rollValue;
    float _pitchValue;
    QString _label;
    QString _pitchRef;
    QString _rollRef;
    QPixmap _card;
    QPixmap _frame;
    QPixmap _glass;
    QPixmap _background;
    QPixmap _bezel;
    
};

#endif // AI_H
