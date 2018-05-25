/*
 *  Created by bobgates on 2011/08/01.
 */

#ifndef HSI_H
#define HSI_H

#include "panelitem.h"
#include "../util/units.h"

enum DataRefType {
    drFloat, drInt, drChar
};

struct DataRefStruct {
    QString         name;
    float           tolerance;
    DataRefType     dataType;
    void *          value;
    bool            enabled;
};

class HSI : public PanelItem
{
    Q_OBJECT

public:
    Q_INVOKABLE HSI(ExtPlanePanel *panel, ExtPlaneConnection *conn);
    virtual void createSettings(QGridLayout *layout);
    virtual void loadSettings(QSettings &settings);
    virtual void storeSettings(QSettings &settings);
    virtual void itemSizeChanged(float w, float h);
    virtual QString typeName();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    void createCard(float w, float h);

public slots:
    void refChanged(QString name, double alt);
    void setShowDegrees(int state);
    void setShowPlane(int state);
        
private:
    float   value2Angle(float value);

private:
    ExtPlaneClient _client;
    bool    _showDegrees;
    bool    _showPlane;
    float   _heading;
    float   _bug;
    float   _course;
    float   _dme;
    float   _dmeTime;
    float   _hdots;
    float   _vdots;
    float    _hasVertical;      // All float because the import mechanism
    float    _hasHorizontal;    // expects floats.
    float    _hasDME;
    float    _glideslopeFlag;
    float    _toFrom;  // 0 = Flag, 1=To, 2=From
    float   _thickBars;
    float   _thinBars;
    float   _numbers;
    //QString _dataRef;
    QPixmap _card;
    QList <DataRefStruct *> _dataRefs;
    QHash <QString, DataRefStruct> _dataRefLookup;
};

#endif // HSI_H



