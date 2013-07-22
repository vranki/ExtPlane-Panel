/*
 *  Created by bobgates on 2011/07/10.
 */

#ifndef TURNANDBANK_H
#define TURNANDBANK_H

#include "panelitem.h"


#define maxRoll 70
#define maxOffbalance 7.5

class TurnAndBank : public PanelItem
{
    Q_OBJECT
public:
    Q_INVOKABLE TurnAndBank(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
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
private:
    void createCard(void);
    void createFrame(void);
    void createGlass(void);
    void createBall(void);
    
    float value2Angle1(float value);
    float value2Angle2(float value);
    ExtPlaneClient _client;
    DistanceUnit units;
    PressureUnit baroUnits;
    float _range1, _range2, _value;
    float _thickBars, _thinBars, _numbers;
    float _numbersScale;
    float _baroPressure;
    
    float _slipValue;
    float _rollValue;
    QString _label;
    QString _rollRef;
    QString _slipRef;
    QPixmap _card;
    QPixmap _frame;
    QPixmap _glass;
    QPixmap _ball;
    QPixmap _bezel;
    
};

#endif // TURNANDBANK_H
