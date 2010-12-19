#ifndef ALTIMETER_H
#define ALTIMETER_H

#include <QLabel>
#include "../widgets/distanceunitcombobox.h"
#include "../extplaneclient.h"
#include "panelitem.h"
#include "../units.h"

class Altimeter : public PanelItem
{
    Q_OBJECT
public:
    Altimeter(QObject *parent, ExtPlaneConnection *conn);
    virtual void storeSettings(QSettings &settings);
    virtual void loadSettings(QSettings &settings);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString typeName();
    static QString typeNameStatic();
    virtual void createSettings(QGridLayout *layout);
    void setNumbers(float div);
    void setLabel(QString text);
    void setRanges(float r1, float r2);
    void setBars(float thick, float thin);
public slots:
    void altChanged(QString name, double alt);
    void setUnit(DistanceUnit un);
private:
    float value2Angle1(float value);
    float value2Angle2(float value);
    ExtPlaneClient _client;
    DistanceUnit units;
    float _range1, _range2, _value;
    float _thickBars, _thinBars, _numbers;
    float _numbersScale;
    QString _label;
};

#endif // ALTIMETER_H
